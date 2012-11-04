argle: main.o module_map.o scan.o
	g++ -o $@ main.o module_map.o scan.o

clean:
	rm -f argle *.o
