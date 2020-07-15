# Vehicle Intersection
# Created by Aaron M. Roth
# https://github.com/AMR-/gym_vehicle_intersection
# MIT License

from VehicleIntersectionSupport import VehicleIntersectionActionSet
from bitarray import bitarray as b
from gym import spaces


class ToggleRoad(VehicleIntersectionActionSet):
    def describe(self):
        return """
        Actions:
        0 - Wait
        1 - Toggle lights on vertical road
        2 - Toggle lights on horizontal road
        
        light_config (passed and returned) is in order [Left, Up, Right, Down]
        0 is Red, 1 is Green
        """

    def get_action_space(self):
        return spaces.Discrete(3)

    def get_light_config_for_action(self, action, light_config):
        return {
            0: lambda lc: lc,
            1: lambda lc: lc ^ b('0101'),
            2: lambda lc: lc ^ b('1010'),
        }[action](light_config)


class ToggleLight(VehicleIntersectionActionSet):
    def describe(self):
        return """
        Actions:
        0 - Wait
        1 - Toggle lights on left approach
        3 - Toggle lights on upper approach
        2 - Toggle lights on right approach
        4 - Toggle lights on lower approach
        
        light_config (passed and returned) is in order [Left, Up, Right, Down]
        0 is Red, 1 is Green
        """

    def get_action_space(self):
        return spaces.Discrete(5)

    def get_light_config_for_action(self, action, light_config):
        return {
            0: lambda lc: lc,
            1: lambda lc: lc ^ b('1000'),
            2: lambda lc: lc ^ b('0100'),
            3: lambda lc: lc ^ b('0010'),
            4: lambda lc: lc ^ b('0001'),
        }[action](light_config)


class SetByRoad(VehicleIntersectionActionSet):
    def describe(self):
        return """
        Actions:
        0 - Wait
        1 - Set lights on vertical road to Green
        2 - Set lights on vertical road to Red
        3 - Set lights on horizontal road to Green
        4 - Set lights on horizontal road to Red
        
        light_config (passed and returned) is in order [Left, Up, Right, Down]
        0 is Red, 1 is Green
        """

    def get_action_space(self):
        return spaces.Discrete(5)

    def get_light_config_for_action(self, action, light_config):
        return {
            0: lambda lc: lc,
            1: lambda lc: lc | b('0101'),   # OR with 1 to turn green (OR with 0 to keep same)
            2: lambda lc: lc & b('1010'),   # AND with 0 to turn red (AND with 1 to keep same)
            3: lambda lc: lc | b('1010'),
            4: lambda lc: lc & b('0101'),
        }[action](light_config)


class SetByLight(VehicleIntersectionActionSet):
    def describe(self):
        return """
        Actions:
        0 - Wait
        1 - Set lights on left approach road to Green
        2 - Set lights on left approach road to Red
        3 - Set lights on upper approach road to Green
        4 - Set lights on upper approach road to Red
        5 - Set lights on right approach road to Green
        6 - Set lights on right approach road to Red
        7 - Set lights on lower approach road to Green
        8 - Set lights on lower approach road to Red
        
        light_config (passed and returned) is in order [Left, Up, Right, Down]
        0 is Red, 1 is Green
        """

    def get_action_space(self):
        return spaces.Discrete(9)

    def get_light_config_for_action(self, action, light_config):
        return {
            0: lambda lc: lc,
            1: lambda lc: lc | b('1000'),   # OR with 1 to turn green (OR with 0 to keep same)
            2: lambda lc: lc & b('0111'),   # AND with 0 to turn red (AND with 1 to keep same)
            3: lambda lc: lc | b('0100'),
            4: lambda lc: lc & b('1011'),
            5: lambda lc: lc | b('0010'),
            6: lambda lc: lc & b('1101'),
            7: lambda lc: lc | b('0001'),
            8: lambda lc: lc & b('1110'),
        }[action](light_config)


class SetExplicitly(VehicleIntersectionActionSet):
    def describe(self):
        return """
        Actions:
        0 - Wait
        1 - Set lights to RRRR
        2 - Set lights to RRRG
        3 - Set lights to RRGR
        4 - Set lights to RRGG
        5 - Set lights to RGRR
        6 - Set lights to RGRG
        7 - Set lights to RGGR
        8 - Set lights to RGRG
        9 - Set lights to RGGG
        10 - Set lights to GRRR
        11 - Set lights to GRRG
        12 - Set lights to GRGR
        13 - Set lights to GRGG
        14 - Set lights to GGRR
        15 - Set lights to GGRG
        16 - Set lights to GGGR
        17 - Set lights to GGGG
        
        light_config (passed and returned) is in order [Left, Up, Right, Down]
        0 is Red, 1 is Green
        """

    def get_action_space(self):
        return spaces.Discrete(9)

    def get_light_config_for_action(self, action, light_config):
        return {
            0: lambda lc: lc,
            1: b('0000'),
            2: b('0001'),
            3: b('0010'),
            4: b('0011'),
            5: b('0100'),
            6: b('0101'),
            7: b('0110'),
            8: b('0101'),
            9: b('0111'),
            10: b('1000'),
            11: b('1001'),
            12: b('1010'),
            13: b('1011'),
            14: b('1100'),
            15: b('1101'),
            16: b('1110'),
            17: b('1111'),
        }[action](light_config)
