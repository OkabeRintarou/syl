#!/usr/bin/python
# -*- coding: utf-8 -*- 

def skip_space(string,strlen,start):
    index = start
    while index < strlen:
        if string[index].isalpha():
            return index
        index += 1
    return -1

def addword(d,string,strlen,index):

    forward = index + 1
    
    while forward < strlen:
        if string[forward].isalpha():
            forward += 1
        else:
            app = string[index:forward]
            if d.get(app,-1) == -1:
                d[app] = 1
            else:
                d[app] += 1
            return forward

    if forward >= strlen:
        app = string[index:forward]
        if d.get(app,-1) == -1:
            d[app] = 1
        else:
            d[app] += 1

    return forward

def word_count(filename):
    file = open(filename,'r')
    data = file.read()
    datalen = len(data)
    lookup ={}

    index = skip_space(data,datalen,0)
    forward = -1
    while index != -1:
        forward = addword(lookup,data,datalen,index)
        index = skip_space(data,datalen,forward + 1)
    
    file.close()

    return lookup

if __name__ == '__main__':
    filename = 'demo.txt'
    for key,value in word_count(filename).items():
        print("%s:%d" % (key,value))
    