
#include "BC127.h"

#define DEBUG
#define db Serial


void BC127::init() {

    // COM.begin(38400);
    // COM.begin(9600);
    _serial->begin(9600);
    // _serial->begin(38400);

    // addQueue(BT::RESET);
    
    addQueue(BT::GET_CONFIG,CFG::BAUD);
    
    // addQueue(BT::SET_ROLE,0,0);
    // addQueue(BT::SET_AUDIO,0);
    // addQueue(BT::GET_MIC_STATE);
    // addQueue(BT::SET_GAIN,15);
    // addQueue(BT::GET_VOLUME);
    // addQueue(BT::SET_VOLUME,15);
    
    initTime = millis();
    bootDelayed = false;
    

}

void BC127::sayHello() {

    Serial.println("The BC127 library says hello!");

}

void BC127::handleByte(int inputChar) {

    // Throw out empty port readings
    if(inputChar != -1) {
    
        // Debug
        
#ifdef DEBUG
        // Serial.write(inputChar);
#endif
        
        // Check for end of line bytes
        if(inputChar == '\r' || inputChar == '\n') {
        
            // If there has been data recieved check for valid message
            if(msgLen > 0) handleMessage();
        
            // Reset the total for the message
            msgLen = 0;
        
        } else {
            
            // Reset the message total if it's about to overflow out of the buffer
            if(msgLen >= RX_MESSAGE_MAX) msgLen = 0;
        
            // Add the character to the string
            messageArray[msgLen] = byte(inputChar);
            
            // Increment the total for the message
            msgLen++;
        
            // Null terminate the string
            messageArray[msgLen] = 0;
        
        }
    
    }

}

void BC127::handleMessage() {

#ifdef DEBUG
    Serial.printf("btMsg %d \"%s\"\r\n",msgLen,messageArray);
#endif

    if(     strMatch(messageArray,"Ready")==0)          readyMsg();
    else if(strMatch(messageArray,"OPEN_")==0)          linkOpened();
    else if(strMatch(messageArray,"AVCRP_")==0)         avrcpCommandMsg();
    else if(strMatch(messageArray,"INQUIRY")==0)        inquiryMsg();
    else if(strMatch(messageArray,"NAME ")==0)          nameMsg();
    else if(strMatch(messageArray,"LIST")==0)           pairedDeviceMsg();
    else if(strMatch(messageArray,"ERROR")==0)          okMsg(true);
    else if(strMatch(messageArray,"OK")==0)             okMsg(false);
    else if(strMatch(messageArray,"Melody Audio ")==0)  melodyVersionMsg();
    // else if(strMatch(messageArray,"RING")==0)           ringMsg();  
    else if(strMatch(messageArray,"CALL_ACTIVE")==0)    {
    
        phoneState = PHONE_IN_CALL;
    
        // addQueue(BT::GET_VOLUME);
        // addQueue(BT::SET_VOLUME,15);
        // addQueue(BT::GET_MIC_STATE);
    
    } else if(strMatch(messageArray,"CALL_END")==0)    {
    
        phoneState = PHONE_IDLE;
    
    } else if(strMatch(messageArray,"CALL")==0)           callMsg();
    else if(strMatch(messageArray,"BATTERY_STATUS")==0) batteryMsg();
    else {
    
        int cfg = isConfig(messageArray);

        if(cfg != -1) {
        
            char dataStr[50];

            // loop that searches for '=' sign
            for(int i=0;messageArray[i]!=0;i++) {
                
                if(messageArray[i] == '=') {
                    
                    i++;
            
                    int count = 0;
                    for(;messageArray[i]!=0;i++) {
                    
                        dataStr[count++] = messageArray[i];
                    
                    }
                    
                    dataStr[count++] = 0;
                    
                    break;
                    
                }
            
            }
            
            configResult(cfg,dataStr);
        
        }
    
    }
    
}

int BC127::isConfig(char * inStr) {

    for(int i=0;i<CFG::TOTAL;i++) if(strMatch(inStr,CFG::names[i]) == 0) return i;
    
    return -1;
    
}

