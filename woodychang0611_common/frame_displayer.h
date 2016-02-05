#ifndef __WOODYCHANG0611_COMMON_FRAME_DISPLAYER__
#define __WOODYCHANG0611_COMMON_FRAME_DISPLAYER__

#include "frame_displayer_base.h"

namespace woodychang0611{
	namespace video{

		class Displayer:public woodychang0611::video::DisplayerBase
		{
		public:
			WOODYCHANG0611_COMMON_API Displayer();
			WOODYCHANG0611_COMMON_API virtual ~Displayer();
			
			WOODYCHANG0611_COMMON_API virtual void UpdateSubject(woodychang0611::design_patterns::Subject *the_changed_subject);

		protected:
			
			int average_count;
			int t1;
			int counter;
			float actual_frame_per_secound;

		};
	}//end of namespace video
}//end of namespace woodychang0611

#endif //end of #ifndef __WOODYCHANG0611_COMMON_FRAME_DISPLAYER__
