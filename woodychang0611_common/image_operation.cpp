#include "image_operation.h"
#include <math.h>
#include <string.h>
using woodychang0611::color::RGB24;
using woodychang0611::geometry::Point2D;
using woodychang0611::geometry::IPoint2D;
using woodychang0611::geometry::FLOAT;



namespace woodychang0611{
	namespace image{

		
		//Encode src Img into BMP
		//Return size of the bitmap without header

		UINT32 EncodeBMP(const ImageRGB24 &src_image, MemBlock &dst_mem){ 
			//Bitmap Format
			//1~14 Byte BMP file header
			//15~54 Byte Bitmap information (DIB header)
			//55~ Byte Bitmap data
			
			if(!src_image.IsValid()) {
				woodychang0611::diagnostics::SendWarning(_T("EncodeBMP Image is not vaild" ));
				dst_mem.SetSize(0);
				return 0;
			}
			BITMAPFILEHEADER m_hdr;
			BITMAPINFOHEADER m_dib;
			UINT16 m_un16Width;
			UINT16 m_un16Height;
			UINT8 u8BytePerPixel;
			UINT32 un32BytePerLine;
			UINT32 un32BMPSize; //Size of the bitmap without header


			m_un16Width = src_image.GetImageWidth();
			m_un16Height = src_image.GetImageHeight();
			u8BytePerPixel = 3; //24Bits BMP
			un32BytePerLine = 4*((m_un16Width * (UINT16) u8BytePerPixel+3)/4);//must be multiple of 4  Round Up
			un32BMPSize = m_un16Height * un32BytePerLine;

			m_dib.un32Size = 40;
			m_dib.un32Width = m_un16Width;
			m_dib.n32Height = m_un16Height;
			m_dib.un16Planes = 1;
			m_dib.un16BitCount = u8BytePerPixel *8;
			m_dib.un32Compression = 0;
			m_dib.un32SizeImg =un32BMPSize;
			m_dib.un32XPelsPerMeter = 0;
			m_dib.un32YPelsPerMeter = 0;
			m_dib.un32ClrUsed = 0;
			m_dib.un32ClrImportant = 0;

			// Initialize the bitmap header
			m_hdr.un16Type          = 0x4D42; //BM
			m_hdr.un16Size_l          = (un32BMPSize + 54)&0xFFFF;
			m_hdr.un16Size_h          = static_cast<UINT16> ((un32BMPSize + 54)>>8);
			m_hdr.un16Reserved1     = 0;
			m_hdr.un16Reserved2     = 0;
			m_hdr.un16OffBits_l       = 54;
			m_hdr.un16OffBits_h       = 0;

			dst_mem.SetSize(un32BMPSize + 54);
			memcpy(dst_mem.GetDataPointer(),(void*)&m_hdr,14);
			memcpy(dst_mem.GetDataPointer()+14,(void*)&m_dib,40);
				for (UINT16 i =0;i<m_un16Height;i++){
					DatCopy(dst_mem.GetDataPointer()+54+(m_un16Height-i-1)*un32BytePerLine,
					src_image.GetDataPointer()+i*m_un16Width,
					m_un16Width*u8BytePerPixel);
				}
			return un32BMPSize;
		}


		//Decode src BMP into Img
		//Return size of the bitmap without header
		UINT32 DecodeBMP(ImageRGB24 &dst, const MemBlock &src){ 
			//Bitmap Format
			//1~14 Byte BMP file header
			//15~54 Byte Bitmap information (DIB header)
			//55~ Byte Bitmap data
			//Note: None Compression 16Bits, 24bits, 32Bits BMP only
			ImageRGB24 dummy;
			dst.CopyFrom(dummy);
			BITMAPFILEHEADER * bitmap_file_header_pointer;
			BITMAPINFOHEADER * bitmap_info_header_pointer;
			if(src.GetSize() < 55) return 0; //BMP must be bigger than 55 Bytes
			bitmap_file_header_pointer =reinterpret_cast <BITMAPFILEHEADER*>
				(const_cast<UINT8*>(src.GetDataPointer()));
			bitmap_info_header_pointer =reinterpret_cast<BITMAPINFOHEADER *>
				(const_cast<UINT8*>(src.GetDataPointer()+14));

			UINT8 byte_per_pixel=static_cast <UINT8> (bitmap_info_header_pointer->un16BitCount/8);
			UINT32 offset_bits = (bitmap_file_header_pointer->un16OffBits_l+(bitmap_file_header_pointer->un16OffBits_h<<8));

			if(bitmap_file_header_pointer->un16Type !=0x4d42) {
				woodychang0611::diagnostics::SendWarning(_T("DecodeBMP Format not 'BM'"));
				return 0; //Check Format must be 'BM'
			}
			if(bitmap_info_header_pointer->un32Compression !=0) {
				woodychang0611::diagnostics::SendWarning(_T("DecodeBMP not Uncompression BMP"));
				return 0;//None Compression  BMP only
			}
			if(bitmap_info_header_pointer->un32Width >IMAGE_MAX_WIDTH ||
				bitmap_info_header_pointer->n32Height > IMAGE_MAX_HEIGHT) {
					woodychang0611::diagnostics::SendWarning(_T("DecodeBMP image size exceed limits"));
					return 0; //Image size exceed limits
			}
			
			UINT16 image_width =static_cast <UINT16>(bitmap_info_header_pointer->un32Width);
			UINT16 image_height = static_cast <UINT16>(bitmap_info_header_pointer->n32Height);
			UINT16 src_bitmap_byte_per_line =
				4*((image_width * static_cast<UINT16> (byte_per_pixel)+3)/4);//must be multiple of 4 
			if(src.GetSize() - offset_bits < static_cast<UINT32>(src_bitmap_byte_per_line*image_height)) return 0; //Check bitmap size

			//Transfer 16b 32b to 24b Color Bitmap
				UINT8 r = 0,g = 0,b = 0;
				ImageRGB24 temp_image(image_width,image_height);
				UINT8 *pBitmapData =const_cast<UINT8 *>	(src.GetDataPointer()+offset_bits);
				UINT32 un32ReadPos=0;
				for (INT16 i =0 ;i<image_height;i++){
					un32ReadPos =(image_height-1-i)*src_bitmap_byte_per_line;
					for (UINT16 j =0;j<image_width;j++){
						if (byte_per_pixel == 3 || byte_per_pixel == 4){ //24Bits 8r8g8b or 32Bits 8r8g8b8x
							r = pBitmapData[un32ReadPos+2];
							g = pBitmapData[un32ReadPos+1];
							b = pBitmapData[un32ReadPos];
						}
						if (byte_per_pixel ==2){ //16Bits 5r5g5b
							UINT32 color_data; //2 Bytes
							color_data = (pBitmapData[un32ReadPos+1] <<8)+pBitmapData[un32ReadPos] ;
							r=(UINT8)(((color_data & 0x7C00) >>7)+((color_data & 0x7C00 )>>12));  //  0x7C00 = 0111110000000000
							g=(UINT8)(((color_data & 0x03E0) >>2)+((color_data & 0x03E0) >>7)); //   0x03E0 = 0000001111100000
							b=(UINT8)(((color_data & 0x001F) <<3)+((color_data & 0x001F) >>2)); //    0x001F = 0000000000011111
						}
						temp_image.GetPixel(j,i).r_=r;
						temp_image.GetPixel(j,i).g_=g;
						temp_image.GetPixel(j,i).b_=b;

						un32ReadPos  +=byte_per_pixel;
					}
				}
				dst.CopyFrom(temp_image);
				return image_height*image_width*sizeof(woodychang0611::color::RGB24);
		}

