#pragma once
#ifndef __WOODYCHANG0611_COMMON_AVI_FRAME_GRABER__
#define __WOODYCHANG0611_COMMON_AVI_FRAME_GRABER__
#include "base.h"
#include "windows_io.h"
//#include "frame_subject_base.h"
#include "video_frame_graber.h"
#include <windows.h>  //This class support windows platform only
#pragma warning (push,2)  // Temporarily setting warning level 2
#include <mmsystem.h> //To avoid C4201 warning form Microsoft bulit-in .h file
#pragma warning (pop)      // Restore back
#include <vfw.h> //add linking to vfw32.lib for the project

namespace woodychang0611{
	namespace video{
		class AviFrameGraber:public VideoFrameGraber{
		public:
			WOODYCHANG0611_COMMON_API AviFrameGraber();
			WOODYCHANG0611_COMMON_API ~AviFrameGraber();
		private:
			static DWORD WINAPI _ThreadFunc (LPVOID lpParameter);
			void _Close(void);
			bool _Open(void);//true:success false:fail
			AVIFILEINFO avi_info_;
			PGETFRAME frame_;
			PAVIFILE avi_file_;
			PAVISTREAM stream_;
		};



	}//end of namespace video
}//end of namespace woodychang0611



#endif //end of #ifndef __WOODYCHANG0611_COMMON_AVI_FRAME_GRABER__
