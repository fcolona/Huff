OUT = build
CC = g++

main:
	$(CC) main.cpp node.cpp -o $(OUT)/main
	./build/main
	
build:
	$(CC) main.cpp node.cpp -o $(OUT)/build
	
debug:
	$(CC) -g main.cpp node.cpp -o $(OUT)/debug
	gdb build/debug

clean:
	rm -r $(OUT)/*