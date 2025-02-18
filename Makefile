OUT = build
CC = g++

dev:
	$(CC) src/main.cpp src/node.cpp src/frequencies.cpp src/encodings.cpp -I libs/boost_1_87_0 -o $(OUT)/huff
	./build/huff
	
.PHONY: build
build:
	$(CC) -O3 src/main.cpp src/node.cpp src/frequencies.cpp src/encodings.cpp -I libs/boost_1_87_0 -o $(OUT)/huff
	
debug:
	$(CC) -g src/main.cpp src/node.cpp src/frequencies.cpp src/encodings.cpp -I libs/boost_1_87_0 -o $(OUT)/huff
	gdb build/huff

clean:
	rm -r $(OUT)/*