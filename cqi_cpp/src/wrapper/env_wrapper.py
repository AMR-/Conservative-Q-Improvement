import random 

class EnvWrapper(object):
    def __init__(self):
        self.mapping = {}

        with open("mapping.csv") as f:
            content = f.readlines()

        for elem in content:
            lst = elem.split(",")

            lst = list(map(lambda x: float(x), lst))

            self.mapping[lst[0]] = lst[1:]

    def get_by_key(self, action):
        return self.mapping[action]

    def get_by_value(self, action):
        for key, val in self.mapping.items():
            if val == list(action):
                return key

    def sample(self):
        random.seed()

        return self.mapping[random.randint(0, len(self.mapping.keys()) - 1)]

ew = EnvWrapper()
