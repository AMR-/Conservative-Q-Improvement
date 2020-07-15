# Vehicle Intersection
# Created by Aaron M. Roth
# https://github.com/AMR-/gym_vehicle_intersection
# MIT License

from abc import ABC, abstractmethod
from six import StringIO
from gym import utils
import sys


excluded_vehicle_colors = ("", "gray", "green", "red", "crimson")


def delete_by_idx(mlist, indexes):
    if len(indexes) > 0:
        for index in sorted(indexes, reverse=True):
            del mlist[index]
    return mlist


class Lanes:
    LEFT = 0
    UPPER = 1
    RIGHT = 2
    LOWER = 3

    @staticmethod
    def to_the_left(lane):
        return 0 if lane == 3 else lane + 1

    @staticmethod
    def to_the_right(lane):
        return 3 if lane == 0 else lane - 1

    @staticmethod
    def opposite(lane):
        return (lane+2) % 4


class TurnSignals:
    LEFT = -1
    RIGHT = 1
    STRAIGHT = 0


class Lights:
    Red = 0
    Green = 1
    RedColor = "red"
    GreenColor = "green"


# Abstract class for Action Set
class VehicleIntersectionActionSet(ABC):
    def __init__(self):
        super().__init__()

    @abstractmethod
    def get_light_config_for_action(self, action, light_config):
        pass

    @abstractmethod
    def get_action_space(self):
        pass

    @abstractmethod
    def describe(self):
        pass


