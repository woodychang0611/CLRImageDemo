#ifndef __IMAGE_OPERATION_H__
#define __IMAGE_OPERATION_H__

#include "base.h"
#include "image.h"
#include "tensor_operation.h"
//#include "vector_operation.h"
#include "mem_block.h"


namespace woodychang0611{
	namespace image{

		enum E_Channel_Type{Lum,R,G,B,X,Y};
			struct BITMAPFILEHEADER { 
			  UINT16   un16Type;
			  UINT16   un16Size_l; //Lower Part
			  UINT16   un16Size_h; //Higher Part
			  UINT16   un16Reserved1; 
			  UINT16   un16Reserved2; 
			  UINT16   un16OffBits_l; 
			  UINT16   un16OffBits_h; 
			} ;
			struct BITMAPINFOHEADER {
				UINT32  un32Size;
				UINT32  un32Width;
				INT32	n32Height;
				UINT16  un16Planes;
				UINT16  un16BitCount;
				UINT32  un32Compression;
				UINT32  un32SizeImg;
				UINT32  un32XPelsPerMeter;
				UINT32  un32YPelsPerMeter;
				UINT32  un32ClrUsed;
				UINT32  un32ClrImportant;
			} ;		
		//This file turns packing of structures off. 
		//(That is, it enables automatic alignment of structure fields.) 


		//! \brief Encode ImageRGB24 class to MemBlock class in bitmap format
		//! \param [in] ImageRGB24 &src_image Source
		//! \param [out] MemBlock &dst_mem Destination
		//! \retval Total bytes count after succesed, 0  if failed
		WOODYCHANG0611_COMMON_API UINT32 EncodeBMP(const ImageRGB24 &src_image, MemBlock &dst_mem); //Encode src Img into BMP
		//! \brief Decode ImageRGB24 class from MemBlock class in bitmap
		//! \param [out] ImageRGB24 &dst Destination
		//! \param [in] MemBlock &src Source
		//! \retval Total bytes count after succesed, 0  if failed
		WOODYCHANG0611_COMMON_API UINT32 DecodeBMP(ImageRGB24 &dst, const MemBlock &src); //Decode src BMP into Img

		// ******************************************************************************************************************
		// Threshold
		//
		// Purpose
		//		1.Gray Level Greater than un16Threshold will be set to 255 and set others to 0 
		//		2.Gray Level between  un16Threshold1 and un16Threshold2 will set to 255 and set others to 0 
		// Useage
		//		Threshold (src, dst,un16Threshold)
		//		Threshold (src, dst,un16Threshold1,un16Threshold2)
		// Description of Parameters
		//		src				-	Img image source
		//		dst				-	Img image destination 
		//		un16Threshold	-	Threshold
		//		un16Threshold1	-	Threshold
		//		un16Threshold2	-	Threshold
		// Return Value
		//		0 if invalid image
		//		1 if success
		// Example
		//		Img Simg,Simg2 ,Simg3;
		//		MemBlock mem;
		//		ReadFile("A.bmp",mem);
		//		DecodeBMP(Simg,mem);
		//		Threshold(Simg,Simg2,150) ;
		//		Threshold(Simg,Simg3,100,200) ;
		//
		// ******************************************************************************************************************

		WOODYCHANG0611_COMMON_API void Threshold (const ImageGray8 &src, ImageGray8 &dst,const UINT16 un16Threshold);
		WOODYCHANG0611_COMMON_API void Threshold (const ImageGray8 &src, ImageGray8 &dst,const UINT16 un16Threshold1,const UINT16 un16Threshold2);
		WOODYCHANG0611_COMMON_API void ThresholdLocal (const ImageGray8 &src, ImageGray8 &dst,const UINT16 un16Threshold,const UINT16 un16AverageRange);
		WOODYCHANG0611_COMMON_API UINT16 Compare(const ImageGray8 &src1, const ImageGray8 &src2, ImageGray8 &dst);

