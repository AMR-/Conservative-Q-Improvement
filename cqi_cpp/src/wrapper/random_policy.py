import gym
import numpy as np

env = gym.make('LunarLander-v2')
env.reset()
train_steps = int(1e4)
reward_per_ep = []

for _ in range(train_steps):
    _, r, _, _ =  env.step(env.action_space.sample()) # take a random action
    reward_per_ep.append(r)
env.close()

average_reward_per_ep = np.mean(reward_per_ep) 

print(f"Average reward: {average_reward_per_ep}")
