NAME = codexion

SRC = main.c parser.c coders.c queue.c dongles.c

CC = cc

CCARGS = -Wall -Wextra -Werror -pthread -fsanitize=thread 

ARGS = 5 500 500 500 500 5 250 edf

all: $(NAME)

$(NAME): 
	TSAN_OPTIONS=second_deadlock_stack=1; $(CC) $(CCARGS) $(SRC) -o $(NAME)

run: clean $(NAME)
	./$(NAME) $(ARGS)

clean:
	- rm codexion
