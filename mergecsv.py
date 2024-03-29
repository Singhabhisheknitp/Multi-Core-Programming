import pandas as pd
import os

files = ['tas.csv', 'ttas.csv', 'alock.csv', 'clh.csv', 'mcs.csv']
dfs = []

for file in files:
    path = f'benchmarking/{file}'
    if os.path.exists(path):
        df = pd.read_csv(path)
        df.set_index('T', inplace=True)
        dfs.append(df)

df = pd.concat(dfs, axis=1)

df.to_csv('locklatency.csv', index=False)
df.reset_index().to_csv('benchmarking/locklatency.csv', index=False)



