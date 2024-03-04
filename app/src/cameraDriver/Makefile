CC = arm-linux-gnueabihf-g++

all: capture runCamera

capture: capture.cpp
	$(CC) capture.cpp -o capture

runCamera: runCamera.cpp
	$(CC) runCamera.cpp -o runCamera

.PHONY: clean

clean:
	rm -f capture runCamera *.mp4 *.raw
