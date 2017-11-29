from naoqi import ALProxy
import vision_definitions

import sys

if __name__ == "__main__":
    if len(sys.argv)<2:
        print "IP?"
        exit(1)
    IP=sys.argv[1]
    PORT=9559

    mem=ALProxy("ALMemory",IP,PORT)
    my_ins=ALProxy("njunaoModule",IP,PORT);
    mem.insertData("njunaoBallPositionStopFlag",1)
    mem.insertData("njunaoPolePositionStopFlag",1)
    my_ins.unRegisterFromVideoDevice();
