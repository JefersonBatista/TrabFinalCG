all: tinyxml2 tempo util obstaculo tiro arena inimigo jogador jogo trabFinal
	@echo "Done."

tinyxml2: tinyxml2.cpp
	g++ -c tinyxml2.cpp

tempo: tempo.cpp
	g++ -c tempo.cpp

util: util.cpp
	g++ -c util.cpp

obstaculo: obstaculo.cpp
	g++ -c obstaculo.cpp

tiro: tiro.cpp
	g++ -c tiro.cpp

arena: arena.cpp
	g++ -c arena.cpp

inimigo: inimigo.cpp
	g++ -c inimigo.cpp

jogador: jogador.cpp
	g++ -c jogador.cpp

jogo: jogo.cpp
	g++ -c jogo.cpp

trabFinal: tinyxml2.o tempo.o util.o obstaculo.o tiro.o arena.o inimigo.o jogador.o jogo.o trabFinal.cpp
	g++ -Wall -o trabalhocg trabFinal.cpp tinyxml2.o tempo.o util.o obstaculo.o tiro.o arena.o inimigo.o jogador.o jogo.o -lGL -lGLU -lglut -lm

clean:
	@rm -f trabalhocg *.o
