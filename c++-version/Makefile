all:
	g++ -ggdb `pkg-config --cflags opencv4` -o `basename edge_detection.cpp .cpp` edge_detection.cpp `pkg-config --libs opencv4`
	./edge_detection
