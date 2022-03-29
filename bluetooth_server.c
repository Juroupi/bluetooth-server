#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "bluetooth_server.h"

typedef struct _ConnectionInfos {
	int* loop;
	int (*onConnection)(BluetoothClient client);
	BluetoothClient client;
} ConnectionInfos;

static void* onConnectionProc(void* args) {
	
	ConnectionInfos* infos = args;
	
	if (!(*infos->onConnection)(infos->client)) {
		*infos->loop = 0;
	}

	close(infos->client);
	
	return NULL;
}

int bluetoothServer(int channel, int (*onConnection)(BluetoothClient client), int inThreads) {
	
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
		
		ConnectionInfos infos;
		infos.loop = &loop;
		infos.onConnection = onConnection;
		infos.client = client;

		if (inThreads) {
			pthread_t threadId;
			pthread_create(&threadId, NULL, &onConnectionProc, &infos);
		} else {
			onConnectionProc(&infos);
		}
		
	} while (loop);
	
	close(server);
	
	return 1;
}

int receiveData(BluetoothClient client, void* data, int size) {
	
	if (size <= 0) {
		return 0;
	}
	
	int n = recv(client, data, size, 0);
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
	
	int n = send(client, data, size, 0);
	if (n < 0) {
		return -1;
	}
	
	return n;
}

int sendString(BluetoothClient client, const char* str) {
	return sendData(client, str, strlen(str));
}
