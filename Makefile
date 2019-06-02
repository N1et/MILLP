CFLAGS := -std=gnu99 
main: main.c ./lib/mem.c
clean:
	rm *.o
