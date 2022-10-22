.PHONY: all b r

all: b r

b:
	clear && ninja -C build

r:
	./build/opengl-learn
