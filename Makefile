# Projekt: tower defense
# Autor: Ondrej Kolonicny
# Verze: 1.1
# Datum: 5.11.2026


NAME = pr
OBJFILES = main.o map.o tower.o

CC = gcc
CFLAGS = -Wall -std=c99 -I. -I/opt/homebrew/include/SDL2		# kouka na .h soubory
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf	# ukaze cestu k LSDL

all: $(NAME)

# vezme vsechny .o soubory a prelozi jejich .c verzi
%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $@ $(LDFLAGS)

dep:
	$(CC) $(CFLAGS) -MM *.c > dep.list

-include dep.list

clean:
	rm -f $(OBJFILES) $(NAME) dep.list