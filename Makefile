output: myserver.o myclient.o
	gcc myserver.o -o myserver -lpthread
	gcc myclient.o -o myclient -lpthread
myserver.o: myserver.c
	gcc -c myserver.c -lpthread
myclient.o: myclient.c
	gcc -c myclient.c -lpthread

clean:
	rm *.o output

	