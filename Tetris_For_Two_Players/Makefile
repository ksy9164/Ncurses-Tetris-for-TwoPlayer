CC = g++
TARGET = tetris
SOURCES = Tetris.cpp Pane.cpp BoardPane.cpp InfoPane.cpp EndPane.cpp StatePane.cpp NextPane.cpp main.cpp TimerPane.cpp
LDFLAGS = -lncurses -std=c++11
all:
	$(CC) -o  $(TARGET) $(SOURCES) $(LDFLAGS)
clean:
	rm -rf *.o $(TARGET)

