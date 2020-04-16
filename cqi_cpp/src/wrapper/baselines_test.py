import argparse
import gym
import numpy as np

# from stable_baselines.common.vec_env import DummyVecEnv # Needed for PPO2

def evaluate(model, num_steps=1000):
  """
  Evaluate an RL agent
  :param model: (BaseRLModel object) the RL Agent
  :param num_steps: (int) number of timesteps to evaluate it
  :return: (float) Mean reward for the last 100 episodes
  """
  episode_rewards = [0.0]
  obs = env.reset()
  for i in range(num_steps):
      # _states are only useful when using LSTM policies
      action, _states = model.predict(obs)

      obs, reward, done, info = env.step(action)
      
      # Stats
      episode_rewards[-1] += reward
      if done:
          obs = env.reset()
          episode_rewards.append(0.0)
  # Compute mean reward for the last 100 episodes
  mean_100ep_reward = round(np.mean(episode_rewards[-100:]), 1)
  
  print(f"Mean reward: {mean_100ep_reward}, Num episodes: {len(episode_rewards)}")
  
  return mean_100ep_reward

parser = argparse.ArgumentParser()
parser.add_argument("--algorithm") 
args = parser.parse_args()

algorithm = args.algorithm 

# PPO1 gets mean reward = 206.6 with 42 episodes and 1e7 total_timesteps on LLC-v2
env = gym.make('LunarLanderContinuous-v2')

if algorithm == "ppo1":
    from stable_baselines import PPO1
    from stable_baselines.common.policies import MlpPolicy
    
    model = PPO1(MlpPolicy, env, verbose=1)
else:
    from stable_baselines import DQN
    from stable_baselines.deepq.policies import MlpPolicy

    model = DQN(MlpPolicy, env, verbose=1)

model.learn(total_timesteps=int(1e7), log_interval=10)
model.save(f"{algorithm}_cartpole")

del model # remove to demonstrate saving and loading

if algorithm == "ppo1":
    model = PPO1.load(f"{algorithm}_cartpole")
else:
    model = DQN.load(f"{algorithm}_cartpole")

mean_reward = evaluate(model, num_steps=10000)
