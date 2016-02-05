#include "frame_observer_base.h"

namespace woodychang0611{
	namespace video{

		FrameObserverBase::FrameObserverBase(void){
			_frame_graber=NULL;
		}

		void FrameObserverBase::SetFrameGraber(woodychang0611::video::FrameSubjectBase * s){
			if (_frame_graber !=NULL){
				_frame_graber->DetachObserver(this);
			}
			_frame_graber = s;
			_frame_graber->AttachObserver(this);
		}

		void FrameObserverBase::SubjectClosed(woodychang0611::design_patterns::Subject * the_closed_subject){
			if (the_closed_subject==_frame_graber)
				_frame_graber=NULL;
		}

	}
}