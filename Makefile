CC = g++
TARGET = tetris
SOURCES = ./src/Tetris.cpp ./src/Pane.cpp ./src/BoardPane.cpp ./src/InfoPane.cpp ./src/EndPane.cpp ./src/StatePane.cpp ./src/NextPane.cpp ./src/main.cpp ./src/TimerPane.cpp
LDFLAGS = -lncurses -std=c++11
all:
	$(CC) -o  $(TARGET) $(SOURCES) $(LDFLAGS)
clean:
	rm -rf *.o $(TARGET)