		//Gray Level Greater than threshold_value will be set to 255 and set others to 0 
		void Threshold (const ImageGray8 &src, ImageGray8 &dst,const UINT16 threshold_value)
		{
			if(!src.IsValid()) {
				woodychang0611::diagnostics::SendWarning(_T("Threshold Image is not vaild" ));
				return;
			}
			ImageGray8 image_temp;
			image_temp.CopyFrom(src);
			dst.CopyFrom (image_temp);
			UINT32 pixel_count = image_temp.GetImagePixelCount();
			for (UINT32 i =0; i <pixel_count;i++)
			{
				if (image_temp.GetDataPointer()[i].value_ >threshold_value)
					dst.GetDataPointer()[i].value_=255;
				else dst.GetDataPointer()[i]  =0;
			}
		}

		//Gray Level between  threshold_value1 and threshold_value2 will set to 255 and set others to 0 
		void Threshold (const ImageGray8& src,ImageGray8 & dst,const UINT16 threshold_value1,const UINT16 threshold_value2)
		{
			if(!src.IsValid()) return; //end if src is not valid
			ImageGray8 image_temp;
			image_temp.CopyFrom(src);
			dst.CopyFrom (image_temp);
			UINT32 pixel_count = image_temp.GetImagePixelCount();
			for (UINT32 i =0; i <pixel_count;i++)
			{
				if (image_temp.GetDataPointer()[i].value_ <threshold_value2  && 
					image_temp.GetDataPointer()[i].value_ >threshold_value1 )
					dst.GetDataPointer()[i]=255;
				else dst.GetDataPointer()[i]  =0;
			}
		}

		void ThresholdLocal (const ImageGray8 &src, ImageGray8 &dst,const UINT16 threshold_value,const UINT16 average_range)
		{
			if(!src.IsValid()) return;
			ImageGray8 image_temp;
			image_temp.CopyFrom(src);
			UINT16 width = image_temp.GetImageWidth();
			UINT16 height = image_temp.GetImageHeight();
			woodychang0611::math::Tensor<FLOAT64,2> data(height,width);
			woodychang0611::math::Tensor<FLOAT64,2> array2D_temp1(height,width);
			woodychang0611::math::Tensor<FLOAT64,2> array2D_temp2(height,width);
			woodychang0611::math::Tensor<FLOAT64,2> array2D_temp3(height,width);
			woodychang0611::math::Tensor<FLOAT64,2> array2D_temp4(height,width);

			UINT32 pixel_count = image_temp.GetImagePixelCount();
			for (UINT32 i=0;i<pixel_count;i++) data.GetDataPointer()[i]=image_temp.GetDataPointer()[i].value_;
			woodychang0611::math::Tensor<FLOAT64,2> filter1(average_range,1);
			for (INT32 i=0;i<filter1.GetElementCount();i++) filter1.GetDataPointer()[i]=1.0/filter1.GetElementCount();
			woodychang0611::math::Tensor<FLOAT64,2> filter2(1,average_range);
			for (INT32 i=0;i<filter2.GetElementCount();i++) filter2.GetDataPointer()[i]=1.0/filter2.GetElementCount();
			Convolution2D(data,filter1,array2D_temp1,true);
			Convolution2D(array2D_temp1,filter2,array2D_temp1,true);
			Convolution2D(array2D_temp1,filter1,array2D_temp2,true);
			Convolution2D(array2D_temp2,filter2,array2D_temp2,true);
			array2D_temp3 =data-array2D_temp1;
			array2D_temp4 =data-array2D_temp2;
			for (UINT32 i=0;i<pixel_count;i++){
				if(array2D_temp3.GetDataPointer()[i]>threshold_value||array2D_temp4.GetDataPointer()[i]>threshold_value)data.GetDataPointer()[i]=255;
				else if(array2D_temp3.GetDataPointer()[i]<-threshold_value||array2D_temp4.GetDataPointer()[i]<-threshold_value)data.GetDataPointer()[i]=0;
				else data.GetDataPointer()[i]=128;
			}
			for (UINT32 i=0;i<pixel_count;i++)
				image_temp.GetDataPointer()[i]=static_cast <UINT8> (fabs(data.GetDataPointer()[i]));
			dst.CopyFrom(image_temp);
		}

