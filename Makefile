map2: main.o read.o ll.o dict.o utils.o data.o k-d_tree.o
	gcc -Wall -o map2 main.o read.o ll.o dict.o utils.o data.o k-d_tree.o -g -lm

main.o: main.c read.h k-d_tree.h
	gcc -Wall -c -o main.o main.c -g -lm

read.o: read.c read.h
	gcc -Wall -c -o read.o read.c -g -lm
    
ll.o: ll.c ll.h
	gcc -Wall -c -o ll.o ll.c -g -lm

dict.o: dict.c dict.h
	gcc -Wall -c -o dict.o dict.c -g -lm

utils.o: utils.c utils.h
	gcc -Wall -c -o utils.o utils.c -g -lm

data.o: data.c data.h
	gcc -Wall -c -o data.o data.c -g -lm
	
k-d_tree.o: k-d_tree.c k-d_tree.h
	gcc -Wall -c -o k-d_tree.o k-d_tree.c -g -lm
