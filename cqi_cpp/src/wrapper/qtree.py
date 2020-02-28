from qfunc import QFunc
import numpy as np
from copy import deepcopy
from abc import ABC, abstractmethod

class QTree(QFunc):
    def __init__(self, state_space, act_space,
                 root=None,
                 gamma=0.99,
                 alpha=0.1,
                 visit_decay=0.99,
                 split_thresh_max=1,
                 split_thresh_decay=0.99,
                 num_splits=2):
        super().__init__(state_space, act_space)
        if root is None:
            low = self.state_space.low
            high = self.state_space.high
            splits = []
            for f in range(len(low)):
                for i in range(num_splits):
                    splits.append(LeafSplit(f,
                        low[f] + (high[f] - low[f])/(num_splits+1)*(i+1), np.zeros(act_space.n), np.zeros(act_space.n), 0.5, 0.5))
            self.root = QTreeLeaf(np.zeros(act_space.n), 1, splits)
        else:
            self.root = root
        self.params = QTreeParamStore()
        self.params.set_param('gamma', gamma)
        self.params.set_param('alpha', alpha)
        self.params.set_param('visit_decay', visit_decay)
        self.params.set_param('num_splits', num_splits)
        self.split_thresh_max = split_thresh_max
        self.split_thresh_decay = split_thresh_decay
        self.split_thresh = self.split_thresh_max
        self._just_split = False  # True if the most recent action resulted in a split

    def select_a(self, s):
        return np.argmax(self.root.get_qs(s))

    def take_tuple(self, s, a, r, s2, done):
        self._just_split = False
        self._self_copy = None
        # update a leaf directly
        self.update(s, a, r, s2, done)
        # modify tree
        if self.root.max_split_util(s) > self.split_thresh:
            print('split')
            self._just_split = True
            if self._make_copies:
                self._self_copy = deepcopy(self)
            self.root = self.root.split(s, self.state_space.low,
                                        self.state_space.high, self.params)
            self.split_thresh = self.split_thresh_max
        else:
            self.split_thresh = self.split_thresh * self.split_thresh_decay

    def update(self, s, a, r, s2, done):
        if done:
            target = r
        else:
            target = r + (self.params.val('gamma') * np.max(self.root.get_qs(s2)))
        self.root.update(s, a, target, self.params)

    def num_nodes(self):
        return self.root.num_nodes()

    def print_structure(self):
        self.root.print_structure('└', ' ')

    def just_split(self):
        return self._just_split


class QTreeNode(ABC):
    def __init__(self, visits):
        self.visits = visits

    @abstractmethod
    def is_leaf(self):
        pass

    @abstractmethod
    def get_qs(self, s):
        pass

    def update(self, s, a, target, params):
        self.visits = self.visits * params.val('visit_decay') + (1 - params.val('visit_decay'))
    def no_visit_update(self, params):
        self.visits = self.visits * params.val('visit_decay')

    @abstractmethod
    def split(self, s, box_low, box_high, params):
        pass

    @abstractmethod
    def max_split_util(self, s):
        pass

    @abstractmethod
    def num_nodes(self):
        pass

    @abstractmethod
    def print_structure(self, prefix_head, prefix_tail):
        pass

class LeafSplit():
    def __init__(self, feature, value, left_qs, right_qs, left_visits, right_visits):
        self.feature = feature
        self.value = value
        self.left_qs = left_qs
        self.right_qs = right_qs
        self.left_visits = left_visits
        self.right_visits = right_visits

    def update(self, s, a, target, params):
        self.left_visits = self.left_visits * params.val('visit_decay')
        self.right_visits = self.right_visits * params.val('visit_decay')
        if s[self.feature] < self.value:
            self.left_qs[a] = (1 - params.val('alpha')) * self.left_qs[a] + params.val('alpha') * target
            self.left_visits = self.left_visits + (1 - params.val('visit_decay'))
        else:
            self.right_qs[a] = (1 - params.val('alpha')) * self.right_qs[a] + params.val('alpha') * target
            self.right_visits = self.right_visits + (1 - params.val('visit_decay'))

    def eval_utility(self, pol_q_vals):
        action_chosen = np.argmax(pol_q_vals)
        left_pot_util = np.max(self.left_qs) - self.left_qs[action_chosen]
        right_pot_util = np.max(self.right_qs) - self.right_qs[action_chosen]

        return left_pot_util*self.left_visits + right_pot_util*self.right_visits


