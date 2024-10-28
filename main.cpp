#include <WinSock2.h>
#include "headers/client.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>

using namespace std; 

int main() {

	/*int roomNum;
	unordered_map<int, int> portMaps;
	portMaps[1] = 55555;
	portMaps[2] = 400;

	cout << "---Pick a room to join---" << endl;
	cout << "1. Room 1" << endl;
	cout << "2. Room 2" << endl;
	cout << "ENTER ROOM NUMBER: ";
	cin >> roomNum;
	


	clnt::Client client(portMaps[roomNum]);
	*/
	clnt::Client client;
	client.start();

	return 0;
}