		template <typename T>   UINT16 MaskOperation3(const ImageGeneric<T> &src,ImageGeneric<T> &dst,
			 const FLOAT32 *mask){
			if(!src.IsValid()) return 0; 
			if(src.GetImageWidth()+2>IMAGE_MAX_WIDTH ||
				src.GetImageHeight()+2>IMAGE_MAX_HEIGHT) return 0; //Size exceed limits
			ImageGeneric <T> temp_image;
			Canvas(src,temp_image,1);
			dst = ImageGeneric <T> (src.GetImageWidth(),src.GetImageHeight());
			UINT16 temp_image_width = temp_image.GetImageWidth();
			UINT16 temp_image_height = temp_image.GetImageHeight();
			//Fill Boundary
			for (UINT16 i=1;i<temp_image_height-1;++i){
				temp_image.GetPixel(0,i)= temp_image.GetPixel(1,i);
				temp_image.GetPixel(temp_image_width-1,i)= 
					temp_image.GetPixel(temp_image_width-2,i);
			}
			for (UINT16 i=0;i<temp_image_width;++i){
				temp_image.GetPixel(i,0)= temp_image.GetPixel(i,1);
				temp_image.GetPixel(i,temp_image_height-1)= 
					temp_image.GetPixel(i,temp_image_height-2);
			}
			for (UINT16 i=0;i<dst.GetImageWidth();++i){
				for (UINT16 j=0;j<dst.GetImageHeight();++j){
					T src_color[9];
					for (UINT16 k=0;k<9;k++){
						src_color[k] = temp_image.GetPixel(i+(k%3),j+(k/3));
					}
					dst.GetPixel(i,j) = woodychang0611::color::MixColor(src_color,mask,9);
				}
			}
			return 0;
		}

		template <typename T>    void Sharpen(const ImageGeneric<T> &src,ImageGeneric<T> &dst){
			if(!src.IsValid()) return;
			FLOAT32 mask[9]={-1.0f,-1.0f,-1.0f,-1.0f,9.0f,-1.0f,-1.0f,-1.0f,-1.0f};
			MaskOperation3(src,dst,mask);
		}
		template <typename T> void Sharpen(const ImageGeneric<T> &src,ImageGeneric<T> &dst,FLOAT32 intensity){
			if(!src.IsValid()) return;
			if(intensity<0.035f && intensity>-0.035f){
				dst.CopyFrom(src);
				return;
			}
			FLOAT32 mask[9]={-1.0f*intensity,-1.0f*intensity,-1.0f*intensity,-1.0f*intensity,9.0f*intensity,-1.0f*intensity,-1.0f*intensity,-1.0f*intensity,-1.0f*intensity};
			MaskOperation3(src,dst,mask);
		}
		template <typename T>    void Blur(const ImageGeneric<T> &src,ImageGeneric<T> &dst){
			if(!src.IsValid()) return;
			FLOAT32 mask[9]={0.111f,0.111f,0.111f,0.111f,0.111f,0.111f,0.111f,0.111f,0.111f};
			MaskOperation3(src,dst,mask);
		}
		template <typename T> void Blur(const ImageGeneric<T> &src,ImageGeneric<T> &dst,FLOAT32 intensity){
			if(!src.IsValid()) return;
			intensity = (woodychang0611::max)(0.0f,intensity);
			intensity = (woodychang0611::min)(1.0f,intensity);
			if(intensity<0.035f){
				dst.CopyFrom(src);
				return;
			}
			FLOAT32 mask[9]={0.111f*intensity,0.111f*intensity,0.111f*intensity,0.111f*intensity,0.111f*intensity,0.111f*intensity,0.111f*intensity,0.111f*intensity,0.111f*intensity};
			MaskOperation3(src,dst,mask);
		}

		template <typename T> void HighPassFilter(const ImageGeneric <T> &src,ImageGeneric <T> &dst,FLOAT32 intensity){
			if(!src.IsValid()) return;
			if(intensity<0.0039f && intensity>-0.0039f){//過小無變化


				return;
			}
			FLOAT32 mask[9]={intensity,intensity*(-2.0f),intensity,intensity,intensity*(-2.0f),intensity,intensity,intensity*(-2.0f),intensity};
			MaskOperation3(src,dst,mask);
		}
		template <typename T> void SobelFilterHorizontal(const ImageGeneric <T> &src,ImageGeneric <T> &dst, FLOAT32 intensity){
			if(!src.IsValid()) return;
			if(intensity<0.0039f && intensity>-0.0039f){//過小無變化


				return;
			}
			FLOAT32 mask[9]={intensity*(-1.0f),intensity*(0),intensity*(1.0f),intensity*(-2.0f),intensity*(0),intensity*(2.0f),intensity*(-1.0f),intensity*(0),intensity*(1.0f)};
			MaskOperation3(src,dst,mask);
		}
		template <typename T> void SobelFilterVertical(const ImageGeneric <T> &src,ImageGeneric <T> &dst, FLOAT32 intensity){
			if(!src.IsValid()) return;
			if(intensity<0.0039f && intensity>-0.0039f){//過小無變化


				return;
			}
			FLOAT32 mask[9]={intensity*(-1.0f),intensity*(-2.0f),intensity*(-1.0f),intensity*(0.0f),intensity*(0.0f),intensity*(0.0f),intensity*(1.0f),intensity*(2.0f),intensity*(1.0f)};
			MaskOperation3(src,dst,mask);
		}



