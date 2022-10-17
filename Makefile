.PHONY: all b r

all: b r

b:
	ninja -C build

r:
	./build/opengl-learn
