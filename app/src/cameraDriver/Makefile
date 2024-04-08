CC = arm-linux-gnueabihf-g++

all: capture

capture: capture.cpp
	$(CC) capture.cpp -o capture

.PHONY: clean

clean:
	rm -f capture runCamera *.mp4 *.raw
