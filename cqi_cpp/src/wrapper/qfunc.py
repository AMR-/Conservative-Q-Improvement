from abc import ABC, abstractmethod

class QFunc(ABC):
    def __init__(self, state_space, act_space):
        self.state_space = state_space
        self.act_space = act_space
        self._self_copy = None
        self._make_copies = False

    @abstractmethod
    def select_a(self, s):
        pass

    @abstractmethod
    def take_tuple(self, s, a, r, s2, done):
        pass

    @abstractmethod
    def just_split(self):
        pass

    def get_pre_split(self):
        return self._self_copy if self._self_copy else self._raise_error()

    @staticmethod
    def _raise_error():
        raise ReferenceError

    def enable_copies(self):
        self._make_copies = True

    def disable_copies(self):
        self._make_copies = False

