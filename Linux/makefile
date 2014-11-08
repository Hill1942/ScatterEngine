VPATH = src:include

CC = g++

LIBS = -lX11 -lGL -lGLEW

FLAGS = -g -c

all: main.o se_window.o se_opengl_driver.o se_math.o se_perspective.o se_loader.o
	$(CC) -g -o main main.o se_window.o se_opengl_driver.o se_math.o se_perspective.o se_loader.o $(LIBS) 

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

se_loader.o: util/SELoader.h
	$(CC) $(FLAGS) -o se_loader.o util/SELoader.cpp

clean:
	rm -f *.o