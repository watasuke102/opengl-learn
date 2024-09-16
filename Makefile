.PHONY: all deploy clean
CXXFLAGS := -Iinc -shared -fno-exceptions -fno-rtti -undefined dynamic_lookup

all: deploy

#build/libglesdraw_android.so
deploy: build/libglesdraw.so build/libs/arm64-v8a/libglesdraw_android.so
	@cp -v $^ ../xreal-sandbox/Assets/Plugins

build/libglesdraw.so: src/lib.cpp
	clang++ $(CXXFLAGS) -lGL -o $@ $<

build/libglesdraw_android.so: src/lib.cpp
	$(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android29-clang++ $(CXXFLAGS) -o $@ $<

build/libs/arm64-v8a/libglesdraw_android.so: src/lib.cpp
	cd build/jni && $(NDK)/ndk-build

clean:
	rm -vf build/libglesdraw.so build/libglesdraw_android.so build/libs/arm64-v8a/libglesdraw_android.so
