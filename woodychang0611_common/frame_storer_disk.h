#ifndef __SUNLIB_FRAME_STORER__
#define __SUNLIB_FRAME_STORER__

#include "frame_observer_base.h"
#include "windows_io.h"
#include "image_directory.h"

namespace sunlib{
	namespace video{

		class Frame_Storer:public sunlib::video::FrameObserverBase{
		private:
			bool m_ExitClass;
			sunlib::win::ImageDirectory imageDir;
		public:
			SUNLIB_API Frame_Storer();
			SUNLIB_API ~Frame_Storer();
			SUNLIB_API virtual void UpdateSubject(sunlib::design_patterns::Subject *the_changed_subject);
			//SUNLIB_API virtual void SubjectClosed(sunlib::design_patterns::Subject *the_closed_subject);

			SUNLIB_API void DirectoryName_Set(TCHAR *sDir);	//�]�w �ҭn�x�s����Ƨ�
		};
	}//end of namespace video
}//end of namespace sunlib

#endif //end of #ifndef __SUNLIB_FRAME_STORER__
