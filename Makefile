.PHONY: all b r

all: b r

b:
	clear && ninja -C build

r:
	gdb -q --batch \
		-ex "set print thread-events off" \
		-ex "run" -ex "bt" --args ./build/opengl-learn
