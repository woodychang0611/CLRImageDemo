#pragma once
#ifndef __WOODYCHANG0611_COMMON_VIDEO_FRAME_GRABER__
#define __WOODYCHANG0611_COMMON_VIDEO_FRAME_GRABER__
#include "base.h"
#include "windows_io.h"
#include "frame_subject_base.h"
#include <windows.h>  //This class support windows platform only
#pragma warning (push,2)  // Temporarily setting warning level 2
#include <mmsystem.h> //To avoid C4201 warning form Microsoft bulit-in .h file
#pragma warning (pop)      // Restore back

namespace woodychang0611{
	namespace video{
		class VideoFrameGraber:public FrameSubjectBase{
		public:
			WOODYCHANG0611_COMMON_API VideoFrameGraber();
			WOODYCHANG0611_COMMON_API ~VideoFrameGraber();
			//true:success false:fail
			WOODYCHANG0611_COMMON_API bool OpenFilePath(const std::tstring&  file_path);
			WOODYCHANG0611_COMMON_API void SetStatus(FrameSubjectStatus status);
			inline FrameSubjectStatus GetStatus(void){return _status;};
			inline  void SetInitialTime(const tm& initial_time){initial_time_=initial_time;};
			inline woodychang0611::image::ImageRGB24 * GetImage(void) {return image_pointer_;};
			inline woodychang0611::image::ImageRGB24 * GetImage(UINT32){return GetImage();};
			inline void SetNote(std::tstring note){frame_info_.note_=note;};
			inline std::tstring GetNote(void){return frame_info_.note_;};
			inline void SetAutoReplay(bool status){auto_replay_=status;}
			WOODYCHANG0611_COMMON_API virtual bool VideoFrameGraber::Play();
			WOODYCHANG0611_COMMON_API virtual bool VideoFrameGraber::Pause();
			WOODYCHANG0611_COMMON_API virtual bool VideoFrameGraber::Stop();
		protected:
			static DWORD WINAPI _ThreadFunc (LPVOID lpParameter);
			virtual void _Close(void);
			virtual bool _Open(void);//true:success false:fail
			long start_frame_;
			long frame_length_;
			long current_frame_; //The frame current playing
			bool auto_replay_; //AutoReplay after end

			std::tstring  file_path_;
			//FrameSubjectStatus status_;
			HANDLE thread_handle_;
			woodychang0611::image::ImageRGB24 *image_pointer_; //image for output
			std::tm initial_time_;
		};



	}//end of namespace video
}//end of namespace woodychang0611



#endif //end of #ifndef __WOODYCHANG0611_COMMON_VIDEO_FRAME_GRABER__
