NAME = codexion

SRC = main.c parser.c

CC = cc

all: $(NAME)

$(NAME): 
	$(CC) $(SRC) -o $(NAME)

run: $(NAME)
	./$(NAME)
