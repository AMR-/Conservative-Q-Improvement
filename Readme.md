# Conservative Q-Improvement

Conservative Q-Improvement is a reinforcement learning method that builds a decision tree as the policy.  Nodes represent abstract states, with leaves corresponding to actions to execute.

#### C++ Dependencies and other information:
To build the C++ wrapper and import it as a Python module, you will need to run `./cython_build_wrapper.sh` in the `cqi_cpp/src/wrapper` directory. You will need to install the system package Cython to build the module.

Run `python cqi_test.py` to see CQI try to solve the environment specified in the file. You can pass in command line
arguments as well to try different hyperparameters. You will need to install the Python package box2d to run the given LunarLander example.

#### Python Dependencies:

* [Ray](https://github.com/ray-project/ray)

## More Information

For a detailed explanation of CQI, please see [this paper](https://arxiv.org/abs/1907.01180)
