all: producerA producerB consumerA consumerB consumerC main remove_mem

producerA:
	gcc -pthread -Wall -o pa producerA.c -lrt
	
producerB:
	gcc -pthread -Wall -o pb producerB.c -lrt

consumerA:
	gcc -pthread -Wall -o ca consumerA.c -lrt

consumerB:
	gcc -pthread -Wall -o cb consumerB.c -lrt

consumerC:
	gcc -pthread -Wall -o cc consumerC.c -lrt

main:
	gcc -pthread -Wall -o prog main.c -lrt

remove_mem:
	gcc -pthread -Wall -o rmem remove_mem.c -lrt

clean:
	rm ca cb cc pa pb prog rmem
