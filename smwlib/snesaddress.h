#ifndef SNESADDRESS_H
#define SNESADDRESS_H

#include "addressmapper.h"

namespace smwlib {

	class SnesAddress {
		public:
			SnesAddress();
			~SnesAddress();
			
			int getAddress();
			AddressMapper getMapper();
	};

}

#endif // SNESADDRESS_H
