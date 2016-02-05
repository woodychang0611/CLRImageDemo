#ifndef __WOODYCHANG0611_COMMON_FRAME_DISPLAYER_BASE__
#define __WOODYCHANG0611_COMMON_FRAME_DISPLAYER_BASE__

#include "frame_subject_base.h"
#include "frame_observer_base.h"
//#include "target_detection.h"
#include "windows_io.h"
#include <windows.h>

namespace woodychang0611{
	namespace video{

		class DisplayerBase:public woodychang0611::video::FrameObserverBase
		{
		public:
			WOODYCHANG0611_COMMON_API DisplayerBase();
			WOODYCHANG0611_COMMON_API virtual ~DisplayerBase();
			
			WOODYCHANG0611_COMMON_API inline virtual void Init(HDC hdc,const UINT16 top,const UINT16 left,const UINT16 width,const UINT16 height);
			WOODYCHANG0611_COMMON_API inline virtual void SetHdc(HDC hdc);
			WOODYCHANG0611_COMMON_API inline virtual void SetPosition(const UINT16 top,const UINT16 left);
			WOODYCHANG0611_COMMON_API inline virtual void SetSize(const UINT16 width,const UINT16 height);
			WOODYCHANG0611_COMMON_API inline virtual HDC Hdc()const;
			WOODYCHANG0611_COMMON_API inline virtual UINT16 Top()const;
			WOODYCHANG0611_COMMON_API inline virtual UINT16 Left()const;
			WOODYCHANG0611_COMMON_API inline virtual UINT16 Width()const;
			WOODYCHANG0611_COMMON_API inline virtual UINT16 Height()const;
			WOODYCHANG0611_COMMON_API inline void Hdc(HDC hdc);
			WOODYCHANG0611_COMMON_API inline void Top(UINT16 top);
			WOODYCHANG0611_COMMON_API inline void Left(UINT16 left);
			WOODYCHANG0611_COMMON_API inline void Width(UINT16 width);
			WOODYCHANG0611_COMMON_API inline void Height(UINT16 height);
			WOODYCHANG0611_COMMON_API virtual void UpdateSubject(woodychang0611::design_patterns::Subject *the_changed_subject)=0;
		protected:
			HDC _hdc;
			UINT16 _top;
			UINT16 _left;
			UINT16 _width;
			UINT16 _height;

		};
	}//end of namespace video
}//end of namespace woodychang0611

#endif //end of #ifndef __WOODYCHANG0611_COMMON_FRAME_DISPLAYER__
