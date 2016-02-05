#ifndef __SUNVISION_COMMON_SIMPLE_FRAME_GRABER_H__
#define __SUNVISION_COMMON_SIMPLE_FRAME_GRABER_H__

#include "frame_subject_base.h"
namespace sunvision{
	namespace video{

		class SimpleFrameGraber:public FrameSubjectBase{
		private:
			sunvision::image::ImageRGB24 *image_pointer_; //image for output
		public:
			SUNVISION_COMMON_API SimpleFrameGraber();
			SUNVISION_COMMON_API ~SimpleFrameGraber();

			inline sunvision::image::ImageRGB24 * GetImage(void) {return image_pointer_;};
			void publish(sunvision::image::ImageRGB24 * img){
				image_pointer_ = img;
				this->NotifyObserver();
			}
		};
	}
}
#endif  //__SUNVISION_COMMON_SIMPLE_FRAME_GRABER_H__