NAME = emission
CC ?= gcc
CFLAGS += -Wall -W -Wextra -std=gnu99
CPPFLAGS = -I/usr/local/include -L/usr/local/lib
LDFLAGS = -lwiringPi

SRC = emission.c

OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ)
	$(CC) -o "$@" $^ $(LDFLAGS)

DEP = $(SRC:.c=.d)
-include $(DEP)

%.d: %.c
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $< -MF $@ -MT "$*.o $@"

clean:
	$(RM) $(OBJ)

mrproper: clean
	$(RM) $(NAME)
	$(RM) $(DEP)


.PHONY: clean mrproper all
