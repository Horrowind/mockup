#include "stdexcept"

#include "pcaddress.h"
#include "addressmapper.h"
#include "snesaddress.h"

namespace smwlib {

	PcAddress::PcAddress(int address, AddressMapper mapper = AddressMapper_LoRom) {
		
		mAddress = address;
		mMapper = mapper;
	}

	PcAddress::PcAddress(SnesAddress snesAddress) {
		mAddress = snesAddress.getAddress();
		mMapper = snesAddress.getMapper();
	}

	PcAddress::~PcAddress() {
	}
	
	int PcAddress::getAddress() {
		return mHeadered ? mAddress + 512 : mAddress;
	}
	
	
	static int snesToPc(int address, AddressMapper mapper = AddressMapper_LoRom) {
		if(address < 0 || address > 0xFFFFFF)
			throw new std::invalid_argument("Address is not 24bit.");
		switch(mapper) {
			case AddressMapper_LoRom:
				if((address & 0xF00000) == 0x700000 || (address&0x408000)==0x000000)
					throw new std::invalid_argument("Address points to a invalid area.");
				return ((address & 0x7F0000) >> 1 | (address & 0x7FFF));
			default:
				//TODO: Implement HiRom, SA1 etc.
				throw new std::invalid_argument("Everything different from LoRom is currently not supported");
		}
	}
}
