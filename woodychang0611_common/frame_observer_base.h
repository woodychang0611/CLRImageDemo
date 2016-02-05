#pragma once
#ifndef __WOODYCHANG0611_COMMON_FRAME_OBSERVER_BASE__
#define __WOODYCHANG0611_COMMON_FRAME_OBSERVER_BASE__
//#include "observer_pattern.h"
#include "frame_subject_base.h"

namespace woodychang0611{
	namespace video{



		class FrameObserverBase:public woodychang0611::design_patterns::Observer
		{
		public:
			WOODYCHANG0611_COMMON_API FrameObserverBase();
			WOODYCHANG0611_COMMON_API virtual void SetFrameGraber(FrameSubjectBase *frame_subject_base);
			
		protected:
			virtual void UpdateSubject(woodychang0611::design_patterns::Subject *the_changed_subject)=0;//subject呼叫後的執行工作
			WOODYCHANG0611_COMMON_API virtual void SubjectClosed(woodychang0611::design_patterns::Subject *the_closed_subject);
			FrameSubjectBase* _frame_graber;
		};

	}
}




#endif //end of #ifdef __WOODYCHANG0611_COMMON_FRAME_FILTER_BASE__