void BC127::power(bool state) {

    if(state == powerState) return;
    
    addQueue(BT::SET_POWER,state);
    
    powerState = state;
    
}


// Recieved bluetooth messages

void BC127::readyMsg() {

#ifdef DEBUG
    Serial.println("Recieved Ready Message");
#endif

    queueCount  = 0;
    queueWait   = 0;
    queuePtr    = 0;
    queueTotal  = 0;
    
    pairedTotal = 0;
    availibleTotal = 0;
    powerState = ON;
    
    phoneState = PHONE_IDLE;

    // queueFinish(BT::RESET);

}

void BC127::linkOpened() {

#ifdef DEBUG
    Serial.println("linkOpened");
#endif

    if(strMatch(messageArray,"OK")==5) {
    
//         int endpos;
//         for(endpos=8;endpos<12;endpos++) if(messageArray[endpos]==' ' || messageArray[endpos]==0) break;
//     
//         stringtoint(messageArray,7,endpos - 1,currentDeviceIndex);
//         
// #ifdef DEBUG
//         Serial.printf("currentDeviceIndex %d\r\n",currentDeviceIndex);
// #endif

        // print currentDeviceIndex here
    
        // addQueue(BT::PLAY_NOISE);
        
        if(strMatch(messageArray,"HFP")==8) {

        } else if(strMatch(messageArray,"A2DP")==8) {

        } else if(strMatch(messageArray,"AVRCP")==8) {

        } else if(strMatch(messageArray,"PBAP")==8) {

            // addQueue(BT::GET_PBOOK);

        }

    
    } else if(strMatch(messageArray,"ERROR")==5) {
    
    }
    

    
    queueFinish(BT::CONNECT);

}

void BC127::avrcpCommandMsg() {

#ifdef DEBUG
    Serial.println("avrcpCommandMsg");
#endif

    if(strMatch(messageArray,"PLAY")==6) {
    
        addQueue(BT::PLAY_NOISE);
    
    } else if(strMatch(messageArray,"PAUSE")==6) {
    
    }


}

void BC127::inquiryMsg() {

#ifdef DEBUG
    Serial.println("inquiryMsg");
#endif

    long long temp = stringToHexAuto(messageArray,8);

#ifdef DEBUG
    Serial.printf("inquiry address %012llX\r\n",temp);
#endif        
    
    // Make sure this address isn't already in the list
    for(int i=0;i<availibleTotal;i++) if(availibleAddress[i] == temp) return;

    availibleAddress[availibleTotal] = temp;    

    availibleTotal++;
    

}

void BC127::nameMsg() {

#ifdef DEBUG
    Serial.println("nameMsg");
#endif

    // Get address out of the string
    long long temp = stringToHexAuto(messageArray,5);
    
    int nameAvailPos = -1,namePairedPos = -1;
    
    // Search for matches in the availible and paired lists
    for(int i=0;i<availibleTotal;i++) { if(availibleAddress[i] == temp) { nameAvailPos = i; break; } } 
    for(int i=0;i<pairedTotal;i++) { if(pairedAddress[i] == temp) { namePairedPos = i; break; } } 

    // Get Name
    char name[50];
    int nmC = 0; for(int i=18;i<msgLen;i++) name[nmC++] = messageArray[i]; name[nmC] = 0;
    
    // Check to see if the address matched an entry in the availible list
    if(nameAvailPos != -1) {
        
        // Call user function
        // btAvailibleFunction(temp,nameAvailPos,name);
    
    }
    
    // Check to see if the address matched an entry in the paired list
    if(namePairedPos != -1) {
    
    }
    
    queueFinish(BT::NAME);

}

void BC127::pairedDeviceMsg() {

#ifdef DEBUG
    Serial.println("pairedDeviceMsg");
#endif


    pairedAddress[pairedTotal] = stringToHexAuto(messageArray,5);    

#ifdef DEBUG
    Serial.printf("pairedAddress[pairedTotal] %llX\r\n",pairedAddress[pairedTotal]);
#endif        

    pairedTotal++;

}

