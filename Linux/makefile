VPATH = src:include

CC = g++

LIBS = -lX11 -lGL -lGLEW

FLAGS = -g -c

all: main.o se_window.o se_opengl_driver.o
	$(CC) -g -o main main.o se_window.o se_opengl_driver.o $(LIBS) 

main.o:
	$(CC) $(FLAGS) -o main.o main.cpp

se_window.o:
	$(CC) $(FLAGS) -o se_window.o platform/SEWindow.cpp

se_opengl_driver.o:
	$(CC) $(FLAGS) -o se_opengl_driver.o render/SEOpenGLDriver.cpp

clean:
	rm -f *.o