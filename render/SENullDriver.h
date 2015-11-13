#ifndef SE_NULLDRIVER_H
#define SE_NULLDRIVER_H

#include "../se_config.h"
#include "SEIDriver.h"

#ifdef SE_WINDOWS
#define ReleaseCOM(x) { if (x) { x->Release(); x = 0;} }
#endif


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



