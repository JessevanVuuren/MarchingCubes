CC = gcc

CFLAGS = -I./lib/raylib/
LDFLAGS = -L./lib/raylib/

LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

FLAGS = -Werror

OUTPUT = build

SRC = main.c

TARGET = main

# Target to compile the program
$(TARGET): $(SRC)
	-mkdir $(OUTPUT)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)/$(TARGET) $(LDFLAGS) $(LIBS) $(FLAGS)

# Clean up build files
clean:
	-rm -fr $(OUTPUT)



# windows
# SHELL=cmd.exe /Q
# if not exist $(OUTPUT) mkdir $(OUTPUT)
# rmdir $(OUTPUT) /s /q