		WOODYCHANG0611_COMMON_API UINT16 DilateDisk(const ImageGray8 &src, ImageGray8 &dst,UINT16 radius);//max of the gray values in a defined neighborhood
		WOODYCHANG0611_COMMON_API UINT16 ErodeDisk(const ImageGray8 &src, ImageGray8 &dst,UINT16 radius);//min of the gray values in a defined neighborhood
		WOODYCHANG0611_COMMON_API UINT16 OpenDisk(const ImageGray8 &src, ImageGray8 &dst, UINT16 radius);//erosion followed by dilation
		WOODYCHANG0611_COMMON_API UINT16 CloseDisk(const ImageGray8 &src, ImageGray8 &dst, UINT16 radius);//dilation followed by erosion
		WOODYCHANG0611_COMMON_API UINT16 WhiteTopHatDisk(const ImageGray8 &src, ImageGray8 &dst, UINT16 radius);//source image minus opened image
		WOODYCHANG0611_COMMON_API UINT16 BlackTopHatDisk(const ImageGray8 &src, ImageGray8 &dst, UINT16 radius);//closed image minus source image
		WOODYCHANG0611_COMMON_API UINT16 MorphoGradientDisk(const ImageGray8 &src, ImageGray8 &dst, UINT16 radius);//difference between dilation and dilation
		
		//點連接
		WOODYCHANG0611_COMMON_API bool ImagePointConnect(	const woodychang0611::image::ImageRGB24 &src, woodychang0611::image::ImageRGB24 &dst, const UINT16 total_join_points,
											const FLOAT64 &connect_angle, const UINT8 connect_point_color_r,const UINT8 connect_point_color_g,
											const UINT8 connect_point_color_b);
		//垂直變化量門檻，超過變化量形成波峰波谷，依波峰波谷中間值為門檻

		WOODYCHANG0611_COMMON_API woodychang0611::image::ImageRGB24 VerticalApexThreshold(const woodychang0611::image::ImageRGB24 &src,const INT16 variation);
		//水平變化量門檻，超過變化量形成波峰波谷，依波峰波谷中間值為門檻

		WOODYCHANG0611_COMMON_API woodychang0611::image::ImageRGB24 HorizontalApexThreshold(const woodychang0611::image::ImageRGB24 &src,const INT16 variation);
		//遮罩處理，to_posive: -r=>r..
		WOODYCHANG0611_COMMON_API woodychang0611::image::ImageRGB24 MaskOperation(const woodychang0611::image::ImageRGB24 &src,const std::vector<FLOAT64> &mask,const bool to_positive =true);
		//Sobel
		WOODYCHANG0611_COMMON_API woodychang0611::image::ImageRGB24 SobelWithVertical( const woodychang0611::image::ImageRGB24 &src,const FLOAT64 &f64);

		
		
		// overlay src on target to dst
		template <typename T> UINT16 Overlay(const ImageGeneric<T>  &src,const ImageGeneric<T>  &target,ImageGeneric<T>  &dst,INT16 left, INT16 top, 
			const T* bg_color=NULL,UINT8 alpha=0xFF);


		// ! \brief Scale to new width and height
		template <typename T>  UINT16 Scale	(const ImageGeneric<T> &src, ImageGeneric<T> &dst,
			const UINT16 new_width,const UINT16 new_height,InterpolationMode mode = INTER_NEAREST);
		template <typename T>  UINT16 RotateDegree	(const ImageGeneric<T> &src, ImageGeneric<T> &dst,
			const woodychang0611::geometry::FLOAT&  degree);
		//! \brief Expand frame_width on each side;
		template <typename T>  UINT16 Canvas (const ImageGeneric<T> &src, ImageGeneric<T> &dst,
			const UINT16 frame_width);
		//! \brief Trim Image
		template <typename T>  UINT16 Trim	(const ImageGeneric<T> &src, ImageGeneric<T> &dst,
			INT16 left, INT16 top, UINT16 width, UINT16 height);

		template <typename T>  UINT16 GetFloodArea	(const ImageGeneric<T> &src,const woodychang0611::geometry::IPoint2D &target_point,
			std::vector < woodychang0611::geometry::IPoint2D> &dst);

		template <typename T>  UINT16 GetFloodArea2	(const ImageGeneric<T> &src,const woodychang0611::geometry::IPoint2D &target_point,
			std::vector < woodychang0611::geometry::IPoint2D> &dst);

