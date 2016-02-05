#include "geometry2d.h"
#include "diagnostics.h"
namespace woodychang0611{
	namespace geometry{

		Point2D Point2D::operator+(const Point2D& src)const {
			Point2D temp;
			temp = *this;
			temp+=src;
			return temp;
		}
		Point2D Point2D::operator-(const Point2D& src)const{
			Point2D temp;
			temp = *this;
			temp-=src;
			return temp;
		}

		IPoint2D IPoint2D::operator+(const IPoint2D& src)const {
			IPoint2D temp;
			temp = *this;
			temp+=src;
			return temp;
		}
		IPoint2D IPoint2D::operator-(const IPoint2D& src)const{
			IPoint2D temp;
			temp = *this;
			temp-=src;
			return temp;
		}
		INT16  IsParallel(const Line2D &line1,const Line2D &line2)
		{
			//If line1 or line2 is not valid reutrn -1
			if ( !line1.IsValid() || !line2.IsValid() )
				return -1;
			//If line1 and line2 is Parallel return 1
			FLOAT64 test_value=
				(line1.end_point_.x_ - line1.start_point_.x_)*
				(line2.end_point_.y_ - line2.start_point_.y_)-
				(line2.end_point_.x_ - line2.start_point_.x_)*
				(line1.end_point_.y_ - line1.start_point_.y_);
			if(test_value==0) 
				return 1;
			//If line1 and line2 is Not Parallel return false
			return 0;
		}
		INT16 Intersection(const Point2D &pt1,const  Point2D &pt2,const Point2D &pt3,const Point2D &pt4,Point2D &pt_dst)
		{
			Line2D line1(pt1,pt2);
			Line2D line2(pt3,pt4);
			return Intersection(line1,line2,pt_dst);
		}
		INT16 Intersection(const Line2D &line1,const Line2D &line2,Point2D &pt_dst)
		{
			if(IsParallel(line1,line2)==1) {
				woodychang0611::diagnostics::SendWarning(_T("Two lines are parallel and have no intersection"));
				return 0;
			}
			if(IsParallel(line1,line2)==-1) {
				woodychang0611::diagnostics::SendWarning(_T("Two lines are not both valid (Start Point is same as End Point)"));
				return 0;
			}
			FLOAT x_intersection,y_intersection,x1,x2,x3,x4,y1,y2,y3,y4;
			x1 = line1.start_point_.x_;
			x2 = line1.end_point_.x_;
			x3 = line2.start_point_.x_;
			x4 = line2.end_point_.x_;

			y1 = line1.start_point_.y_;
			y2 = line1.end_point_.y_;
			y3 = line2.start_point_.y_;
			y4 = line2.end_point_.y_;

			//Equation from wiki Line-line intersection http://en.wikipedia.org/wiki/Line-line_intersection
			x_intersection=((x1*y1-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
			y_intersection=((x1*y1-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
			pt_dst = Point2D(x_intersection,y_intersection);
			return 1;
		}
		INT16  IsIntersected(const Line2D &line1,const Line2D &line2)
		{
			FLOAT x1,x2,x3,x4,y1,y2,y3,y4;
			x1 = line1.start_point_.x_;
			x2 = line1.end_point_.x_;
			x3 = line2.start_point_.x_;
			x4 = line2.end_point_.x_;

			y1 = line1.start_point_.y_;
			y2 = line1.end_point_.y_;
			y3 = line2.start_point_.y_;
			y4 = line2.end_point_.y_;
			
			if (
				(x3*(-y1+y2)+x2*(y1-y3)+x1*(-y2+y3))*(x4*(-y1+y2)+x2*(y1-y4)+x1*(-y2+y4))<=0 &&
				(x1*(-y3+y4)+x4*(y3-y1)+x3*(-y4+y1))*(x2*(-y3+y4)+x4*(y3-y2)+x3*(-y4+y2))<=0
				) return 1;
			return 0;
		}

		Point2D Rotate(const Point2D& src_point, const Point2D& center_point,const FLOAT& angle){
			FLOAT x = (src_point.x_ - center_point.x_ )*cos(angle) - (src_point.y_ - center_point.y_ )*sin(angle);
			FLOAT y = (src_point.x_ - center_point.x_ )*sin(angle) + (src_point.y_ - center_point.y_ )*cos(angle);
			return center_point+Point2D(x,y);
		}

	} //namespace geometry
} //namespace woodychang0611


