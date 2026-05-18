NAME = codexion

SRC = main.c parser.c coders.c queue.c

CC = cc

CCARGS = -Wall -Wextra -Werror -fsanitize=thread

ARGS = 5 500 500 500 500 20 250 fifo

all: $(NAME)

$(NAME): 
	$(CC) $(CCARGS) $(SRC) -o $(NAME)

run: clean $(NAME)
	./$(NAME) $(ARGS)

clean:
	- rm codexion
