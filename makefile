CC=g++
SRC=main.cpp
TOO=main.o
FLASG=-Wall
TARGET=executable

main: $(TOO)
	$(CC) -o $(TARGET) $(TOO) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

main.o: main.cpp
	$(CC) -c $(SRC) $(FLASG)

clean:
	rm *.o

cleanall:
	rm *.o $(TARGET)