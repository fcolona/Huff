OUT = build
CC = g++

main:
	$(CC) main.cpp node.cpp frequencies.cpp encodings.cpp -I libs/boost_1_87_0 -o $(OUT)/main
	./build/main
	
.PHONY: build
build:
	$(CC) main.cpp node.cpp frequencies.cpp encodings.cpp -I libs/boost_1_87_0 -o $(OUT)/build
	
debug:
	$(CC) -g main.cpp node.cpp frequencies.cpp encodings.cpp -I libs/boost_1_87_0 -o $(OUT)/debug
	gdb build/debug

clean:
	rm -r $(OUT)/*