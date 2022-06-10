all_objects = $(wildcard *.o)
LIB_DIR = libs
objects_list = info.o my_socket.o
CXX = -g -Wall -fstack-protector #-O3

APP_NAME = program
COMPILER = g++
EXTENSION = .cpp
COMPILER_ADDIONAL_FLAGS = 
VALGRIND_FLAGS = --tool=memcheck --track-origins=yes --leak-check=full

file = pub.txt
ftp_add = ftp://speedtest.tele2.net/pipe.txt

all:$(APP_NAME)

$(APP_NAME): %.o: %.cpp
	$(COMPILER) $(COMPILER_ADDIONAL_FLAGS) main$(EXTENSION) info.o my_socket.o -o $(APP_NAME)
	rm *.o
	clean
# info.o: info.cpp
# 	clear
# 	$(COMPILER) -c $(COMPILER_ADDIONAL_FLAGS) info$(EXTENSION)

# my_socket.o:
# 	clear
# 	$(COMPILER) -c $(COMPILER_ADDIONAL_FLAGS) my_socket$(EXTENSION)

pipe.txt: $(APP_NAME)
	clear
	./$(APP_NAME) ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt

pub.txt: $(APP_NAME)
	clear
	./$(APP_NAME) ftp://netlab1.fe.up.pt/pub.txt

val: $(APP_NAME)
	valgrind $(VALGRIND_FLAGS) ./$(APP_NAME) $(ftp_add)

gdb: $(APP_NAME)	
	gdb ./$(APP_NAME) $(ftp_add)

clean:
	rm $(APP_NAME)

