
#ifndef BC127_
#define BC127_
    
    #include "Arduino.h"
    #include <SdFat.h>

    #define ON 		1
    #define OFF 	0

    #define DEBUG DEBUG

    #define BT_MAX_AVAILIBLE 	20
    #define BT_MAX_PAIRED 		20

    #define SINK 	0
    #define SOURCE 	1

    #define SINGLE 			0
    #define MULTIPOINT 		1
    #define TWS 			2

    #define MELODY_5_0		0
    #define MELODY_5_2		1
    #define MELODY_5_5		2

    #define FULL_ADDRESS	0
    #define PAIR_INDEX		1
    #define AVAILIBLE_INDEX	2
    
    #define PHONE_IDLE              0
    #define PHONE_DIALING           1
    #define PHONE_IN_CALL           2
    #define PHONE_RINGING           3
    
    #define COM Serial2

    const int RX_MESSAGE_MAX = 100;

    const int BT_QUEUE_LENGTH       = 20;
    const int BT_QUEUE_DATA_LENGTH  = 50;

    const int BT_RESET			= 0;
    const int BT_INQUIRY		= 1;
    const int BT_DISCOVERABLE	= 2;
    const int BT_LIST			= 3;
    const int BT_CONNECT		= 4;
    const int BT_STATUS			= 5;
    const int BT_CONFIG 		= 6;
    const int BT_FACTORY 		= 7;
    const int BT_WRITE 			= 8;
    const int BT_GET_PBOOK		= 9;
    const int BT_NAME			= 10;
    const int BT_SET_ROLE		= 11;
    const int BT_PLAY_NOISE		= 12;
    const int BT_AUDIO_TRANSFER = 13;
    const int BT_CALL_NUMBER    = 14;
    const int BT_SET_AUDIO      = 15;
    const int BT_GET_MIC_STATE  = 16;
    const int BT_SET_GAIN       = 17;
    const int BT_GET_VOLUME     = 18;
    const int BT_SET_VOLUME     = 19;
    const int BT_END_CALL       = 20;
    const int BT_REJECT_CALL    = 21;
    const int BT_ANSWER_CALL    = 22;
    const int BT_SET_POWER      = 23;


    const char btfunctions[30][20] = {

        "BT_RESET",
        "BT_INQUIRY",
        "BT_DISCOVERABLE",
        "BT_LIST",
        "BT_CONNECT",
        "BT_STATUS",
        "BT_CONFIG",
        "BT_FACTORY",
        "BT_WRITE",
        "BT_GET_PBOOK",
        "BT_NAME",
        "BT_SET_ROLE",
        "BT_PLAY_NOISE",
        "BT_AUDIO_TRANSFER",
        "BT_CALL_NUMBER",
        "BT_SET_AUDIO",
        "BT_GET_MIC_STATE",
        "BT_SET_GAIN",
        "BT_GET_VOLUME",
        "BT_SET_VOLUME",
        "BT_END_CALL",
        "BT_REJECT_CALL",
        "BT_ANSWER_CALL",
        "BT_SET_POWER"
    
    };
    
    class BC127 {

        // friend class BC127device;

        public:
        
            int currentDeviceIndex = -1;
            int phoneState = PHONE_IDLE;
            char phoneNumber[15] {0};
            
            // Functions
        
            BC127(void (*btstartedFunction)(),
                  void (*connectedFunction)(long long),
                  void (*disconnectedFunction)(long long),
                  void (*availibleFunction)(long long,int,char *));
                  
            void init();
            void loop();
            void power(bool state);
            
            void reset();
            void status();
            void inquiry(byte length = 4);
            void discoverable(int onoff);
            void listPaired();
            void getAllConfig();
            void factoryReset();
            void getPbook();
            void setRole(int role,int mode = SINGLE);
            void connect(long long address);
            void connectPaired(int pairedIndex);
            void connectAvailible(int availibleIndex);
            void audioTransfer();
            
            void sayHello();
            
            void callNumber(char * pNumber);
            void endCall();
            void answerCall();

        private:
        
            // Variables
        
            const bool D = true;
            
            int initTime = 0;
            bool bootDelayed = false;
            
            bool powerState = ON;
            
            int lastRingTime = 0;
            
            int melVer = -1;

            bool started = false;
            char messageArray[RX_MESSAGE_MAX];
            int msgLen = 0;
            
            byte dataQueue[BT_QUEUE_LENGTH][BT_QUEUE_DATA_LENGTH]; // For caching data for queued bluetooth operations
            int queueCount=0,queueWait=0,queuePtr=0,queueTotal=0;
            byte queue[BT_QUEUE_LENGTH];
            
            int availibleTotal = 0;
            long long availibleAddress[BT_MAX_AVAILIBLE];
            
            int pairedTotal = 0;
            long long pairedAddress[BT_MAX_PAIRED];
            
            // Pointers to user functions 
            
            void (*btStartFunction)();
            void (*btConnectedFunction)(long long);
            void (*btDisconnectedFunction)(long long);
            void (*btAvailibleFunction)(long long,int,char *);
            
            // Functions
            
            void run(int function);
            
            void btSend(char * charArray);
            void btSend(byte * arrayToSend,uint16_t length);
            
            void insertQueue(int function,byte *data,int length);
            void insertQueue(int function);
            void insertQueue(int function,byte d);    		
            void insertQueue(int function,byte d0,byte d1);
            void insertQueue(int function,byte d0,byte d1,byte d2);
            
            void addQueue(int function,byte *data,int length);
            void addQueue(int function);
            void addQueue(int function,byte d0);
            void addQueue(int function,byte d0,byte d1);
            void addQueue(int function,byte d0,byte d1,byte d2);
            
            bool queueFinish(int function);
            byte curFunc();
            
            bool stringtoint(char* string,int strt,int ending,int &returnValue);
            bool isDigit(char character);
            
            int strMatch(char* mystring,char* searchstring);
            int strLength(char* string);
            long long stringToHexAuto(char *string,int strt);
            byte asciitohexnibble(char value);
            bool asciiIsHex(char value);
            
            
            void handleByte(int inputChar);
            void handleMessage();
            
            void readyMsg();
            void linkOpened();
            void avrcpCommandMsg();
            void inquiryMsg();
            void nameMsg();
            void pairedDeviceMsg();
            void okMsg(bool error);
            void melodyVersionMsg();            
            void ringMsg(); 
            void callMsg(); 


    };
    
#endif