void BC127::okMsg(bool error) {

#ifdef DEBUG
    Serial.println("okMsg");
#endif

    // Serial.printf("Got OK msg function %s %d\r\n",BT::names[queue[curFunc()]],queue[curFunc()]);

    switch(queue[curFunc()]) {
    
        case BT::INQUIRY:
            for(int i=0;i<availibleTotal;i++) addQueue(BT::NAME,i);
            break;
            
        case BT::CALL_NUMBER:
            if(!error) phoneState = PHONE_DIALING;
            break;
            
        case BT::END_CALL:
            if(!error) phoneState = PHONE_IDLE;
            break;
            
    }

    queueFinish(queue[curFunc()]);

}

void BC127::melodyVersionMsg() {

#ifdef DEBUG
    Serial.println("melodyVersionMsg");
#endif

    if(strMatch(messageArray,"V5.0")==13)         melVer = MELODY_5_0; 
    else if(strMatch(messageArray,"V5.2")==13)     melVer = MELODY_5_2; 
    else if(strMatch(messageArray,"V5.5")==13)     melVer = MELODY_5_5;

}

void BC127::ringMsg() {

#ifdef DEBUG
    Serial.println("ringMsg");
#endif
    
    phoneState = PHONE_RINGING;

    lastRingTime = millis();
    
}

void BC127::callMsg() {

#ifdef DEBUG
    Serial.println("callMsg");
#endif
    
    phoneState = PHONE_RINGING;

    lastRingTime = millis();
    
    for(int i=0;i<11;i++) {
    
        if(messageArray[i + 5] <= '9' && messageArray[i + 5] >= '0') {
            
            phoneNumber[i] = messageArray[i + 5];
            phoneNumber[i + 1] = 0;
        
        } else {
            
            break;
        
        }
    
    } 
    
#ifdef DEBUG
    Serial.printf("new number is %s\r\n",phoneNumber);
#endif
    
}

void BC127::batteryMsg() {

    int state = 0;
    char outStr[40];
    char strC = 0;
    
    for(int i=0;messageArray[i]!=0;i++) {
    
        switch(state) {
        
            case 0:
                if(messageArray[i] == ' ') state = 1;
                break;
            case 1:
                outStr[strC] = messageArray[i];
                if(outStr[strC] == 0) goto out;
                strC++;
                break;
                
        }
    
    }
    out:
    outStr[strC] = 0;
    
    bt_event event;
    
    event.id = BT::BATTERY;
    
    if(strMatch(outStr,"CHARGER CONNECTED") == 0) {

        event.batCharging = true;
        
        // if(D) Serial.println("BC127 says that the battery is charging");
    
    } else if(strMatch(outStr,"LEVEL") == 0 && strlen(outStr) == 7) {
    
        event.batCharging = false;    
        event.batLevel = outStr[6] - '0';
    
    } else {
    
        if(D) db.printf("ERROR: unrecognized battery message %s %d\r\n",outStr,strlen(outStr));
        
        event.error = true;
    
    }

    sendEvent(&event);

}

void BC127::sendEvent(bt_event * event) {

    // add universal things here, like mac address

    btCallback(*event);

}

void BC127::callNumber(char * pNumber) {

    if(strLength(pNumber) <= 11) {
    
        addQueue(BT::CALL_NUMBER,(byte*)pNumber,strLength(pNumber) + 1);
    
    }

}

void BC127::endCall() {

    switch(phoneState) {
    
        case PHONE_IDLE: return;
        case PHONE_RINGING:
            addQueue(BT::REJECT_CALL);
            break;
        default:
            addQueue(BT::END_CALL);
            break;
    }

}

void BC127::answerCall() {

    switch(phoneState) {
    
        case PHONE_IDLE: return;
        case PHONE_RINGING:
            addQueue(BT::ANSWER_CALL);
            break;
        default:
            break;
    }

}


void BC127::getPbook() {

    addQueue(BT::GET_PBOOK);

}