		UINT16 Compare(const ImageGray8 &src1,const ImageGray8 &src2, ImageGray8 &dst){

			if(!src1.IsValid()) return 0;
			if(!src2.IsValid()) return 0;
			if (src1.GetImageWidth ()== src2.GetImageWidth() &&
				src1.GetImageHeight()  == src2.GetImageHeight())
				{
				UINT32 pixel_count = src1.GetImagePixelCount ();
				ImageGray8 image_temp (src1.GetImageWidth(),src1.GetImageHeight());
				for(UINT32 i=0;i<pixel_count;i++){
					if (src1.GetDataPointer()[i].value_>src2.GetDataPointer()[i].value_)
					image_temp.GetDataPointer()[i] = (src1.GetDataPointer()[i].value_ - src2.GetDataPointer()[i].value_);				
					else
					image_temp.GetDataPointer()[i] = (src2.GetDataPointer()[i].value_ - src1.GetDataPointer()[i].value_);				
				}
				dst.CopyFrom(image_temp);
				}
			return 0;
		}

		UINT16 DilateDisk(const ImageGray8 &src, ImageGray8 &dst,UINT16 radius){ //min of the gray values in a defined neighborhood
			if(!src.IsValid()) return 0;
			if(radius>IMAGE_MORPHOLOGY_MAX_RADIUS) radius=IMAGE_MORPHOLOGY_MAX_RADIUS;
			if(src.GetImageWidth()+radius*2>IMAGE_MAX_WIDTH ||
				src.GetImageHeight()+radius*2>IMAGE_MAX_HEIGHT) return 0; //Size exceed limits
			UINT16 width = src.GetImageWidth ();
			UINT16 height = src.GetImageHeight ();
			ImageGray8 dst_image_temp,src_image_temp;
			src_image_temp.CopyFrom(src);
			Canvas (src_image_temp,dst_image_temp,radius);
			UINT16 dst_byte_per_line = dst_image_temp.GetBytePerLine();
			UINT16 src_byte_per_line = src_image_temp.GetBytePerLine();

			//Disk 
			UINT16 disk_height = 2 * radius  +1;
			UINT16 * disk_width = new UINT16 [disk_height];
			UINT16 disk_pixel_count = 0; //Pixel Count of the disk
			INT16 * byte_offset;
			for (UINT16 i =0; i < disk_height; i++){
				FLOAT64 R = radius +0.4;
				FLOAT64 H;
				H = radius - i	;
				disk_width [i] =UINT16 (sqrt(R*R-H*H))*2+1;
				disk_pixel_count += disk_width [i];
			}
			byte_offset = new INT16[disk_pixel_count]; //Offet for each pixel of the disk
			UINT16 count =0;

			//Example :
			// radius = 2
			// byte_offset
			//
			//        1    0    2
			//
			// 6     4    3    5    7
			//
			//11    9    8  10  12
			//
			//16  14  13  15  17
			//
			//      19  18  20

			for (UINT16 i =0;i<disk_height;i++){
				for (UINT16 j=0;j<=(disk_width[i] -1)/2;j++){
					if (j==0){
						byte_offset[count] = (i - radius) * dst_byte_per_line;
						count++;
					}
					if (j!=0){
						byte_offset[count] = (i - radius) * dst_byte_per_line+j;
						count++;
						byte_offset[count] = (i - radius) * dst_byte_per_line-j;
						count++;
					}
				}
			}
			UINT32 read_address = 0;
			UINT32 write_address = 0;

			for (UINT16 i =0;i<height;i++){
				for (UINT16 j=0;j<width;j++){
					read_address = i*src_byte_per_line + j;
					write_address = (i+radius)*dst_byte_per_line + (j+radius);
					UINT8  src_gray_level = src_image_temp.GetDataPointer()[read_address].value_;
					if (src_gray_level ==0x00 ) continue; //continue when src is black
						for (UINT16 k=0; k<disk_pixel_count;k++){
							if (dst_image_temp.GetDataPointer()[write_address+ byte_offset[k]].value_<src_gray_level)
								dst_image_temp.GetDataPointer()[write_address+ byte_offset[k]].value_ =src_gray_level;
					}
				}
			}

			delete []disk_width;
			delete []byte_offset;
			Trim(dst_image_temp,dst,radius,radius,width,height);
			return 0;
		}


		UINT16 ErodeDisk(const ImageGray8 &src, ImageGray8 &dst,UINT16 radius){ //min of the gray values in a defined neighborhood
			if(!src.IsValid()) return 0;
			if(radius>IMAGE_MORPHOLOGY_MAX_RADIUS) radius=IMAGE_MORPHOLOGY_MAX_RADIUS;
			if(src.GetImageWidth()+radius*2>IMAGE_MAX_WIDTH ||
				src.GetImageHeight()+radius*2>IMAGE_MAX_HEIGHT) return 0; //Size exceed limits
			UINT16 width = src.GetImageWidth ();
			UINT16 height = src.GetImageHeight ();
			ImageGray8 dst_image_temp,src_image_temp;
			src_image_temp.CopyFrom(src);
			Canvas (src_image_temp,dst_image_temp,radius);
			UINT16 dst_byte_per_line = dst_image_temp.GetBytePerLine();
			UINT16 src_byte_per_line = src_image_temp.GetBytePerLine();

			//Disk 
			UINT16 disk_height = 2 * radius  +1;
			UINT16 * disk_width = new UINT16 [disk_height];
			UINT16 disk_pixel_count = 0; //Pixel Count of the disk
			INT16 * byte_offset;
			for (UINT16 i =0; i < disk_height; i++){
				FLOAT64 R = radius +0.4;
				FLOAT64 H;
				H = radius - i	;
				disk_width [i] =UINT16 (sqrt(R*R-H*H))*2+1;
				disk_pixel_count += disk_width [i];
			}
			byte_offset = new INT16[disk_pixel_count]; //Offet for each pixel of the disk
			UINT16 count =0;

			//Example :
			// radius = 2
			// byte_offset
			//
			//        1    0    2
			//
			// 6     4    3    5    7
			//
			//11    9    8  10  12
			//
			//16  14  13  15  17
			//
			//      19  18  20

			for (UINT16 i =0;i<disk_height;i++){
				for (UINT16 j=0;j<=(disk_width[i] -1)/2;j++){
					if (j==0){
						byte_offset[count] = (i - radius) * dst_byte_per_line;
						count++;
					}
					if (j!=0){
						byte_offset[count] = (i - radius) * dst_byte_per_line+j;
						count++;
						byte_offset[count] = (i - radius) * dst_byte_per_line-j;
						count++;
					}
				}
			}
			UINT32 read_address = 0; 
			UINT32 write_address = 0;

			for (UINT16 i =0;i<height;i++){
				for (UINT16 j=0;j<width;j++){
					read_address = i*src_byte_per_line + j;
					write_address = (i+radius)*dst_byte_per_line + (j+radius);
					UINT8  src_gray_level = src_image_temp.GetDataPointer()[read_address].value_;
					if (src_gray_level ==0xFF) continue; //continue when src is white
					for (UINT16 k=0; k<disk_pixel_count;k++){
							if (dst_image_temp.GetDataPointer()[write_address+ byte_offset[k]].value_ >src_gray_level)
								dst_image_temp.GetDataPointer()[write_address+ byte_offset[k]].value_ =src_gray_level;
					}
				}
			}

			delete []disk_width;
			delete []byte_offset;
			Trim(dst_image_temp,dst,radius,radius,width,height);
			return 0;
		}

