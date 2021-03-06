#ifndef __WOODYCHANG0611_COMMON_GEOMETRY_H__
#define __WOODYCHANG0611_COMMON_GEOMETRY_H__

// 2D Geometry 
// Default Point2D will be floating point 
// For Integer Point use IPoint2D

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4251) //Disable C4251
#endif

#include "base.h"
#include <math.h>
#include <vector>

namespace woodychang0611{
	namespace geometry{
		typedef woodychang0611::FLOAT64 FLOAT;
 		class Point2D
		{
		public:
			FLOAT x_;
			FLOAT y_;
			inline Point2D(void){};
			inline Point2D(FLOAT x,FLOAT y){x_=x;y_=y;};
			inline bool operator ==(const Point2D &src)const{
				return (x_==src.x_ && y_==src.y_);
			};
			inline bool operator !=(const Point2D &src)const{
				return (x_!=src.x_ || y_ != src.y_);
			}
			inline void Offset(FLOAT offset_x,FLOAT offset_y){
				x_+=offset_x; y_+=offset_y;
			};
			inline void operator +=(const Point2D& src){
				x_+=src.x_; y_+=src.y_;
			};
			inline void operator -=(const Point2D& src){
				x_-=src.x_; y_-=src.y_;
			};
			WOODYCHANG0611_COMMON_API Point2D operator+(const Point2D& src)const;
			WOODYCHANG0611_COMMON_API Point2D operator-(const Point2D& src)const;
			inline Point2D operator * (FLOAT a)const{return Point2D(x_*a,y_*a);};
			inline Point2D operator / (FLOAT a)const{return Point2D(x_/a,y_/a);};
			inline void Round(void){
				x_ = ( x_>0 ? floor(x_+0.5f):ceil(x_-0.5f));
				y_ = ( y_>0 ? floor(y_+0.5f):ceil(y_-0.5f));
			}
			inline void SetPoint(const FLOAT x,const FLOAT y){
				x_=x;y_=y;
			}
		};

		class IPoint2D
		{
		public:
			INT16 x_;
			INT16 y_;
			inline IPoint2D(void){};
			inline IPoint2D(INT16 x,INT16 y){x_=x;y_=y;};
			inline IPoint2D(const Point2D& src){
				//Round
				x_ = static_cast <INT16>(( src.x_>0 ? src.x_+0.5:src.x_-0.5));
				y_ = static_cast <INT16>(( src.y_>0 ? src.y_+0.5:src.y_-0.5));
			}
			
			inline operator Point2D(){
				Point2D temp(x_,y_);
				return temp;
			}

			inline bool operator ==(const IPoint2D &src)const{
				return (x_==src.x_ && y_==src.y_);
			};
			inline bool operator !=(const IPoint2D &src)const{
				return (x_!=src.x_ || y_ != src.y_);
			}
			inline void Offset(INT16 offset_x,INT16 offset_y){
				x_+=offset_x; y_+=offset_y;
			};
			inline void operator +=(const IPoint2D& src){
				x_+=src.x_; y_+=src.y_;
			};
			inline void operator -=(const IPoint2D& src){
				x_-=src.x_; y_-=src.y_;
			};
			WOODYCHANG0611_COMMON_API IPoint2D operator+(const IPoint2D& src)const;
			WOODYCHANG0611_COMMON_API IPoint2D operator-(const IPoint2D& src)const;
			inline void SetPoint(const INT16 x,const INT16 y){
				x_=x;y_=y;
			}
		};
		inline FLOAT GetDistance(Point2D pt1,Point2D pt2){
			return sqrt ( (pt1.x_-pt2.x_)*(pt1.x_-pt2.x_)+(pt1.y_-pt2.y_)*(pt1.y_-pt2.y_) );
		}
		class Line2D
		{
		public:
			Point2D start_point_;
			Point2D end_point_;
			inline Line2D(void){};
			inline Line2D(const Point2D &start_point, const Point2D &end_point){
				start_point_ = start_point; end_point_ =end_point;
			};
			inline Line2D(
				FLOAT start_point_x,FLOAT start_point_y,FLOAT end_point_x,FLOAT end_point_y){
				start_point_ = Point2D(start_point_x,start_point_y);
				end_point_ =Point2D(end_point_x,end_point_y);
			};
			//If ptStart and ptEnd is the same return true, else false
			inline bool IsValid() const{
				return (start_point_ != end_point_);
			}
			inline void SetLine(const Point2D &start_point, const Point2D &end_point){
				start_point_ = start_point; end_point_ =end_point;
			};
		};
		