long long BC127::stringToHexAuto(char *string,int strt) {

    int const HEX_MAX = 20;

    // char localString[HEX_MAX];
    int strLength = -1;
    
    for(int count=0;count<HEX_MAX;count++) {
    
        // if(D) db << string[strt+count] << ' ';
    
        if(!asciiIsHex(string[strt+count])) {
        
            // First char that was read wasn't valid, no hex data here
            if(count == 0) return -1;
        
            // Got a string of hex data, save the length off
            strLength = count;
        
            break;
        
        }
    
    }
    
    // if(D) db << endl; 
    
    long long value = 0;
    long long tmp;
    int valueC=0;
    
    // Serial.printf("strLength %d\r\n",strLength);
    
    for(int count=(strLength-1);count>=0;count--) {

        tmp = asciitohexnibble(string[strt+count]);
        
        tmp <<= (4 * valueC);
    
        // Serial.printf("value %12llX %2d %2d tmp %12llX\r\n",value,count,strt,tmp);
        
        value += tmp;
        // value += tmp << (4 * valueC);
        
        valueC++;
    
    }
    
    return value;

}

bool BC127::asciiIsHex(char value) {

    if((value>='A' && value<='F') || 
       (value>='a' && value<='f') ||
       (value>='0' && value<='9')) 
    {
       
       return true;
       
    }    

    return false;
    
}

byte BC127::asciitohexnibble(char value) {

    if(value>='0' && value<='9') {
    
        value -= '0';
        
    } else if(value>='A' && value<='F') {
    
        value -= ('A'-10); //value-=55;
        
    } else if(value>='a' && value<='f') {
    
        value -= ('a'-10); //value-=87;
        
    }

    return value;

}

void BC127::loop() {
    
    static elapsedMillis timer;

    handleByte(_serial->read());
    
    if(!bootDelayed) {

        if(millis() - initTime > 1000) bootDelayed = true;
    
    } else {
    
        if(queueWait && timer > 2000) {
        
            timer = 0;
            
            #ifdef DEBUG        
                Serial.printf("waiting for BT function %s %d\r\n",BT::names[queue[curFunc()]],queue[curFunc()]);
            #endif        
        
        }
        
        if(queueWait == 0 && queueCount > 0) {

            int function = queue[curFunc()];
            
    #ifdef DEBUG        
            Serial.printf("btQueueCheck function %s %d\r\n",BT::names[function],function);
    #endif        

            queueWait = 1;
            run(function);
        
            timer = 0;

        }
        
        // Looks like the BC127 doesn't tell me if the ringing just stops
        // So we time out when it stops giving us ring numbers, go back to phone idle
        if(phoneState == PHONE_RINGING && millis() - lastRingTime > 4000) {
        
            phoneState = PHONE_IDLE;
            
            #ifdef DEBUG        
                Serial.println("Ringing timed out");
            #endif        
            
        }
        
    }
    
    // int time = millis();
    // static int lasttime = 0;
    // static bool connecting = false;
    // static int lastAttemptedAddress = -1;
    // 
    // if(pairedTotal>0 && time>lasttime+1000 && !connecting) {
    // 
    //     lastAttemptedAddress++; if(lastAttemptedAddress>=pairedTotal) lastAttemptedAddress = 0;
    // 
    //     addQueue(BT::CONNECT,PAIR_INDEX,lastAttemptedAddress);
    // 
    //     lasttime = millis();
    //     connecting = true;
    // 
    // }

}

void BC127::reset() {

    addQueue(BT::RESET);

}

void BC127::status() {

    addQueue(BT::STATUS);
    
}

void BC127::inquiry(byte length) {

    if(length < 1) length = 1;
    if(length > 48) length = 48;

    addQueue(BT::INQUIRY,length);

}

void BC127::discoverable(int onoff) {

    addQueue(BT::DISCOVERABLE,onoff);

}

void BC127::listPaired() {

    pairedTotal = 0;

    addQueue(BT::LIST);

}

void BC127::getAllConfig() {

    addQueue(BT::CONFIG);

}

void BC127::factoryReset() {

    addQueue(BT::FACTORY);

    addQueue(BT::WRITE);

    addQueue(BT::RESET);

}