		template <typename T>  UINT16 RotateRadian(const ImageGeneric<T> &src, ImageGeneric<T> &dst,const woodychang0611::geometry::FLOAT&  radian);
		template <typename T>  UINT16 RotateDegree(const ImageGeneric<T> &src, ImageGeneric<T> &dst,const woodychang0611::geometry::FLOAT&  degree);
		template <typename T> void Blur(const ImageGeneric<T> &src,ImageGeneric<T> &dst);
		template <typename T> void Blur(const ImageGeneric<T> &src,ImageGeneric<T> &dst,FLOAT32 weight);
		template <typename T> void Sharpen(const ImageGeneric<T> &src,ImageGeneric<T> &dst);
		template <typename T> void Sharpen(const ImageGeneric<T> &src,ImageGeneric<T> &dst,FLOAT32 weight);
		template <typename T> void HighPassFilter(const ImageGeneric <T> &src,ImageGeneric <T> &dst,FLOAT32 intensity);
		template <typename T> void SobelFilterHorizontal(const ImageGeneric <T> &src,ImageGeneric <T> &dst, FLOAT32 intensity);
		template <typename T> void SobelFilterVertical(const ImageGeneric <T> &src,ImageGeneric <T> &dst, FLOAT32 intensity);
		template <typename T> void JoinImageHorizontal(const std::vector<ImageGeneric <T> >&src,ImageGeneric <T> &dst);
		template <typename T> void JoinImageVertical(const std::vector<ImageGeneric <T> > &src,ImageGeneric <T> &dst);
		template <typename T> void JoinImage2D(const std::vector< std::vector<ImageGeneric <T> > > &src,ImageGeneric <T> &dst);

		template <typename T>   UINT16 MaskOperation3(const ImageGeneric<T> &src,ImageGeneric<T> &dst,  const FLOAT32 *mask);





//		WOODYCHANG0611_COMMON_API void Thin(const Img &src_image,Img &imgDst,UINT8 num=5);
//		WOODYCHANG0611_COMMON_API int GetCrossPointCount(const Img &src_image,bool bVertical,int iLinePos,int iPixelColor=0,bool bCountOther=false,bool bCountSerial=false);
//		WOODYCHANG0611_COMMON_API int GetExtremeCount(const Img &src_image,bool bVertical,int iPixelColor=0,float fMin=0,float fMax=1,float iThreshold=0.7);

		//template<typename T> WOODYCHANG0611_COMMON_API void ShowVector(const Array1D<T> &vecSrc,Img & imgDst,const UINT16 un16Resolution);
		//template WOODYCHANG0611_COMMON_API void ShowVector <INT8> (const Array1D<INT8> &vecSrc,Img & imgDst,const UINT16 un16Resolution);
		//template WOODYCHANG0611_COMMON_API void ShowVector <UINT8> (const Array1D<UINT8> &vecSrc,Img & imgDst,const UINT16 un16Resolution);
		//template WOODYCHANG0611_COMMON_API void ShowVector <INT16> (const Array1D<INT16> &vecSrc,Img & imgDst,const UINT16 un16Resolution);
		//template WOODYCHANG0611_COMMON_API void ShowVector <UINT16> (const Array1D<UINT16> &vecSrc,Img & imgDst,const UINT16 un16Resolution);
		//template WOODYCHANG0611_COMMON_API void ShowVector <INT32> (const Array1D<INT32> &vecSrc,Img & imgDst,const UINT16 un16Resolution);
		//template WOODYCHANG0611_COMMON_API void ShowVector <UINT32> (const Array1D<UINT32> &vecSrc,Img & imgDst,const UINT16 un16Resolution);
		//template WOODYCHANG0611_COMMON_API void ShowVector <FLOAT64> (const Array1D<FLOAT64> &vecSrc,Img & imgDst,const UINT16 un16Resolution);
		//template WOODYCHANG0611_COMMON_API void ShowVector <FLOAT64> (const Array1D<FLOAT64> &vecSrc,Img & imgDst,const UINT16 un16Resolution);

