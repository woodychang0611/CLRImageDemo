#ifndef  __WOODYCHANG0611_COMMON_COLOR_H__
#define __WOODYCHANG0611_COMMON_COLOR_H__

#include "base.h"
namespace woodychang0611{
//! \namespace woodychang0611::color
//! \brief Color Namespace for woodychang0611
//! \details Including Gray8 and RGB24 2 color system and related function 
//! \author Woody
//! \date 2009/5/16
	namespace color{
		//! \brief Type for 8Bits gray scale system
		class Gray8{
		public:
			UINT8 value_;
			inline Gray8(){};
			inline Gray8(UINT8 value) {value_=value;};
			inline Gray8 operator-(){
				return Gray8(255-value_);
			}
			inline void operator +=(Gray8 src){
				value_=(UINT8)(woodychang0611::min)(255,value_+src.value_);
			}
			inline void operator -=(Gray8 src){
				value_=(UINT8)(woodychang0611::max)(0,value_-src.value_);
			}
			inline Gray8 operator+(Gray8 src){
				Gray8 temp(*this);
				temp+=src;
				return temp;
			}
			inline Gray8 operator-(Gray8 src){
				Gray8 temp(*this);
				temp-=src;
				return temp;
			}
			template <typename Type> inline void operator*=(Type multiplier){
					value_=(UINT8)(woodychang0611::max<Type>)(0,(woodychang0611::min<Type>)(255,value_*multiplier));
			}
			template <typename Type> inline void operator/=(Type divisor){
				value_=(UINT8)(woodychang0611::max<Type>)(0,(woodychang0611::min<Type>)(255,value_/divisor));
			}

			template <typename Type> inline Gray8 operator*(Type multiplier)const{
				Gray8 temp(*this);
				temp*=multiplier;
				return temp;
			}
			template <typename Type> inline Gray8 operator/(Type divisor)const{
				Gray8 temp(*this);
				temp/=divisor;
				return temp;
			}
		};
		template <typename Type> inline Gray8 operator*(Type multiplier,const Gray8& color){
			return color*multiplier;
		}


		//! \brief Type for 24Bits RGB color system
		//! \todo check casting problem between RGB24 and Gray8 
		class RGB24{
		public:
			UINT8 b_; //!< BLUE parts
			UINT8 g_; //!< GREEN parts
			UINT8 r_; //!< RED parts
			inline RGB24(void){}
			inline RGB24(UINT8 r, UINT8 g, UINT8 b){r_=r;g_=g;b_=b;}
			inline explicit  RGB24(Gray8 bw8){r_=bw8.value_;g_=bw8.value_;b_=bw8.value_;;}
			inline operator Gray8() const{
				Gray8 temp;
				temp.value_=static_cast<UINT8>((r_*77 + g_*150 +b_*29)>>8);
				return temp;
			};
			inline RGB24 operator-(){
				return RGB24(255-r_,255-g_,255-b_);
			}
			inline void operator +=(RGB24 src){
				r_=(UINT8)(woodychang0611::min)(255,r_+src.r_);
				g_=(UINT8)(woodychang0611::min)(255,g_+src.g_);
				b_=(UINT8)(woodychang0611::min)(255,b_+src.b_);
			}
			inline void operator -=(RGB24 src){
				r_=(UINT8)(woodychang0611::max)(0,r_-src.r_);
				g_=(UINT8)(woodychang0611::max)(0,g_-src.g_);
				b_=(UINT8)(woodychang0611::max)(0,b_-src.b_);
			}
			inline RGB24 operator+(RGB24 src){
				RGB24 temp(*this);
				temp+=src;
				return temp;
			}
			inline RGB24 operator-(RGB24 src){
				RGB24 temp(*this);
				temp-=src;
				return temp;
			}
			template <typename Type>  inline void operator*=(Type multiplier){
					r_=(UINT8)(woodychang0611::max<Type>)(0,(woodychang0611::min<Type>)(255,r_*multiplier));
					g_=(UINT8)(woodychang0611::max<Type>)(0,(woodychang0611::min<Type>)(255,g_*multiplier));
					b_=(UINT8)(woodychang0611::max<Type>)(0,(woodychang0611::min<Type>)(255,b_*multiplier));
			}
			template <typename Type> inline void operator/=(Type divisor){
				r_=(UINT8)(woodychang0611::max<Type>)(0,(woodychang0611::min<Type>)(255,r_/divisor));
				g_=(UINT8)(woodychang0611::max<Type>)(0,(woodychang0611::min<Type>)(255,g_/divisor));
				b_=(UINT8)(woodychang0611::max<Type>)(0,(woodychang0611::min<Type>)(255,b_/divisor));
			}