		class Circle2D
		{
		private:
			Point2D center_point_;
			FLOAT radius_;
		public:
			inline Circle2D(){}
			inline Circle2D(const Point2D& center_point, FLOAT radius){
				center_point_ = center_point;
				radius_=radius;
				if (radius_<0) radius_=0;
			}
			inline Circle2D(FLOAT x,FLOAT y, FLOAT radius){
				center_point_ = Point2D(x,y);
				radius_=radius;
				if (radius_<0) radius_=0;
			}
			inline Point2D GetCenterPoint(void)const {return center_point_;}
			inline FLOAT GetRadius(void)const {return radius_;}
		};
		class Ellipse2D
		{
		private:
			Point2D focus_point1_ ;
			Point2D focus_point2_ ;
			FLOAT major_radius_; 
		public:
			inline Ellipse2D(void){};
			inline Ellipse2D(
				const Point2D &focus_point1,
				const Point2D &focus_point2,
				const FLOAT major_radius){
				focus_point1_=focus_point1;
				focus_point2_=focus_point2;
				major_radius_=major_radius;
				//Major radius should be at least distance between two focus
				if(major_radius_ < GetDistance(focus_point1_,focus_point2_)/2)
					major_radius_ =GetDistance(focus_point1_,focus_point2_)/2;
			}
			inline void GetFocusPoint(Point2D& focus_point1,
				Point2D& focus_point2)const{
				focus_point1 = focus_point1_;
				focus_point2 = focus_point2_;
			}
			inline FLOAT GetMajorRadius(void)const{
				return major_radius_;
			}
		};
			//XLine2D是無限長的線,即沒有端點的線

			//class XLine2D: public Line2D
			//{
			//public:
			//	WOODYCHANG0611_COMMON_API XLine2D(void);
			//	WOODYCHANG0611_COMMON_API XLine2D(const Point2D &ptStart,const Point2D &ptEnd);
			//	WOODYCHANG0611_COMMON_API XLine2D(const Line2D& lineSrc);
			//};
#ifdef _WIN32
	template class WOODYCHANG0611_COMMON_API std::vector<woodychang0611::geometry::Point2D>; //used by Polygon2D
	template class WOODYCHANG0611_COMMON_API std::vector<woodychang0611::geometry::IPoint2D>; 
	template class WOODYCHANG0611_COMMON_API std::vector<std::vector<woodychang0611::geometry::IPoint2D> >; 
#endif

		class Polygon2D{
		private:
			std::vector <Point2D> point_data_;
		public:
			inline UINT32 GetSideCount(void) const {return point_data_.size();};
			inline void InsertPoint(const Point2D& pt)	{point_data_.push_back(pt);};
			inline const Point2D& GetPoint(UINT16 i)const {return point_data_[i];};
			inline Point2D& GetPoint(UINT16 i) {return point_data_[i];};
			inline void Offset(FLOAT offset_x, FLOAT offset_y){
				for (UINT16 i=0;i<point_data_.size();++i) 
					point_data_[i].Offset(offset_x,offset_y);
			};
		};

		//Return 1 if there is intersection
		WOODYCHANG0611_COMMON_API INT16 Intersection(
			const Point2D &pt1,const Point2D &pt2,	const Point2D &pt3,
			const Point2D &pt4,Point2D &pt_dst);
		WOODYCHANG0611_COMMON_API INT16 Intersection(const Line2D &line1,
			const Line2D &line2, Point2D &pt_dst);
		//If line1 and line2 is Parallel return true else false
		WOODYCHANG0611_COMMON_API INT16 IsParallel(
			const Line2D &line1,const Line2D &line2);
		WOODYCHANG0611_COMMON_API INT16  IsIntersected(const Line2D &line1,const Line2D &line2);
		//If line1 and line2 is Intersect return true else false
		//Including parallel situation
		WOODYCHANG0611_COMMON_API INT16  IsIntersected(
			const Line2D &line1,const Line2D &line2);
		
		WOODYCHANG0611_COMMON_API Point2D Rotate(const Point2D& src_point, const Point2D& center_point,const FLOAT& angle);
	

	
	} //namespace geometry
} //namespace woodychang0611

#ifdef _WIN32
 #pragma warning(pop)
#endif

#endif //__WOODYCHANG0611_COMMON_GEOMETRY_H__
