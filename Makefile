.PHONY: all clean
CXXFLAGS := -Iinc -shared -fPIC -fno-exceptions -fno-rtti -undefined dynamic_lookup

all: build/libglesdraw_android.so build/libglesdraw.so

build/libglesdraw.so: src/lib.cpp
	clang++ $(CXXFLAGS) -o $@ $<
	@cp -v $@ ../xreal-sandbox/Assets/Plugins

build/libglesdraw_android.so: src/lib.cpp
	$(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android26-clang++ $(CXXFLAGS) -o $@ $<
	@cp -v $@ ../xreal-sandbox/Assets/Plugins

clean:
	rm -vf build/libglesdraw.so build/libglesdraw_android.so
