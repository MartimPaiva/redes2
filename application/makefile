COMPILER = gcc
EXTENSION = .cpp
COMPILER_ADDIONAL_FLAGS = -g -Wall -fstack-protector -O3

default: url.o my_socket.o
	$(COMPILER) $(COMPILER_ADDIONAL_FLAGS) main$(EXTENSION) url.o my_socket.o -o main_application
	rm *.o
url.o:
	$(COMPILER) -c $(COMPILER_ADDIONAL_FLAGS) url$(EXTENSION)

my_socket.o:
	$(COMPILER) -c $(COMPILER_ADDIONAL_FLAGS) my_socket$(EXTENSION)
