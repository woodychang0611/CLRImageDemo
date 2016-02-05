#ifndef __WOODYCHANG0611_COMMON_IMAGE__H__
#define __WOODYCHANG0611_COMMON_IMAGE__H__

#include "tensor.h"
#include "data_operation.h"
#include "geometry2d.h"
#include "color.h"
//#include <vector>
//#include <algorithm>
// ******************************************************************************************************************
// Img Img Type
// RGB24 : 24 bits Color bitmap picture.
// BW8   :  8 bits Gray level bitmap picture.
// ******************************************************************************************************************


template class woodychang0611::math::Tensor <woodychang0611::color::RGB24,2>;

namespace woodychang0611{
	namespace image{
		//! \author Woody
		//! \date 2009/8/8
		//! \brief Generic template for Image class
		//! \note Currently Support RGB24 and Gray8 format 


		enum InterpolationMode{
			INTER_NEAREST,
			INTER_BILINEAR,
			INTER_BICUBIC 
		};

		template  <typename PixelFormat> class ImageGeneric : public woodychang0611::math::Tensor<PixelFormat,2>{
		protected:
			//	UINT16 width_;//!< Image Width 
			//	UINT16 height_;//!< Image Height
			bool static_mode_;//! Use static memory area
		public:
			//! \brief Default Constructor
			ImageGeneric();
			//! \brief Constructor with given width and height 
			ImageGeneric(UINT16 width, UINT16 height);
			//! \brief Constructor with given vector<IPoint>
			ImageGeneric(std::vector<woodychang0611::geometry::IPoint2D> vector_pt,PixelFormat bg_color, PixelFormat fg_color);
			//! \brief Copy constructor
			ImageGeneric(const ImageGeneric<PixelFormat> & src);
			//! \brief Copy from different type image
			template <typename PixelFormat2> explicit ImageGeneric(
				const ImageGeneric<PixelFormat2>& src);
			//! \brief Destrustor
			~ImageGeneric();
			//! \brief Return pointer to where image data is stored
			//! \param none
			//! \retval Return pointer to image data
			inline PixelFormat* GetDataPointer(void) {return this->_data_pointer;}; 
			inline const PixelFormat* GetDataPointer(void)const {return this->_data_pointer;}; 
			//! \brief Get width of image
			//! \param none
			//! \retval Return width of the image
			inline UINT16 GetImageWidth(void) const {
				return (UINT16) this->_size[1];
			};
			inline UINT16 Width(void) const {
				return (UINT16) this->_size[1];
			};
			//! \brief Get height of image
			//! \param none
			//! \retval Return height of the image
			inline UINT16 GetImageHeight(void) const{
				return (UINT16) this->_size[0];
			};
			inline UINT16 Height(void) const{
				return (UINT16) this->_size[0];
			};
			//! \brief Get  byte per pixel of image
			//! \param none
			//! \retval Return byte per pixel of the image
			//! \par Examples
			//! 3 byte for  ImageRGB24 and 1 byte for ImageGray8
			inline UINT16 GetBytePerPixel(void) const{
				return sizeof(PixelFormat);
			};
			//! \brief Get  byte per pixel of image
			//! \param none
			//! \retval Return byte per pixel of the image
			inline UINT16 GetBytePerLine(void) const{
				return GetBytePerPixel()*GetImageWidth();
			};
			//! \brief Get  pixel count of image
			//! \param none
			//! \retval Return  pixel count of the image
			inline UINT32 GetImagePixelCount(void) const{
				return GetImageWidth()*GetImageHeight();
			}; 
			//! \brief Get  total byte count of image
			//! \param none
			//! \retval Return byte count of the image
			inline UINT32 GetImageByteCount(void) const{
				return GetBytePerPixel()*GetImagePixelCount();
			};
			//! \brief Get Pixel value at coordinate (x,y) of image
			//! \param [in] x X coordinate
			//! \param [in] y Y coordinate
			//! \retval Return byte count of the image
			//! Coordinate (0,0) at the Upper-Right 
			//! \warning Input coordinate won't be checked\n
			//!	Runtime error will occur if input is not correct!!
			inline const PixelFormat& GetPixel(INT16 x, INT16 y)const{
				return this->_data_pointer[x+y*GetImageWidth()];
			};
			inline PixelFormat& GetPixel(INT16 x, INT16 y){
				return this->_data_pointer[x+y*GetImageWidth()];
			};
			inline PixelFormat& GetPixel(woodychang0611::geometry::IPoint2D point){
				return this->_data_pointer[point.x_+point.y_*GetImageWidth()];
			};
			inline const PixelFormat& Pixel(INT16 x, INT16 y)const{
				return this->_data_pointer[x+y*GetImageWidth()];
			};
			inline PixelFormat& Pixel(INT16 x, INT16 y){
				return this->_data_pointer[x+y*GetImageWidth()];
			};
			inline PixelFormat& Pixel(woodychang0611::geometry::IPoint2D point){
				return this->_data_pointer[point.x_+point.y_*GetImageWidth()];
			};

			PixelFormat GetPixel(const woodychang0611::geometry::FLOAT& x,const woodychang0611::geometry::FLOAT& y,InterpolationMode mode)const;
			//! \brief Test if image is valid or not
			//! \param none
			//! \retval Return true if image is valid (width and height is not 0), false otherwise
			inline bool IsValid(void)const{
				return(this->_data_pointer!=NULL);
			}
			//! \brief Copy from other image
			void CopyFrom(const ImageGeneric<PixelFormat>& src);
			//! \brief Copy from different type image
			template <typename PixelFormat2> void CopyFrom(const ImageGeneric<PixelFormat2> &src);

			bool IsWithin(INT16 x,INT16 y) const;//!<Check (X,Y) is inside the image or not
			bool IsWithin(const woodychang0611::geometry::IPoint2D& ptSrc) const;
			void MakeInside(INT16& x, INT16& y) const; //Make (X,Y) inside image
			ImageGeneric<PixelFormat>& operator=(const ImageGeneric<PixelFormat> & src);
			void Clear(void);
			void Clear(const PixelFormat& clear_color);
		};
		//! \details Create an empty image
		//! \param none
		//! \retval none

