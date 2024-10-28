
#include "..\headers\client.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <iomanip>
#include <tchar.h>
#include <thread>

/*
	Steps for Client Functions

	1. Initialize WSA - WSAStartup()
	2. Create a socket - socket()
	3. Connect to the server - connect()
	4. Send and receive data - recv(), send(), recvfrom(), sendto().
	5. Disconnect - closesocket()

*/

using namespace std;

namespace clnt {
	Client::Client(){

		// port auto sets to port: 55,555
		port = 55555;
		allowedToSend = true;
	}
	Client::Client(int portNumber)
			: port(portNumber){
	}

	void Client::start(){

		// the created socket is stored in clientSocket
		SOCKET clientSocket;

		this->loadWSAdll();
		clientSocket = this->buildSocket();
		 
		if (this->connectToServer(port, clientSocket) == 0) {

			cerr << "Try and restart the client." << endl;
		}
		else {


			while (true) {

				std::thread receiveMessages([this, clientSocket]() {
					this->recieveFromServer(clientSocket);
					});
				receiveMessages.detach();

				this->sendData(clientSocket);
			}
			
		}
		this->stop(clientSocket);
	}

	void Client::stop(SOCKET socketToClose = NULL){

		WSACleanup();
		if (socketToClose != NULL) {

			closesocket(socketToClose);
		}
	}

	void Client::loadWSAdll(){
		 
		WSAData wsaData;
		int wsaErr;

		WORD wVersRequested = MAKEWORD(2, 2);			// version 2.2 of Winsock
		wsaErr = WSAStartup(wVersRequested, &wsaData);	// start up Winsock

		if (wsaErr == 0) {
			
			cout << "Winsock dll found!" << endl;
			cout << "Winsock dll status: " << wsaData.szSystemStatus << endl;
		}
		else {
			cerr << "Winsock dll was not found!" << endl;
			this->stop();
		}


	}

	SOCKET Client::buildSocket(){

		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (clientSocket != INVALID_SOCKET) {

			cout << "The client socket was built successfully!" << endl;
		}
		else {
			cerr << "Error building client socket: " << WSAGetLastError() << endl;
			this->stop(clientSocket);
		}

		return clientSocket;
	}

	// connect to the server socket
	int Client::connectToServer(int port, SOCKET clientSocket){

		sockaddr_in clientService;			// holds the address information of the server

		clientService.sin_family = AF_INET; // sets the address family to AF_NET = IPv4 address
		clientService.sin_port = htons(port);	// sets to the server port

		// puts the server address into clientService.sin_addr.s_add
		InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);

		if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {

			cerr << "Client failed to connect: " << WSAGetLastError() << endl;
			this->stop();
			return 0;
		}
		else {
			cout << "Client connected successfully! " << endl;
			cout << "Client can start sending and recieving data..." << endl;
		}

		return 1;

	}

	void Client::sendData(SOCKET clientSocket){

		/*
			send() function:

			- sends data on a connected socket

			int send(SOCKET s, const char *buf, int len, int flags)

			-s: the descriptor that identifies a connected socket.
			-buf: A pointer to the buffer to the data to be transmitted.
			-len: The length in bytes of the buffer pointed to by the buf paremeter
			-flags: optional set of flags that influences the behavior of this function

			-if no errors occur, send() returns the number of bytes sent. Otherwise
			SOCKET_ERROR is returned.

		*/

		char buffer[200];
		int byteCount;		// will hold the amount of bytes sent to the server

		cin.getline(buffer, sizeof(buffer));

		if (sizeof(buffer) > 200) {
			cout << "ERROR : Message exeeds the length of 200!" << endl;
		}
		else{

			// send() returns the number of bytes sent
			byteCount = send(clientSocket, buffer, sizeof(buffer), 0);

			if (byteCount == SOCKET_ERROR) {
			
				cerr << "Error sending message to server: " << WSAGetLastError() << endl;
				this->stop();
			}

			/*
				this may not be needed in the future, since we dont need to wait for other people
				to send a message. this for now just waits for a message to be recieved first from
				the server before sending another message.
			*/
			this->allowedToSend = false;	// need to wait for server message
		}
	}

	void Client::recieveFromServer(SOCKET clientSocket) {

		char buffer[1024];
		int bytesReceived;

		while (true) {

			bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

			if (bytesReceived < 0) {

				cerr << "Error recieving from server: " << WSAGetLastError() << endl;
				break;
			}
			else {
				buffer[bytesReceived] = '\0';	// print only actual amount of bytes recieved
				cout << buffer << endl;

				this->allowedToSend = true;
			}
		}
	}
}
