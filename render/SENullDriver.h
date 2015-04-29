#ifndef SE_NULLDRIVER_H
#define SE_NULLDRIVER_H

#include "SEIDriver.h"

namespace se
{
	namespace render
	{
		class SENullDriver: public SEIDriver
		{
		public:
			SENullDriver(void);
			virtual ~SENullDriver(void);
		};


	}
}


#endif