	//具現化 Instantiation
#define IMAGE_OPERATION_INSTANTIATION(T) \
	template WOODYCHANG0611_COMMON_API UINT16 woodychang0611::image::Canvas(const ImageGeneric <T> &src, ImageGeneric <T>  &dst,const UINT16 frame_width);			\
	template WOODYCHANG0611_COMMON_API UINT16 woodychang0611::image::Trim	(const ImageGeneric <T>  &src, ImageGeneric <T>  &dst,INT16 left, INT16 top, UINT16 width, UINT16 height); \
	template WOODYCHANG0611_COMMON_API UINT16 woodychang0611::image::Scale	(const ImageGeneric <T>  &src, ImageGeneric  <T> &dst,const UINT16 new_width,const UINT16 new_height,InterpolationMode mode);  \
	template WOODYCHANG0611_COMMON_API UINT16 woodychang0611::image::Overlay(const ImageGeneric<T>  &src,const ImageGeneric<T>  &target,ImageGeneric<T>  &dst,INT16 left, INT16 top, const T* bg_color,UINT8 alpha);  \
	template WOODYCHANG0611_COMMON_API UINT16 woodychang0611::image::RotateDegree (const ImageGeneric<T> &src, ImageGeneric<T> &dst,const woodychang0611::geometry::FLOAT& degree); \
	template WOODYCHANG0611_COMMON_API UINT16 woodychang0611::image::RotateRadian(const ImageGeneric<T> &src, ImageGeneric<T> &dst,const woodychang0611::geometry::FLOAT&  radian); \
	template WOODYCHANG0611_COMMON_API UINT16 woodychang0611::image::MaskOperation3(const ImageGeneric<T> &src,ImageGeneric<T> &dst,const float *mask); \
	template WOODYCHANG0611_COMMON_API void woodychang0611::image::Blur(const ImageGeneric<T> &src,ImageGeneric<T> &dst); \
	template WOODYCHANG0611_COMMON_API void woodychang0611::image::Blur(const ImageGeneric<T> &src,ImageGeneric<T> &dst,FLOAT32 weight); \
	template WOODYCHANG0611_COMMON_API void woodychang0611::image::Sharpen(const ImageGeneric<T> &src,ImageGeneric<T> &dst); \
	template WOODYCHANG0611_COMMON_API void woodychang0611::image::Sharpen(const ImageGeneric<T> &src,ImageGeneric<T> &dst,FLOAT32 weight); \
	template WOODYCHANG0611_COMMON_API void woodychang0611::image::HighPassFilter(const ImageGeneric <T> &src,ImageGeneric <T> &dst,FLOAT32 intensity); \
	template WOODYCHANG0611_COMMON_API void woodychang0611::image::SobelFilterHorizontal(const ImageGeneric <T> &src,ImageGeneric <T> &dst, FLOAT32 intensity); \
	template WOODYCHANG0611_COMMON_API void woodychang0611::image::SobelFilterVertical(const ImageGeneric <T> &src,ImageGeneric <T> &dst, FLOAT32 intensity); \
	template WOODYCHANG0611_COMMON_API void woodychang0611::image::JoinImageHorizontal(const std::vector<ImageGeneric <T> >&src,ImageGeneric <T> &dst); \
	template WOODYCHANG0611_COMMON_API void woodychang0611::image::JoinImageVertical(const std::vector<ImageGeneric <T> > &src,ImageGeneric <T> &dst); \
	template WOODYCHANG0611_COMMON_API void woodychang0611::image::JoinImage2D(const std::vector< std::vector<ImageGeneric <T> > > &src,ImageGeneric <T> &dst);  \
	template WOODYCHANG0611_COMMON_API UINT16 woodychang0611::image::GetFloodArea	(const ImageGeneric<T> &src,const woodychang0611::geometry::IPoint2D &target_point, std::vector < woodychang0611::geometry::IPoint2D>& dst);
		


#ifdef _WIN32
	IMAGE_OPERATION_INSTANTIATION(woodychang0611::color::RGB24)
	IMAGE_OPERATION_INSTANTIATION(woodychang0611::color::Gray8)
#endif

	}//namespace woodychang0611::image
}//end of namespace woodychang0611






#endif //__IMAGE_OPERATION_H__
