CC = g++
TARGET = code
SRCS =  Memory.cpp MipsParser.cpp MipsSimulator.cpp main.cpp
CXXFLAG =  -std=c++11 -O2 -pthread
OBJS = $(SRCS:.cpp=.o)

$(TARGET):$(OBJS)
	$(CC) $(CXXFLAG) $(OBJS) -o $(TARGET)

%.o:%.cpp %.h
	$(CC) $(CXXFLAG) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)
