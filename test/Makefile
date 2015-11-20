CXXFLAGS = -Wall -Wextra -pthread -std=c++11 -O2
INCLUDE = ./

SOURCES = $(wildcard *.cpp)
GTEST_SRC = $(wildcard ./gtest/*.cc)
OBJS  = $(addprefix ./, $(SOURCES:.cpp=.o))
OBJS  += $(addprefix , $(GTEST_SRC:.cc=.o))

TARGET = test.out

$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) -I $(INCLUDE) $(OBJS) -o $(TARGET)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I $(INCLUDE) -o $@ -c $<
%.o: %.cc
	$(CXX) $(CXXFLAGS) -I $(INCLUDE) -o $@ -c $<

clean:
	- rm -rf $(OBJS) $(TARGET)
