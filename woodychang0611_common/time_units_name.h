#ifndef __WOODYCHANG0611_TIME_UNITS_NAME_H__
#define __WOODYCHANG0611_TIME_UNITS_NAME_H__
#include "time_units.h"
#include "stl.h"

namespace woodychang0611{
	namespace math{
		namespace time{
			namespace function{

				WOODYCHANG0611_COMMON_API inline bool GetTimeUnitsName(std::tstring &name_buffer,const TimeUnits time_units_enum)
				{
					bool ret =true;
					switch(time_units_enum)
					{
					case WEEK:
						name_buffer=(_T("WEEK"));break;
					case HOUR:
						name_buffer=(_T("HOUR"));break;
					case MINUTE:
						name_buffer=(_T("MINUTE"));break;
					case SECOND:
						name_buffer=(_T("SECOND"));break;
					case MILLISECOND:
						name_buffer=(_T("MILLISECOND"));break;
					case MICROSECOND:
						name_buffer=(_T("MICROSECOND"));break;
					case NANOSECOND:
						name_buffer=(_T("NANOSECOND"));break;
					case PICOSECOND:
						name_buffer=(_T("PICOSECOND"));break;
					case FEMTOSECOND:
						name_buffer=(_T("FEMTOSECOND"));break;
					default :
						ret = false;
					}
					return ret;
				}
			}
		}
	}
}

#endif