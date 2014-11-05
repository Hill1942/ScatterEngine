VPATH = src:include

CC = g++

LIBS = -lX11 -lGL -lGLEW

FLAGS = -g -c

all: main.o se_window.o se_opengl_driver.o se_math.o se_perspective.o
	$(CC) -g -o main main.o se_window.o se_opengl_driver.o se_math.o se_perspective.o $(LIBS) 

main.o: main.cpp
	$(CC) $(FLAGS) -o main.o main.cpp

se_window.o: platform/SEWindow.cpp
	$(CC) $(FLAGS) -o se_window.o platform/SEWindow.cpp

se_opengl_driver.o: render/SEOpenGLDriver.cpp
	$(CC) $(FLAGS) -o se_opengl_driver.o render/SEOpenGLDriver.cpp

se_math.o: math/SEMath.cpp
	$(CC) $(FLAGS) -o se_math.o math/SEMath.cpp

se_perspective.o: SEPerspective.cpp
	$(CC) $(FLAGS) -o se_perspective.o SEPerspective.cpp

clean:
	rm -f *.o