#! /usr/bin/python2
# coding=utf-8
#================================================================
#   Copyright (C) 2018 * Ltd. All rights reserved.
#   
#   File name   : golftest.py
#   Author      : FanmingL
#   Created date: 2018-11-21 13:50:20
#   Description : 
#
#================================================================

from naoqi import ALProxy
import time

if __name__ == '__main__':
    ip = "172.16.1.244"
    sss = ALProxy("naogolf", ip, 9559)
    if sss.getExecuteStatus():
        sss.stopExecute()
        time.sleep(0.5)
    sss.loadParameter()
    sss.execute(-1)
    time.sleep(0.5)
    for i in range(10):
        name = str(i) + '.jpg'
        sss.saveImage(name)
        time.sleep(0.1)
    sss.stopExecute()
