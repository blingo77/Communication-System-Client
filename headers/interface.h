#ifndef INTERFACE_H
#define INTERFACE_H

#include <unordered_map>
#include <vector>

using namespace std;

class Interface{
	public:

		Interface();

		void setCursosrPos(int cursorX, int cursorY);
	private:
		unordered_map<int, int> portsMap;
};



#endif
