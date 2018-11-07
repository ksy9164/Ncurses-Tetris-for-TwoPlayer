CC = g++
TARGET = tetris
SOURCES = Tetris.cpp Pane.cpp BoardPane.cpp InfoPane.cpp EndPane.cpp StatePane.cpp NextPane.cpp main.cpp TimerPane.cpp
LDFLAGS = -lncurses -std=c++11
all:
	g++ -o  tetris Tetris.cpp Pane.cpp BoardPane.cpp InfoPane.cpp EndPane.cpp StatePane.cpp NextPane.cpp main.cpp TimerPane.cpp -lncurses -std=c++11
	$(CC) -o  $(TARGET) $(SOURCES) $(LDFLAGS)
clean:
	rm -rf *.o $(TARGET)
