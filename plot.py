import pandas as pd
import matplotlib.pyplot as plt
import os

## plot files in the benchmarking folder for queue performace
files = ['twolockqueue.csv', 'msqueue.csv', 'optimistic.csv']  
dfs = []
for file in files:
    path = f'benchmarking/{file}'
    if os.path.exists(path):
        df = pd.read_csv(path)
        df.columns = df.columns.str.strip()
        df.set_index('No of threads', inplace=True)
        dfs.append(df)
if dfs:
    df = pd.concat(dfs, axis=1)
    df.reset_index().to_csv('benchmarking/queueperformance.csv', index=False)
    df = pd.read_csv('benchmarking/queueperformance.csv')
    columns = df.columns.tolist()
    columns_num = len(columns) - 1
    styles = ['*-', 'o-', 'x-', '+-', 's-']
    ax = df.plot(x=columns[0], y=columns[1:], kind='line', linewidth=2, style=styles[:columns_num], markersize=8)
    plt.title('Thread vs latency', fontsize=8)
    plt.xlabel('Number of threads', fontsize=8)
    plt.ylabel('time elapsed', fontsize=8)
    ax.legend(df.columns[1: ], fontsize=8)
    plt.grid(True)
    plt.savefig('benchmarking/totaltimeplot.pdf', format='pdf')
    plt.close()



##plot files in the benchmarking folder of fixlistcaller
files1 = ["fixlistcallerenqdeqpair.csv", "fixlistcaller50%enqueue.csv"]  
dfs1 = []

for file in files1:
    path = f'benchmarking/{file}'
    if os.path.exists(path):
        df = pd.read_csv(path)
        df.columns = df.columns.str.strip()
        df.set_index('No of threads', inplace=True)
        dfs1.append(df)
if dfs1:
    df = pd.concat(dfs1, axis=1)
    df.reset_index().to_csv('benchmarking/fixlistcaller.csv', index=False)
    df = pd.read_csv('benchmarking/fixlistcaller.csv')
    columns = df.columns.tolist()
    columns_num = len(columns) - 1
    styles = ['*-', 'o-', 'x-', '+-', 's-']
    ax = df.plot(x=columns[0], y=columns[1:], kind='line', linewidth=2, style=styles[:columns_num], markersize=8)
    plt.title('Thread vs Fixlist Calls', fontsize=8)
    plt.xlabel('Number of threads', fontsize=8)
    plt.ylabel('Number of Fixlist Calls', fontsize=8)
    ax.legend(df.columns[1: ], fontsize=8)
    plt.grid(True)
    plt.savefig('benchmarking/fixlist_plot.pdf', format='pdf')
    plt.close()




## plot files in the benchmarking folder of failedcascounter

if os.path.exists('benchmarking/failedcascounteroptimistic.csv'):
    dfcas1 = pd.read_csv('benchmarking/failedcascounteroptimistic.csv')
if os.path.exists('benchmarking/failedcascountermsqueue.csv'):
    dfcas2 = pd.read_csv('benchmarking/failedcascountermsqueue.csv')

if 'dfcas1' in locals() and 'dfcas2' in locals():
    dfs2 = pd.merge(dfcas1, dfcas2, on='No of threads', suffixes=('_optimistic', '_msqueue'))
    dfs2.to_csv('benchmarking/failedcascounter.csv', index=False)
    dfs2 = pd.read_csv('benchmarking/failedcascounter.csv')
    columns = dfs2.columns.tolist()
    columns_num = len(columns) - 1
    styles = ['*-', 'o-', 'x-', '+-', 's-']
    ax = dfs2.plot(x=columns[0], y=columns[1:], kind='line', linewidth=2, style=styles[:columns_num], markersize=8)
    plt.title('Thread vs Failed CAS calls in 1000s', fontsize=8)
    plt.xlabel('Number of threads', fontsize=8)
    plt.ylabel('Number of Failed CAS calls', fontsize=8)
    ax.legend(dfs2.columns[1: ], fontsize=8)
    plt.grid(True)
    plt.savefig('benchmarking/failedcas_plot.pdf', format='pdf')
    plt.close()

print("All plots are saved in the benchmarking folder")

    