void BC127::setRole(int role,int mode) {

    // Make sure role and mode are in range
    if(role != SINK && role != SOURCE) role = SOURCE;
    if(mode != SINGLE && mode != MULTIPOINT && mode != TWS) role = SINGLE;
    
    // Add to the bluetooth message queue
    addQueue(BT::SET_ROLE,role,mode);
    addQueue(BT::WRITE);
    addQueue(BT::RESET);

}

void BC127::connectAvailible(int availibleIndex) {

    addQueue(BT::CONNECT,AVAILIBLE_INDEX,availibleIndex);

}

void BC127::connectPaired(int pairedIndex) {

    addQueue(BT::CONNECT,PAIR_INDEX,pairedIndex);

}

void BC127::connect(long long address) {

    // Splitting the address into 6 bytes so that it will go into the byte alligned buffer
    byte splitAddress[7];
    for(int i=0;i<6;i++) splitAddress[6-i] = byte(address >> (8 * i));
    splitAddress[0] = FULL_ADDRESS;
    
    addQueue(BT::CONNECT,splitAddress,7);

}

void BC127::audioTransfer() {

    addQueue(BT::AUDIO_TRANSFER);

}

void BC127::btSend(char * charArray) {

    btSend((byte*)charArray,strLength(charArray));

}

void BC127::btSend(byte * arrayToSend,uint16_t length) {

#ifdef DEBUG
    Serial.print("btSend ");
    for(int i=0;i<length;i++) Serial.printf("%02X ",arrayToSend[i]);
    Serial.println();
#endif

    for(int i=0;i<length;i++) COM.write(arrayToSend[i]);
    
}

void BC127::getConfig(int configValue) {

    const int TSTR_MAX = 75;
    char str[TSTR_MAX];
    
    int len = snprintf(str,TSTR_MAX,"GET %s\r",CFG::names[configValue%CFG::TOTAL]);

    btSend((byte*)str,len);
    
}

void BC127::configResult(int index,char * dataStr) {

    #ifdef DEBUG
    
        Serial.printf("configResult %s recieved with data %s\r\n",CFG::names[index],dataStr);
    
    #endif
    
    switch(index) {
    
        case CFG::AUDIO               : break; 
        case CFG::AUTOCONN            : break; 
        case CFG::BATT_THRESH         : break; 
        case CFG::BAUD                : break; 
        case CFG::BLE_ROLE            : break; 
        case CFG::BPS                 : break; 
        case CFG::CLASSIC_ROLE        : break; 
        case CFG::CMD_TO              : break; 
        case CFG::COD                 : break; 
        case CFG::CODEC               : break; 
        case CFG::DEEP_SLEEP          : break;
        case CFG::DEVICE_ID           : break;
        case CFG::DISCOVERABLE        : break;
        case CFG::ENABLE_A2DP         : break;
        case CFG::ENABLE_ANDROID_BLE  : break;
        case CFG::ENABLE_AVRCP        : break;
        case CFG::ENABLE_BATTERY_IND  : break;
        case CFG::ENABLE_HFP          : break;
        case CFG::ENABLE_HFP_CVC      : break;
        case CFG::ENABLE_HFP_NREC     : break;
        case CFG::ENABLE_HFP_WBS      : break;
        case CFG::ENABLE_LED          : break;
        case CFG::ENABLE_MAP          : break;
        case CFG::ENABLE_PBAP         : break;
        case CFG::ENABLE_SPP          : break;
        case CFG::ENABLE_SPP_SNIFF    : break;
        case CFG::FLOW_CTRL           : break;
        case CFG::FORCE_ANALOG_MIC    : break;
        case CFG::GPIOCONTROL         : break;
        case CFG::I2S                 : break;
        case CFG::INPUT_GAIN          : break;
        case CFG::LOCAL_ADDR          : break;
        case CFG::MAX_REC             : break;
        case CFG::MUSIC_META_DATA     : break;
        case CFG::NAME                : break;
        case CFG::NAME_SHORT          : break;
        case CFG::PARITY              : break;
        case CFG::PIN                 : break;
        case CFG::REMOTE_ADDR         : break;
        case CFG::RSSI_THRESH         : break;
        case CFG::SPP_TRANSPARENT     : break;
        case CFG::UUID_DATA           : break;
        case CFG::UUID_SPP            : break;
        case CFG::UUID_SRV            : break;
    
    }

}

