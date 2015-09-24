CC = mingw32-g++
INC = -I "C:\Libraries\glew-1.12.0source\include" -I "C:\Libraries\glfw-3.1.1\include" -I "C:\Libraries\glm"
LLIBS = -lglfw3 -lopengl32 -lgdi32 -lglew32 -lstdc++
LNK = -L "C:\Libraries\glew-1.12.0source\lib"
TARGET = test

test: test.o shader.o block.o piece.o board.o grid.o
	$(CC) -o $(TARGET) $(TARGET).o shader.o block.o grid.o board.o piece.o $(LNK) $(LLIBS) -Wl -g

test.o: test.cpp
	$(CC) -c -g $(TARGET).cpp $(INC)

shader.o: shader.cpp
	$(CC) -c -g shader.cpp $(INC)

block.o: block.cpp
	$(CC) -c -g block.cpp $(INC)

piece.o: piece.cpp
	$(CC) -c -g piece.cpp $(INC)

board.o: board.cpp
	$(CC) -c -g board.cpp $(INC)

grid.o: grid.cpp
	$(CC) -c -g grid.cpp $(INC)

clean:
	del *.o test.exe
