import gym
from gym import spaces
import numpy as np
from numpy import random
from numpy.linalg import norm
import PIL.Image
import PIL.ImageDraw
from math import pi
from math import atan2


# This environment was implemented by Nicholay Topin and Aaron M. Roth


class RobotNavEnv(gym.Env):
        def __init__(self, max_side=10, goal_rad=1, hole_rad=2,
                     random_hole_pos=True, is_state_cart=True, include_stage_boolean=True):
                self.include_stage = include_stage_boolean
                self.max_side = max_side
                if is_state_cart:
                        if include_stage_boolean:
                                high = np.ones(5)*self.max_side
                                high[4] = 1
                                self.observation_space = spaces.Box(0*high, high, dtype=np.float32)
                        else:
                                high = np.ones(4)*self.max_side
                                self.observation_space = spaces.Box(0*high, high, dtype=np.float32)
                        self.is_state_cart = True
                else:
                        # noinspection PyRedundantParentheses
                        max_dist = max_side * (2)**(1/2)
                        max_angle = pi
                        if include_stage_boolean:
                                high = np.array([max_angle, max_dist,
                                                 max_angle, max_dist, 1])
                                low = np.array([-max_angle, 0,
                                                -max_angle, 0, 0])
                        else:
                                high = np.array([max_angle, max_dist,
                                                 max_angle, max_dist])
                                low = np.array([-max_angle, 0,
                                                -max_angle, 0])

                        self.observation_space = spaces.Box(low, high, dtype=np.float32)
                        self.is_state_cart = False
                self.action_space = spaces.Discrete(3)
                # positions and sizes of objects in domain
                self.goal_pos = np.array([self.max_side*0.9, self.max_side*0.9])
                self.agent_pos = np.ones(2)
                self.hole_pos = np.ones(2)
                self.goal_rad = goal_rad
                self.hole_rad = hole_rad
                self.random_hole_pos = random_hole_pos

        def step(self, action):
                if action == 0:  # toward goal
                        a_to_g_vec = self.goal_pos - self.agent_pos
                        self.agent_pos += a_to_g_vec/norm(a_to_g_vec)
                elif action == 1:  # left perp
                        non_normed = np.flip(self.goal_pos - self.agent_pos, 0)
                        self.agent_pos += non_normed/norm(non_normed) \
                            * np.array([1, -1])
                elif action == 2:  # right perp
                        non_normed = np.flip(self.goal_pos - self.agent_pos, 0)
                        self.agent_pos += non_normed/norm(non_normed) \
                            * np.array([-1, 1])
                else:
                        print('illegal action')
                        return None, None, True, {}
                done = False
                reward = -1
                if self._is_agent_in_X(self.hole_pos, self.hole_rad) \
                   or (self.agent_pos > self.max_side).any() \
                   or (self.agent_pos < 0).any():
                        done = True
                        reward = -100
                elif norm(self.agent_pos - self.goal_pos) < self.goal_rad:
                        done = True
                        reward = 0
                return self._get_state(), reward, done, {}

        @staticmethod
        def angle_between(vec1, vec2):
                dot = vec1[0]*vec2[0] + vec1[1]*vec2[1]
                det = vec1[0]*vec2[1] - vec1[1]*vec2[0]
                return atan2(det, dot)

        def _is_robot_past_obstacle(self):
                dist_agent_to_goal = norm(self.agent_pos - self.goal_pos)
                dist_obstacle_to_goal = norm(self.hole_pos - self.goal_pos)
                return 1 if dist_agent_to_goal < dist_obstacle_to_goal else 0

        def _get_state(self):
                if self.is_state_cart:
                        # return np.concatenate([self.agent_pos, self.hole_pos, self._is_robot_past_obstacle()])
                        if self.include_stage:
                                return np.array([self.agent_pos[0], self.agent_pos[1],
                                                 self.hole_pos[0], self.hole_pos[1], self._is_robot_past_obstacle()])
                        else:
                                return np.array([self.agent_pos[0], self.agent_pos[1],
                                                 self.hole_pos[0], self.hole_pos[1]])
                else:
                        if self.include_stage:
                                return np.array([
                                        self.angle_between([1, 0], (self.goal_pos - self.agent_pos)),
                                        norm(self.agent_pos - self.goal_pos),  # dist between agent and goal
                                        self.angle_between([1, 0], (self.hole_pos - self.agent_pos)),
                                        norm(self.agent_pos - self.hole_pos),  # dist between agent and hole
                                        self._is_robot_past_obstacle()
                                        ])
                        else:
                                return np.array([
                                        self.angle_between([1, 0], (self.goal_pos - self.agent_pos)),
                                        norm(self.agent_pos - self.goal_pos),  # dist between agent and goal
                                        self.angle_between([1, 0], (self.hole_pos - self.agent_pos)),
                                        norm(self.agent_pos - self.hole_pos),  # dist between agent and hole
                                ])

        def reset(self):
                if self.random_hole_pos:
                        self.hole_pos = (random.random(2)*0.3 + 0.3)\
                                        * self.max_side
                else:
                        self.hole_pos = np.ones(2)*0.45*self.max_side
                self.agent_pos = (random.random(2)*0.2)*self.max_side
                if self._is_agent_in_X(self.hole_pos, self.hole_rad):
                        self.reset()
                return self._get_state()

        # noinspection PyPep8Naming
        def _is_agent_in_X(self, X_pos, X_rad):
                return norm(self.agent_pos - X_pos) < X_rad

        def draw_state_to_file(self, filename, image_dim=200):
                im = self.draw_state(image_dim)
                im.save(filename, 'PNG')

        def draw_state(self, image_dim=200):
                agent_rad = 0.3
                unit_len = float(image_dim)/(self.max_side + 2*agent_rad)
                im = PIL.Image.new('RGB',
                                   [image_dim, image_dim], color='black')
                draw = PIL.ImageDraw.Draw(im)
                self.draw_ellipse(draw, unit_len*self.goal_pos,
                                  unit_len*(self.goal_rad - agent_rad),
                                  unit_len*agent_rad,
                                  fill='Lime' if self._is_agent_in_X(self.goal_pos, self.goal_rad)
                                  else 'Green')
                self.draw_ellipse(draw, unit_len*self.hole_pos,
                                  unit_len*(self.hole_rad - agent_rad),
                                  unit_len*agent_rad,
                                  fill='Red' if self._is_agent_in_X(self.hole_pos, self.hole_rad)
                                  else 'Maroon')
                self.draw_ellipse(draw, unit_len*self.agent_pos,
                                  unit_len*agent_rad,
                                  unit_len*agent_rad, fill='Yellow')
                return im

        @staticmethod
        def draw_ellipse(obj, loc, rad, agent_rad, fill):
                obj.ellipse([loc[0] - rad + agent_rad,
                             loc[1] - rad + agent_rad,
                             loc[0] + rad + agent_rad,
                             loc[1] + rad + agent_rad], fill=fill)

        def render(self, mode='human'):
                return self.draw_state()
