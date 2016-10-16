CXX       = g++
SRC       =  top.cpp interp.cpp parser.cpp error_handling.cpp environment.cpp
OBJS      = $(SRC:.cpp=.o)
CXXFLAGS  = -std=c++11 -O2 -Wall -Werror -pedantic-errors -fmessage-length=0 -I ../include
TARGET    = one2one

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS)
fclean:
	rm -f $(TARGET) $(TARGET).exe
re: fclean all
