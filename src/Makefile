CC=g++
CFLAGS=-O3 -std=c++11
#CFLAGS=-std=c++11 -gdwarf-2

DEP=/usr/local
INC_PATH=../include $(DEP)/include
LIB_PATH=$(DEP)/lib
INC_FLAGS=$(foreach d, $(INC_PATH), -I$(d))
LD_FLAGS=$(foreach d, $(LIB_PATH), -L$(d))

SRCS=$(shell find ./ -name "*.cpp")
OBJS=${SRCS:.cpp=.o}

graph_coloring: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $^ $(LD_FLAGS) -DDEBUG
	-rm $(OBJS)

$(OBJS): %.o:%.cpp
	$(CC) -c $(CFLAGS) $(INC_FLAGS) -o $@ $< -DDEBUG

.PHONY: clean
clean:
	-rm $(OBJS)
	-rm graph_color
