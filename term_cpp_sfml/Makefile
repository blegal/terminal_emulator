CC=clang++
CFLAGS= --stdlib=libc++ -m32 -O3 -march=native -mtune=native -msse4.1 -I./src/filters
LDFLAGS=-m32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
EXEC=main

SRC= \
	./src/CTimer/CTimer.cpp \
	./src/GL_Button.cpp \
	./src/SoundSystem.cpp \
	./src/main.cpp

OBJ= $(SRC:.cpp=.o)

all: $(EXEC)

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean mrproper

clean: $(OBJ)
	rm src/*.o
	rm src/filters/*.o
	rm ./src/filters/low_pass/*.o
	rm ./src/filters/high_pass/*.o
	rm ./src/filters/band_pass/*.o
	rm ./src/filters/band_stop/*.o
	rm ./src/filters/effects/*.o
	rm $(EXEC)

mrproper: clean
	rm $(EXEC)
