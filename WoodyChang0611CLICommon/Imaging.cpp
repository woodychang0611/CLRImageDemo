#include "stdafx.h"
#include "..\woodychang0611_common\woodychang0611_common.h"

using namespace System::Windows::Media::Imaging;
using namespace woodychang0611::image;

namespace WoodyChang0611{
	namespace Image{
		public ref class Imaging{
		public:
			static BitmapImage ^ Imaging::GetBitmapImage(System::IntPtr image_ptr);
		};

		//從woodychang0611::image::ImageRGB24 變成 System.Windows.Media.Imaging.BitmapImage
		BitmapImage ^ Imaging::GetBitmapImage(System::IntPtr image_ptr){
		 ImageRGB24* sourceImage = (ImageRGB24*)  (void*)image_ptr;
		if (sourceImage ==nullptr) {
				woodychang0611::diagnostics::SendInfomation(_T("WoodyChang0611::Image::Imaging::GetBitmapImage source is nullptr"));
				return nullptr;
			}
			LARGE_INTEGER t1,t2,freq;
			QueryPerformanceFrequency(&freq);

			QueryPerformanceCounter(&t1);
			System::Windows::Media::Imaging::BitmapImage ^bitmap = 
				gcnew System::Windows::Media::Imaging::BitmapImage();
			woodychang0611::MemBlock mem;
			woodychang0611::image::EncodeBMP(*sourceImage,mem);
			System::IO::UnmanagedMemoryStream ^ byteStreamTemp =
				gcnew System::IO::UnmanagedMemoryStream(mem.GetDataPointer(),mem.GetSize());
			try{
				bitmap->BeginInit();
				bitmap->CacheOption = System::Windows::Media::Imaging::BitmapCacheOption::OnLoad;
				bitmap->StreamSource = byteStreamTemp;
				bitmap->EndInit();
				//不同Thread傳遞Bitmap 要在EndInit()之前Freeze
				bitmap->Freeze();
			}
			catch(System::Exception ^e){
				woodychang0611::diagnostics::SendInfomation(_T("WoodyChang0611::Image::Imaging::GetBitmapImage Bitmap Init Error"));
				bitmap=nullptr;
			}
			TCHAR str[200];

			QueryPerformanceCounter(&t2);
			double t =(double) (t2.QuadPart -t1.QuadPart)/(freq.QuadPart/1000);
			_stprintf(str,_T("WoodyChang0611::Image::Imaging::GetBitmapImage Bitmap time:%f ms"),t);
			woodychang0611::diagnostics::SendInfomation(str);
			return  bitmap;
		}
	}
}