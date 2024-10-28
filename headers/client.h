#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>
#include "interface.h"

namespace clnt {

	class Client {

	public:

		Client();
		Client(int portNumber);

		void start();
		void stop(SOCKET socketToClose);

	private:

		int port;
		bool allowedToSend;
		Interface IFACE;

		void loadWSAdll();
		SOCKET buildSocket();
		int connectToServer(int port, SOCKET clientSocket);
		void sendData(SOCKET clientSocket);
		void recieveFromServer(SOCKET clientSocket);
	};
}

#endif