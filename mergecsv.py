import pandas as pd
df1 = pd.read_csv('benchmarking/tas.csv')
df2 = pd.read_csv('benchmarking/ttas.csv')
df3 = pd.read_csv('benchmarking/alock.csv')
df4 = pd.read_csv('benchmarking/clh.csv')
df5 = pd.read_csv('benchmarking/mcs.csv')

df1.set_index('T', inplace=True)
df2.set_index('T', inplace=True)
df3.set_index('T', inplace=True)
df4.set_index('T', inplace=True)
df5.set_index('T', inplace=True)

df = pd.concat([df1, df2, df3, df4, df5], axis=1)

df.to_csv('locklatency.csv', index=False)
df.reset_index().to_csv('benchmarking/locklatency.csv', index=False)