			template <typename Type> inline RGB24 operator*(Type multiplier)const{
				RGB24 temp(*this);
				temp*=multiplier ;
				return temp;
			}
			template <typename Type> inline RGB24 operator/(Type divisor)const{
				RGB24 temp(*this);
				temp/=divisor;
				return temp;
			}

		};

		template <typename Type> inline RGB24 operator*(Type multiplier,const RGB24& color){
			return color*multiplier;
		}



	//Default Color
	const Gray8 GRAY8_DEFAULT = Gray8(0x80);//!< \brief Default Color
	const RGB24 RGB24_DEFAULT  =RGB24(0x0,0x0,0x0);//!< \brief Default Color
	const RGB24 RGB24_BLACK	=RGB24(0x00,0x00,0x00); //!< \brief Default RGB24 BLACK Color
	const RGB24 RGB24_WHITE	=RGB24(0xFF,0xFF,0xFF);//!< \brief Default RGB24 WHITE Color
	const RGB24 RGB24_GRAY =RGB24(0x80,0x80,0x80);//!< \brief Default RGB24 GRAY Color
	const RGB24 RGB24_MAROON	=RGB24(0x80,0x00,0x00);//!< \brief Default RGB24 MAROON Color
	const RGB24 RGB24_GREEN=RGB24(0x00,0x80,0x00);//!< \brief Default RGB24 GREEN Color
	const RGB24 RGB24_NAVY=RGB24(0x00,0x00,0x80);//!< \brief Default RGB24 NAVY Color
	const RGB24 RGB24_RED	=RGB24(0xFF,0x00,0x00);//!< \brief Default RGB24 RED Color
	const RGB24 RGB24_LIME=RGB24(0x00,0xFF,0x00);//!< \brief Default RGB24 LIME Color
	const RGB24 RGB24_BLUE=RGB24(0x00,0x00,0xFF);//!< \brief Default RGB24 BLUE Color
	const RGB24 RGB24_YELLOW =RGB24(0xFF,0xFF,0x00);//!< \brief Default RGB24 YELLOW Color
	const RGB24 RGB24_MAGENTA =RGB24(0xFF,0x00,0xFF);//!< \brief Default RGB24 MAGENTA Color
	const RGB24 RGB24_CYAN=RGB24(0x00,0xFF,0xFF);//!< \brief Default RGB24 CYAN Color

	inline bool operator==(RGB24 c1,RGB24 c2){
		return (c1.r_ ==c2.r_ && c1.g_==c2.g_ && c1.b_ ==c2.b_ ); }
	inline bool operator!=(RGB24 c1,RGB24 c2){
		return  (c1.r_ !=c2.r_  || c1.g_!=c2.g_ || c1.b_ !=c2.b_ ); }
	inline bool operator==(Gray8 c1,Gray8 c2){
		return (c1.value_ ==c2.value_); }
	inline bool operator!=(Gray8 c1,Gray8 c2){
		return  (c1.value_ !=c2.value_ ); }

/*! 

	\brief Mix Color1 on Color2 with certain weight
	\param[in] c1 Color1
	\param[in] c2 Color2
	\param[in] un8Alpha Weight of c1 ( value is from 0 to 255 ), so weight of c2 is 255 - un8Alpha.
	\retval  mix result
	\details
	\par Example:
	\code
	RGB24 color1=RGB24_RED ;
	RGB24 color2=RGB24_GREEN ;
	RGB24 color3=MixColor(color1,color2,0x55) ;
	\endcode
*/
	WOODYCHANG0611_COMMON_API RGB24 MixColor(RGB24 c1,RGB24 c2,UINT8 un8Alpha);
/*! 

	\brief Mix Color1 on Color2 with certain weight
	\param[in] c1 Color1
	\param[in] c2 Color2
	\param[in] un8Alpha Weight of c1 ( value is from 0 to 255 ), so weight of c2 is 255 - un8Alpha.
	\retval  mix result
	\details
	\par Example:
	\code
	Gray8 color1=Gray8(0xFF) ;
	Gray8 color2=Gray8(0x20) ;
	Gray8 color3=MixColor(color1,color2,0x55) ;
	\endcode
*/
		inline RGB24 MixColor(RGB24 c1,RGB24 c2,UINT8 un8Alpha){
			//100% Transparency
			if (un8Alpha==0) return c2;
			//0% Transparency
			if (un8Alpha==0xFF) return c1;
			//Same color
			if (c1==c2) return c1;
			UINT8 R,G,B;
			R = (UINT8) (( (UINT16) c1.r_ * un8Alpha +   (UINT16) c2.r_ * (0xFF-un8Alpha) )/0xFF);
			G = (UINT8) (( (UINT16) c1.g_ * un8Alpha +   (UINT16) c2.g_ * (0xFF-un8Alpha) )/0xFF);
			B = (UINT8) (( (UINT16) c1.b_ * un8Alpha +   (UINT16) c2.b_ * (0xFF-un8Alpha) )/0xFF);
			return RGB24(R,G,B);
		}

