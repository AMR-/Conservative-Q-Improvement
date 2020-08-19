# Conservative Q-Improvement

Conservative Q-Improvement is a reinforcement learning method that builds a decision tree as the policy.  Nodes represent abstract states, with leaves corresponding to actions to execute.

## Installation

CQI currently only works with Python 3.6 and is hosted on TestPyPI. To install it, first install `ray` and `Cython` using `pip` and then run

    pip install -i https://test.pypi.org/simple/ cqi-rl

## More Information

For a detailed explanation of CQI, please see [this paper](https://arxiv.org/abs/1907.01180)

## Usage

CQI is designed for environments that are written according to [OpenAI Gym](https://gym.openai.com/) specifications. 
Here is a small example of initializing a QTree for training on CartPole. Note that you must have `gym` installed to run the example.

    import gym
    
    from cqi_cpp.src.wrapper.qtree_wrapper import PyBox as Box
    from cqi_cpp.src.wrapper.qtree_wrapper import PyDiscrete as Discrete
    from cqi_cpp.src.wrapper.qtree_wrapper import PyQTree as QTree
    from cqi_cpp.src.wrapper.qtree_wrapper import PyVector as Vector
    
    def convert_to_pybox(b):
        low = Vector()
        high = Vector()

        for i in b.low: low.add(i)

        for i in b.high: high.add(i)

        return Box(low, high)

    env = gym.make('CartPole-v0')
    discrete = Discrete(env.action_space.n)
    
    box = convert_to_pybox(env.observation_space)
    
    qfunc = QTree(box, discrete, None, 
            gamma=0.99, 
            alpha=0.01, 
            visit_decay=0.999, 
            split_thresh_max=10, 
            split_thresh_decay=0.99, 
            num_splits=3)
            
    qfunc.print_structure()


You should see this as the output.

    └ (vis: 1.00) qvals: [0, 0]
 
## Building From Repo
To build the C++ wrapper and import it as a Python module, you will need to run `./cython_build_wrapper.sh` in the `cqi_cpp/src/wrapper` directory. You will need to install the system package Cython to build the module.

Run `python cqi_test.py` to see CQI try to solve the environment specified in the file. You can pass in command line
arguments as well to try different hyperparameters. You will need to install the Python package `box2d` to run the given LunarLander example.

#### Python Dependencies:

* Cython

* gym

* [Ray](https://github.com/ray-project/ray)

## Citation Information

Please cite using the following bibtex:

```
@inproceedings(roth19cqi,
    title={Conservative Q-Improvement: Reinforcement Learning for an
Interpretable Decision-Tree Policy},
    author="Aaron M. Roth and Nicholay Topin and Pooyan Jamshidi and Manuela Veloso",
    year={2019},
    booktitle={arXiv 1907.01180}
}
```

<!-- > Aaron M. Roth, Nicholay Topin, Pooyan Jamshidi, and Manuela Veloso.  Conservative Q-improvement:  Reinforcement Learning for an Interpretable Decision-Tree Policy.  In _arXiv 1907.01180_, 2019. -->
