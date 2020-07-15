# Vehicle Intersection
# Created by Aaron M. Roth
# https://github.com/AMR-/gym_vehicle_intersection
# MIT License

# noinspection PyPep8Naming
from VehicleIntersectionSupport import TurnSignals as TS, Lights as L, VehicleIntersectionActionSet, \
    VehicleIntersectionRenderer, delete_by_idx, Lanes, excluded_vehicle_colors
# noinspection PyUnresolvedReferences
from VehicleIntersectionActionSets import ToggleRoad, ToggleLight, SetByRoad, SetByLight
from gym.utils.colorize import color2num as valid_highlight_colors
from collections import OrderedDict
from bitarray import bitarray as b
from random import randint, choice as rand_choice
from numpy.random import choice as wrand_choice
from gym import spaces
import numpy as np
import gym


class StateInformation(object):
    """
    The information that changes over time is
    - vehicles remaining
    - vehicles present on map
    - list of vehicles on map
        each element in list is a list:
            [lane, position, length, turn]
            lane - 0-3 (for Left, Up, Right, Down respectively)
            position - 0 at end of intersection, increasing numbers (with intersection length 2, road 20, range is 0-21)
            length - length of vehicle
            turn - -1 left, 0 straight, 1 right
        elements that have 0 length indicate no vehicle
    - lights
    """
    def __init__(self, road_length, vehicle_types, lights_init, turn_signals, max_vehicles_on_map):
        self._vehicle_types = vehicle_types
        self._turn_signals = turn_signals
        self.intersection_length = 2
        self.intersection_turn_lane_ends = self.intersection_length - 1  # where turns end
        self.max_vehicles_on_map = max_vehicles_on_map
        self.vector_length = self.max_vehicles_on_map*4 + 4
        self._renderer = VehicleIntersectionRenderer(road_length, self.intersection_length)

        # Changing information
        self.collision = False
        # Vehicles
        self.vehicles = []      # [  [ (L/U/R/D), front position, length, turn s ],  [ L, 0, 0, 0 ] ]
        self.vcolors = []
        # The var on the next line refers to the lane into which the vehicle is turning, not vehicle's original lane
        self.lanes_with_left_turns_in_progress = b('0000')
        # Lights - L, U, R, D
        self.lights = b(list(lights_init))

        # Beginning and ending of intersection along the lines, inclusive
        self.intersection_zone = (road_length+1, road_length+self.intersection_length)

    def reorder_vehicles_by_nearest_to_intersection(self):
        num_v = len(self.vehicles)
        if num_v > 1:
            for v in range(num_v-1):
                if self.vehicles[v][1] > self.vehicles[v+1][1]:
                    self.vehicles[v], self.vehicles[v+1] = self.vehicles[v+1], self.vehicles[v]
                    self.vcolors[v], self.vcolors[v+1] = self.vcolors[v+1], self.vcolors[v]

    def note_left_turn_from(self, v_num, lane):
        self.lanes_with_left_turns_in_progress[Lanes.opposite(lane)] = 1
        self._renderer.set_left_turn_color(Lanes.opposite(lane), self.vcolors[v_num])

    def render(self, mode):
        return self._renderer.render(self.lights, self.vehicles, self.vcolors,
                                     self.lanes_with_left_turns_in_progress, mode)

    def to_vector(self):
        vector = np.zeros(self.vector_length, dtype=int)
        # noinspection PyTypeChecker
        vector[0:4] = [1 if light else 0 for light in self.lights]  # attach lights
        num_v = len(self.vehicles)
        for v in range(num_v):
            vector[v*4+4:v*4+8] = self.vehicles[v]
        for v in range(num_v, self.max_vehicles_on_map):
            vector[v*4+4:v*4+8] = [0, 0, 0, 0]
        return vector

    # Note that since state does include color information, loading from vector chooses random colors
    def from_vector(self, vector):
        self.lights = b(vector[0:4])
        self.vehicles = []
        # noinspection PyTypeChecker
        for v in range(int((len(vector)/4)-1)):
            vinfo = vector[v*4+4:v*4+8]
            if vinfo[2] == 0:
                break
            else:
                self.vehicles.append(list(vinfo))
        self.vcolors = [self._rand_color(excluded_vehicle_colors)] * len(self.vehicles)

    @staticmethod
    def _rand_color(excluded_colors):
        color = ""
        while color in excluded_colors:
            color = rand_choice(list(valid_highlight_colors.keys()))
        return color


