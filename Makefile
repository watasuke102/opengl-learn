.PHONY: all b r

l:
	ninja -C build && cp -v build/libglesdraw.so ../xreal-sandbox/Assets/Plugins/

all: b r

b:
	ninja -C build

r:
	./build/opengl-learn
