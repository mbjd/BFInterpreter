EXEC=bf

$(EXEC): bf.c
	gcc -std=c99 -Wall -Wextra -pedantic -o bf bf.c

clean:
	rm -f $(EXEC)
