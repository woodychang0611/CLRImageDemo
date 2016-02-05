#include "image.h"

#ifdef _WIN32
	template class WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::RGB24>;
	template void  WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::RGB24>::
		CopyFrom(const  woodychang0611::image::ImageGeneric<woodychang0611::color::Gray8>& src);


	template class WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::Gray8>;
	template void  WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::Gray8>::
		CopyFrom(const  woodychang0611::image::ImageGeneric<woodychang0611::color::RGB24>& src);

	//template class WOODYCHANG0611_COMMON_API  std::vector <woodychang0611::image::ImageRGB24>;
	//template class WOODYCHANG0611_COMMON_API  std::vector <woodychang0611::image::ImageGray8>;
	//template class WOODYCHANG0611_COMMON_API  std::vector <std::vector <woodychang0611::image::ImageRGB24> >;
	//template class WOODYCHANG0611_COMMON_API  std::vector <std::vector <woodychang0611::image::ImageGray8> >;
#endif