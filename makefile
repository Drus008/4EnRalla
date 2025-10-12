CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -I.
LDFLAGS = -lm

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
TARGET := 4enratlla

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)