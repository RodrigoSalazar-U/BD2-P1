#!/usr/bin/env python3
def getSum(filename):
    file = open(filename)
    total = 0
    for num in file.readlines():
        num = int(num)
        total += num
    return total

executables = {
    "./seqf.out":(1,"seqf_time"),
    "./ehash.out":(0, "ehash_time"),
}

flags = {
    "add" : ("add","-i"),
    "search" : ("search","-s"),
    "rangesearch" : ("rangeSearch","-rs"),
    "remove" : ("remove","-d"),
}

files = ["100.csv", "1k.csv", "10k.csv", "100k.csv",]

for action in flags:
    #action = "add"
    print("ACTION:",action)
    for ex in executables:
        (ind, prefix) = executables[ex]
        flag = flags[action][ind]
        print("EXECUTABLE:",ex)
        for f in files:
            out_file="Results/"+prefix+"_"+action+"_"+f
            print(out_file, getSum(out_file))