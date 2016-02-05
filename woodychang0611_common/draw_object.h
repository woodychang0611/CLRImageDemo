
#ifndef ___WOODYCHANG0611_COMMON_DRAW_OBJECT__ 
#define ___WOODYCHANG0611_COMMON_DRAW_OBJECT__


#include "image.h"
#include <math.h>
#include <algorithm>
#include <vector>
//using woodychang0611::Color::RGB24;
// ******************************************************************************************************************
// DrawObject
//
// Purpose
//		Geometry drawing method. 
// Useage
//		DrawObject() ;
// Description of Parameters
//		border_alpha_	-	Border transparancy. Value is 0 to 255. 0 for 100% transparancy,255 for 0%(No transparent)
//		border_color_			-	Border color
//		fill_alpha_	-	Filling transparancy. Value is 0 to 255. 0 for 100% transparancy,255 for 0%(No transparent)
//		fill_color_			-	Filling color
// Return Value
//		None.
// Example
//		DrawObject Sdwo ;
//		Sdwo.border_alpha_ = 0x11 ;
//		Sdwo.border_color_ = Black ;
//		Sdwo.fill_alpha_ = 0x0 ;
//		Sdwo.fill_color_ = Red ;
//
// Member function
//		Purpose
//			Drawing point method.
//		Useage
//			Draw(ptSrc,img)
//		Description of Parameters
//			ptSrc	-	Point to draw.
//			img		-	Img to be drawn.
//		Return Value
//			None.
//		Example
//			DrawObject Sdwo ;
//			Img Simg ;
//			MemBlock mem;
//			ReadFile("A.bmp",mem);
//			DecodeBMP(Simg,mem);
//			Simg.TransferRGB24() ;
//			woodychang0611::geometry::Point2D Spt(20,30) ;
//			Sdwo.Draw(Spt,Simg) ;
//
// Member function
//		Purpose
//			Drawing line method.
//		Useage
//			Draw(line,img)
//		Description of Parameters
//			line	-	Line to draw.
//			img		-	Img to be drawn.
//		Return Value
//			None.
//		Example
//			DrawObject Sdwo ;
//			Img Simg ;
//			MemBlock mem;
//			ReadFile("A.bmp",mem);
//			DecodeBMP(Simg,mem);
//			Simg.TransferRGB24() ;
//			woodychang0611::geometry::Point2D Spt1(20,30) , Spt2(50,60) ;
//			woodychang0611::geometry::Line2D  SLn(Spt1,Spt2) ;
//			Sdwo.Draw(SLn,Simg) ;
//
// Member function
//		Purpose
//			Drawing Polygon2D method.
//		Useage
//			Draw(Polygon2D,img,bColsed,bFill)
//		Description of Parameters
//			shapeSrc	-	Polygon2D to draw.
//			img			-	Img to be drawn.
//			bColsed		-	Last edge is connected or not.
//			bFill		-	Fill color on the shape or not.
//		Return Value
//			None.
//		Example
//			DrawObject Sdwo ;
//			Img Simg ;
//			MemBlock mem;
//			ReadFile("A.bmp",mem);
//			DecodeBMP(Simg,mem);
//			Simg.TransferRGB24() ;
//			woodychang0611::geometry::Point2D Spt1(20,30) , Spt2(50,60) ,Spt3(-50,-60);
//			Polygon2D Spgn ;
//			Spgn.AddPoint(Spt1) ;
//			Spgn.AddPoint(Spt2) ;
//			Spgn.AddPoint(Spt3) ;
//			Sdwo.Draw(Spgn,Simg) ;
//
// Member function
//		Purpose
//			Drawing Circle2D method.
//		Useage
//			Draw(shapeSrc,img,bFill)
//		Description of Parameters
//			shapeSrc	-	Circle2D to draw.
//			img			-	Img to be drawn.
//			bFill		-	Fill color on the shape or not.
//		Return Value
//			None.
//		Example
//			DrawObject Sdwo ;
//			Img Simg ;
//			MemBlock mem;
//			ReadFile("A.bmp",mem);
//			DecodeBMP(Simg,mem);
//			Simg.TransferRGB24() ;
//			woodychang0611::geometry::Point2D Spt(20,30);
//			Circle2D Scre(Spt,20,1) ;
//			Sdwo.Draw(Scre,Simg) ;
//
// Member function
//		Purpose
//			Drawing Ellipse2D method.
//		Useage
//			Draw(shapeSrc,img,bFill)
//		Description of Parameters
//			shapeSrc	-	Ellipse2D to draw.
//			img			-	Img to be drawn.
//			bFill		-	Fill color on the shape or not.
//		Return Value
//			None.
//		Example
//			DrawObject Sdwo ;
//			Img Simg ;
//			MemBlock mem;
//			ReadFile("A.bmp",mem);
//			DecodeBMP(Simg,mem);
//			Simg.TransferRGB24() ;
//			woodychang0611::geometry::Point2D Spt1(20,30),Spt2(20,50);
//			Ellipse2D Selp(Spt1,Spt2,40,1) ;
//			Sdwo.Draw(Selp,Simg) ;
//
// ******************************************************************************************************************


