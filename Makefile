all: tinyxml2.o tempo.o texto.o util.o obstaculo.o tiro.o arena.o inimigo.o jogador.o jogo.o
	g++ -Wall -o trabalhocg trabFinal.cpp tinyxml2.o tempo.o texto.o util.o obstaculo.o tiro.o arena.o inimigo.o jogador.o jogo.o -lGL -lGLU -lglut -lm
	@echo "Done."

tinyxml2.o: tinyxml2.cpp tinyxml2.h
	g++ -c tinyxml2.cpp

tempo.o: tempo.cpp tempo.h
	g++ -c tempo.cpp

texto.o: texto.cpp texto.h
	g++ -c texto.cpp

util.o: util.cpp util.h
	g++ -c util.cpp

obstaculo.o: obstaculo.cpp obstaculo.h
	g++ -c obstaculo.cpp

tiro.o: tiro.cpp tiro.h
	g++ -c tiro.cpp

arena.o: arena.cpp arena.h
	g++ -c arena.cpp

inimigo.o: inimigo.cpp inimigo.h
	g++ -c inimigo.cpp

jogador.o: jogador.cpp jogador.h
	g++ -c jogador.cpp

jogo.o: jogo.cpp jogo.h
	g++ -c jogo.cpp

clean:
	@rm -f trabalhocg *.o
