#ifndef PCADDRESS_H
#define PCADDRESS_H

#include "addressmapper.h"
namespace smwlib {

	class SnesAddress;
	
	class PcAddress {
		public:
			PcAddress(int address, AddressMapper mapper);
			PcAddress(const SnesAddress snesAddress);
			~PcAddress();
			int getAddress();
			AddressMapper getMapper();
			
			
			
		private:
			int mAddress;
			AddressMapper mMapper;
			bool mHeadered;
			
			static int snesToPc(int address);
	};

}

#endif // PCADDRESS_H