namespace woodychang0611{
	namespace image{
		class DrawObject
		{
		public:
			UINT8 border_alpha_; //transparency 255 for 100% 0, for 0%
			woodychang0611::color::RGB24 border_color_;
			UINT8 fill_alpha_; //transparency 255 for 100% 0, for 0%
			woodychang0611::color::RGB24 fill_color_;
			UINT16 border_width_;
			bool fill_;//Fill Shape or not;
		public:
			WOODYCHANG0611_COMMON_API DrawObject(void);
			WOODYCHANG0611_COMMON_API ~DrawObject(void);
			WOODYCHANG0611_COMMON_API void Draw (const woodychang0611::geometry::Line2D &line,woodychang0611::image::ImageRGB24 &img)const;
			WOODYCHANG0611_COMMON_API void Draw (const woodychang0611::geometry::IPoint2D &pt,woodychang0611::image::ImageRGB24 &img)const;
			WOODYCHANG0611_COMMON_API void Draw (const std::vector< woodychang0611::geometry::IPoint2D> &vector_pt,woodychang0611::image::ImageRGB24 &img)const;
			WOODYCHANG0611_COMMON_API void Draw (const woodychang0611::geometry::Polygon2D &src_polygon,ImageRGB24 &img)const;
			WOODYCHANG0611_COMMON_API void Draw(const woodychang0611::geometry::Circle2D &src_circle,ImageRGB24 &img)const ;
			WOODYCHANG0611_COMMON_API void Draw(const woodychang0611::geometry::Ellipse2D &src_ellipse,ImageRGB24 &img)const ;
		};
	}
	
/*!
\class woodychang0611::image::DrawObject
	\brief DrawObject is used to draw geometry on image class
	\details
	Draw  dependence relation
	\dot
			digraph DrawObject{
				node [shape=ellipse,width=1,style=fill,fontcolor="#000000",color="#FF0000"];
				edge [style=dotted,color="#0000FF"];
				ImageDilate[label="ImageDilate"];
				Point2D_1[label="border width=1~N\nalpha=0x00~0xFF"];
				Line2D_1 [label="border width=1\nalpha=0x00~0xFF"];
				Line2D_2 [label="border width=2~N\nalpha=0x00~0xFF"];
				Ellipse2D_1 [label="border width=0~N\nalpha=0x00~0xFF"];
				Circle2D_1 [label="border width=0~N\nalpha=0x00~0xFF"];
				Polygon2D_1 [label="border width=0~1\nalpha=0xFF"];
				Polygon2D_2 [label="border width=0~N\nalpha=0x00~0xFF"];
				node [style=filled];
				subgraph cluster1 {
					Point2D_1;
					label = "Point2D";
				}
				subgraph cluster2 {
					Line2D_1;
					Line2D_2;
					label = "Line2D";
				}
				subgraph cluster3 {
					Ellipse2D_1;
					label = "Ellipse2D";
				}
				subgraph cluster4 {
					Circle2D_1;
					label = "Circle2D";
				}
				subgraph cluster5{
					Polygon2D_1;
					Polygon2D_2;
					label="Polygeon2D";
				}
				subgraph cluster6{
					ImageDilate;
					label="Image Operation";
				}
				subgraph cluster7{
					Line2D_1 ->Point2D_1;
					Ellipse2D_1->Line2D_1;
					Circle2D_1->Ellipse2D_1;
					Polygon2D_1->Line2D_1;
					Polygon2D_2->Polygon2D_1;
					Polygon2D_2->ImageDilate;
					Line2D_2->Polygon2D_2;
				}
			}


	\enddot
*/
} //end of namespace woodychang0611

#endif //___WOODYCHANG0611_COMMON_DRAW_OBJECT__

