# -*- coding: utf-8 -*-
"""
Created on Thu Nov 12 14:17:51 2020

@author: Sharjeel Ali
"""

import os
import h5py
import sys
import numpy as np
import random
import json
import shutil
import copy
import decimal

SRC= "assets/meshes/asteroids/"
data = {}
np.set_printoptions(precision=16)

def read_obj(filename):
    vertex = []
    other = []
    with open(filename) as file:
        print(filename)
        i = 0
        for line in file:
            if(line[0]=='v' and line[1] ==' '):
                 line = ' '.join(line.split())
                 #pos = [float(digit) for digit in line[2:-2].split(' ')]
                 pos = line[2:-1].split(' ')
                 for j in range(3):
                     if('e' in pos[j]):
                         pos[j] = 0
                 pos = np.asarray(pos,dtype=np.float16, order='C')
                 vertex.append(pos)
                 i+=1
            else:
                other.append(line)
            
    #return {"v": vertex, "data": other}
    return {"v": np.array(vertex),"data": other}

#source is from https://github.com/charlesq34/pointnet/blob/d64d2398e55b24f69e95ecb549ff7d4581ffc21e/part_seg/test.py
def pc_normalize(data):
    l = data.shape[0]
    centroid = np.mean(data, axis=0)
    data = data - centroid
    m = np.max(np.sqrt(np.sum(data**2, axis=1)))
    data = data / m
    return data

def output_file(data, filename):
    
    with open(filename, 'w') as f:
        for pos in data["v"]:
            line = "v {} {} {} \n".format(pos[0], pos[1], pos[2])
            f.write("%s" % line)
        for line in data["data"]:
            f.write("%s" % line)

def converter(files):
    final_data = {}
    for file in files:
        data = read_obj(file)
        data["v"] = pc_normalize(data["v"])
        output_file(data, file)
        final_data[file] = data
    return final_data
        
        
def main():
    files =[]
    if(SRC.endswith('/')):
        files = os.listdir(SRC)
        for i in range(len(files)):
            files[i] = SRC+files[i]
    else:
        files.append(SRC)
    
    global data
    data = converter(files)
if __name__ == "__main__":  
    main()    
