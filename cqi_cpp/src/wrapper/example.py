import argparse 
import gym
import math

from qtree import QTree
from py_train import Train

env = gym.make('LunarLander-v2')
env = gym.make('CartPole-v0')

def truncate(number, digits):
    stepper = 10.0 ** digits
    return math.trunc(stepper * number) / stepper

# Optional command line args
parser = argparse.ArgumentParser()
parser.add_argument("--gamma") 
parser.add_argument("--alpha")
parser.add_argument("--visit_decay")
parser.add_argument("--split_thresh_max")
parser.add_argument("--split_thresh_decay")
parser.add_argument("--num_splits")
parser.add_argument("--grid_search")
parser.add_argument("--steps")
args = parser.parse_args()

gamma = float(args.gamma) if args.gamma else 0.99
alpha = float(args.alpha) if args.alpha else 0.01
visit_decay = float(args.visit_decay) if args.visit_decay else 0.999
split_thresh_max = float(args.split_thresh_max) if args.split_thresh_max else 0.1
split_thresh_decay = float(args.split_thresh_decay) if args.split_thresh_decay else 0.99
num_splits = int(args.num_splits) if args.num_splits else 2
grid_search = bool(args.grid_search) if args.grid_search else False

qfunc = QTree(env.observation_space, env.action_space, None,
                  # Hyperparameters
                  gamma,
                  alpha,
                  visit_decay,
                  split_thresh_max,
                  split_thresh_decay,
                  num_splits)
t = Train(qfunc, env)

eps_func = (lambda step: max(0.05, 1 - step/1e5))

train_steps = int(args.steps) if args.steps else int(5e4)

# normal execution:
# Training
history = t.train(train_steps, eps_func, verbose=True, qfunc_hist=None)

# Evaluation:
results, history2, avg_r_per_ep, _ = t.train(50000, lambda step: 0.05, verbose=True, eval_only=True, penalty_check=lambda s, r: r <= -1000, track_data_per=1, run_tag="some descriptive tag for logging")

# qfunc.print_structure()
nodes = f"\nNumber of nodes: {qfunc.num_nodes()}\n" 
reward = f"\nAverage reward per episode: {truncate(avg_r_per_ep, 3)}\n"
hparams_str = f"gamma={gamma}, alpha={alpha}, visit_decay={visit_decay}," 
hparams_str += f"split_thresh_max={split_thresh_max}," 

nodes = f"\nNumber of nodes: {qfunc.num_nodes()}\n"
reward = f"\nAverage reward per episode: {truncate(avg_r_per_ep, 3)}\n"
hparams_str = f"gamma={gamma}, alpha={alpha}, visit_decay={visit_decay},"
hparams_str += f"split_thresh_max={split_thresh_max},"
hparams_str += f"split_thresh_decay={split_thresh_decay}, num_splits={num_splits}"

if grid_search:
    with open("grid_search_results.txt", "a") as myfile:
        myfile.write(nodes + reward + hparams_str)

    myfile.close()
else:
    print(nodes + reward + hparams_str)