class VehicleIntersection(gym.Env):
    def __init__(self,
                 action_set=ToggleRoad(),  # Action Set to use
                 road_length=14,  # Road length BEFORE intersection
                 use_turn_signals=True,  # True to allow vehicles to signal turns, False for always going straight
                 turn_signal_dist=(0.33, 0.34, 0.33),  # probability of left, straight, right signals if used
                 max_vehicles_on_map=8,  # when this limit is reached, spawn no more vehicles
                                         # note that this value affects the size of the observation space
                 # vehicle_types is an OrderedDict of vehicle info, each pair: ("name" -> ("length min", "length max"))
                 vehicle_types=OrderedDict([("car", (4, 4)), ("van", (6, 6)), ("truck", (8, 12))]),
                 vehicle_dist=(0.4, 0.35, 0.25),  # probability of each vehicle spawning
                 vehicle_colors=OrderedDict([("car", "yellow"), ("van", "magenta"), ("truck", "cyan")]),
                 vehicle_speed=1,  # distance an unimpeded vehicle moves each timestep
                 spawn_interval=(3, 7),  # Min/Max time to wait between attempting to spawn a vehicle
                 vehicles_to_spawn_per_episode=10,  # Number of vehicles to spawn
                 lights_init=(L.Green, L.Green, L.Green, L.Green),  # Initial state of Lights - L, U, R, D
                 waiting_penalty=-1,
                 collision_penalty=-1000,
                 verbose=False
                 ):
        # Environment Attributes
        self.waiting_penalty = waiting_penalty
        self.collision_penalty = collision_penalty
        self._actionset_info = action_set
        self._num_lights = 4
        self._road_length = road_length
        self._lights_init = lights_init
        self._use_turn_signals = use_turn_signals
        self._turn_signals = [TS.STRAIGHT, TS.LEFT, TS.RIGHT] if self._use_turn_signals else [TS.STRAIGHT]
        self._turn_signal_dist = turn_signal_dist
        self._num_turn_signals = len(self._turn_signals)
        self._vehicle_types = vehicle_types
        self._num_vehicles = len(vehicle_types)
        self._vehicles_to_spawn_per_episode = vehicles_to_spawn_per_episode
        self._vehicles_remaining_to_spawn = self._vehicles_to_spawn_per_episode
        self._vehicle_dist = vehicle_dist
        self._vehicle_colors = vehicle_colors
        self._vehicle_speed = vehicle_speed
        self.max_vlength = self._find_max_vehicle_length()
        self._max_vehicles_on_map = max_vehicles_on_map
        self._spawn_interval = spawn_interval
        self._time_to_next_spawn = 0
        self._verbose = verbose
        self._validate()
        # State
        self.state_info = self._get_state_info()
        # Setup Action and Observation Spaces
        self.observation_space = self._get_observation_space()
        self.action_space = self._actionset_info.get_action_space()

    '''
    Please note that lights update after vehicles move!
    This simulates simultaneous action.
    '''
    def step(self, action):
        reward = self._step_move_vehicles()
        self._step_spawn_vehicles()
        self._step_update_lights(action)
        state, done, info = self._step_get_return_vals()
        return state, reward, done, info

    def reset(self):
        self.state_info = self._get_state_info()
        self._vehicles_remaining_to_spawn = self._vehicles_to_spawn_per_episode
        self._time_to_next_spawn = self._get_time_to_next_spawn()
        return self._get_obs()

    def render(self, mode='human'):
        return self.state_info.render(mode)

    def _find_max_vehicle_length(self):
        max_vtype = max(self._vehicle_types.keys(), key=(lambda key: self._vehicle_types[key][1]))
        return self._vehicle_types[max_vtype][1]     # maximum vehicle length

    def _validate(self):
        if not isinstance(self._actionset_info, VehicleIntersectionActionSet):
            raise ValueError("Please pass an action space description that extends VehicleIntersectionActionSet")
        if self._road_length <= self.max_vlength:
            raise ValueError("Road length must be longer than maximum vehicle length.")
        if not (len(self._lights_init) == self._num_lights):
            raise ValueError("Please specify exactly %s initial lights." % self._num_lights)
        if self._num_vehicles < 1:
            raise ValueError("There must be at least one vehicle specified.")
        for k, v in self._vehicle_types.items():
            if v[0] > v[1]:
                raise ValueError("Minimum size for vehicle %s is greater than maximum." % k)
        if sum(self._vehicle_dist) != 1:
            raise ValueError("The vehicle distribution must sum to 1.")
        if self._vehicle_speed < 1:
            raise ValueError("Vehicle speed must be an integer greater than 0.")
        if self._spawn_interval[0] < 1:
            raise ValueError("Minimum spawn interval must be greater than 0.")
        for color in self._vehicle_colors.values():
            if color not in valid_highlight_colors.keys():
                raise ValueError("%s is not a valid highlight color for a vehicle, please use one of the following:\n"
                                 "%s" % (color, str(valid_highlight_colors.keys())))

    def _get_observation_space(self):
        lights_low = [0, 0, 0, 0]
        lights_high = [1, 1, 1, 1]
        vehicle_low = [0, 0, 0, -1]     # lane, position, length, turn
        vehicle_high = [3, self.state_info.intersection_length + self._road_length - 1, self.max_vlength, 1]
        box_low = np.array(lights_low + self._max_vehicles_on_map * vehicle_low)
        box_high = np.array(lights_high + self._max_vehicles_on_map * vehicle_high)
        if self._verbose:
            print("Box low and high")
            print(box_low)
            print(box_high)
        return spaces.Box(
            low=box_low,
            high=box_high,
            dtype=int
        )

    def _get_time_to_next_spawn(self):
        return randint(self._spawn_interval[0], self._spawn_interval[1])

    def _get_state_info(self):
        return StateInformation(self._road_length, self._vehicle_types, self._lights_init,
                                self._turn_signals, self._max_vehicles_on_map)

    def _get_obs(self):
        return self.state_info.to_vector()

    def _step_move_vehicles(self):
        reward = 0
        vehicles_in_intersection = []
        idx_to_remove = []
        self.state_info.lanes_with_left_turns_in_progress = b('0000')
        first_free_pos = [0, 0, 0, 0]   # First free position by lane (for use in 2nd+ vehicles at red light)
        for v in range(len(self.state_info.vehicles)):
            veh = self.state_info.vehicles[v]
            next_spos = veh[1]-self._vehicle_speed
            # >> about to leave intersection:
            if next_spos < 0 or \
                    (veh[3] == TS.RIGHT and next_spos < self.state_info.intersection_turn_lane_ends):
                # fully exiting:
                if (veh[3] == TS.STRAIGHT and abs(next_spos) >= veh[2]) \
                        or \
                        (veh[3] == TS.RIGHT and (abs(next_spos)+self.state_info.intersection_turn_lane_ends >= veh[2]))\
                        or \
                        (veh[3] == TS.LEFT and (abs(next_spos) >= veh[2]+self.state_info.intersection_turn_lane_ends)):
                    idx_to_remove.append(v)
                # part of vehicle remains in intersection
                else:
                    if veh[3] == TS.STRAIGHT:
                        veh[1] = 0
                        veh[2] += next_spos  # decrease length by amt that exits
                    elif veh[3] == TS.LEFT:
                        veh[1] = 0
                        veh[2] += next_spos  # decrease length by amt that continues into turn
                        if veh[2] == 0:  # all vehicle is engaged in the left turn
                            idx_to_remove.append(v)
                        self.state_info.note_left_turn_from(v, veh[0])
                    else:          # assuming TS.RIGHT
                        veh[1] = self.state_info.intersection_turn_lane_ends
                        amt_diff_from_straight = next_spos-self.state_info.intersection_turn_lane_ends
                        veh[2] += amt_diff_from_straight    # decrease length by amt that turns
                    vehicles_in_intersection.append(veh)
            # >> about to enter intersection (or already in it but not started to exit)
            elif next_spos < self.state_info.intersection_length:
                if self.state_info.lights[veh[0]]:  # light is green
                    if veh[3] == TS.STRAIGHT:
                        veh[1] = next_spos
                    elif veh[3] == TS.LEFT:
                        veh[1] = next_spos  # until turn begins is the same as straight
                    else:       # assuming TS.RIGHT
                        veh[1] = self.state_info.intersection_turn_lane_ends
                        amt_diff_from_straight = next_spos-self.state_info.intersection_turn_lane_ends
                        veh[2] += amt_diff_from_straight    # decrease length by amt that turns
                    vehicles_in_intersection.append(veh)
                else:                               # light is red
                    if veh[1] < self.state_info.intersection_length:    # if vehicle is partially in already
                        veh[1] = next_spos  # until turn begins is the same as straight
                    else:       # vehicle would enter or not enter
                        veh[1] = self.state_info.intersection_length
                        reward += self.waiting_penalty
            # >> norm driving along lane
            else:
                if self.state_info.lights[veh[0]]:  # light is green
                    veh[1] = next_spos
                else:                               # light is red
                    if first_free_pos[veh[0]] >= next_spos:   # if there is a vehicle blocking the way
                        veh[1] = first_free_pos[veh[0]]
                        reward += self.waiting_penalty
                    else:
                        veh[1] = next_spos
            first_free_pos[veh[0]] = veh[1] + veh[2]
            self.state_info.vehicles[v] = veh
        self.state_info.vehicles = delete_by_idx(self.state_info.vehicles, idx_to_remove)
        self.state_info.vcolors = delete_by_idx(self.state_info.vcolors, idx_to_remove)
        self._step_reorder_vehicles_by_nearest_to_intersection()
        reward = self._step_check_for_collision(reward, vehicles_in_intersection)
        return reward

    def _step_reorder_vehicles_by_nearest_to_intersection(self):
        self.state_info.reorder_vehicles_by_nearest_to_intersection()

    def _step_check_for_collision(self, reward, vehicles):
        if self._check_collisions_with_left_turns(vehicles) or \
                self._check_collision_with_actual_vehicles(vehicles):
            self.state_info.collision = True
            return reward + self.collision_penalty
        else:
            return reward

    def _check_collisions_with_left_turns(self, vehicles):
        if sum(self.state_info.lanes_with_left_turns_in_progress) > 0:
            lanes_with_vehicles = np.zeros(4)
            for veh in vehicles:
                lanes_with_vehicles[veh[0]] = 1
            return sum(self.state_info.lanes_with_left_turns_in_progress & b(list(lanes_with_vehicles))) > 0
        else:
            return False

    def _check_collision_with_actual_vehicles(self, vehicles):
        if len(vehicles) > 1:       # fill up the matrix, if a spot is hit twice, collision.  0,0 is in upper left
            intersection = np.zeros([self.state_info.intersection_length, self.state_info.intersection_length])
            for veh in vehicles:
                if veh[0] == Lanes.LEFT:
                    v_front = self.state_info.intersection_length-veh[1]
                    v_back_or_int = max(v_front-veh[2], 0)
                    intersection[self.state_info.intersection_length-1, v_back_or_int:v_front] += 1
                elif veh[0] == Lanes.UPPER:
                    v_front = self.state_info.intersection_length - veh[1]
                    v_back_or_int = max(self.state_info.intersection_length - (veh[1]+veh[2]), 0)
                    intersection[v_back_or_int:v_front, 0] += 1
                elif veh[0] == Lanes.RIGHT:
                    intersection[0, veh[1]:min(veh[1]+veh[2], self.state_info.intersection_length)] += 1
                else:       # DOWN
                    v_front = veh[1]
                    v_back_or_int = min(veh[1]+veh[2], self.state_info.intersection_length)
                    # intersection[v_back_or_int:v_front, self.state_info.intersection_length-1] += 1
                    intersection[v_front:v_back_or_int, self.state_info.intersection_length-1] += 1
            return sum(sum(intersection > 1)) > 0
        else:
            return False

    def _step_spawn_vehicles(self):
        # If time to spawn, there are veh left to spawn, and veh not at max:
        # noinspection PyChainedComparisons
        if self._time_to_next_spawn <= 0 and self._vehicles_remaining_to_spawn > 0 \
                and len(self.state_info.vehicles) < self._max_vehicles_on_map:
            open_lanes = self._lanes_open_for_spawning()
            if len(open_lanes) > 0:     # If there is room to spawn:
                veh, color = self._spawn_random_vehicle(open_lanes)
                if self._verbose:
                    print("Spawning: %s " % str(veh))
                self.state_info.vehicles.append(veh)
                self.state_info.vcolors.append(color)
                # update counts
                self._vehicles_remaining_to_spawn -= 1
                self._time_to_next_spawn = self._get_time_to_next_spawn()
        else:
            self._time_to_next_spawn -= 1

    def _lanes_open_for_spawning(self):
        open_lanes = b('1111')
        lane_info = [[], [], [], []]
        for veh in self.state_info.vehicles:
            lane_info[veh[0]].append(veh)
        for lane in range(4):
            for veh in lane_info[lane]:
                if veh[1] + veh[2] >= (self._road_length + self.state_info.intersection_length - 1):
                    open_lanes[lane] = 0
        return [i for i in range(4) if open_lanes[i]]

    def _spawn_random_vehicle(self, open_lanes):
        slane = rand_choice(open_lanes)
        vtype = wrand_choice(list(self._vehicle_types.keys()), 1, p=self._vehicle_dist)[0]
        vlength = randint(self._vehicle_types[vtype][0], self._vehicle_types[vtype][1])
        vturn = (wrand_choice(self._turn_signals, 1, p=self._turn_signal_dist)
                 if len(self._turn_signals) > 1 else self._turn_signals[0])[0]
        color = self._vehicle_colors[vtype]
        return [slane, self._road_length + self.state_info.intersection_length-1, vlength, vturn], color

    def _step_update_lights(self, action):
        self.state_info.lights = self._actionset_info.get_light_config_for_action(action, self.state_info.lights)

    def _step_get_return_vals(self):
        state = self._get_obs()
        done = self._done()
        return state, done, {}

    def _done(self):
        return ((self._vehicles_remaining_to_spawn <= 0) and (len(self.state_info.vehicles) <= 0)) \
               or self.state_info.collision
