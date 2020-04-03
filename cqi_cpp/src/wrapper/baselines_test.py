import argparse
import gym
import numpy as np

# from stable_baselines.common.vec_env import DummyVecEnv # Needed for PPO2

# TODO: add more flags for environment and hparams, rewrite grid_search.sh 

def evaluate(model, num_steps=1000):
  """
  Evaluate a RL agent
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
  
  # print(f"Mean reward: {mean_100ep_reward}, Num episodes: {len(episode_rewards)}")
  
  return mean_100ep_reward

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--algorithm") 
    parser.add_argument("--env")
    parser.add_argument("--steps")
    parser.add_argument("--alpha")
    parser.add_argument("--grid_search")
    args = parser.parse_args()

    algorithm = args.algorithm 
    env = gym.make(args.env)
    grid_search = args.grid_search
    alpha = args.alpha

    if algorithm == "ppo1":
        from stable_baselines import PPO1
        from stable_baselines.common.policies import MlpPolicy
        
        model = PPO1(MlpPolicy, env, verbose=1)
    else:
        from stable_baselines import DQN
        from stable_baselines.deepq.policies import MlpPolicy

        model = DQN(MlpPolicy, env, learning_rate=alpha, verbose=1)

    model.learn(total_timesteps=int(args.steps), log_interval=10)
    model.save(f"{algorithm}_cartpole")

    del model # remove to demonstrate saving and loading

    if algorithm == "ppo1":
        model = PPO1.load(f"{algorithm}_cartpole")
    else:
        model = DQN.load(f"{algorithm}_cartpole")

    mean_reward = evaluate(model, env, num_steps=10000)
    
    hparams_str = f" algorithm={algorithm} env={args.env} steps={args.steps} alpha={alpha}"

    if grid_search:
        with open("grid_search_results.txt", "a") as myfile:
            myfile.write(str(mean_reward) + hparams_str)

        myfile.close()
    else:
        print(str(mean_reward) + hparams_str)

if __name__ == '__main__': 
    main()

  print(f"Mean reward: {mean_100ep_reward}, Num episodes: {len(episode_rewards)}")
  
  return mean_100ep_reward

parser = argparse.ArgumentParser()
parser.add_argument("--algorithm") 
args = parser.parse_args()

algorithm = args.algorithm 

env = gym.make('CartPole-v0')

if algorithm == "ppo1":
    from stable_baselines import PPO1
    from stable_baselines.common.policies import MlpPolicy
    
    model = PPO1(MlpPolicy, env, verbose=1)
else:
    from stable_baselines import DQN
    from stable_baselines.deepq.policies import MlpPolicy

    model = DQN(MlpPolicy, env, verbose=1)

model.learn(total_timesteps=int(2e4), log_interval=10)
model.save(f"{algorithm}_cartpole")

del model # remove to demonstrate saving and loading

if algorithm == "ppo1":
    model = PPO1.load(f"{algorithm}_cartpole")
else:
    model = DQN.load(f"{algorithm}_cartpole")

mean_reward = evaluate(model, num_steps=10000)
