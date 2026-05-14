NAME = codexion

SRC = main.c parser.c coders.c

CC = cc

ARGS = 5 500 500 500 500 20 250 fifo

all: $(NAME)

$(NAME): 
	$(CC) $(SRC) -o $(NAME)

run: clean $(NAME)
	./$(NAME) $(ARGS)

clean:
	- rm codexion
