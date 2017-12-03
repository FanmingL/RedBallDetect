
把autoload.ini 放入nao的/home/nao/naoqi/preferences文件夹下
把libnjunaoModule.so放入 /home/nao/naoqi/lib 文件夹下然后重启。


文件夹findBallModule里的是用框图去调用识别程序的例子，会主动去寻找球。

要开始找球可以像findballnew框里面一样调用函数
要停止找球endfind框里面一样
要切换摄像头可以像selectcam一样

由njuFindBall事件发出的数据有15位，0、1位为红球在图像中的相对值（范围0-1），2、3位为红球位置所对应的与摄像头中心的夹角，4、5、6位为当前摄像头的[x y z]坐标，7、8、9位为当前摄像头的旋转坐标[roll pitch yaw]，10，11位是小球在机器人坐标系（ROBOT_FRAME）下的坐标，12、13、14位是头的[roll pitch yaw]值

一定要是红球，一定要放在绿色地上。


