
把autoload.ini 放入nao的/home/nao/naoqi/preferences文件夹下
把libnjunaoModule.so放入 /home/nao/naoqi/lib 文件夹下然后重启。


运行test.py可以直接运行识别程序，两秒找一次，找着球之后会引起njuFindBall事件，事件的传出的数据为小球x,y占整幅图的百分比，比如 图片是1280*960 然后 球的像素坐标为 426,320那就会传出0.33，0.33。

运行test_stop.py可以终止识别程序把摄像头释放出来。

文件夹findBallModule里的是用框图去调用识别程序的例子，找到球就会喊 找着球了

一定要是红球，一定要放在绿色地上。