		inline Gray8 MixColor(Gray8 c1,Gray8 c2,UINT8 un8Alpha){
			//100% Transparency
			if (un8Alpha==0) return c2;
			//0% Transparency
			if (un8Alpha==0xFF) return c1;
			//Same color
			if (c1==c2) return c1;
			UINT8 value;
			value = (UINT8) (( (UINT16) c1.value_ * un8Alpha +   (UINT16) c2.value_ * (0xFF-un8Alpha) )/0xFF);
			return Gray8(value);
		}



		inline Gray8 MixColor(const Gray8* src_color,const FLOAT32* ratio, UINT16 count){
			FLOAT32 value;
			value=0;
			for (UINT16 i=0;i<count;++i){
				value+= (src_color[i].value_*ratio[i]);
			}
			value=(woodychang0611::min)(value,255.0f);
			value=(woodychang0611::max)(value,0.0f);
			return Gray8(static_cast<UINT8>(value));
		}
		inline RGB24 MixColor(const RGB24* src_color,const FLOAT32* ratio, UINT16 count){
			double r,g,b;
			r=0;g=0;b=0;
			for (UINT16 i=0;i<count;++i){
				r+= (src_color[i].r_*ratio[i]);
				g+= (src_color[i].g_*ratio[i]);
				b+= (src_color[i].b_*ratio[i]);

			}
			r=(woodychang0611::min)(r,255.0);
			r=(woodychang0611::max)(r,0.0);
			g=(woodychang0611::min)(g,255.0);
			g=(woodychang0611::max)(g,0.0);
			b=(woodychang0611::min)(b,255.0);
			b=(woodychang0611::max)(b,0.0);
			return RGB24(static_cast<UINT8>(r),static_cast<UINT8>(g),static_cast<UINT8>(b));
		}
	inline void Clear(RGB24& src){src=RGB24_DEFAULT;}
	inline void Clear(Gray8& src){src=GRAY8_DEFAULT;}

/*!
	\class Gray8
*/

/*!
	\fn Gray8::Gray8();
	\brief Default contrustor
	\note No default value will be assigned
*/

/*!
	\fn Gray8::Gray8(UINT8 value)
	\brief Contrustor with given value
	\param [in] value 0x00 is black 0xFF is white
*/

/*!
	\class RGB24
*/

/*!
	\fn RGB24::RGB24()
	\brief Default contrustor
	\note No default value will be assigned
*/

/*!
	\fn RGB24::RGB24(UINT8 r, UINT8 g, UINT8 b)
	\brief Contrustor with given value
	\param [in] r RGB24_RED parts
	\param [in] g RGB24_GREEN parts
	\param [in] b RGB24_BLUE parts
*/

/*!
	\fn RGB24::RGB24(Gray8 bw8)
	\brief Copy contrustor from Gray8
	\param [in] bw8 Source Gray8
	\note Explicit copy allowed
*/

/*!
	\fn RGB24 RGB24::operator Gray8()
	\brief Casting to Gray8
	\retval Gray8 after casted
	\note Explicit copy allowed 
	\note For efficiency reason, approximate  function is used
	\note (r*77+g*150+b*29)/256 instead of 0.299*r+0.587*g+0.114b
*/

/*!
	\fn RGB24 ConvertColor(const Gray8& src)
	\brief Convert RGB24_GRAY to RGB24
	\param [in] Source Gray8 
	\retval RGB24 after converted
*/

/*!
	\fn Gray8 ConvertColor(const RGB24& src)
	\brief Convert RGB24 to BW8
	\param [in] Source RGB24 
	\retval Gray8 after converted
	\note For efficiency reason, approximate  function is used
	\note (r*77+g*150+b*29)/256 instead of 0.299*r+0.587*g+0.114b
*/

/*! 
	\fn bool operator==(RGB24 c1,RGB24 c2)
	\brief Test two RGB24 are identical or not
	\param[in] c1 Color1
	\param[in] c2 Color2
	\return  result true/false if identical/not identical
*/

/*! 
	\fn bool operator==(Gray8 c1,Gray8 c2)
	\brief Test two Gray8 are identical or not
	\param[in] c1 Color1
	\param[in] c2 Color2
	\return  result true/false if identical/not identical
*/

/*!
	\fn bool operator!=(RGB24 c1,RGB24 c2)
	\brief Test two RGB24 are identical or not
	\param[in] c1 Color1
	\param[in] c2 Color2
	\return  result false/true if identical/not identical
*/


