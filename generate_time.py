#!/usr/bin/env python3
import os
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

#for action in flags:
#    print(action)


for ex in executables:
    (ind, prefix) = executables[ex]
    for f in files:
        for action in flags:
            #action = "add"
            flag = flags[action][ind]

            #f = "100.csv"
            add_file = "InputFiles/"+"add"+"_"+f
            file = "InputFiles/"+action+"_"+f
            out_file="Results/"+prefix+"_"+action+"_"+f

            if action=="search":
                # Set file identical to remove
                file = "InputFiles/"+"remove"+"_"+f


            cmd = "./cmd_formatter.sh "+ ex + " " + flag + " " + file + " " + out_file

            if action == "add":
                # DROP DATABASES
                print("DROP DATABASE")
                os.system("make drop")
            
            # Base de datos ya esta inicializada de ejecutar and previamente
            """
            elif:
                # DROP DATABASES
                print("DROP DATABASE")
                os.system("make drop")
                # INIT DATABSE
                print("INIT DATABASE")
                print("./add_ehash.sh "+add_file)
                os.system("./add_ehash.sh "+file)
            """

            print(cmd)
            os.system(cmd)

#process = subprocess.call(["./seqf.out", "-i", first])
