
#ifndef BC127_
#define BC127_
    
    #include "Arduino.h"
    // #include <HardwareSerial.h>
    #include <SdFat.h>
    #include <Stream.h>

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
    
    // typedef const int8_t config_t;

    // class BC127;


    int const

    CFG_AUDIO               =  0, 
    CFG_AUTOCONN            =  1, 
    CFG_BATT_THRESH         =  2, 
    CFG_BAUD                =  3, 
    CFG_BLE_ROLE            =  4, 
    CFG_BPS                 =  5, 
    CFG_CLASSIC_ROLE        =  6, 
    CFG_CMD_TO              =  7, 
    CFG_COD                 =  8, 
    CFG_CODEC               =  9, 
    CFG_DEEP_SLEEP          = 10,
    CFG_DEVICE_ID           = 11,
    CFG_DISCOVERABLE        = 12,
    CFG_ENABLE_A2DP         = 13,
    CFG_ENABLE_ANDROID_BLE  = 14,
    CFG_ENABLE_AVRCP        = 15,
    CFG_ENABLE_BATTERY_IND  = 16,
    CFG_ENABLE_HFP          = 17,
    CFG_ENABLE_HFP_CVC      = 18,
    CFG_ENABLE_HFP_NREC     = 19,
    CFG_ENABLE_HFP_WBS      = 20,
    CFG_ENABLE_LED          = 21,
    CFG_ENABLE_MAP          = 22,
    CFG_ENABLE_PBAP         = 23,
    CFG_ENABLE_SPP          = 24,
    CFG_ENABLE_SPP_SNIFF    = 25,
    CFG_FLOW_CTRL           = 26,
    CFG_FORCE_ANALOG_MIC    = 27,
    CFG_GPIOCONTROL         = 28,
    CFG_I2S                 = 29,
    CFG_INPUT_GAIN          = 30,
    CFG_LOCAL_ADDR          = 31,
    CFG_MAX_REC             = 32,
    CFG_MUSIC_META_DATA     = 33,
    CFG_NAME                = 34,
    CFG_NAME_SHORT          = 35,
    CFG_PARITY              = 36,
    CFG_PIN                 = 37,
    CFG_REMOTE_ADDR         = 38,
    CFG_RSSI_THRESH         = 39,
    CFG_SPP_TRANSPARENT     = 40,
    CFG_UUID_DATA           = 41,
    CFG_UUID_SPP            = 42,
    CFG_UUID_SRV            = 43,

    CFG_TOTAL               = 44;

    const char configNames[CFG_TOTAL][25] {

        "AUDIO",
        "AUTOCONN",
        "BATT_THRESH",
        "BAUD",
        "BLE_ROLE",
        "BPS",
        "CLASSIC_ROLE",
        "CMD_TO ",
        "COD",
        "CODEC",
        "DEEP_SLEEP",
        "DEVICE_ID",
        "DISCOVERABLE",
        "ENABLE_A2DP ",
        "ENABLE_ANDROID_BLE",
        "ENABLE_AVRCP",
        "ENABLE_BATTERY_IND",
        "ENABLE_HFP",
        "ENABLE_HFP_CVC",
        "ENABLE_HFP_NREC",
        "ENABLE_HFP_WBS",
        "ENABLE_LED",
        "ENABLE_MAP",
        "ENABLE_PBAP",
        "ENABLE_SPP ",
        "ENABLE_SPP_SNIFF",
        "FLOW_CTRL",
        "FORCE_ANALOG_MIC",
        "GPIOCONTROL",
        "I2S",
        "INPUT_GAIN",
        "LOCAL_ADDR",
        "MAX_REC",
        "MUSIC_META_DATA",
        "NAME",
        "NAME_SHORT",
        "PARITY",
        "PIN",
        "REMOTE_ADDR",
        "RSSI_THRESH",
        "SPP_TRANSPARENT",
        "UUID_DATA",
        "UUID_SPP",
        "UUID_SRV"

    };

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
    const int BT_GET_CONFIG     = 24;
    const int BT_GET_BATTERY    = 25;

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
        "BT_SET_POWER",
        "BT_GET_CONFIG",
        "BT_GET_BATTERY"
    
    };
    
    class BC127 {
    
        // friend class BC127device;

        public:
         
            HardwareSerial * _serial;
        
            BC127(HardwareSerial * ser);
            
            BC127(void (*btstartedFunction)(),
                  void (*connectedFunction)(long long),
                  void (*disconnectedFunction)(long long),
                  void (*availibleFunction)(long long,int,char *));

            int currentDeviceIndex = -1;
            int phoneState = PHONE_IDLE;
            char phoneNumber[15] {0};
            
            // Functions
        
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
            void getBaud() { addQueue(BT_GET_CONFIG,CFG_BAUD); }
            void getBattery() { addQueue(BT_GET_BATTERY); }
            

            
            void sayHello();
            
            void callNumber(char * pNumber);
            void endCall();
            void answerCall();

        private:
        
            int isConfig(char * inStr);

            void getConfig(int configValue);
            void setConfig();

            void configResult(int index,char * data);
            
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
            
            int strMatch(char* mystring,const char* searchstring);
            int strLength(const char* string);
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
            void batteryMsg();


    };
    
#endif