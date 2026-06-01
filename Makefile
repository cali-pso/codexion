NAME = codexion

SRC = source/parser.c \
	  source/coders.c \
	  source/queue.c \
	  source/queue_utils.c \
	  source/dongles.c \
	  source/monitor.c \
	  source/system.c \
	  source/utils.c \
	  source/coders_utils.c \
	  source/main.c \
	  source/dongles_utils.c \

OBJ = $(SRC:%.c=%.o)

CC = cc

CCARGS =-Wall -Wextra -Werror -pthread

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CCARGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CCARGS) -c $< -o $@

clean:
	- rm -f $(OBJ) 

fclean: clean
	- rm -f $(NAME)

re: fclean all