		template <typename PixelFormat> ImageGeneric<PixelFormat>::ImageGeneric(void){
		}

		//! \details Create an image with given width, height
		//! \param[in] UINT16 width
		//! \param[in] UINT16 height
		//! \retval none
		//! \warning maxiuam width and height is limited by IMAGE_MAX_WIDTH, IMAGE_MAX_HEGIHT\n
		//! Empty image will be create if either width or height is 0
		template <typename PixelFormat>  ImageGeneric<PixelFormat>::ImageGeneric(UINT16 width, UINT16 height){
			//// Check Max value
			if(width >IMAGE_MAX_WIDTH) width= IMAGE_MAX_WIDTH;
			if(height >IMAGE_MAX_HEIGHT) height= IMAGE_MAX_HEIGHT;
			//Check zero
			if(width==0 || height==0){
				width=0;
				height=0;
			}
			this->_size[0]=height;
			this->_size[1]=width;
			this->woodychang0611::math::Tensor<PixelFormat,2>::_Create();
		}
		template <typename PixelFormat> ImageGeneric<PixelFormat>::ImageGeneric(std::vector<woodychang0611::geometry::IPoint2D> vector_pt,PixelFormat bg_color, PixelFormat fg_color){
			this->_data_pointer=NULL;
			if (vector_pt.size()==0) return;
			INT16 min_x,max_x,min_y,max_y;
			min_x = vector_pt[0].x_;
			max_x = vector_pt[0].x_;
			min_y = vector_pt[0].y_;
			max_y = vector_pt[0].y_;
			for (UINT32 i=1;i<vector_pt.size();++i){
				min_x = (woodychang0611::min)(min_x,vector_pt[i].x_);
				max_x = (woodychang0611::max)(max_x,vector_pt[i].x_);
				min_y = (woodychang0611::min)(min_y,vector_pt[i].y_);
				max_y = (woodychang0611::max)(max_y,vector_pt[i].y_);
			}
			UINT16 width = static_cast <UINT16> (max_x - min_x+1);
			UINT16 height = static_cast <UINT16> (max_y - min_y+1);
			if((width >IMAGE_MAX_WIDTH) || (height >IMAGE_MAX_HEIGHT))return;
			this->_size[0]=height;
			this->_size[1]=width;
			this->woodychang0611::math::Tensor<PixelFormat,2>::_Create();
			this->Clear(bg_color);
			for (UINT32 i=0;i<vector_pt.size();++i){
				GetPixel(vector_pt[i].x_-min_x,vector_pt[i].y_-min_y) = fg_color;
			}
		}
		//! \details Copy from src image
		//! \param[in] src to be copied
		//! \retval none
		template <typename PixelFormat>  ImageGeneric<PixelFormat>::ImageGeneric(const ImageGeneric<PixelFormat>& src){
			//call default contrustor			
			this->ImageGeneric::ImageGeneric();
			this->CopyFrom(src);
		}
		template <typename PixelFormat>  ImageGeneric<PixelFormat>::~ImageGeneric(void){
			this->_Destory();
		}
		//! \details Copy from src image which is other type
		//! \param[in] src to be copied
		//! \retval none
		template <typename PixelFormat> template <typename PixelFormat2> 
		ImageGeneric<PixelFormat>::ImageGeneric(const ImageGeneric<PixelFormat2>& src){
			//call default contrustor
			this->ImageGeneric::ImageGeneric();
			this->CopyFrom(src);
		}

