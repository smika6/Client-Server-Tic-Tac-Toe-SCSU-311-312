Makefile:

all: serverC serverG client run

serverC: ServerC.c
	g++ ServerC.c -o serverC -pthread -std=c++0x

serverG: ServerG.c
	g++ ServerG.c -o serverG -std=c++0x

client: Client.c
	g++ Client.c -o client -std=c++0x

run: serverC
	./serverC

clean:
	rm -f serverC
	rm -f serverG
	rm -f client
