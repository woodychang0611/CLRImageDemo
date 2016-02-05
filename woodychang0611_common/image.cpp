#include "image.h"

#ifdef _WIN32
	template class WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::RGB24>;
	template void  WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::RGB24>::
		CopyFrom(const  woodychang0611::image::ImageGeneric<woodychang0611::color::Gray8>& src);


	template class WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::Gray8>;
	template void  WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::Gray8>::
		CopyFrom(const  woodychang0611::image::ImageGeneric<woodychang0611::color::RGB24>& src);

#endif