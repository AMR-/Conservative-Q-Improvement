# Conservative Q-Improvement

Conservative Q-Improvement is a reinforcement learning method that builds a decision tree as the policy.  Nodes represent abstract states, with leaves corresponding to actions to execute.


## Installation

## More Information

For a detailed explanation of CQI, please see [this paper](https://arxiv.org/abs/1907.01180)

## Usage

CQI is designed for environments that are written according to [OpenAI Gym](https://gym.openai.com/) specifications. Here is a small example of using the library on CartPole.

    import gym
    
    from qtree_wrapper import PyBox as Box
    from qtree_wrapper import PyDiscrete as Discrete
    from qtree_wrapper import PyQTree as QTree
    from qtree_wrapper import PyVector as Vector
    
    from train import Train
    
    from utils import convert_to_pybox
    
    env = gym.make('CartPole-v0')
    discrete = Discrete(env.action_space.n)
    
    box = convert_to_pybox(env.observation_space)
    
    qfunc = QTree(box, discrete, None, 
            gamma=0.99, 
            alpha=0.01, 
            visit_decay=0.999, 
            split_thresh_max=10, 
            split_thresh_decay=0.99, 
            num_splits=3, 
            cql=0)
    
    t = Train(qfunc, env, continuous=False, cql=0)
    
    train_steps=int(1e6)
    
    eps_func = (lambda step: max(0.05, 1 - step/1e5))
    
    # Training
    history = t.train(train_steps, eps_func, verbose=True, qfunc_hist=None)
     
    # Evaluation
    results, history2, avg_r_per_ep, _ = t.train(50000, lambda step: 0.05, \
    verbose=True, eval_only=True, penalty_check=lambda s, r: r <= -1000, \
    track_data_per=1, run_tag="some descriptive tag for logging")
 
    qfunc.print_structure()
    
    nodes = f"\nNumber of nodes: {qfunc.num_nodes()}\n"
    reward = f"\nAverage reward per episode: {avg_r_per_ep}\n"
    
    print(nodes, reward)
    
Note that you need to build the library in the right directory as explained earlier in this Readme. 

## Building From Repo

#### Python Dependencies:

* [Ray](https://github.com/ray-project/ray)

#### C++ Dependencies and other information:
To build the C++ wrapper and import it as a Python module, you will need to run `./cython_build_wrapper.sh` in the `cqi_cpp/src/wrapper` directory. You will need to install the system package Cython to build the module.

Run `python cqi_test.py` to see CQI try to solve the environment specified in the file. You can pass in command line
arguments as well to try different hyperparameters. You will need to install the Python package box2d to run the given LunarLander example.

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
