#include "frame_subject_base.h"

namespace woodychang0611{
	namespace video{
		bool FrameSubjectBase::Play(){
			_status=FRAME_SUBJECT_PLAY;
			return true;
		};
		bool FrameSubjectBase::Pause(){
			_status=FRAME_SUBJECT_PAUSE;
			return true;};
		bool FrameSubjectBase::Stop(){
			_status=FRAME_SUBJECT_STOP;
			return true;
		};
		bool FrameSubjectBase::IsPlay(){
			return _status==FRAME_SUBJECT_PLAY;
		};
		bool FrameSubjectBase::IsPause(){
			return _status==FRAME_SUBJECT_PAUSE;
		};
		bool FrameSubjectBase::IsStop(){
			return _status==FRAME_SUBJECT_STOP;
		};
	}
}