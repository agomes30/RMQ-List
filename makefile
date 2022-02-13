CXX = g++
CXXFLAGS = -Wall

driver: rmqlist.h driver.cpp
	$(CXX) $(CXXFLAGS) rmqlist.h driver.cpp -o driver

run:
	./driver

val:
	valgrind ./driver