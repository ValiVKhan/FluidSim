#!/bin/bash

/usr/bin/clang++ -std=c++17 -fcolor-diagnostics -fansi-escape-codes -Wall -g -I./dependencies/include -L./dependencies/library ./main.cpp ./src/ParticleManager.cpp ./src/Wall.cpp ./src/glad.c ./dependencies/library/libglfw.3.4.dylib -o ./app -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated