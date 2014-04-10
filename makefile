CC=g++
CFLAGS=-Wall
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -Wall
EXEC=convert
OBJ=main.o

%.o : %.cpp %.h
	$(CC) -c $*.cpp $(CFLAGS)

main.o :
	$(CC) -c main.cpp $(CFLAGS)  

convert : $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) 
	rm -rf *.o

clean:
	rm -rf *.o
