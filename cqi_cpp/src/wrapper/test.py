import qtree_wrapper as qtree

# Simple usage of QTree ported from C++
# Need to run './cython_build_wrapper.sh' after writing to qtree_wrapper.pyx or any of the C++ files

# Getting the constructors
Vector = qtree.PyVector
Box = qtree.PyBox
Discrete = qtree.PyDiscrete
QTree = qtree.PyQTree

low = Vector()
low.add(1.0)
low.add(2.0)

high = Vector()
high.add(3.0)
high.add(4.0)

states = Box(low, high)
actions = Discrete(4)

# qtree = QTree(states, actions, None)

# print(qtree.print_structure())
