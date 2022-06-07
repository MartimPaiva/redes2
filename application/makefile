default: url.o my_socket.o
	gcc main.c url.o my_socket.o -o main_application
	rm *.o


url.o:
	gcc -c url.cpp

my_socket.o:
	gcc -c my_socket.cpp