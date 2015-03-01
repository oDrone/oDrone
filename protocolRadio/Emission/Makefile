NAME            = emission
SRC             = $(wildcard *.c)

CPPFLAGS        = -I /usr/local/include/
LDFLAGS         = -L /usr/local/lib -lwiringPi
CFLAGS          = -W -Wall -Wextra -std=gnu99

CC              = gcc

OBJ             = $(SRC:.c=.o)
DEP             = $(SRC:.c=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o "$@" $^ $(LDFLAGS)

-include $(DEP)

%.d: %.c
	$(CC) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT "$*.o $@"

clean:
	$(RM) $(OBJ)
	$(RM) $(NAME)

mrproper: clean
	$(RM) $(DEP)

distclean: mrproper
	$(RM) $(addsuffix ~,$(SRC))
	$(RM) $(wildcard $(addsuffix .sw*,$(addprefix .,$(SRC))))

.PHONY: all clean mrproper distclean