		UINT16 OpenDisk(const ImageGray8 &src, ImageGray8 &dst, UINT16 radius){   //erosion followed by dilation
			ErodeDisk(src,dst,radius);
			DilateDisk(dst,dst,radius);
			return 0;
		}
		UINT16 CloseDisk(const ImageGray8 &src, ImageGray8 &dst, UINT16 radius){ //dilation followed by dilation
			DilateDisk(src,dst,radius);
			ErodeDisk(dst,dst,radius);
			return 0;
		}

		UINT16 WhiteTopHatDisk(const ImageGray8 &src, ImageGray8 &dst, UINT16 radius){ //source image minus opened image
			if(!src.IsValid()) return 0; //end if src is not valid
			ImageGray8 image_temp1,image_temp2;
			image_temp1.CopyFrom(src);
			OpenDisk(image_temp1,image_temp2,radius);
			Compare(image_temp1,image_temp2,dst);
			return 0;
		}

		UINT16 BlackTopHatDisk(const ImageGray8 &src, ImageGray8 &dst, UINT16 radius){ //closed image minus source image
			if(!src.IsValid()) return 0; //end if src is not valid
			ImageGray8 image_temp1,image_temp2;
			image_temp1.CopyFrom(src);
			CloseDisk(image_temp1,image_temp2,radius);
			Compare(image_temp1,image_temp2,dst);
			return 0;
		}
		UINT16 MorphoGradientDisk(const ImageGray8 &src, ImageGray8 &dst, UINT16 radius){//difference between dilation and dilation
			if(!src.IsValid()) return 0; //end if src is not valid
			ImageGray8 image_temp;
			DilateDisk(src,image_temp,radius);
			ErodeDisk(src,dst,radius);
			Compare(dst,image_temp,dst);
			return 0;
		}
		template <typename T> UINT16 Overlay(const ImageGeneric<T>  &src,const ImageGeneric<T>  &target,ImageGeneric<T>  &dst,INT16 left, INT16 top, 
			const T* bg_color,UINT8 alpha){
			if (!src.IsValid()) return 0;
			if (!target.IsValid()) return 0;
			if (alpha==0) return 0;
			if (&target != &dst){
				dst.CopyFrom(target);
			}
			INT16 target_left = left;
			INT16 target_top = top;
			if (target_left<0) target_left=0;
			if (target_top<0) target_top=0;
			if (target_left >target.GetImageWidth()-1 ||
				target_top>target.GetImageHeight()-1) return 0;
			INT16 target_right = left+src.GetImageWidth();
			INT16 target_bottom = top+src.GetImageHeight();
			if (target_right > target.GetImageWidth()) target_right = target.GetImageWidth();
			if (target_bottom> target.GetImageHeight()) target_bottom = target.GetImageHeight();
			if (target_right<1 || target_bottom<1) return 0;
			if (alpha ==0xFF){
				for (UINT16 i=target_left;i<target_right;++i){
					for (UINT16 j=target_top;j<target_bottom;++j){
						T src_color = src.GetPixel(i-left,j-top);
						if (bg_color ==NULL)
							dst.GetPixel(i,j) = src_color;
						else if(*bg_color != src_color)
							dst.GetPixel(i,j) = src_color;
					}
				}
			}else{
				for (UINT16 i=target_left;i<target_right;++i){
					for (UINT16 j=target_top;j<target_bottom;++j){
						T src_color = src.GetPixel(i-left,j-top);
						if (bg_color ==NULL)
							dst.GetPixel(i,j) = MixColor(src_color,dst.GetPixel(i,j),alpha);
						else if(*bg_color != src_color)
							dst.GetPixel(i,j) = MixColor(src_color,dst.GetPixel(i,j),alpha);
					}
				}
			}
			return 0;
		}


	template <typename T>  UINT16 Scale(
		const ImageGeneric<T> &src, ImageGeneric<T> &dst,
		const UINT16 new_width,const UINT16 new_height,InterpolationMode mode){
			if(!src.IsValid()) return 0;
			if(new_width>IMAGE_MAX_WIDTH||new_height>IMAGE_MAX_HEIGHT) return 0; //Size exceed limit
			if (src.GetImageWidth() == new_width && src.GetImageHeight() == new_height){
				dst.CopyFrom(src);
			}
			else{
			ImageGeneric<T>  src_image_temp; 
			src_image_temp.CopyFrom(src);
			ImageGeneric<T> dst_image_temp(new_width,new_height); 
			UINT16 src_width = src_image_temp.GetImageWidth ();
			UINT16 src_height = src_image_temp.GetImageHeight ();
			FLOAT scale_x = static_cast <FLOAT>(src_width)/new_width;
			FLOAT scale_y = static_cast <FLOAT>(src_height)/new_height;
			for (UINT16 i =0;i<new_width;i++){
				for (UINT16 j=0;j<new_height;j++){
					FLOAT src_x = (i+0.5f) *scale_x -0.5f; 
					FLOAT src_y =(j+0.5f) *scale_y -0.5f;
					dst_image_temp.GetPixel(i,j)  = src_image_temp.GetPixel(src_x,src_y,mode);
				}//j
			}//i
			dst.CopyFrom(dst_image_temp);
			}

			return 0;
	}
	template <typename T>  UINT16 RotateDegree(const ImageGeneric<T> &src, ImageGeneric<T> &dst,
		const woodychang0611::geometry::FLOAT& degree){
		return RotateRadian(src,dst,degree*3.14159f/180);
	}

