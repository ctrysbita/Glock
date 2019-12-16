CXX = g++

c_objects := $(patsubst %.c,%.o,$(wildcard *.c))
cc_objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(c_objects) $(cc_objects)
	$(CXX) $(c_objects) $(cc_objects) -lglfw3 -o main

%.o: %.c
	$(CXX) -g -I. -I../include -std=c++17 -c $< -o $@

%.o: %.cpp
	$(CXX) -g -I. -I../include -std=c++17 -c $< -o $@

clean:
	rm *.o