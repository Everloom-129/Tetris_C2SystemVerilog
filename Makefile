CC = gcc
CFLAGS = -Wall -g
TARGET = tetris

all: $(TARGET)

$(TARGET): main.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	./$(TARGET)