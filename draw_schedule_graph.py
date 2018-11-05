#!/usr/bin/python3

from matplotlib import pyplot as plt
import numpy as np
import re
import argparse


def read_data(path:str):
    r=[]
    with open(path,encoding="utf-8") as f:
        line=f.readline()
        while line:
            t=re.findall("\d*\.?\d+",line)
            r.append(list(map(float,t)))
            line=f.readline()
    return r



if __name__=='__main__':
    parser=argparse.ArgumentParser(description="Draw picture for scheduling result..")
    parser.add_argument("--path",required=True,type=str,help="Set input data path")
    parser.add_argument("--node","-n",required=True,type=int,help="Set node size")
    args=parser.parse_args()
    node_sum=args.node
    simple_ga_path=args.path+"/node_"+str(node_sum)+"/ga"
    cuckoo_path=args.path+"/node_"+str(node_sum)+"/cuckoo"
    nsga_path=args.path+"/node_"+str(node_sum)+"/PADC"
    simple_ga_data=read_data(simple_ga_path)
    size=len(simple_ga_data)
    simple_ga_data=list(zip(*simple_ga_data))
    cuckoo_data=list(zip(*read_data(cuckoo_path)))
    nsga_data=list(zip(*read_data(nsga_path)))

    fig=plt.figure()
    point=15
    gap=size//point
    ax=fig.add_subplot(311)
    x=np.arange(size)
    x_cr=[]
    # legends={'square':'x','circle':'o','tir':'*'}
    for i in x:
        if i%gap==0:
            x_cr.append(i)
    y_simple_ga=[simple_ga_data[0][i] for i in x_cr]
    y_cuckoo=[cuckoo_data[0][i] for i in x_cr]
    y_nsga=[nsga_data[0][i] for i in x_cr]
    ax.plot(x,simple_ga_data[0],color='r')
    ax.plot(x,cuckoo_data[0],color="g")
    ax.plot(x,nsga_data[0],color="b")
    ax.scatter(x_cr,y_simple_ga,marker='s',c='r',s=50,label='GA')
    ax.scatter(x_cr,y_cuckoo,marker='o',c='g',s=50,label='CS')
    ax.scatter(x_cr,y_nsga,marker='^',c='b',s=50,label='PADC')
    ax.set_ylabel("Energy(mJ)")
    ax.legend()


    ax2=fig.add_subplot(312)
    y_simple_ga = [simple_ga_data[1][i] for i in x_cr]
    y_cuckoo = [cuckoo_data[1][i] for i in x_cr]
    y_nsga = [nsga_data[1][i] for i in x_cr]
    ax2.plot(x,simple_ga_data[1],color="r")
    ax2.plot(x,cuckoo_data[1],color="g")
    ax2.plot(x,nsga_data[1],color="b")
    ax2.scatter(x_cr, y_simple_ga, marker='s', c='r', s=50,label='GA')
    ax2.scatter(x_cr, y_cuckoo, marker='o', c='g', s=50,label='CS')
    ax2.scatter(x_cr, y_nsga, marker='^', c='b', s=50,label='PADC')
    ax2.set_ylabel("Time(ms)")
    ax2.legend()

    ax3=fig.add_subplot(313)
    y_simple_ga = [simple_ga_data[2][i] for i in x_cr]
    y_cuckoo = [cuckoo_data[2][i] for i in x_cr]
    y_nsga = [nsga_data[2][i] for i in x_cr]
    ax3.plot(x,simple_ga_data[2],color="r")
    ax3.plot(x,cuckoo_data[2],color="g")
    ax3.plot(x,nsga_data[2],color="b")
    ax3.scatter(x_cr, y_simple_ga, marker='s', c='r', s=50,label='GA')
    ax3.scatter(x_cr, y_cuckoo, marker='o', c='g', s=50,label='CS')
    ax3.scatter(x_cr, y_nsga, marker='^', c='b', s=50,label='PADC')
    ax3.set_xlabel("Generation")
    ax3.set_ylabel("Constraint(ms)")
    ax3.legend()
    plt.savefig(args.path+"/node_"+str(node_sum)+"/figure.png",dpi=300)
    plt.close()

