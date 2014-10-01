# 英国的货币制度，以便士为单位分为1,2,5,10,20,50,100,200几种面额，
# 问2英镑（200便士）有多少种组合方法？
import time

def Combination(target,value):
    if target == []:
        return 0
    elif value < 0:
        return 0
    elif value == 0:
        return 1
    elif target[-1] == -1:
        return value
    else:
        return Combination(target[0:-1],value) + Combination(target,value - target[-1])

start = time.clock()

print Combination([1,2,5,10,20,50,100,200],200)

print time.clock() - start
