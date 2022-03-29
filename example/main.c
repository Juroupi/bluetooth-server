#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../bluetooth_server.h"

static int onConnection(BluetoothClient client) {
	
	char str[512];
	
	puts("a client is connected");
	
	while (receiveString(client, str, sizeof(str)) >= 0) {
		
		sendString(client, "ok");
		
		if (strcmp(str, "stop") == 0) {
			puts("the client has stopped the server");
			return 0;
		}
		
		printf("received: %s\n", str);
	}
	
	puts("the client has disconnected");
	
	return 1;
}

int main() {
	
	if (!bluetoothServer(1, &onConnection, 0)) {
		puts("error");
		return 1;
	}
	
	return 0;
}
