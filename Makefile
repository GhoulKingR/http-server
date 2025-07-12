main: main.c
	gcc main.c -o main

clean:
	rm main

run: main
	./main
