import pandas as pd
import matplotlib.pyplot as plt
import os

## merge csv files in the benchmarking folder
files = ['twolockqueue.csv', 'msqueue.csv', 'optimistic.csv']  
dfs = []

for file in files:
    path = f'benchmarking/{file}'
    if os.path.exists(path):
        df = pd.read_csv(path)
        df.columns = df.columns.str.strip()
        df.set_index('No of threads', inplace=True)
        dfs.append(df)




##plot the curve for time performace of the queues
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



## merge csv files in the benchmarking folder of fixlistcaller
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






#plotting of failedcascounter
plt.figure()
if os.path.exists('benchmarking/failedcascounter.csv'):
    print('voila you are done!')
    df_failedcas = pd.read_csv('benchmarking/failedcascounter.csv')
    df_failedcas.columns = df_failedcas.columns.str.strip()
    df_failedcas.set_index('No of threads', inplace=True)
    ax_failedcas = df_failedcas.plot(kind='line', linewidth=2, markersize=8)
    plt.title('Thread vs Failed CAS calls in 1000s', fontsize=8)
    plt.xlabel('Number of threads', fontsize=8)
    plt.ylabel('Number of Failed CAS calls', fontsize=8)
    ax_failedcas.legend(df_failedcas.columns, fontsize=8)
    plt.grid(True)
    plt.savefig('benchmarking/failedcas_plot.pdf', format='pdf')
    
    

# #plotting error plot for the queues
# plt.figure()
# df_errorplot = pd.DataFrame()
# with open('benchmarking/errorplotmsqueue.csv', 'r') as file:
#     for line in file:
#         if line.strip() == 'No of threads,MSQueue, No of iterations':
#             continue
#         columns = line.strip().split(',')
#         df_errorplot = df_errorplot._append(pd.DataFrame([columns], columns=['No of threads', 'MSQueue', 'No of iterations']), ignore_index=True)

# # Convert the columns to the correct data types
# df_errorplot['No of threads'] = df_errorplot['No of threads'].astype(int)
# df_errorplot['MSQueue'] = df_errorplot['MSQueue'].astype(float)
# df_errorplot['No of iterations'] = df_errorplot['No of iterations'].astype(int)

# import matplotlib.pyplot as plt

# # Create a figure and a set of subplots
# fig, ax = plt.subplots()

# # Create a bar chart
# for i in df_errorplot['No of iterations'].unique():
#     df_subset = df_errorplot[df_errorplot['No of iterations'] == i]
#     ax.bar(df_subset['No of threads'], df_subset['MSQueue'])

# # Set the title and labels
# ax.set_title('Error Plot for MSQueue')
# ax.set_xlabel('Number of threads')
# ax.set_ylabel('Time elapsed')

# # Add a legend
# ax.legend()

# # Save the figure as a PDF
# plt.savefig('error_plot.pdf', format='pdf')



