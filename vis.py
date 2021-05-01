import numpy as np
import matplotlib.pyplot as plt
"""
    importing Libraries
"""

"""
    Part 1 of Visualization
    Here we are visualizing all the points in the test case
    Points will be represented as Black dots on Graph
    Input Format of points.txt :-
        Line 1 -- Number of Points 
        Following Lines contains all the points in x, y with each line conatining one point
        
    Note :- The same Input format is given to cpp file used
"""
f = open("./points.txt", "r")
c=int(f.readline())
for i in range(c):
    a = f.readline().split()
    x = float(a[0])
    y = float(a[1] )
    plt.scatter(x,y)
    plt.plot(x,y,'o',color='black')
f = open("./lines.txt", "r")

"""
    Part 2 of Visualization
    Here we are visualizing all the Lines that we gets after running the Algorithm
    Lines will be represented as Red Lines on Graph
    Input Format of points.txt :-
        Line 1 -- Value of OPT
        Line 2 -- Number of Lines 
        Following Lines contains all the Lines endpoints in x1, y1 , x2, y2 format with each line conatining one Line
"""
f.readline()
f.readline()
d=f.readline()
f.readline()
for x in d.split():
   if x.isdigit():
      c=int(x)
for i in range(c):
    a = f.readline().split()
    x = [float(a[0]) ,float(a[2])]
    y = [float(a[1]) , float(a[3])]
    plt.plot(x,y,color='red')
plt.show()