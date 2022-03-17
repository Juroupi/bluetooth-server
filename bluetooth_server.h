#ifndef _BLUETOOTH_SERVER_H_
#define _BLUETOOTH_SERVER_H_

typedef int BluetoothClient;

/**
 * Create a bluetooth RFCOMM server
 * onConnection() is called at each client connection
 * the server stops when onConnection() returns 0
 * @return 0 on error, another value otherwise
 */
int bluetoothServer(int channel, int (*onConnection)(BluetoothClient client));

/**
 * Receive data from client
 * @return the number of bytes received, a negative value on error
 */
int receiveData(BluetoothClient client, void* data, int size);

int receiveString(BluetoothClient client, char* str, int size);

/**
 * Send data to client
 * @return the number of bytes sent, a negative value on error
 */
int sendData(BluetoothClient client, const void* data, int size);

int sendString(BluetoothClient client, const char* str);

#endif
