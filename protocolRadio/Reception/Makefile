NAME = a.out
CXX ?= g++
CPPFLAGS =
LDFLAGS = -lwiringPi
CXXFLAGS = -Wall -W -Wextra

SRC = reception.cpp

OBJ = $(SRC:.cpp=.o)

$(NAME): $(OBJ)
	$(CXX) -o "$@" $^ $(LDFLAGS)

DEP = $(SRC:.cpp=.d)
-include $(DEP)

%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $< -MF $@ -MT "$*.o $@"

clean:
	$(RM) $(OBJ)

mrproper: clean
	$(RM) $(NAME)


.PHONY: clean mrproper all