void BC127::setConfig() {

}

void BC127::run(int function) {

#ifdef DEBUG  
    Serial.printf("run function %s %d\r\n",BT::names[function],function);
#endif

    #define sttm snprintf(str,TSTR_MAX,

    byte * bufAry;
    bufAry = dataQueue[curFunc()];
    
    const int TSTR_MAX = 75;
    char str[TSTR_MAX];
    
    long long temp = 0;
    
    switch (function) {
        case BT::BATTERY:        btSend("BATTERY_STATUS\r"); break;
        case BT::GET_CONFIG:     getConfig(bufAry[0]); break;
        case BT::RESET:          btSend("RESET\r"); break;
        case BT::SET_POWER:      sttm "POWER %s\r", (bufAry[0] == ON ? "ON" : "OFF") ); break;
        case BT::INQUIRY:        sttm "INQUIRY %d\r",bufAry[0]);                                 btSend(str); break;
        case BT::NAME:           sttm "NAME %012llX\r",availibleAddress[bufAry[0]]);             btSend(str); break;
        case BT::SET_AUDIO:      sttm "SET AUDIO=%1d\r",bufAry[0]);                              btSend(str); break;
        case BT::SET_VOLUME:     sttm "VOLUME HFP=%d\r",bufAry[0]);                              btSend(str); break;
        case BT::SET_GAIN:       sttm "SET INPUT_GAIN=%d\r",bufAry[0]);                          btSend(str); break;
        case BT::CALL_NUMBER:    sttm "CALL %s\r",bufAry);                                       btSend(str); break;
        case BT::DISCOVERABLE:   sttm "DISCOVERABLE %s\r", (bufAry[0] == ON ? "ON" : "OFF") );   btSend(str); break;
        case BT::LIST:            btSend("LIST\r"); break;
        case BT::STATUS:            btSend("STATUS\r"); break;
        case BT::CONFIG:            btSend("CONFIG\r"); break;
        case BT::FACTORY:        btSend("RESTORE\r"); break;
        case BT::WRITE:            btSend("WRITE\r"); break;
        case BT::GET_PBOOK:      btSend("PULL_PBOOK\r"); queueFinish(BT::GET_PBOOK); break;
        case BT::AUDIO_TRANSFER: btSend("TRANSFER_CALL 2\r"); break;
        case BT::GET_MIC_STATE:  btSend("VOLUME MIC_MUTE\r"); break;
        case BT::GET_VOLUME:     btSend("VOLUME HFP\r"); break;
        case BT::END_CALL:       btSend("END\r");  break;
        case BT::REJECT_CALL:    btSend("REJECT\r");  break;
        case BT::ANSWER_CALL:    btSend("ANSWER\r"); break;
        case BT::PLAY_NOISE:  
        
            btSend("TONE TE 400 V 64 TI 0 N C5 L 8 N R0 L 32 N E5 L 8 N R0 L 32 N G5 L 8 N R0 L 32 N B5 L 4 N R0 L 1 N C6 L 2 TN C6 L 8\r"); 
            break;
            
        case BT::SET_ROLE:  
        
            sttm melVer == MELODY_5_0 ? "SET CLASSIC_ROLE=%d\r" : "SET CLASSIC_ROLE=%d %d\r",bufAry[0],bufAry[1]); 
            btSend(str); 
            break;
            
        case BT::CONNECT:    
        
            switch(bufAry[0]) {
            
                case FULL_ADDRESS:
            
                    temp = 0;
                    
                    for(int i=0;i<6;i++) temp += ((long long)bufAry[6-i]) << (i * 8);
                    
                    sttm "OPEN %012llX A2DP\r",temp); btSend(str);
                    
                    break;
                    
                case PAIR_INDEX: sttm "OPEN %012llX HFP\r",pairedAddress[bufAry[1]]); btSend(str); break;
                case AVAILIBLE_INDEX: sttm "OPEN %012llX HFP\r",availibleAddress[bufAry[1]]); btSend(str); break;
                
            }
        
            // COM.printf("NAME %012llX\r",pairedAddress[dataQueue[curFunc()][0]]);
            
            break;
    }
    
    #undef sttm
    
}

