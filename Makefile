# 指令编译器和选项


# 目标文件
TARGET=code
SRCS = main.cpp

OBJS = $(SRCS:.c=.o)

$(TARGET):$(OBJS)
	g++ -c Memory.cpp -std=c++11 -Wall -Wextra -Wconversion -O2
	g++ -c MipsParser.cpp -std=c++11 -Wall -Wextra -Wconversion -O2
	g++ -c MipsSimulator.cpp -std=c++11 -Wall -Wextra -Wconversion -O2
	g++ -c main.cpp -std=c++11 -Wall -Wextra -Wconversion -O2
	g++ Memory.o MipsParser.o MipsSimulator.o main.o -o code -std=c++11 -Wall -Wextra -Wconversion -O2
# -O1 -Wall -Wextra -Wconversion
#	@echo TARGET:$@
#	@echo OBJECTS:$^
#	$(CC) -o $@ $^

clean:
	rm -rf code
