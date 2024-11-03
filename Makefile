.PHONY: all deploy clean
UNITY_DIR := $(HOME)/Unity/Hub/Editor/2022.3.11f1
CXXFLAGS := -Iinc -I$(UNITY_DIR)/Editor/Data/PluginAPI -shared -fno-exceptions -fno-rtti -undefined dynamic_lookup -g
SRCS     := $(wildcard src/*.cpp)

all: deploy

deploy: build/libglesdraw.so build/libs/arm64-v8a/libglesdraw_android.so
	@cp -v $^ ../xreal-sandbox/Assets/Plugins

build/libglesdraw.so: $(SRCS)
	clang++ $(CXXFLAGS) -o $@ $^

build/libglesdraw_android.so: $(SRCS)
	$(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android29-clang++ $(CXXFLAGS) -o $@ $<

build/libs/arm64-v8a/libglesdraw_android.so: $(SRCS) $(wildcard build/jni/*)
	cd build/jni && $(NDK)/ndk-build

clean:
	rm -vf build/libglesdraw.so build/libglesdraw_android.so build/libs/arm64-v8a/libglesdraw_android.so

build/libglesdraw.a: $(SRCS)
	clang++ $(CXXFLAGS) -c -o build/a.o $<
	ar rcs $@ build/a.o
