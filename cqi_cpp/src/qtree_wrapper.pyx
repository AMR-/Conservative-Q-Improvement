from libcpp.unordered_map cimport unordered_map
from libcpp.vector cimport vector
from libcpp.string cimport string

cdef extern from "../include/state.hpp":
    cdef cppclass State:
        vector[float]* state
        State(vector[float]*)

cdef extern from "../include/action.hpp":
    cdef cppclass Action:
        int value
        Action(int)

cdef extern from "../include/discrete.hpp":
    cdef cppclass Discrete:
        int n
        Discrete(int)

        int sample()
        int size()
        bint contains(int)

cdef extern from "../include/box.hpp":
    cdef cppclass Box:
        vector[float]* low
        vector[float]* high

        Box(vector[float]*, vector[float]*)
        vector[float]* sample()
        bint contains(vector[float]*)

cdef extern from "../include/qtreenode.hpp":
    cdef cppclass QTreeNode:
        float visits

        QTreeNode(float visits)
        bint isLeaf()
        vector[float]* getQS(State*)
        void update(State* s, Action* a, int target, unordered_map[string, float]* params)
        void noVisitUpdate(unordered_map[string, float]* params)
        QTreeNode* split(State*, vector[float]*, vector[float]*, unordered_map[string, float]*)
        float maxSplitUntil(State*)
        int numNodes()
        void printStructure(string, string)

cdef extern from "../include/qtree.hpp":
    cdef cppclass QTree:
        float splitThreshMax
        float splitThreshDecay
        float splitThresh
        QTreeNode* root
        bint _justSplit
        unordered_map[string, float]* params

        QTree(Box*, Discrete*, QTreeNode*, float, float, float, float, float, int)
        int selectA(State*)
        void takeTuple(State*, Action*, float, State*, bint)
        void update(State*, Action*, float, State*, bint)
        int numNodes()
        void printStructure()
        bint justSplit()

cdef class PyVector:
    cdef vector[float]* thisptr

    def __cinit__(self):
        self.thisptr = new vector[float]()
    def add(self, float f):
        self.thisptr.push_back(f)

cdef class PyBox:
    cdef Box* thisptr

    def __cinit__(self, PyVector low, PyVector high):
        self.thisptr = new Box(low.thisptr, high.thisptr)
    def contains(self, PyVector vec):
        return self.thisptr.contains(vec.thisptr)

cdef class PyDiscrete:
    cdef Discrete* thisptr

    def __cinit__(self, int n):
        self.thisptr = new Discrete(n)
    def sample(self):
        return self.thisptr.sample()
    def size(self):
        return self.thisptr.size()
    def contains(self, int x):
        return self.thisptr.contains(x)

cdef class PyState:
    cdef State* thisptr

    def __cinit__(self, PyVector state):
        self.thisptr = new State(state.thisptr)

cdef class PyAction:
    cdef Action* thisptr

    def __cinit__(self, int value):
        self.thisptr = new Action(value)

cdef class PyQTree:
    cdef QTree* thisptr

    def __cinit__(self, PyBox stateSpace, PyDiscrete actionSpace, None, float \
        gamma=0.99, float alpha=0.1, float visitDecay=0.99, float splitThresh=1,\
        float splitThreshDecay=0.99, int numSplits=2):
        self.thisptr = new QTree(stateSpace.thisptr, actionSpace.thisptr, NULL, \
        gamma, alpha, visitDecay, splitThresh, splitThreshDecay, numSplits)
    def select_a(self, PyState s):
        return self.thisptr.selectA(s.thisptr)
    def take_tuple(self, PyState s, PyAction a, float r, PyState s2, bint done):
        return self.thisptr.takeTuple(s.thisptr, a.thisptr, r, s2.thisptr, done)
    def update(self, PyState s, PyAction a, float r, PyState s2, bint done):
        return self.thisptr.update(s.thisptr, a.thisptr, r, s2.thisptr, done)
    def num_nodes(self):
        return self.thisptr.numNodes()
    def print_structure(self):
        return self.thisptr.printStructure()
    def just_split(self):
        return self.thisptr.justSplit()
