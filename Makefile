CC = gcc
CFLAGS = -I./include -I./deps/cjson/include -I./deps/libyaml/include -Wall -Wextra
LDFLAGS = -L./deps/cjson -L./deps/libyaml -lcjson -lyaml

SRCS = src/main.c src/skills.c src/mcp_utils.c
OBJS = $(SRCS:.c=.o)
TARGET = hermes-mcp-server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
