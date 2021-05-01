import random


def Rand(start, end, num):
    """
        The function creates the unique points when called, in the range of start and end 
        the function calls a for loop which runs num times where num is the number of points we want to create
        the function returns a list of all the points
        
        this function has to be called separetely for x and y

    """
    res = []
    set_res = set([])
    for j in range(num):
        temp = random.randint(start, end)
        while temp in set_res:
            temp = random.randint(start, end)
        res.append(temp)
        set_res.add(temp)
    return res
  
# Driver Code
num = 1000
start_point = -5000
end_point = 5000
x = Rand(start_point, end_point, num)
y = Rand(start_point , end_point , num)


"""
    start_c and end_c specifies the limit on penalty for partition 

"""
start_c = 0
end_c = 100

c = round(random.uniform(start_c, end_c),2)

"""
    Putting the generated test case in the file in required format

"""

f = open("./points.txt", "w")
f.write(str(num) + '\n')
for i in range(num):
    #f.write('{} {}\n'.format(x[i], y[i]))
    temp = str(x[i]) + ' ' + str(y[i]) + '\n'
    f.write(temp)

f.write(str(c))