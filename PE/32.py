# Ӣ���Ļ����ƶȣ��Ա�ʿΪ��λ��Ϊ1,2,5,10,20,50,100,200������
# ��2Ӣ����200��ʿ���ж�������Ϸ�����
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
