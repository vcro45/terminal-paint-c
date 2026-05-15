CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
SRC = src/main.c src/canvas.c src/commands.c
TARGET = paint

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
