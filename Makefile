export LD_LIBRARY_PATH:=.
CC = gcc
FLAGS = -Wall
LIBFLAGS = --shared -fPIC
EFLAGS = -ldl
RM = rm -f 

.PHONY: all parta partb partc default clean

all: parta partb partc
	
default: all

parta: cmp copy
			$(CC) $(FLAGS) cmp.c -o cmp.o
			$(CC) $(FLAGS) copy.c -o copy.o


partb:	
			gcc --shared -fPIC codecA.c -o codecA.so
			gcc --shared -fPIC codecB.c -o codecB.so
			$(CC) encode.c $(EFLAGS) -o encode -Wl,-rpath,.
			$(CC) decode.c $(EFLAGS) -o decode -Wl,-rpath,.
			
			
partc: stshell
			$(CC) $(FLAGS) stshell.c -o stshell.o


clean:
	$(RM) *.o *.a *.so *.dll *.dylib cmp copy encode decode stshell