		//! \details Copy from src image
		//! \param[in] src to be copied
		//! \retval none
		template <typename PixelFormat>  void ImageGeneric<PixelFormat>::CopyFrom(const ImageGeneric<PixelFormat>& src){
			if(this==&src) return;
			this->_Copy(src);

		}
		//! \details Copy from different type src image
		//! \param[in] src to be copied
		//! \retval none
		template <typename PixelFormat> template <typename PixelFormat2> 
		void ImageGeneric<PixelFormat>::CopyFrom(const ImageGeneric<PixelFormat2> &src){
			this->_Copy(src);
		}
		template <typename PixelFormat> ImageGeneric<PixelFormat>& 
			ImageGeneric<PixelFormat>::operator=(const ImageGeneric<PixelFormat>& src){
				CopyFrom(src);
				return *this;
		}

		template  <typename PixelFormat> bool ImageGeneric<PixelFormat>::
			IsWithin(INT16 n16X,INT16 n16Y)const
		{
			return (n16X >=0 &&
				n16Y >=0 &&	
				n16X <=GetImageWidth()-1 && 
				n16Y <=GetImageHeight()-1);
		}
		template  <typename PixelFormat> bool ImageGeneric<PixelFormat>::
			IsWithin(const woodychang0611::geometry::IPoint2D& pt) const{
				return IsWithin(pt.x_,pt.y_);
		}

		template  <typename PixelFormat> void ImageGeneric<PixelFormat>::
			MakeInside(INT16& x,INT16& y)const
		{
			if (!this->IsValid()) return;
			if (x<0) x=0;
			if (x>GetImageWidth()-1) x= GetImageWidth()-1;
			if (y<0) y=0;
			if (y>GetImageHeight()-1) y= GetImageHeight()-1;
		}

		template  <typename PixelFormat> void ImageGeneric<PixelFormat>::Clear(void){
			for (UINT32 i=0;i<GetImagePixelCount();++i)
				woodychang0611::color::Clear(this->_data_pointer[i]);
		}

		template  <typename PixelFormat> void ImageGeneric<PixelFormat>::Clear(const PixelFormat & clear_color){
			for (UINT32 i=0;i<GetImagePixelCount();++i)
				this->_data_pointer[i]=clear_color;
		}


		inline FLOAT32 BicubicCalculate(woodychang0611::geometry::FLOAT distance){
			if (distance<1)
				return (FLOAT32)(1.5f*pow(distance,3)-2.5f*pow(distance,2)+1);
			else if (distance <2)
				return 0;
			//return -0.5f*pow(distance,3)+2.5f*pow(distance,2)-4*distance+1;
			else return 0.0f;
		}

