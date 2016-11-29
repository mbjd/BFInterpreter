EXEC=bf

$(EXEC): bf.c
	gcc -std=gnu99 -Wall -Wextra -pedantic -o bf bf.c

clean:
	rm -f $(EXEC)
