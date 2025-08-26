CC = gcc
TARGET = stack
SRCS = main.c

build:
	$(CC) -o $(TARGET) $(SRCS) -Wall -Wextra

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: build run clean
