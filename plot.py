import pandas as pd
import matplotlib.pyplot as plt
import os

## merge csv files in the benchmarking folder
files = ['tas.csv', 'ttas.csv', 'alock.csv', 'clh.csv', 'mcs.csv']
dfs = []

for file in files:
    path = f'benchmarking/{file}'
    if os.path.exists(path):
        df = pd.read_csv(path)
        df.set_index('T', inplace=True)
        dfs.append(df)

df = pd.concat(dfs, axis=1)
df.reset_index().to_csv('benchmarking/locklatency.csv', index=False)


##plot the curve
df = pd.read_csv('benchmarking/locklatency.csv')
columns = df.columns.tolist()
columns_num = len(columns) - 1
styles = ['*-', 'o-', 'x-', '+-', 's-']

ax = df.plot(x=columns[0], y=columns[1:], kind='line', linewidth=3, style=styles[:columns_num], markersize=8)

plt.title('Thread vs latency', fontsize=7)
plt.xlabel('Number of threads', fontsize=7)
plt.ylabel('Time elapsed', fontsize=7)
ax.legend(df.columns[1: ], fontsize=7)
plt.grid(True)

plt.savefig('benchmarking/plot.pdf', format='pdf')



