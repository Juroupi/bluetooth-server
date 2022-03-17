#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "bluetooth_server.h"

int bluetoothServer(int channel, int (*onConnection)(BluetoothClient client)) {
	
	int server = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if (server < 0) {
		return 0;
	}
	
	struct sockaddr_rc addr = { 0 };
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_bdaddr = *BDADDR_ANY;
	addr.rc_channel = channel;

	int b = bind(server, (struct sockaddr*)&addr, sizeof(struct sockaddr_rc));
	if (b < 0) {
		close(server);
		return 0;
	}
	
	int l = listen(server, 10);
	if (l < 0) {
		close(server);
		return 0;
	}
	
	int loop = 1;
	
	do {
		
		BluetoothClient client = accept(server, NULL, NULL);
		if (client < 0) {
			close(server);
			return 0;
		}

		loop = (*onConnection)(client);
		
		close(client);
		
	} while (loop);
	
	close(server);
	
	return 1;
}

int receiveData(BluetoothClient client, void* data, int size) {
	
	if (size <= 0) {
		return 0;
	}
	
	int n = read(client, data, size);
	if (n < 0) {
		return -1;
	}
	
	return n;
}

int receiveString(BluetoothClient client, char* str, int size) {
	
	int n = receiveData(client, str, size - 1);
	if (n < 0) {
		return -1;
	}
	
	str[n] = '\0';
	
	return n;
}

int sendData(BluetoothClient client, const void* data, int size) {
	
	if (size <= 0) {
		return 0;
	}
	
	int n = write(client, data, size);
	if (n < 0) {
		return -1;
	}
	
	return n;
}

int sendString(BluetoothClient client, const char* str) {
	return sendData(client, str, strlen(str));
}
