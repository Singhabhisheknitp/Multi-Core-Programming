import pandas as pd
import matplotlib.pyplot as plt
import os

## merge csv files in the benchmarking folder
files = ['msqueue.csv', 'twolockqueue.csv', 'optimistic.csv']  # Removed the empty string
dfs = []

for file in files:
    path = f'benchmarking/{file}'
    if os.path.exists(path):
        df = pd.read_csv(path)
        df.columns = df.columns.str.strip()
        df.set_index('No of threads', inplace=True)
        dfs.append(df)

df = pd.concat(dfs, axis=1)
df.reset_index().to_csv('benchmarking/queueperformance.csv', index=False)

# print(df.columns)
##plot the curve
df = pd.read_csv('benchmarking/queueperformance.csv')
columns = df.columns.tolist()
columns_num = len(columns) - 1
styles = ['*-', 'o-', 'x-', '+-', 's-']

ax = df.plot(x=columns[0], y=columns[1:], kind='line', linewidth=3, style=styles[:columns_num], markersize=8)

plt.title('Thread vs latency', fontsize=8)
plt.xlabel('Number of threads', fontsize=8)
plt.ylabel('time elapsed', fontsize=8)
ax.legend(df.columns[1: ], fontsize=8)
plt.grid(True)

plt.savefig('benchmarking/plot.pdf', format='pdf')