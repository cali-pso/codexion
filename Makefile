NAME = codexion

SRC = main.c parser.c coders.c queue.c dongles.c

CC = cc

CCARGS = -Wall -Wextra -Werror -pthread -fsanitize=thread 

ARGS = 5 1000 199 199 199 10 25 fifo

all: $(NAME)

$(NAME): 
	TSAN_OPTIONS=second_deadlock_stack=1; $(CC) $(CCARGS) $(SRC) -o $(NAME)

run: clean $(NAME)
	./$(NAME) $(ARGS)

clean:
	- rm codexion
