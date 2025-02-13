OUT = build
CC = g++

main:
	$(CC) main.cpp -o $(OUT)/main
	./build/main
	
build:
	$(CC) main.cpp -o $(OUT)/main

clean:
	rm -r $(OUT)/*