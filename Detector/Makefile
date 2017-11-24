CC=g++
CFLAGS=-Wall

LIBVAR = -lopencv_core \
	-lopencv_highgui \
	-lopencv_imgproc 

TARGET=test
SRCS = test.cpp \
  Detect.cpp \
  pole.cpp
INC = -I/usr/local/include
LIB = -L/usr/local/lib
OBJS = $(SRCS:.cpp=.o)
$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(INC) $(LIB) $(LIBVAR)
	rm -rf $(OBJS)
%.o:%.cpp
	$(CC) $(CFLAGS) $(INC)  -o $@ -c $<  $(LIB) $(LIBVAR)
clean:
	rm -rf $(TARGET) $(SOTARGET)

