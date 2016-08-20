CXX       = g++
CPP_FILES = $(wildcard *.cpp)
OBJS      = $(patsubst %.cpp,%.o,$(CPP_FILES))
CXXFLAGS  = -std=c++11 -O2 -Wall -Werror -pedantic-errors -fmessage-length=0
TARGET    = one2one

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
