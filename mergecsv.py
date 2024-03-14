import pandas as pd
df1 = pd.read_csv('locklatency1.csv')
df2 = pd.read_csv('locklatency2.csv')

df1.set_index('T', inplace=True)
df2.set_index('T', inplace=True)

df = pd.concat([df1, df2], axis=1)
df.to_csv('locklatency.csv', index=False)
df.reset_index().to_csv('locklatency.csv', index=False)


