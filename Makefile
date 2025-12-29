game: main.c logic.c render.c main.h logic.h render.h
	gcc -g  -I./SDL2-2.0.10/include -L. -o game main.c logic.c render.c -lSDL2 -lpthread -ldl -lrt -lm

run:
	@gcc -O2 -I./SDL2-2.0.10/include -L. -o basic basic.c -lSDL2 -lpthread -ldl -lrt -lm
	./basic

build:
	g++ -O2 -I./SDL2-2.0.10/include -L. -o szablon szablon.cpp -lm -lSDL2 -lpthread -ldl -lrt
	gcc -O2 -I./SDL2-2.0.10/include -L. -o basic basic.c -lSDL2 -lpthread -ldl -lrt -lm
	gcc -O2 -I./SDL2-2.0.10/include -L. -o ataki ataki.c -lSDL2 -lpthread -ldl -lrt -lm
	gcc -O2 -I./SDL2-2.0.10/include -L. -o combo combo.c -lSDL2 -lpthread -ldl -lrt -lm
	gcc -O2 -I./SDL2-2.0.10/include -L. -o hitboxy hitboxy.c -lSDL2 -lpthread -ldl -lrt -lm
	gcc -O2 -I./SDL2-2.0.10/include -L. -o animacje animacje.c -lSDL2 -lpthread -ldl -lrt -lm
	gcc -O2 -I./SDL2-2.0.10/include -L. -o example_window example_window.c -lSDL2 -lpthread -ldl -lrt -lm

