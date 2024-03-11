import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('locklatency.csv', skiprows=1, names=['T', 'Time_TAS', 'Time_TTAS'])

# Plot with specified line width and marker style
ax = df.plot(x='T', y=['Time_TAS', 'Time_TTAS'], kind='line', linewidth=3, 
             style=['o-', 'x-', '*-'], markersize=8)

plt.title('Threadnum vs Time', fontsize=7)
plt.xlabel('Threadnum', fontsize=7)
plt.ylabel('Time', fontsize=7)

# Adjust legend
ax.legend(["Time_TAS", "Time_TTAS"], fontsize=7)

plt.grid(True)
plt.savefig('plot.png')


