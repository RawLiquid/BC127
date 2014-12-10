
#ifndef BC127_
#define BC127_
    
    #include "Arduino.h"
    // #include <HardwareSerial.h>
    #include <SdFat.h>
    #include <Stream.h>

    #define ON                  1
    #define OFF                 0

    #define DEBUG DEBUG

    #define BT_MAX_AVAILIBLE    20
    #define BT_MAX_PAIRED       20

    #define SINK                0
    #define SOURCE              1

    #define SINGLE              0
    #define MULTIPOINT          1
    #define TWS                 2

    #define MELODY_5_0          0
    #define MELODY_5_2          1
    #define MELODY_5_5          2

    #define FULL_ADDRESS        0
    #define PAIR_INDEX          1
    #define AVAILIBLE_INDEX     2
    
    #define PHONE_IDLE          0
    #define PHONE_DIALING       1
    #define PHONE_IN_CALL       2
    #define PHONE_RINGING       3
    
    #define COM Serial2
    
    // typedef const int8_t config_t;

    // class BC127;

    namespace CFG {
    
        int const

        AUDIO               =  0, 
        AUTOCONN            =  1, 
        BATT_THRESH         =  2, 
        BAUD                =  3, 
        BLE_ROLE            =  4, 
        BPS                 =  5, 
        CLASSIC_ROLE        =  6, 
        CMD_TO              =  7, 
        COD                 =  8, 
        CODEC               =  9, 
        DEEP_SLEEP          = 10,
        DEVICE_ID           = 11,
        DISCOVERABLE        = 12,
        ENABLE_A2DP         = 13,
        ENABLE_ANDROID_BLE  = 14,
        ENABLE_AVRCP        = 15,
        ENABLE_BATTERY_IND  = 16,
        ENABLE_HFP          = 17,
        ENABLE_HFP_CVC      = 18,
        ENABLE_HFP_NREC     = 19,
        ENABLE_HFP_WBS      = 20,
        ENABLE_LED          = 21,
        ENABLE_MAP          = 22,
        ENABLE_PBAP         = 23,
        ENABLE_SPP          = 24,
        ENABLE_SPP_SNIFF    = 25,
        FLOW_CTRL           = 26,
        FORCE_ANALOG_MIC    = 27,
        GPIOCONTROL         = 28,
        I2S                 = 29,
        INPUT_GAIN          = 30,
        LOCAL_ADDR          = 31,
        MAX_REC             = 32,
        MUSIC_META_DATA     = 33,
        NAME                = 34,
        NAME_SHORT          = 35,
        PARITY              = 36,
        PIN                 = 37,
        REMOTE_ADDR         = 38,
        RSSI_THRESH         = 39,
        SPP_TRANSPARENT     = 40,
        UUID_DATA           = 41,
        UUID_SPP            = 42,
        UUID_SRV            = 43,

        TOTAL               = 44;
        
        const char names[TOTAL][25] {

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

    }

    const int RX_MESSAGE_MAX = 100;
    
    const int BT_QUEUE_LENGTH       = 20;
    const int BT_QUEUE_DATA_LENGTH  = 50;

    namespace BT {

        const int 
        
        RESET           =  0,
        INQUIRY         =  1,
        DISCOVERABLE    =  2,
        LIST            =  3,
        CONNECT         =  4,
        STATUS          =  5,
        CONFIG          =  6,
        FACTORY         =  7,
        WRITE           =  8,
        GET_PBOOK       =  9,
        NAME            = 10,
        SET_ROLE        = 11,
        PLAY_NOISE      = 12,
        AUDIO_TRANSFER  = 13,
        CALL_NUMBER     = 14,
        SET_AUDIO       = 15,
        GET_MIC_STATE   = 16,
        SET_GAIN        = 17,
        GET_VOLUME      = 18,
        SET_VOLUME      = 19,
        END_CALL        = 20,
        REJECT_CALL     = 21,
        ANSWER_CALL     = 22,
        SET_POWER       = 23,
        GET_CONFIG      = 24,
        BATTERY         = 25;

        const char names[30][15] = {

            "RESET",
            "INQUIRY",
            "DISCOVERABLE",
            "LIST",
            "CONNECT",
            "STATUS",
            "CONFIG",
            "FACTORY",
            "WRITE",
            "GET_PBOOK",
            "NAME",
            "SET_ROLE",
            "PLAY_NOISE",
            "AUDIO_TRANSFER",
            "CALL_NUMBER",
            "SET_AUDIO",
            "GET_MIC_STATE",
            "SET_GAIN",
            "GET_VOLUME",
            "SET_VOLUME",
            "END_CALL",
            "REJECT_CALL",
            "ANSWER_CALL",
            "SET_POWER",
            "GET_CONFIG",
            "BATTERY"
        
        };

    }
    
    struct bt_event {

        uint16_t id;
        uint64_t mac;
        bool error;
        union {
        
            struct { // battery
            
                bool batCharging;
                int batLevel;
            
            };
            int volume;
            struct {
                
                int configID;
                union {
                
                    int baud;
                
                };
              
            };
        
        };

    };

    class BC127 {
    
        // friend class BC127device;

        public:
         
            HardwareSerial * _serial;
        
            BC127(HardwareSerial * ser,void (*tmpCallback)(bt_event)) {

                _serial = ser;
                btCallback = tmpCallback;
                
            }

            // int currentDeviceIndex = -1;
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
            void getBaud() { addQueue(BT::GET_CONFIG,CFG::BAUD); }
            void getBattery() { addQueue(BT::BATTERY); }
            

            
            void sayHello();
            
            void callNumber(char * pNumber);
            void endCall();
            void answerCall();

        private:
        
            void sendEvent(bt_event * event);

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
            
            void (*btCallback)(bt_event);
            
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