	//! \var GRAY8_DEFAULT
	//! \dot
	//!		digraph GRAY {
	//!		node [shape=box,width=2,style=filled,fontcolor="#FFFFFF",fillcolor="#808080",color="000000"];
	//!		GRAY8_DEFAULT
	//!		}
	//! \enddot

	//! \var RGB24_DEFAULT
	//! \dot
	//!		digraph GRAY {
	//!		node [shape=box,width=2,style=filled,fontcolor="#FFFFFF",fillcolor="#808080",color="000000"];
	//!		RGB24_DEFAULT
	//!		}
	//! \enddot

	//! \var RGB24_BLACK
	//! \dot
	//!		digraph BLACK {
	//!		node [shape=box,width=2,style=filled,fontcolor="#FFFFFF",fillcolor="#000000",color="000000"];
	//!		RGB24_BLACK
	//!		}
	//! \enddot

	//! \var RGB24_WHITE
	//! \dot
	//!		digraph WHITE {
	//!		node [shape=box,width=2,style=filled,fontcolor="#000000",fillcolor="#FFFFFF",color="000000"];
	//!		RGB24_WHITE
	//!		}
	//! \enddot

	//! \var RGB24_GRAY
	//! \dot
	//!		digraph GRAY {
	//!		node [shape=box,width=2,style=filled,fontcolor="#FFFFFF",fillcolor="#808080",color="000000"];
	//!		RGB24_GRAY
	//!		}
	//! \enddot

	//! \var RGB24_MAROON
	//! \dot
	//!		digraph MAROON {
	//!		node [shape=box,width=2,style=filled,fontcolor="#FFFFFF",fillcolor="#800000",color="000000"];
	//!		RGB24_MAROON
	//!		}
	//! \enddot

	//! \var RGB24_GREEN
	//! \dot
	//!		digraph GREEN {
	//!		node [shape=box,width=2,style=filled,fontcolor="#FFFFFF",fillcolor="#008000",color="000000"];
	//!		RGB24_GREEN
	//!		}
	//! \enddot

	//! \var RGB24_NAVY
	//! \dot
	//!		digraph NAVY {
	//!		node [shape=box,width=2,style=filled,fontcolor="#FFFFFF",fillcolor="#000080",color="000000"];
	//!		RGB24_NAVY
	//!		}
	//! \enddot

	//! \var RGB24_RED
	//! \dot
	//!		digraph RED {
	//!		node [shape=box,width=2,style=filled,fontcolor="#FFFFFF",fillcolor="#FF0000",color="000000"];
	//!		RGB24_RED
	//!		}
	//! \enddot

	//! \var RGB24_LIME
	//! \dot
	//!		digraph LIME {
	//!		node [shape=box,width=2,style=filled,fontcolor="#000000",fillcolor="#00FF00",color="000000"];
	//!		RGB24_LIME
	//!		}
	//! \enddot

	//! \var RGB24_BLUE
	//! \dot
	//!		digraph BLUE {
	//!		node [shape=box,width=2,style=filled,fontcolor="#FFFFFF",fillcolor="#0000FF",color="000000"];
	//!		RGB24_BLUE
	//!		}
	//! \enddot

	//! \var RGB24_YELLOW
	//! \dot
	//!		digraph YELLOW {
	//!		node [shape=box,width=2,style=filled,fontcolor="#000000",fillcolor="#FFFF00",color="000000"];
	//!		RGB24_YELLOW
	//!		}
	//! \enddot

	//! \var RGB24_MAGENTA
	//! \dot
	//!		digraph MAGENTA {
	//!		node [shape=box,width=2,style=filled,fontcolor="#000000",fillcolor="#FF00FF",color="000000"];
	//!		RGB24_MAGENTA
	//!		}
	//! \enddot

	//! \var RGB24_CYAN
	//! \dot
	//!		digraph  RGB24_CYAN {
	//!		node [shape=box,width=2,style=filled,fontcolor="#000000",fillcolor="#00FFFF",color="000000"];
	//!		RGB24_CYAN
	//!		}
	//! \enddot

	}//namespace woodychang0611::color
}// namespace woodychang0611


#endif
