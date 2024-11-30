import pandas as pd
import plotly.express as px

df = pd.read_csv('file.txt')
fig = px.scatter(df)
#fig = px.line(df)
#fig.show()
largest_value = df.max().max()

# Print the largest number
print(f"The largest number in the CSV file is: {largest_value}")
