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

    from collections import defaultdict
    import gym
    import numpy as np

    from cqi_cpp.src.wrapper.qtree_wrapper import PyBox as Box
    from cqi_cpp.src.wrapper.qtree_wrapper import PyDiscrete as Discrete
    from cqi_cpp.src.wrapper.qtree_wrapper import PyQTree as QTree
    from cqi_cpp.src.wrapper.qtree_wrapper import PyVector as Vector
    from cqi_cpp.src.wrapper.qtree_wrapper import PyState as State
    from cqi_cpp.src.wrapper.qtree_wrapper import PyAction as Action

    class Train(object):
        def __init__(self, qfunc, gym_env):
            self.qfunc = qfunc
            self.env = gym_env

        def train(self, num_steps, eps_func, eval_only=False, penalty_check=lambda s, r: 0,
                  track_data_per=0, qfunc_hist=None, qfunc_hist_directory=None,
                  qfunc_hist_per_every_nn=1):
            if eval_only:
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
                    if eval_only and step > 0:
                        r_per_ep.append(ep_r)
                        pen_per_ep.append(penalty_check(s, ep_r))
                        ts = step - last_step_ep
                        ts_per_ep.append(ts)
                        last_step_ep = step
                    if track_data_per > 0 and num_eps % track_data_per == 0:
                        hist[self.qfunc.num_nodes()].append(ep_r)
                        num_nodes = self.qfunc.num_nodes()
                    s = self.env.reset()
                    ep_r = 0
                    num_eps = num_eps + 1
                    done = False
                if np.random.random() < eps_func(step):
                    a = self.env.action_space.sample()
                else:
                    s = convert_to_pystate(s)
                    a = self.qfunc.select_a(s)
                s2, r, done, _ = self.env.step(a)
                if not eval_only:
                    s, s2 = convert_to_pystate(s), convert_to_pystate(s2)
                    a = Action(a)

                    self.qfunc.take_tuple(s, a, r, s2, done)
                    if qfunc_hist is not None and self.qfunc.just_split():
                        qfunc_hist.append(self.qfunc.get_pre_split())
                s = s2
                ep_r += r
            if eval_only:
                avg_r_per_ep = np.mean(r_per_ep)
                return hist, avg_r_per_ep, (single_change_features, istates, fstates)
            else:
                return hist

    def convert_to_pybox(b):
        low = Vector()
        high = Vector()

        for i in b.low: low.add(i)

        for i in b.high: high.add(i)

        return Box(low, high)

    def convert_to_pystate(s):
        if type(s) is State: return s

        v = Vector()

        for i in s: v.add(i)

        return State(v)

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

    t = Train(qfunc, env)

    eps_func = (lambda step: max(0.05, 1 - step/1e5))

    # Training
    t.train(1000, eps_func, qfunc_hist=None)

    # Evaluation:
    t.train(500, lambda step: 0.05, eval_only=True, penalty_check=lambda s, r: r <= -1000, track_data_per=1)

    qfunc.print_structure()
   
You should see this as the final tree.
    
    └ (vis: 0.30) if f[3] ? 0.000000:
      ├< (vis: 0.51) qvals: [3.98075, 4.19054]
      └> (vis: 0.49) qvals: [3.98075, 4.19054]
      
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
