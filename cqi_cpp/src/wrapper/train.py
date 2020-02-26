import numpy as np
import os
import pickle

from collections import defaultdict

from utils import convert_to_pystate

from qtree_wrapper import PyAction

class StateChangeTracker(object):
    def __init__(self, init_states):
        self.init_states = init_states
        self.final_states = init_states
        self.single_change = [False] * len(init_states)

    def new_states(self, s):
        # Capture First Change True, Second+ Change False, No change - keep same
        self.single_change = [((i[0] != i[2] and i[1] == i[2]) or (i[0] == i[1] and i[3]))
                              for i in zip(s, self.final_states, self.init_states, self.single_change)]
        self.final_states = s


class Train(object):
    def __init__(self, qfunc, gym_env, expl_data_filename="explain_data.csv"):
        self.qfunc = qfunc
        self.env = gym_env
        self.expl_data_filename = expl_data_filename
        self._self_tree_ct = 0
        self._pickle_filename = "qfunc_copy_%d.pkl"

    def note_expla_data(self, tag, nodes, reward):
        with open(self.expl_data_filename, "a") as myfile:
            myfile.write(f"\n{tag},{nodes},{reward}")

    def train(self, num_steps, eps_func, verbose=False, eval_only=False, penalty_check=lambda s, r: 0,
              track_data_per=0, run_tag="?", qfunc_hist=None, qfunc_hist_directory=None,
              qfunc_hist_per_every_nn=1,
              while_watch=False):
        if eval_only:
            print("DEbUG DEbuG EVAL ONLY")
            self.qfunc.print_structure()
        hist = defaultdict(list)   # number of nodes, reward per ep
        ep_r = 0
        done = True
        sct = None
        r_per_ep = []
        pen_per_ep = []
        ts_per_ep = []
        num_eps = 0
        last_step_ep = -1
        single_change_features = None
        istates, fstates = None, None
        for step in range(num_steps):
            if done:
                if verbose:
                    print(f"Episode reward: {ep_r}; Elapsed steps: {step}")
                    if while_watch and step > 0:
                        print("Any single state changes?")
                        print(sct.single_change)
                        print(sct.init_states)
                        print(sct.final_states)
                if while_watch and step > 0:
                    single_change_features = (single_change_features and
                                              sct.single_change) if single_change_features else sct.single_change
                    istates = sct.init_states
                    fstates = sct.final_states
                if eval_only and step > 0:
                    r_per_ep.append(ep_r)
                    pen_per_ep.append(penalty_check(s, ep_r))
                    ts = step - last_step_ep
                    ts_per_ep.append(ts)
                    last_step_ep = step
                if track_data_per > 0 and num_eps % track_data_per == 0:
                    hist[self.qfunc.num_nodes()].append(ep_r)
                    num_nodes = self.qfunc.num_nodes()
                    print(f"At episode {num_eps}, hist is {num_nodes}, {ep_r}")
                    self.note_expla_data(run_tag, num_nodes, ep_r)
                s = self.env.reset()
                ep_r = 0
                num_eps = num_eps + 1
                done = False
                if while_watch:
                    sct = StateChangeTracker(s)
                    print("--------------------A NEW EP BEGINS------------------------------")
            # Ɛ-greedy action selection 
            if np.random.random() < eps_func(step):
                a = self.env.action_space.sample()
            else:
                s = convert_to_pystate(s)
                a = self.qfunc.select_a(s)
            s2, r, done, _ = self.env.step(a)
            if while_watch:
                sct.new_states(s2)
            if not eval_only:
                s, s2 = convert_to_pystate(s), convert_to_pystate(s2)
                a = PyAction(a)

                self.qfunc.take_tuple(s, a, r, s2, done)
                if qfunc_hist is not None and self.qfunc.just_split():
                    qfunc_hist.append(self.qfunc.get_pre_split())
                if qfunc_hist_directory is not None and self.qfunc.just_split():
                    tree_pre_split = self.qfunc.get_pre_split()
                    nn = tree_pre_split.num_nodes()
                    if nn < 10 or nn % qfunc_hist_per_every_nn == 0:
                        self._self_tree_ct = self._self_tree_ct + 1
                        with open(os.path.join(qfunc_hist_directory, (self._pickle_filename % self._self_tree_ct)), 'wb') as pfile:
                            sf = tree_pre_split.sfuncs
                            tree_pre_split.sfuncs = None
                            pickle.dump(tree_pre_split, pfile)
                            tree_pre_split.sfuncs = sf
            s = s2
            ep_r += r
        if eval_only:
            avg_r_per_ep = np.mean(r_per_ep)
            results = f"Num_eps: {num_eps}\nReward per ep: {avg_r_per_ep}\nTimesteps per ep: {np.mean(ts_per_ep)}\nPenalties per ep: {np.mean(pen_per_ep)}"
            print(results)
            return results, hist, avg_r_per_ep, (single_change_features, istates, fstates)
        else:
            return hist
