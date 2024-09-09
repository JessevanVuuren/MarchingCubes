CC = gcc
SHELL=cmd


CFLAGS = -I./lib/raylib/
LDFLAGS = -L./lib/raylib/

LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

OUTPUT = build

SRC = main.c

TARGET = main

# Target to compile the program
$(TARGET): $(SRC)
	if not exist $(OUTPUT) mkdir $(OUTPUT)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)/$(TARGET) $(LDFLAGS) $(LIBS)

# Clean up build files
clean:
	rmdir $(OUTPUT) /s /q

