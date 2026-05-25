NAME = codexion

SRC = main.c parser.c coders.c queue.c dongles.c orchestrator.c monitor.c system.c utils.c coders_utils.c dongles_utils.c

CC = cc

CCARGS = -Wall -Wextra -Werror -pthread

ARGS = 5 403 100 100 100 10 50 edf

all: $(NAME)

$(NAME): 
	$(CC) $(CCARGS) $(SRC) -o $(NAME)

run: clean $(NAME)
	./$(NAME) $(ARGS)

clean:
	- rm codexion