	template <typename T>  UINT16 RotateRadian(const ImageGeneric<T> &src, ImageGeneric<T> &dst,
		const woodychang0611::geometry::FLOAT& radian){

	FLOAT ang = radian;//degree*3.14159f*2/360;
	ImageGeneric<T> src_temp(src);
	UINT16 new_width= static_cast <UINT16>(
		src_temp.GetImageWidth() *fabs(cos(ang)) +src_temp.GetImageHeight()*fabs(sin(ang)));
	UINT16 new_height=static_cast <UINT16>(
		src_temp.GetImageWidth() *fabs(sin(ang))+src_temp.GetImageHeight()*fabs(cos(ang)));
	dst = ImageGeneric<T> (new_width,new_height);
	dst.Clear();

	Point2D src_center_point(src_temp.GetImageWidth()/2.0f,src_temp.GetImageHeight()/2.0f);
	Point2D dst_center_point(new_width/2.0f,new_height/2.0f);
	Point2D offset=dst_center_point-src_center_point;
	for(UINT16 i=0;i<new_width;++i){
		for(UINT16 j=0;j<new_height;++j){
			Point2D dst_point(i,j);
			Point2D src_point = Rotate(dst_point,dst_center_point,-ang)-offset;
			if (! src_temp.IsWithin(src_point)) continue;
	
			dst.GetPixel(i,j) =src.GetPixel(src_point.x_,src_point.y_,INTER_BILINEAR);
		}
	}

		return 0;
	}
	template <typename T>  UINT16 Canvas(
		const ImageGeneric<T> &src, ImageGeneric<T> &dst,
		const UINT16 frame_width){
			if(!src.IsValid()) return 0;
			if(src.GetImageWidth() + 2* frame_width>IMAGE_MAX_WIDTH ||
				src.GetImageHeight() + 2* frame_width >IMAGE_MAX_HEIGHT) return 0; //Size exceed limit
			UINT16 src_width, src_height;
			src_width =src.GetImageWidth();
			src_height =src.GetImageHeight();
			ImageGeneric<T>  image_temp(src_width+frame_width*2,src_height+frame_width*2);
			for (UINT16 i =0; i<src_width;i++){
				for (UINT16 j =0; j<src_height;j++){
					image_temp.GetPixel(i+frame_width,j+frame_width)=src.GetPixel(i,j);
				}
			}
			dst.CopyFrom(image_temp);
			return 0;
		}

		template <typename T>  UINT16 Trim	(const ImageGeneric<T> &src, ImageGeneric<T> &dst,
			INT16 left, INT16 top, UINT16 dst_width, UINT16 dst_height){	
			if(!src.IsValid()) return 0;
			UINT16 src_width, src_height;
			INT16 right, bottom;
			src_width =src.GetImageWidth ();
			src_height =src.GetImageHeight ();

			if (dst_width <1 ) dst_width =1;
			if (dst_height <1 ) dst_height =1;
			if(left<0) left=0;
			if(left >src_width-1) left=src_width-1;
			if(top<0) top=0;
			if(top >src_height-1) top=src_height-1;
			right = left + dst_width-1;
			bottom = top + dst_height-1;
			if(right >src_width-1) right=src_width-1;
			if(bottom >src_height-1) bottom=src_height-1;
			dst_width = right -  left + 1;
			dst_height = bottom - top +1;
			ImageGeneric<T>  image_temp(dst_width,dst_height);
			for (UINT16 i=0;i<dst_width;++i){
				for (UINT16 j=0;j<dst_height;++j){
					image_temp.GetPixel(i,j) = src.GetPixel(i+left,j+top);
				}
			}
		//Direct Data Copy
		//	DatCopy2D(image_temp.GetDataPointer(),
		//		src.GetDataPointer()+top*src.GetImageWidth()+left,
		//		image_temp.GetBytePerLine(),dst_height,
		//		src.GetBytePerLine(),E_DAT_2D1D);
			dst.CopyFrom(image_temp);
			return 0;
		}
	
		template <typename T>  UINT16 GetFloodArea	(const ImageGeneric<T> &src,const woodychang0611::geometry::IPoint2D &target_point,
			std::vector < woodychang0611::geometry::IPoint2D> & dst){
				dst.clear();
				if (!src.IsValid()) return 0;
				if (!src.IsWithin(target_point)) return 0;
				woodychang0611::math::Tensor<UINT8,2> result_buffer(src.GetImageHeight(),src.GetImageWidth()); //紀綠結果的0 表示未使用 1表示已使用 
				std::vector < woodychang0611::geometry::IPoint2D> boundary_point[2]; //新產生的邊緣 兩個暫存區交叉使用 
				T target_color = src.GetPixel(target_point.x_,target_point.y_);
				result_buffer.SetValue(0);
				dst.push_back(target_point);
				result_buffer[target_point.y_][target_point.x_] =1;
				boundary_point[0].push_back(target_point);
				UINT8 buffer_index=0;

				// (1,0), (-1,0) (0,1) (0,-1)
				INT16 offset_x[4] ={1,-1,0,0};
				INT16 offset_y[4] ={0,0,1,-1};

				bool done = true;
				do{
					done = true;
					boundary_point[(buffer_index+1)%2].clear();
					for (UINT32 i=0;i<boundary_point[buffer_index].size();++i){
						for (INT16 j=0;j<4;++j){
							INT16 x = boundary_point[buffer_index][i].x_+offset_x[j];
							INT16 y = boundary_point[buffer_index][i].y_+offset_y[j];
							if (src.IsWithin(x,y) && src.GetPixel(x,y) ==target_color && result_buffer[y][x] ==0){
								boundary_point[(buffer_index+1)%2].push_back(woodychang0611::geometry::IPoint2D(x,y));
								dst.push_back(woodychang0611::geometry::IPoint2D(x,y));
								result_buffer[y][x] =1;
								done = false;
							}
						}
					}
					buffer_index = (buffer_index+1)%2;
				}while(done==false);
			return 0;
		}
							
