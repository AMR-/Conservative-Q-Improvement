from qtree_wrapper import PyBox
from qtree_wrapper import PyDiscrete
from qtree_wrapper import PyState
from qtree_wrapper import PyVector

# Converts OpenAI Box to PyBox 
def convert_to_pybox(b):
    low = PyVector()
    high = PyVector()

    for i in b.low:
        low.add(i)

    for i in b.high:
        high.add(i)

    return PyBox(low, high)

# Converts an array to a PyState
def convert_to_pystate(s):
    if type(s) is PyState:
        return s

    v = PyVector()

    for i in s:
        v.add(i)
    
    return PyState(v)
