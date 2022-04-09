program_name = main
all: 
	gcc *.c -o $(program_name) 
debug: 
	gcc -g *.c -o $(program_name) 
clean:
	 rm -f $(program_name) rm -f *.o