		template <typename T> void JoinImageHorizontal(const std::vector<ImageGeneric <T> >&src,ImageGeneric <T> &dst){
			if (src.size() ==0) return; // No image inside the vector
			UINT16 max_height=0;
			INT32 total_width=0; //Delcare as INT32 instead of UINT16 to avoid overflow
			for (UINT32 i=0;i<src.size();++i){
				max_height = (woodychang0611::max)(max_height,src[i].GetImageHeight());
				total_width += src[i].GetImageWidth();
			}
			if(total_width>IMAGE_MAX_WIDTH ||
				max_height >IMAGE_MAX_HEIGHT) return; //Size exceed limit
			dst = ImageGeneric <T> ( static_cast <UINT16>(total_width), static_cast <UINT16>(max_height));

			UINT16 left=0;
			for (UINT32 i=0;i<src.size();++i){
				Overlay(src[i],dst,dst,left,0);
				left += src[i].GetImageWidth();
			}
		}
		template <typename T> void JoinImageVertical(const std::vector<ImageGeneric <T> > &src,ImageGeneric <T> &dst){
			if (src.size() ==0) return; // No image inside the vector
			UINT16 max_width =0;
			INT32 total_height=0; //Delcare as INT32 instead of UINT16 to avoid overflow
			for (UINT32 i=0;i<src.size();++i){
				max_width = (woodychang0611::max)(max_width,src[i].GetImageWidth());
				total_height += src[i].GetImageHeight();
			}
			if(max_width>IMAGE_MAX_WIDTH ||
				total_height >IMAGE_MAX_HEIGHT) return; //Size exceed limit
			dst = ImageGeneric <T> ( static_cast <UINT16>(max_width), static_cast <UINT16>(total_height));

			UINT16 top=0;
			for (UINT32 i=0;i<src.size();++i){
				Overlay(src[i],dst,dst,0,top);
				top += src[i].GetImageHeight();
			}
		}
		template <typename T> void JoinImage2D(const std::vector< std::vector<ImageGeneric <T> > > &src,ImageGeneric <T> &dst){
			std::vector<ImageGeneric <T> > image_array;
			for (UINT32 i=0; i < src.size() ;++i){
				ImageGeneric <T> image_temp;
				for (UINT32 j=0; j < src[i].size();++j){
					JoinImageHorizontal(src[i],image_temp);
				}
				image_array.push_back(image_temp);
			}
			JoinImageVertical(image_array,dst);
		}

