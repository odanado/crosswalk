#!/usr/local/bin/python3

import os
import glob

os.system("g++ -std=c++11 -O3 -march=native ConvertGGF.cpp -o ConvertGGF.out")

files = glob.glob('./original/*.ggf')
for file in files:
    newfile = file
    newfile = newfile.replace('original', 'dataset')
    newfile = newfile.replace('.ggf', '.data')
    if len(glob.glob(newfile)) == 0:
        print("convert %s -> %s" % (file, newfile))
        os.system("./ConvertGGF.out < %s > %s 2> convert.log" % (file, newfile))
    else:
        print("pass: %s" % (newfile))

