# 指令编译器和选项
CC=g++
CFLAGS=-Wall -std=c++11
 
# 目标文件
TARGET=code
SRCS = main.cpp 
 
OBJS = $(SRCS:.c=.o)
 
$(TARGET):$(OBJS)
	g++ main.cpp -o code -std=c++11
#	@echo TARGET:$@
#	@echo OBJECTS:$^
#	$(CC) -o $@ $^
 
clean:
	rm -rf code