class QTreeLeaf(QTreeNode):
    def __init__(self, qs, visits, splits):
        super().__init__(visits)
        self.qs = qs
        self.splits = splits

    def is_leaf(self):
        return True

    def get_qs(self, s):
        return self.qs

    def update(self, s, a, target, params):
        super().update(s, a, target, params)
        self.qs[a] = (1 - params.val('alpha')) * self.qs[a] + params.val('alpha') * target

        for split in self.splits:
            split.update(s, a, target, params)

    def split(self, s, box_low, box_high, params):
        split_index = np.argmax([sp.eval_utility(self.qs) for sp in self.splits])
        sf_split = self.splits[split_index]
        split_feature = sf_split.feature
        l_splits = []
        r_splits = []
        for sp in self.splits:
            if sp.feature != split_feature:
                l_splits.append(LeafSplit(sp.feature, sp.value, sf_split.left_qs.copy(), sf_split.left_qs.copy(), 0.5, 0.5))
                r_splits.append(LeafSplit(sp.feature, sp.value, sf_split.right_qs.copy(), sf_split.right_qs.copy(), 0.5, 0.5))
        for i in range(params.val('num_splits')):
            l_splits.append(LeafSplit(split_feature, box_low[split_feature] + (sf_split.value - box_low[split_feature])/(params.val('num_splits')+1)*(i+1), \
                                      sf_split.left_qs.copy(), \
                                      sf_split.left_qs.copy(), 0.5, 0.5))
            r_splits.append(LeafSplit(split_feature, sf_split.value + (box_high[split_feature] - sf_split.value)/(params.val('num_splits')+1)*(i+1), \
                                      sf_split.right_qs.copy(), \
                                      sf_split.right_qs.copy(), 0.5, 0.5))
        left_child = QTreeLeaf(sf_split.left_qs, sf_split.left_visits, l_splits)
        right_child = QTreeLeaf(sf_split.right_qs, sf_split.right_visits, r_splits)
        val = (box_high[split_feature] + box_low[split_feature])/2
        visits = self.visits

        return QTreeInternal(left_child, right_child, split_feature, val, visits)

    def max_split_util(self, s):
        return self.visits * np.max([sp.eval_utility(self.qs) \
                                     for sp in self.splits])

    def num_nodes(self):
        return 1

    def print_structure(self, prefix_head, prefix_tail):
        print(prefix_head + '(vis: ' + "{:1.2f}".format(self.visits) + ') qvals: ' + str(self.qs))


class QTreeInternal(QTreeNode):
    def __init__(self, left_child, right_child, feature, value, visits):
        super().__init__(visits)
        self.left_child = left_child
        self.right_child = right_child
        self.feature = feature
        self.value = value

    def is_leaf(self):
        return False

    def get_qs(self, s):
        return self.select_child(s)[0].get_qs(s)

    def update(self, s, a, target, params):
        super().update(s, a, target, params)
        it, not_it = self.select_child(s)
        it.update(s, a, target, params)
        not_it.no_visit_update(params)

    def split(self, s, box_low, box_high, params):
        if s[self.feature] < self.value:
            box_high[self.feature] = self.value
            self.left_child = self.left_child.split(s, box_low, box_high, params)
        else:
            box_low[self.feature] = self.value
            self.right_child = self.right_child.split(s, box_low, box_high, params)
        return self

    def select_child(self, s):
        if s[self.feature] < self.value:
            return self.left_child, self.right_child
        else:
            return self.right_child, self.left_child

    def max_split_util(self, s):
        return self.visits * self.select_child(s)[0].max_split_util(s)

    def num_nodes(self):
        return 1 + self.left_child.num_nodes() + self.right_child.num_nodes()

    def print_structure(self, prefix_head, prefix_tail):
        print(prefix_head + '(vis: ' + "{:1.2f}".format(self.visits) + ') if f[' + str(self.feature) +'] ? ' + str(self.value) + ':')
        self.left_child.print_structure(prefix_tail + ' ├<', prefix_tail + ' │')
        self.right_child.print_structure(prefix_tail + ' └>', prefix_tail + '  ')


class QTreeParamStore():
    def __init__(self):
        self.param_dict = {}

    def set_param(self, name, val):
        self.param_dict[name] = val

    def val(self, name):
        return self.param_dict[name]