// Adding to the bluetooth queue

void BC127::insertQueue(int function,byte *data,int length){
#ifdef DEBUG  
    Serial.printf("insertQueue %s queueCount %d queuePtr %d queueWait %d function %d\r\n",BT::names[function],queueCount,queuePtr,queueWait,function);
#endif
    if(queueCount==0 || (queueCount==1 && queueWait)) { addQueue(function,data,length); return; }

    queueCount++;

    if(queueCount>BT_QUEUE_LENGTH){
    
        queueCount = BT_QUEUE_LENGTH;
        // handleError(ERR_BTQUEUE_INSERT_FULL,"Queue FULL");
        
    } else {

        int tmpQueuePtrC = 0,tmpQueuePtrP = 0;
        int total;

        total = (queueCount-1);
        if(queueWait) total--;

        for(int i=0;i<total;i++) {

            // Finding the index of the array to copy data from
            tmpQueuePtrC=queuePtr-i-1;
            if(tmpQueuePtrC>BT_QUEUE_LENGTH-1) tmpQueuePtrC -= BT_QUEUE_LENGTH;
            if(tmpQueuePtrC<0) tmpQueuePtrC += BT_QUEUE_LENGTH;

            // Finding the index of the array to copy data to      
            tmpQueuePtrP=queuePtr-i;
            if(tmpQueuePtrP>BT_QUEUE_LENGTH-1) tmpQueuePtrP -= BT_QUEUE_LENGTH;
            if(tmpQueuePtrP<0) tmpQueuePtrP += BT_QUEUE_LENGTH;

            // Moving the Queued bluetooth item     
            queue[tmpQueuePtrP]=queue[tmpQueuePtrC];
            queue[tmpQueuePtrC]=0;

            for(int k=0;k<BT_QUEUE_DATA_LENGTH;k++){ 
            
                dataQueue[tmpQueuePtrP][k]=dataQueue[tmpQueuePtrC][k]; 
                dataQueue[tmpQueuePtrC][k]=0; 
                
            }

        }

        //Inserting the new queue item
        queue[tmpQueuePtrC] = function;

        //Inserting the new queue item's data
        if(length<BT_QUEUE_DATA_LENGTH) for(int i=0;i<length;i++) dataQueue[tmpQueuePtrC][i]=data[i];

        //  Advancing the queue pointers
        if(++queuePtr>BT_QUEUE_LENGTH-1) queuePtr = 0;
        if(++queueTotal>BT_QUEUE_LENGTH) queueTotal = BT_QUEUE_LENGTH;

    }

}

void BC127::insertQueue(int function) {                                                                           insertQueue(function,0,0); }

void BC127::insertQueue(int function,byte d) {  byte dA[2]; dA[0]=d;                                              insertQueue(function,dA,1); }
  
void BC127::insertQueue(int function,byte d0,byte d1) { byte dA[2]; dA[0]=d0; dA[1]=d1;                         insertQueue(function,dA,2); }
  
void BC127::insertQueue(int function,byte d0,byte d1,byte d2) { byte dA[3]; dA[0]=d0; dA[1]=d1; dA[2]=d2;          insertQueue(function,dA,3); }

void BC127::addQueue(int function,byte *data,int length){
#ifdef DEBUG  
    Serial.printf("addQueue %s queueCount %d queuePtr %d queueWait %d function %d\r\n",BT::names[function],queueCount,queuePtr,queueWait,function);
#endif  
    queueCount++;

    if(queueCount>BT_QUEUE_LENGTH){
    
        queueCount = BT_QUEUE_LENGTH;  
        // handleError(ERR_BTQUEUE_ADD_FULL,"queue FULL");
        
    } else {

        //  Inserting the new queue item
        queue[queuePtr] = function;

        //  Inserting the new queue item's data
        if(length < BT_QUEUE_DATA_LENGTH) for(int i=0;i<length;i++) dataQueue[queuePtr][i]=data[i];

        //  Advancing the queue pointers
        if(++queuePtr > BT_QUEUE_LENGTH-1) queuePtr = 0;
        if(++queueTotal > BT_QUEUE_LENGTH) queueTotal = BT_QUEUE_LENGTH;

    }

}

