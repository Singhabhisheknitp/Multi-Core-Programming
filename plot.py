import pandas as pd
import matplotlib.pyplot as plt
df = pd.read_csv('locklatency.csv')
columns = df.columns.tolist()
columns_num = len(columns) - 1
styles = ['*-', 'o-', 'x-', '+-']
ax = df.plot(x=columns[0], y=columns[1:], kind='line', linewidth=3, style=styles[:columns_num], markersize=8)

plt.title('Threadnum vs Time', fontsize=7)
plt.xlabel('Threadnum', fontsize=7)
plt.ylabel('Time', fontsize=7)
ax.legend(df.columns[1: ], fontsize=7)
plt.grid(True)
plt.savefig('plot.png')


