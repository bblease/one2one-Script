CXX       = g++
SRC       = top.cpp interp.cpp error_handling.cpp expression.cpp parser.cpp environment.cpp
HEADERS   = intero.h error_handling.h expression.h parser.h environment.h
OBJS      = $(SRC:.cpp=.o)
CXXFLAGS  = -std=c++11 -O2 -Wall -Werror -pedantic-errors -fmessage-length=0
TARGET    = one2one

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
