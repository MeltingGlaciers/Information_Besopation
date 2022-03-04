import matplotlib.pyplot as plt
import numpy as np

vals = []
with open('output.txt') as file:
    for line in file:
        vals.append(int(line))

plt.bar(range(0,len(vals)),vals)
plt.show()

x = []
y = []
z = []
with open('star.txt') as file:
    for line in file:
        curr = line.split(',')
        x.append(int(curr[0]))
        y.append(int(curr[1]))
        z.append(int(curr[2]))
print(x)
print(y)
print(z)
fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')
ax.bar3d(x,y,np.zeros_like(z),1,1,z,shade=True)
plt.show()