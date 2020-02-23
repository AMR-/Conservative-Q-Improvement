import gym

from stable_baselines.common.vec_env import DummyVecEnv
from stable_baselines.deepq.policies import MlpPolicy
# from stable_baselines.common.policies import MlpPolicy
from stable_baselines import PPO1, DQN

env = gym.make('CartPole-v1')

model = DQN(MlpPolicy, env, verbose=1)
# model = PPO1(MlpPolicy, env, verbose=1)
model.learn(total_timesteps=250)
# model.save("ppo1_cartpole")
model.save("deepq_cartpole")

del model # remove to demonstrate saving and loading

# model = PPO1.load("ppo1_cartpole")
model = DQN.load("deepq_cartpole")

obs = env.reset()
while True:
    action, _states = model.predict(obs)
    obs, rewards, done, info = env.step(action)
    env.render()
