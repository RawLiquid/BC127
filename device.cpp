
#include "device.h"

BC127device::BC127device() {

}

BC127device::BC127device(uint64_t address) {

	btAddress = address;

}

void BC127device::init() {

	Serial.println("BC127device::init");

}