void BC127::addQueue(int function) {                                                                            addQueue(function,0,0); }

void BC127::addQueue(int function,byte d0) {  byte dA[2]; dA[0]=d0;                                             addQueue(function,dA,1); }
  
void BC127::addQueue(int function,byte d0,byte d1) { byte dA[2]; dA[0]=d0; dA[1]=d1;                              addQueue(function,dA,2); }
  
void BC127::addQueue(int function,byte d0,byte d1,byte d2) { byte dA[3]; dA[0]=d0; dA[1]=d1; dA[2]=d2;           addQueue(function,dA,3); }

// Removing from the bluetooth queue

bool BC127::queueFinish(int function) {
#ifdef DEBUG
    Serial.printf("queueFinish %s queueCount %d queuePtr %d queueWait %d function %d\r\n",BT::names[function],queueCount,queuePtr,queueWait,function);
#endif
    //int nextInQueueIndex; nextInQueueIndex=queuePtr-btQueueCount; if(nextInQueueIndex<0)nextInQueueIndex+=10;

    if(queueCount>0 && function==queue[curFunc()]) {

        queue[curFunc()]=0;
        queueCount--;
        queueWait=0;

        return true;

    } else {
#ifdef DEBUG
        Serial.printf("btQueue Error function %d %s queue[curFunc()] %d %s queueCount %d",function,BT::names[function],queue[curFunc()],BT::names[queue[curFunc()]],queueCount);
#endif
        return false;

    }

    return false;

}

byte BC127::curFunc() {
  
    int z = queuePtr - queueCount;
    
    if(z < 0) z += BT_QUEUE_LENGTH;

    return z;

}



bool BC127::stringtoint(char* string,int strt,int ending,int &returnValue) {
  
    int stringLen=0,actualStart=0;
    bool negative = false;
    
    // Find a valid number in the string
    for(int i=strt;i<=ending;i++) {
    
        if(string[i] == '-' && stringLen == 0) {
        
            negative = true; stringLen++;
            actualStart = i + 1;
            
        } else if(isDigit(string[i])) {
        
            if(stringLen == 0) actualStart = i;
            stringLen++;
            
        } else {
        
            // Not a valid number
            if((stringLen <= 1 && negative) || stringLen <= 0) { 
            
                // Reset variables and keep searching
                negative = false;
                stringLen = 0;
                actualStart = 0;
                
            } else {
            
                // Found a number
                break;
                
            }
            
        }
    }
    
    // Convert string to a number
    if((stringLen > 1 && negative) || stringLen>0) {

        if(negative) stringLen--;
        int multiplier = 1; for(int n=0;n<(stringLen - 1);n++) multiplier *= 10;
        
        returnValue = 0;
        for(int n=0;n<stringLen;n++) {
        
            returnValue += (string[actualStart + n] - '0') * multiplier;
            multiplier /= 10;
            
        }
        
        if(negative) returnValue *= -1;
        
        return true;
    
    }
    
    return false;
  
}

bool BC127::isDigit(char character) {

  if(character <= '9' && character >= '0') return true;
  return false;

}

int BC127::strMatch(char* mystring,const char* searchstring) {

    int mystringLen = strLength(mystring);
    int searchstringLen = strLength(searchstring);

    for(int i=0;i<mystringLen;i++) {
        
        if(mystring[i]==searchstring[0]) {
        
            for(int n=1;n<searchstringLen;n++) {
            
                if(mystring[i+n]!=searchstring[n])
                break;
                else if(n==searchstringLen-1)
                return i;
                
            }
            
        }
        
    }

    return -1;

}

int BC127::strLength(const char* string) {
  
    int count = 0;
    
    for(int z=0;z<150;z++) { if(string[z]==0) { break; } else { count++; } }

    return count;

}
