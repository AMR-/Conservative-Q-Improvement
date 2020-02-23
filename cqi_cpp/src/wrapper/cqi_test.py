import gym

from qtree_wrapper import PyBox as Box
from qtree_wrapper import PyDiscrete as Discrete
from qtree_wrapper import PyQTree as QTree
from qtree_wrapper import PyVector as Vector

from train import Train

from utils import convert_to_pybox

env = gym.make('CartPole-v0')

# Wrapping env's spaces
# low = Vector()
# high = Vector()

# for i in env.observation_space.low:
#     low.add(i)

# for i in env.observation_space.high:
#     high.add(i)

box = convert_to_pybox(env.observation_space)
discrete = Discrete(env.action_space.n)
qfunc = QTree(box, discrete, None)  

t = Train(qfunc, env)

eps_func = (lambda step: max(0.05, 1 - step/1e5))
train_steps = int(5e5)

# Training
history = t.train(train_steps, eps_func, verbose=True, qfunc_hist=None)

# Evaluation:
results, history2, avg_r_per_ep, _ = t.train(50000, lambda step: 0.05, \
        verbose=True, eval_only=True, penalty_check=lambda s, r: r <= -1000, \
        track_data_per=1, run_tag="some descriptive tag for logging")

qfunc.print_structure()
print(f"Number of nodes: {qfunc.num_nodes()}") 
print(f"Average reward per episode: {avg_r_per_ep}")

