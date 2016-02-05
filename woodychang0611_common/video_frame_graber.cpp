#include "video_frame_graber.h"
#include <ctime>
#include <time.h>
#include <sstream>



namespace woodychang0611{
	namespace video{

		VideoFrameGraber::VideoFrameGraber(){
			thread_handle_= NULL;
			start_frame_=0;
			frame_length_=0;
			_status = FRAME_SUBJECT_STOP;
			auto_replay_=false;
			std::time_t now=std::time(NULL); //default initial time is current time
			tm tm_now;
			localtime_s(&tm_now,&now);
			initial_time_=tm_now;	
		}

		VideoFrameGraber::~VideoFrameGraber(){
			_Close();
		}
		
		bool VideoFrameGraber::_Open(void){
			return false;
		}

		void VideoFrameGraber::_Close(void){
		}

		bool VideoFrameGraber::OpenFilePath(const std::tstring& file_path){
			_Close(); //Make sure it is closed 
			file_path_=file_path;
			return _Open();
		}


		void VideoFrameGraber::SetStatus(FrameSubjectStatus status){
			switch(status){
				case FRAME_SUBJECT_PLAY:
					_status = FRAME_SUBJECT_PLAY;
					break;
				case FRAME_SUBJECT_PAUSE:
					if(_status==FRAME_SUBJECT_PLAY)
					_status = FRAME_SUBJECT_PAUSE;
					break;
				case FRAME_SUBJECT_STOP:
					_Close();
					break;
				default:
					break;
			}
		}
		bool VideoFrameGraber::Play(){
			_status = FRAME_SUBJECT_PLAY;
			return true;
		}
		bool VideoFrameGraber::Pause(){
			if(_status==FRAME_SUBJECT_PLAY)
				_status = FRAME_SUBJECT_PAUSE;
			return true;
		}
		bool VideoFrameGraber::Stop(){
			_Close();
			return true;
		}

	}//end of namespace video
}//end of namespace woodychang0611

