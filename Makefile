CXX = g++
CPPFLAGS = -fmax-errors=5 -lstdc++ -std=c++17 -DGLEW
LIBS = -lGLEW -lglfw -lGL -I/usr/include/freetype2 -lfreetype -lpng -lavcodec -lavutil -lavformat -lva
VPATH = ./src ./src/transform
OBJ = shader.o 										\
	  juicer.o 										\
	  buffer.o vertex-buffer.o element-buffer.o 	\
	  vertex-array.o 								\
	  transform.o model.o projection.o view.o		\
	  vertex-pool.o										\
	  geometry-data.o geometry.o cube.o

all: 		main
%.o:		%.cpp
			$(CXX) -c -o $@ $< $(CPPFLAGS) $(LIBS)
main:		$(OBJ) 
			$(CXX) -o run src/main.cpp $^ $(CPPFLAGS) $(LIBS)
.PHONY:		clean
clean:
			rm $(OBJ)
