all: compile_and_run

compile_and_run:
	/usr/bin/clang++ -std=c++17 -fcolor-diagnostics -fansi-escape-codes -Wall -g -I/Users/sahajbhakta/Documents/FluidSimGitlab/FluidSim/dependencies/include -L/Users/sahajbhakta/Documents/FluidSimGitlab/FluidSim/dependencies/library /Users/sahajbhakta/Documents/FluidSimGitlab/FluidSim/main.cpp /Users/sahajbhakta/Documents/FluidSimGitlab/FluidSim/src/*.cpp /Users/sahajbhakta/Documents/FluidSimGitlab/FluidSim/src/glad.c /Users/sahajbhakta/Documents/FluidSimGitlab/FluidSim/dependencies/library/libglfw.3.4.dylib -o /Users/sahajbhakta/Documents/FluidSimGitlab/FluidSim/app -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated
	./app