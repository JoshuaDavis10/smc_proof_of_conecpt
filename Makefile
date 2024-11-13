CC = gcc
CFLAGS = -O1 -Wall -std=c99 -Wno-missing-braces
LIBS = -lm -lraylib -lopengl32 -lgdi32 -lwinmm

SRC := src
OBJ := obj
BIN := demo.exe

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

$(BIN): $(OBJECTS)
	@echo linking...
	@$(CC) $^ -o $@ -I include/ -L lib/ $(LIBS)
	@echo compilation complete.

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	@echo compiling $<
	@$(CC) $(CFLAGS) -I$(SRC) -c $< -o $@

$(OBJ):
	@mkdir $(OBJ)

.PHONY: web
web: game.html

game.html: $(SOURCES)
	@emcc -o game.html $^ -Os -Wall C:/raylib-5.0/src/web/libraylib.a -I. -Iinclude -L. -LC:/raylib-5.0/src/web/libraylib.a -s USE_GLFW=3 --shell-file C:/raylib-5.0/src/minshell.html -DPLATFORM_WEB --preload-file res

.PHONY: clean
clean:
	@if exist $(OBJ) echo deleting obj files...
	@if exist $(OBJ) rmdir /s /q $(OBJ)
	@if exist $(BIN) echo deleting demo.exe...
	@if exist $(BIN) del $(BIN)
	@echo clean complete.
