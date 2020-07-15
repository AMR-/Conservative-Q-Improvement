import argparse 
import gym
import math

from qtree_wrapper import PyBox as Box
from qtree_wrapper import PyDiscrete as Discrete
from qtree_wrapper import PyQTree as QTree
from qtree_wrapper import PyVector as Vector

from RobotNavigation import RobotNavEnv
from VehicleIntersection import VehicleIntersection

from train import Train

from utils import convert_to_pybox

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
parser.add_argument("--cql")
parser.add_argument("--epsilon")
parser.add_argument("--env")
args = parser.parse_args()

gamma = float(args.gamma) if args.gamma else 0.99
alpha = float(args.alpha) if args.alpha else 0.01
visit_decay = float(args.visit_decay) if args.visit_decay else 0.999
split_thresh_max = float(args.split_thresh_max) if args.split_thresh_max else 10
split_thresh_decay = float(args.split_thresh_decay) if args.split_thresh_decay else 0.99
num_splits = int(args.num_splits) if args.num_splits else 3
grid_search = bool(args.grid_search) if args.grid_search else False
cql = int(args.cql) if args.cql else 0
epsilon = float(args.epsilon) if args.epsilon else 0.05
env_str = args.env if args.env else 'cp'

if env_str == 'cp':
    env = gym.make('CartPole-v0')
    continuous = False
    discrete = Discrete(env.action_space.n)
elif env_str == 'llc':
    env = gym.make('LunarLanderContinuous-v2')
    continuous = True
    discrete = Discrete(4)
elif env_str == 'rn':
    env = RobotNavEnv()
    continuous = False
    discrete = Discrete(env.action_space.n)
elif env_str == 'vi':
    env = VehicleIntersection()
    continuous = False
    discrete = Discrete(env.action_space.n)

def truncate(number, digits):
    stepper = 10.0 ** digits
    return math.trunc(stepper * number) / stepper

box = convert_to_pybox(env.observation_space)

qfunc = QTree(box, discrete, None,
        # Hyperparameters
        gamma,
        alpha,
        visit_decay,
        split_thresh_max,
        split_thresh_decay,
        num_splits,
        cql)

t = Train(qfunc, env, continuous, cql) # Added continuous, cql flags

eps_func = (lambda step: max(epsilon, 1 - step/1e5)) 
train_steps = int(args.steps) if args.steps else int(2e7)

# Training
history = t.train(train_steps, eps_func, verbose=True, qfunc_hist=None)

# Evaluation:
results, history2, avg_r_per_ep, _ = t.train(50000, lambda step: 0.05, \
        verbose=True, eval_only=True, penalty_check=lambda s, r: r <= -1000, \
        track_data_per=1, run_tag="some descriptive tag for logging")

qfunc.print_structure()
nodes = f"\nNumber of nodes: {qfunc.num_nodes()}\n" 
reward = f"\nAverage reward per episode: {truncate(avg_r_per_ep, 3)}\n"
hparams_str = f"gamma={gamma}, alpha={alpha}, visit_decay={visit_decay}, " 
hparams_str += f"split_thresh_max={split_thresh_max}, " 
hparams_str += f"split_thresh_decay={split_thresh_decay}, num_splits={num_splits}"

if grid_search:
    with open("grid_search_results.txt", "a") as myfile:
        myfile.write(nodes + reward + hparams_str)

    myfile.close()
else:
    print(nodes + reward + hparams_str)

