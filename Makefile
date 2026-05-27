NAME = codexion

SRC = source/main.c \
	  source/parser.c \
	  source/coders.c \
	  source/queue.c \
	  source/queue_utils.c \
	  source/dongles.c \
	  source/monitor.c \
	  source/system.c \
	  source/utils.c \
	  source/coders_utils.c \
	  source/dongles_utils.c

CC = cc

CCARGS = -Wall -Wextra -Werror -pthread

ARGS = 5 600 100 100 100 10 50 edf

all: $(NAME)

$(NAME): 
	$(CC) $(CCARGS) $(SRC) -o $(NAME)

run: clean $(NAME)
	./$(NAME) $(ARGS)

clean:
	- rm codexion

fclean:
	- rm -f codexion

re: fclean all
