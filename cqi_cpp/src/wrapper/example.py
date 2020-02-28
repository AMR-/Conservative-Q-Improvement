import gym

from qtree import QTree
from py_train import Train

env = gym.make('MountainCar-v0')

qfunc = QTree(env.observation_space, env.action_space,
                  # Hyperparameters
                  gamma=0.99,
                  alpha=0.01,
                  visit_decay=0.999,
                  split_thresh_max=0.1,
                  split_thresh_decay=0.99)
t = Train(qfunc, env)

eps_func = (lambda step: max(0.05, 1 - step/1e5))
train_steps = int(5e3)

# normal execution:
# Training
history = t.train(train_steps, eps_func, verbose=True, qfunc_hist=None)

# Evaluation:
results, history2, avg_r_per_ep, _ = t.train(50000, lambda step: 0.05, verbose=True, eval_only=True, penalty_check=lambda s, r: r <= -1000, track_data_per=1, run_tag="some descriptive tag for logging")

# qfunc.print_structure()
nn = qfunc.num_nodes()
print("Number of nodes: %d, %d" % (nn, qfunc.num_nodes()))
print("Average reward per episode: %f" % avg_r_per_ep)
