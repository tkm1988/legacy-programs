import pandas as pd

df = pd.read_csv("./datasets/members.csv")
sampling = df.sample(n=4)
print(sampling)
