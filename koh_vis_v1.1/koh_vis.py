import matplotlib.pyplot as plt
import math

import numpy as np

clusters = np.loadtxt('changed_clusters.txt')
indexes = [3, 6, 7, 8, 9, 11]
greedy_indexes = []
data = []

for clust in clusters:
    temp_data = []
    for i in range(len(clust)):
        if i in indexes:
            temp_data.append(clust[i])
    data.append(np.array(temp_data))

data = np.array(data)

for clust in data:
    if clust[5] == 0:
        clust[5] = 1
    greedy_ind = round(((clust[1]/clust[0])*clust[3]*clust[5])/(clust[2]*clust[4]), 3)
    greedy_indexes.append(greedy_ind)

greedy_indexes = np.array(greedy_indexes)

x_axis = [x/10 for x in range(201)]
x_axis = np.array(x_axis)

y_axis = []

for grd_ind in greedy_indexes:
    y_axis_temp = []
    for val in x_axis:
        y_axis_temp.append(grd_ind - grd_ind/math.exp(val))
    y_axis.append(y_axis_temp)

y_axis = np.array(y_axis)

for i in range(len(y_axis)):
    plt.plot(x_axis, y_axis[i])

for i, gi in enumerate(greedy_indexes):
    print(f"For cluster {i} greedy index is equal to {gi}")

plt.show()