		bool ImagePointConnect(const woodychang0611::image::ImageRGB24 &src, woodychang0611::image::ImageRGB24 &dst, const UINT16 total_join_points,const FLOAT64 &connect_angle,
							   const UINT8 connect_point_color_r,const UINT8 connect_point_color_g,const UINT8 connect_point_color_b){
			if(!src.IsValid())return false;

			//把角度修正到-90~90
			INT16 shang;
			FLOAT64 angle;
			shang=(INT16)(connect_angle/180);
			angle=connect_angle-shang*180.0; //-180~180
			if(angle<-90.0)    angle+=180.0; //0~90
			if(angle>90.0)     angle-=180.0; //0~90
			dst.CopyFrom(src);

			UINT16 width=src.GetImageWidth();
			UINT16 height=src.GetImageHeight();

			UINT16 start_position= 0;
			UINT16 end_position =0;
			bool have_start_position=false;

			//水平方向連接
			if(-0.1<angle && angle<0.1 ){
				for(INT16 y=0;y<height;y++){
					have_start_position=false;
					for(INT16 x=0;x<width;x++){
						if(dst.GetPixel(x,y).r_==connect_point_color_r &&
						   dst.GetPixel(x,y).g_==connect_point_color_g &&
						   dst.GetPixel(x,y).b_==connect_point_color_b){
							
						   if(have_start_position){
								end_position=x;
								if(end_position-start_position<total_join_points){
										for(INT16 k=start_position+1;k<end_position;k++){
											dst.GetPixel(k,y).r_=connect_point_color_r; 
											dst.GetPixel(k,y).g_=connect_point_color_g;
											dst.GetPixel(k,y).b_=connect_point_color_b;
										}
								}
								start_position=x;
							}
							else{
								start_position=x;
								have_start_position=true;
							}
						}
					}
				}
			}
			//垂直方向連接
			else if(angle>89.9 || angle<-89.9){
				for(INT16 x=0;x<width;x++){
					have_start_position=false;
					for(INT16 y=0;y<height;y++){
						if(dst.GetPixel(x,y).r_==connect_point_color_r &&
						   dst.GetPixel(x,y).g_==connect_point_color_g &&
						   dst.GetPixel(x,y).b_==connect_point_color_b){
							
						   if(have_start_position){
								end_position=y;
								if(end_position-start_position<total_join_points){
										for(INT16 k=start_position+1;k<end_position;k++){
											dst.GetPixel(x,k).r_=connect_point_color_r; 
											dst.GetPixel(x,k).g_=connect_point_color_g;
											dst.GetPixel(x,k).b_=connect_point_color_b;
										}
								}
								start_position=y;
							}
							else{
								start_position=y;
								have_start_position=true;
							}
						}
					}
				}
			}
			//角度偏垂直

			else if(angle>45.0 || angle<-45.0){

				//計算起點跟結束點(X方向)
				FLOAT64 tan_value = tan(angle/57.29578);
				INT16 max_deviation = (INT16)(-height/tan_value);
				INT16 x_start = max_deviation>0 ? -max_deviation : 0;
				INT16 x_end = max_deviation>0 ?   width			 : width+max_deviation;
				
				INT16 deviation=0;
				//開始連接
				for(INT16 i=x_start;i<x_end;i++){//走Width

					have_start_position=false;

					for(INT16 j=0;j<height;j++){

						deviation=(INT16)(-j/tan_value);
						INT16 x=i+deviation;

						if(x<0){
							if(max_deviation>0)
								break;
							continue;
						}
						if(x>width){
							if(max_deviation<0)
								break;
							continue;
						}

						if(dst.GetPixel(x,j).r_==connect_point_color_r &&
						   dst.GetPixel(x,j).g_==connect_point_color_g &&
						   dst.GetPixel(x,j).b_==connect_point_color_b ){

							if(have_start_position){
								end_position=j;
								if(end_position-start_position<total_join_points){
									for(INT16 k=start_position+1;k<end_position;k++){
										deviation=(INT16)(-k/tan_value);
										INT16 x2=i+deviation;
										dst.GetPixel(x2,k).r_=connect_point_color_r; 
										dst.GetPixel(x2,k).g_=connect_point_color_g;
										dst.GetPixel(x2,k).b_=connect_point_color_b;
									}
								}
								start_position=j;
							}
							else{
								start_position=j;
								have_start_position=true;
							}
						}
					}
				}
			
			}
			//角度偏水平

			else{

				//計算起點跟結束點(Y方向)
				FLOAT64 tan_value = tan(angle/57.29578);
				INT16 max_deviation =(INT16)(-width*tan_value);
				INT16 y_start = max_deviation>0 ? -max_deviation : 0;
				INT16 y_end = max_deviation>0 ?   height	     : height-max_deviation;

				INT16 deviation=0;
				//開始連接
				for(INT16 i=y_start;i<y_end;i++){//走Height

					have_start_position=false;

					for(INT16 j=0;j<width;j++){

						deviation=(INT16)(-j*tan_value);
						INT16 y=i+deviation;

						if(y<0){
							if(max_deviation<0)
								break;
							continue;
						}
						if(y>height-1){
							if(max_deviation>0)
								break;
							continue;
						}

						if(dst.GetPixel(j,y).r_==connect_point_color_r &&
						   dst.GetPixel(j,y).g_==connect_point_color_g &&
						   dst.GetPixel(j,y).b_==connect_point_color_b ){
							if(have_start_position){
								end_position=(UINT16)j;
								if(end_position-start_position<total_join_points){
									for(INT16 k=start_position+1;k<end_position;k++){
										deviation=(INT16)(-k*tan_value);
										INT16 y2=i+deviation;
										dst.GetPixel(k,y2).r_=connect_point_color_r; 
										dst.GetPixel(k,y2).g_=connect_point_color_g;
										dst.GetPixel(k,y2).b_=connect_point_color_b;
									}
								}
								start_position=(UINT16)j;
							}
							else{
								start_position=(UINT16)j;
								have_start_position=true;
							}
						}
					}
				}
			}

			return true;
		}
		
		woodychang0611::image::ImageRGB24 MaskOperation(const woodychang0611::image::ImageRGB24 &src,const std::vector<FLOAT64> &mask,const bool to_positive){
			if(!src.IsValid())return src;
			INT16 edge_range=1;
			while(edge_range*edge_range<(INT32) mask.size()){
				edge_range += 2;
			}
			INT16 extend_range= edge_range/2;
			INT16 width=src.GetImageWidth();
			INT16 height=src.GetImageHeight();
			INT16 max_x=width-1;
			INT16 max_y=height-1;
			woodychang0611::image::ImageRGB24 dst(src);
			UINT32 mask_index;
			FLOAT64 r,g,b;
			INT16 x,y;

			for(INT16 i=0;i<width;i++){
				for(INT16 j=0;j<height;j++){
					mask_index=0;
					r=0; g=0; b=0;
					for(INT16 m= -extend_range; m<= extend_range;m++){
						for(INT16 n= -extend_range; n<= extend_range;n++){
							if(mask[mask_index]==0){
								mask_index++;
								continue;
							}
							x=i+n;
							y=j+m;
							if(x<0)x=0;
							else if(x>max_x)x=max_x;
							if(y<0)y=0;
							else if(y>max_y)y=max_y;
							r+=mask[mask_index]*src.GetPixel(x,y).r_;
							g+=mask[mask_index]*src.GetPixel(x,y).g_;
							b+=mask[mask_index]*src.GetPixel(x,y).b_;
							mask_index++;
						}
					}
					if(to_positive){
						if(r<0)r=-r;
						if(g<0)g=-g;
						if(b<0)b=-b;
					}
					if(r>255)r=255;
					if(r<0)r=0;								
					if(g>255)g=255;
					if(g<0)g=0;
					if(b>255)b=255;
					if(b<0)b=0;
					dst.GetPixel(i,j)= woodychang0611::color::RGB24((UINT8)r,(UINT8)g,(UINT8)b);
				}
			}
			return dst;
		}
		woodychang0611::image::ImageRGB24 SobelWithVertical( const woodychang0611::image::ImageRGB24 &src,const FLOAT64 &f64){
			std::vector<FLOAT64> mask(9);
			mask[0]=-f64;	mask[1]=0;	mask[2]=f64;
			mask[3]=-f64*2;	mask[4]=0;	mask[5]=f64*2;
			mask[6]=-f64;	mask[7]=0;	mask[8]=f64;
			return woodychang0611::image::MaskOperation(src,mask,true);
			
		};


	}//namespace woodychang0611::image
}//end of namespace woodychang0611




#undef IMAGE_OPERATION_INSTANTIATION

