
#pragma once

namespace WoodyChang0611 {
	namespace Image{
		public ref class Imaging{
		public:
			static System::Windows::Media::Imaging::BitmapImage ^ Imaging::GetBitmapImage(System::IntPtr image_ptr);
		};
	};
}