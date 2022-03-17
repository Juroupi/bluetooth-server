main: main.c bluetooth_server.o
	gcc main.c bluetooth_server.o -o main

bluetooth_server.o: ../bluetooth_server.h ../bluetooth_server.c
	gcc -c ../bluetooth_server.c -o bluetooth_server.o -lbluetooth

exec: main
	./main