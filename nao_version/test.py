from naoqi import ALProxy
import vision_definitions

import sys
import time

if __name__ == "__main__":
    if len(sys.argv)<2:
        print "IP?"
        exit(1)
    IP=sys.argv[1]
    PORT=9559

    my_ins=ALProxy("njunaoModule",IP,PORT);
    my_ins.registerToVideoDevice(vision_definitions.kVGA, vision_definitions.kBGRColorSpace)
    my_ins.njuModule.post.ContinuousRefreshCam()
    time.sleep(1)
    my_ins.post.ContinuousFindBall()
    my_ins.post.ContinuousFindPole()
