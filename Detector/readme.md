用make命令即可编译
MAC下可以编译，
UBUNTU似乎会报错报错说少库
这种情况就在makefile的-lopencv_imgproc后面加一个\然后在下面加一行-lopencv_imgcodecs
