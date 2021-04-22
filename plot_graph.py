from matplotlib import pyplot as plt 
import pandas as pd

df = pd.read_table("convergence.txt", names=['średnie', 'najlepsze'], index_col=0)
df.plot(ylabel="Dostosowanie", xlabel="Numer populacji", ylim=(0,1.1))
plt.show()