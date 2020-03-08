import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

# cp_times = pd.read_csv("cp_times.csv")
# g = sns.relplot(x="Iterations", y="Training time (seconds)", data=cp_times, kind="line", hue="Version", palette="Dark2", legend="full")
# g.set(ylim=(0, 20), xlabel="Iterations", ylabel="Training time (seconds)", xscale="log")
# plt.savefig("cp_times.png", bbox_inches="tight")

# ll_times = pd.read_csv("ll_times.csv")
# g = sns.relplot(x="Iterations", y="Training time (seconds)", data=ll_times, kind="line", hue="Version", palette="Dark2", legend="full")
# g.set(ylim=(0, 65), xlabel="Iterations", ylabel="Training time (seconds)", xscale="log")
# plt.savefig("ll_times.png", bbox_inches="tight")

cp_reward = pd.read_csv("cp_reward.csv")
g = sns.relplot(x="Iterations", y="Average Reward per Episode", data=cp_reward, kind="line", hue="Version", palette="Dark2", legend="full")
g.set(ylim=(8, 30), xlabel="Iterations", ylabel="Average Reward per Episode", xscale="log")
plt.savefig("cp_reward.png", bbox_inches="tight")

ll_reward = pd.read_csv("ll_reward.csv")
g = sns.relplot(x="Iterations", y="Average Reward per Episode", data=ll_reward, kind="line", hue="Version", palette="Dark2", legend="full")
g.set(ylim=(-900, 0), xlabel="Iterations", ylabel="Average Reward per Episode", xscale="log")
plt.savefig("ll_reward.png", bbox_inches="tight")
