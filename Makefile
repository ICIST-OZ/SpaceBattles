COMPILADOR = g++.exe

ARCHIVOS = $(wildcard *.cpp) $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)

CABEZERAS = $(wildcard *.hpp) $(wildcard src/*.hpp) $(wildcard src/*/*.hpp) $(wildcard src/*/*/*.hpp)

LIB_PATH = C:/SDL2/i686-w64-mingw32

CFLAGS = -std=c++17 -O2 -I "$(LIB_PATH)/include/SDL2"

LFLAGS = -L "$(LIB_PATH)/lib" -lmingw32 -mwindows -mconsole -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

MACROS = #-D RECTANGULOS

PROG = ./main

$(PROG) : $(ARCHIVOS) $(CABEZERAS)
	$(COMPILADOR) $(CFLAGS) $(MACROS) $(ARCHIVOS) -o $(PROG) $(LFLAGS)

correr:
	$(PROG)