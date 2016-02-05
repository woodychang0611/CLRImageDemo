#pragma once
#ifndef __WOODYCHANG0611_COMMON_DUMMY_FRAME_GRABER__
#define __WOODYCHANG0611_COMMON_DUMMY_FRAME_GRABER__
#include "base.h"
#include "windows_io.h"
//#include "frame_subject_base.h"
#include "video_frame_graber.h"
#include <windows.h>  //This class support windows platform only
#pragma warning (push,2)  // Temporarily setting warning level 2
#include <mmsystem.h> //To avoid C4201 warning form Microsoft bulit-in .h file
#pragma warning (pop)      // Restore back

//此DummyFrameGraber
//依照指定數量 亂數產生顏色不同的移動圈圈

namespace woodychang0611{
	namespace video{
		class DummyFrameGraber:public FrameSubjectBase{
		public:
			WOODYCHANG0611_COMMON_API DummyFrameGraber();
			WOODYCHANG0611_COMMON_API ~DummyFrameGraber();
			virtual woodychang0611::image::ImageRGB24 * GetImage(){return image_pointer_;};
			virtual woodychang0611::image::ImageRGB24 * GetImage(UINT32){return GetImage();};
			virtual inline FrameSubjectStatus GetStatus(void){return _status;};
			virtual inline void Open(){_Open();};
			WOODYCHANG0611_COMMON_API void SetElementCount(INT16);
			inline INT16 GetElementCount() {return _elementCount;};

		private:
			//最大的圈圈數量
			const static INT16 MAX_ELEMENT_COUNT = 255;
			woodychang0611::color::RGB24 _color[MAX_ELEMENT_COUNT];
			INT16 _x[MAX_ELEMENT_COUNT];
			INT16 _y[MAX_ELEMENT_COUNT];
			INT16 _x_speed[MAX_ELEMENT_COUNT];
			INT16 _y_speed[MAX_ELEMENT_COUNT];
			INT16 _bg_graylevel;
			const static INT16 _width = 720;
			const static INT16 _height = 480;
			INT16 _elementCount;
			static DWORD WINAPI _ThreadFunc (LPVOID lpParameter);
			WOODYCHANG0611_COMMON_API void _Close(void);
			WOODYCHANG0611_COMMON_API bool _Open(void);//true:success false:fail
			woodychang0611::image::ImageRGB24 * image_pointer_;
			HANDLE thread_handle_;
		};



	}//end of namespace video
}//end of namespace woodychang0611



#endif //end of #ifndef __WOODYCHANG0611_COMMON_DUMMY_FRAME_GRABER__