		template  <typename PixelFormat>  PixelFormat ImageGeneric<PixelFormat>::GetPixel(const woodychang0611::geometry::FLOAT& x,const woodychang0611::geometry::FLOAT& y,InterpolationMode mode)const{
			if (mode==INTER_NEAREST){
				INT16 x_rounded = static_cast <INT16>((x>0 ? x+0.5f:x-0.5f));
				INT16 y_rounded = static_cast <INT16>((y>0 ? y+0.5f:y-0.5f));
				this->MakeInside(x_rounded,y_rounded);
				return this->GetPixel(x_rounded,y_rounded);
			}else if(mode ==INTER_BILINEAR){
				INT16 x1 = static_cast <INT16> (floor(x));
				INT16 x2 = x1+1;
				INT16 y1 = static_cast <INT16> (floor(y));
				INT16 y2 = y1+1;
				this->MakeInside(x1,y1);
				this->MakeInside(x2,y2);
				FLOAT32 ratio_x = static_cast <FLOAT32> (x-floor(x));
				FLOAT32 ratio_y = static_cast <FLOAT32>  (y-floor(y));
				PixelFormat src_color[4];
				src_color[0] = this->GetPixel(x1,y1);
				src_color[1]= this->GetPixel(x1,y2);
				src_color[2]= this->GetPixel(x2,y1);
				src_color[3]= this->GetPixel(x2,y2);
				FLOAT32  ratio[4];
				ratio[0] = (1.0f-ratio_x) * (1.0f-ratio_y);
				ratio[1] =(1.0f-ratio_x) *ratio_y;
				ratio[2] = ratio_x *(1.0f-ratio_y);
				ratio[3] = ratio_x*ratio_y;
				return MixColor(src_color,ratio,4);
			}else if(mode ==INTER_BICUBIC){
				INT16 x_start = static_cast<INT16> (ceil(x-2));
				INT16 x_end = static_cast<INT16> (floor(x+2));
				INT16 y_start = static_cast<INT16> (ceil(y-2));
				INT16 y_end = static_cast<INT16> (floor(y+2));
				FLOAT32 * ratio =NULL;
				PixelFormat * color=NULL;

				UINT16 count=0;

#ifndef NO_SUPPORT_EXCEPTION
				try{
#endif 
					ratio = new FLOAT32[(x_end-x_start+1)*(y_end-y_start+1)];
					color = new PixelFormat[(x_end-x_start+1)*(y_end-y_start+1)];

					for (INT16 i= x_start;i<=x_end;i++){
						for (INT16 j= y_start;j<=y_end;j++){
							INT16 x_temp = i;
							INT16 y_temp =j;
							this->MakeInside(x_temp,y_temp);
							woodychang0611::geometry::FLOAT distance_x = fabs(i-x); 
							woodychang0611::geometry::FLOAT distance_y = fabs(j-y);
							color[count] = this->GetPixel(x_temp,y_temp);
							ratio[count] = BicubicCalculate(distance_x) * BicubicCalculate(distance_y);
							count++;
						}
					}
					FLOAT32 sum=0;
					for (UINT16 q=0;q<count;q++){
						sum+=ratio[q];
					}
					for (UINT16 q=0;q<count;q++){
						ratio[q]=ratio[q]/sum;
					}
					PixelFormat return_color = MixColor(color,ratio,count); 
					delete [] color;
					delete [] ratio;
					return return_color;
					//return PixelFormat();
#ifndef NO_SUPPORT_EXCEPTION
				}//end of try
				catch(std::bad_alloc &ba){
					ba =ba;
					if(color!=NULL) delete [] color;
					if(ratio!=NULL)  delete [] ratio;
					return PixelFormat();
				}
#endif

			}else{
				return PixelFormat();
			}
		}


		typedef ImageGeneric<woodychang0611::color::RGB24> ImageRGB24;
		typedef ImageGeneric<woodychang0611::color::Gray8> ImageGray8;


	}//namespace woodychang0611::image
} //end of namespace woodychang0611 

#ifdef _WIN32
template class WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::RGB24>;
template void  WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::RGB24>::
	CopyFrom(const  woodychang0611::image::ImageGeneric<woodychang0611::color::Gray8>& src);


template class WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::Gray8>;
template void  WOODYCHANG0611_COMMON_API woodychang0611::image::ImageGeneric<woodychang0611::color::Gray8>::
	CopyFrom(const  woodychang0611::image::ImageGeneric<woodychang0611::color::RGB24>& src);

template class WOODYCHANG0611_COMMON_API  std::vector <woodychang0611::image::ImageRGB24>;
template class WOODYCHANG0611_COMMON_API  std::vector <woodychang0611::image::ImageGray8>;
template class WOODYCHANG0611_COMMON_API  std::vector <std::vector <woodychang0611::image::ImageRGB24> >;
template class WOODYCHANG0611_COMMON_API  std::vector <std::vector <woodychang0611::image::ImageGray8> >;
#endif

#endif //__WOODYCHANG0611_COMMON_IMAGE__H__
