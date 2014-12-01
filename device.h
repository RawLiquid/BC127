
#include "Arduino.h"

class BC127device {

	friend class BC127;

	public:
	
		BC127device();
		BC127device(uint64_t address);
		void init();
	
	private:
		
		int strMatch(char* mystring,char* searchstring);
		int strLength(char* string);
		uint64_t btAddress;
		uint32_t timeLastSeen;

};