class VehicleIntersectionRenderer(object):
    def __init__(self, road_length, intersection_length):
        self.map = self._build_base_map(road_length, intersection_length)
        self.light_locs = self._light_locs(road_length, intersection_length)
        self.lane_0_locs = self._lane_0_locs(road_length, intersection_length)
        self._left_turn_dist = intersection_length-1
        self._max_pavement = road_length + intersection_length
        self._light_char = 'O'
        self._left_turn_colors = ["", "", "", ""]

    def render(self, lights, vehicles, colors, left_turn_info, mode='human'):
        outfile = StringIO() if mode == 'ansi' else sys.stdout
        out = self.map.copy()
        # out = [[c.decode('utf-8') for c in line] for line in out]
        out = [[c for c in line] for line in out]
        self._render_lights(out, lights)
        self._render_vehicles_in_lanes(out, vehicles, colors)
        self._render_left_turns(out, left_turn_info)
        outfile.write("\n".join(["".join(row) for row in out]) + "\n")
        if mode != 'human':
            return outfile

    def set_left_turn_color(self, lane, color):
        self._left_turn_colors[lane] = color

    def _render_lights(self, out, lights):
        for lnum in range(len(self.light_locs)):
            loc = self.light_locs[lnum]
            out[loc[0]][loc[1]] = utils.colorize(self._light_char,
                                                 (Lights.GreenColor if lights[lnum] else Lights.RedColor),
                                                 highlight=True)

    def _render_vehicles_in_lanes(self, out, vehicles, colors):
        for v in range(len(vehicles)):
            veh = vehicles[v]
            color = colors[v]
            (
                self._fill_left_lane_vehicle,
                self._fill_upper_lane_vehicle,
                self._fill_right_lane_vehicle,
                self._fill_lower_lane_vehicle,
            )[veh[0]](out, veh, color)

    def _fill_left_lane_vehicle(self, out_map, veh, color):
        self._set_leftturn_lane_color_if_appropriate(Lanes.LEFT, veh[1], color)
        spos = (self.lane_0_locs[Lanes.LEFT][0], self.lane_0_locs[Lanes.LEFT][1] - veh[1])
        schar = ('^', '>', 'v')[veh[3]+1]
        vlen = min(veh[2], self._max_pavement - veh[1])    # the min of total length or length to end of map
        vchar = '='
        out_map[spos[0]][spos[1]] = utils.colorize(schar, color)
        for vl in range(1, vlen):
            out_map[spos[0]][spos[1]-vl] = utils.colorize(vchar, color)

    def _fill_upper_lane_vehicle(self, out_map, veh, color):
        self._set_leftturn_lane_color_if_appropriate(Lanes.UPPER, veh[1], color)
        spos = (self.lane_0_locs[Lanes.UPPER][0]-veh[1], self.lane_0_locs[Lanes.UPPER][1])
        schar = ('>', 'v', '<')[veh[3]+1]
        vlen = min(veh[2], self._max_pavement - veh[1])    # the min of total length or length to end of map
        vchar = 'H'
        out_map[spos[0]][spos[1]] = utils.colorize(schar, color)
        for vl in range(1, vlen):
            out_map[spos[0]-vl][spos[1]] = utils.colorize(vchar, color)

    def _fill_right_lane_vehicle(self, out_map, veh, color):
        self._set_leftturn_lane_color_if_appropriate(Lanes.RIGHT, veh[1], color)
        spos = (self.lane_0_locs[Lanes.RIGHT][0], self.lane_0_locs[Lanes.RIGHT][1]+veh[1])
        schar = ('v', '<', '^')[veh[3]+1]
        vlen = min(veh[2], self._max_pavement - veh[1])    # the min of total length or length to end of map
        vchar = '='
        out_map[spos[0]][spos[1]] = utils.colorize(schar, color)
        for vl in range(1, vlen):
            out_map[spos[0]][spos[1]+vl] = utils.colorize(vchar, color)

    def _fill_lower_lane_vehicle(self, out_map, veh, color):
        self._set_leftturn_lane_color_if_appropriate(Lanes.LOWER, veh[1], color)
        spos = (self.lane_0_locs[Lanes.LOWER][0]+veh[1], self.lane_0_locs[Lanes.LOWER][1])
        schar = ('<', '^', '>')[veh[3]+1]
        vlen = min(veh[2], self._max_pavement - veh[1])    # the min of total length or length to end of map
        vchar = 'H'
        out_map[spos[0]][spos[1]] = utils.colorize(schar, color)
        for vl in range(1, vlen):
            out_map[spos[0]+vl][spos[1]] = utils.colorize(vchar, color)

    def _set_leftturn_lane_color_if_appropriate(self, lane, spos, color):
        if spos == 0:
            self._left_turn_colors[lane] = color

    def _render_left_turns(self, out_map, left_turns):
        for lane in range(4):
            if left_turns[lane]:
                vchar = ('v', '<', '^', '>')[lane]
                spos_offset = ((+1, -1), (-1, -1), (-1, +1), (+1, +1))[lane]
                spos = self.lane_0_locs[Lanes.to_the_right(lane)]
                for i in range(1, self._left_turn_dist+1):
                    spos = (spos[0] + spos_offset[0], spos[1] + spos_offset[1])
                    out_map[spos[0]][spos[1]] = utils.colorize(vchar, self._left_turn_colors[lane])

    @staticmethod
    def _build_base_map(rlen, ilen):
        # Top line
        base_map = ["-" * (rlen - 1) + "|" + " " * ilen + "|" + "-" * (rlen - 1)]
        # Lines before first light
        for r in range(rlen-3):
            base_map.append("|" + " " * (rlen - 2) + "|" + " " * ilen + "|" + " " * (rlen - 2) + "|")
        # Top lights
        base_map.append("|" + " " * (rlen - 3) + "O" + "|" + " " * ilen + "|" + " " * (rlen - 2) + "|")
        # Top of Intersection
        base_map.append("_" * (rlen - 1) + "/" + " " * ilen + "\\" + "O" + "_" * (rlen - 2))
        # Intersection
        for r in range(ilen):
            base_map.append(" "*(2*rlen+ilen))
        # Bottom of Intersection
        base_map.append("\u203e" * (rlen - 2) + "O" + "\\" + " " * ilen + "/" + "\u203e" * (rlen - 1))
        # Bottom Lights
        base_map.append("|" + " " * (rlen - 2) + "|" + " " * ilen + "|" + "O" + " " * (rlen - 3) + "|")
        # Lines after light after intersection
        for r in range(rlen-3):
            base_map.append("|" + " " * (rlen - 2) + "|" + " " * ilen + "|" + " " * (rlen - 2) + "|")
        base_map.append("-" * (rlen - 1) + "|" + " " * ilen + "|" + "-" * (rlen - 1))
        return base_map

    @staticmethod
    def _light_locs(rlen, ilen):
        return [
            (rlen+ilen, rlen-2),
            (rlen-2, rlen-2),
            (rlen-1, rlen+ilen+1),
            (rlen+ilen+1, rlen+ilen+1)
        ]

    @staticmethod
    def _lane_0_locs(rlen, ilen):
        return [
            (rlen+ilen-1, rlen+ilen-1),
            (rlen+ilen-1, rlen),
            (rlen, rlen),
            (rlen, rlen+ilen-1),
        ]
