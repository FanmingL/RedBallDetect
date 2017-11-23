# 指令编译器和选项
CC=g++
CFLAGS=-Wall
LIBVAR = -lopencv_core \
	-lopencv_highgui \
	-lopencv_imgproc \
	-lopencv_imgcodecs
# 目标文件
TARGET=test
SOTARGET=libredballdetector.so
SRCS = test.cpp \
  Detect.cpp 
INC = -I/usr/local/include
LIB = -L/usr/local/lib
OBJS = $(SRCS:.cpp=.o)
$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(INC) $(LIB) $(LIBVAR)
	rm -rf $(OBJS)
%.o:%.cpp
	$(CC) $(CFLAGS) $(INC)  -o $@ -c $< 
clean:
	rm -rf $(TARGET) $(SOTARGET)
