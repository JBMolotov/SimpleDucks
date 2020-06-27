all: clean
	@g++ -o main base.h ducks.c -lglut -lGLU -lGL -lm
run: all 
	./main
clean:
	rm main || true
