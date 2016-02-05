
#include "draw_object.h"
#include "image_operation.h"

using woodychang0611::geometry::FLOAT;
using woodychang0611::geometry::Point2D;
using woodychang0611::geometry::Line2D;
using woodychang0611::geometry::Ellipse2D;
using woodychang0611::geometry::Polygon2D;
using woodychang0611::geometry::GetDistance;
using woodychang0611::color::RGB24;
using woodychang0611::color::MixColor;

namespace woodychang0611{
	namespace image{
		DrawObject::DrawObject(void)
		{
			border_alpha_ = 0xFF;
			border_color_ = woodychang0611::color::RGB24_BLACK;
			fill_alpha_ = 0xFF;
			fill_color_ = woodychang0611::color::RGB24_WHITE;
			border_width_=1;
			fill_=false;
		}

		void DrawObject::Draw (const woodychang0611::geometry::IPoint2D &pt,ImageRGB24 &img)const
		{
			if(!img.IsValid()) return;
			if (!img.IsWithin(pt)) return;
			if(border_alpha_==0x00) return; //0% 

			img.GetPixel(pt.x_,pt.y_)=MixColor(border_color_,img.GetPixel(pt.x_,pt.y_),border_alpha_);
		}
		void DrawObject::Draw (const std::vector<woodychang0611::geometry::IPoint2D> &vector_pt,ImageRGB24 &img)const
		{
			for (UINT32 i=0;i<vector_pt.size();++i){
				Draw(vector_pt[i],img);
			}
		}

		DrawObject::~DrawObject(void)
		{
		}
		//! \todo optimization by detecting edge
		void DrawObject::Draw (const woodychang0611::geometry::Line2D &line,ImageRGB24 &img)const
		{
			if(!img.IsValid()) return;
			if(border_alpha_==0x00) return; //0% 
			if(border_width_==0) return; //0% 

			if (border_width_==1){
				FLOAT distance_x = line.end_point_.x_ - line.start_point_.x_;
				FLOAT distance_y = line.end_point_.y_ - line.start_point_.y_;
				INT16 step;
				step = static_cast<INT16> (ceil ( (woodychang0611::max) (fabs(distance_x),fabs(distance_y))+1));
				if (step==1){
					Draw(line.start_point_,img);
				}else{
					FLOAT det_x=distance_x/(step-1);
					FLOAT det_y=distance_y/(step-1);
					for (INT16 i=0;i<step;++i){
						Point2D pt_temp(line.start_point_.x_ + i*det_x,line.start_point_.y_ +i*det_y);
						Draw(pt_temp,img);
					}
				}
			}//border_width==1
			else if(border_width_>1){
			// Draw as a rectangle
				Point2D pt1,pt2;
				pt1 = line.start_point_;
				pt2 = line.end_point_;
				FLOAT ang=atan((pt1.y_-pt2.y_)/(pt1.x_-pt2.x_));
				Point2D offset(border_width_/2.0f*sin(ang),-border_width_/2.0f*cos(ang));
				offset.x_=(offset.x_>0 ? offset.x_-0.1f:offset.x_+0.1f);
				offset.y_=(offset.y_>0 ? offset.y_-0.1f:offset.y_+0.1f);
				Polygon2D rectangle;
				rectangle.InsertPoint(pt1+offset);
				rectangle.InsertPoint(pt2+offset);
				rectangle.InsertPoint(pt2-offset);
				rectangle.InsertPoint(pt1-offset);
				DrawObject obj;
				obj.border_width_=0;
				obj.fill_=true;
				obj.fill_color_=border_color_;
				obj.fill_alpha_=border_alpha_;
				obj.Draw(rectangle,img);
			}// border_width!=1
		}



		 //Used only for Draw return x by given y
			bool GetXValue(const woodychang0611::geometry::Ellipse2D &src_ellipse,FLOAT y, FLOAT& x1, FLOAT& x2){
				Point2D focus_point1,focus_point2;
				src_ellipse.GetFocusPoint(focus_point1,focus_point2);
				FLOAT major_diameter = src_ellipse.GetMajorRadius()*2;
				FLOAT dx = focus_point1.x_ - focus_point2.x_; 
				FLOAT dy = focus_point1.y_ - focus_point2.y_; 
				FLOAT dia_pow_2 = pow(major_diameter,2);
				//! \todo Check 
				if (y>(focus_point1.y_ + focus_point2.y_)/2)
					y = (focus_point1.y_ + focus_point2.y_)/2-y-0.5f;
				else if(y<(focus_point1.y_ + focus_point2.y_)/2)
					y = (focus_point1.y_ + focus_point2.y_)/2-y+0.5f;
				else
					y=0;
					FLOAT determinant = (dia_pow_2-dx*dx-dy*dy)*(dia_pow_2  - pow(dx,2) - 4*pow(y,2));
				if (determinant<0) return false;
				//x1+x2 = sum; x1-x2=difference
				FLOAT sum=2*(dx*dy*y)/(dx*dx-dia_pow_2);
				FLOAT difference= sqrt( dia_pow_2* determinant) / (dx*dx-dia_pow_2);
				x1 = (focus_point1.x_ + focus_point2.x_)/2+sum/2 +difference/2;
				x2 = (focus_point1.x_ + focus_point2.x_)/2+sum/2 - difference/2;
				//Make  x1 smaller than x2
				if (x1>x2){
					FLOAT temp;
					temp=x1;
					x1=x2;
					x2=temp;
				}
				return true;
			}
		void DrawObject::Draw(const woodychang0611::geometry::Ellipse2D &src_ellipse,ImageRGB24 &img)const{
			if(!img.IsValid()) return;
			if(border_alpha_==0x00 && fill_alpha_==0x00) return; //0% 
			if(border_width_==0 && fill_ ==false) return; //0% 
	
			DrawObject fill_obj; 
			fill_obj.border_color_ = fill_color_;
			fill_obj.border_alpha_ = fill_alpha_;
			fill_obj.border_width_=1;

			DrawObject border_obj; 
			border_obj.border_color_ = border_color_;
			border_obj.border_alpha_ = border_alpha_;
			border_obj.border_width_=1;

			Point2D src_focus_point1,src_focus_point2;	
			src_ellipse.GetFocusPoint(src_focus_point1,src_focus_point2);

			FLOAT src_major_radius = src_ellipse.GetMajorRadius();
			FLOAT src_focus_distance = GetDistance(src_focus_point1,src_focus_point2);
			FLOAT src_minor_radius = sqrt(pow(src_major_radius,2)-pow(src_focus_distance/2,2));
			// minor_radius=0 the ellipse is totally flat
			if(src_minor_radius==0) return;
			FLOAT outer_major_radius =  src_major_radius+border_width_/2.0f;
			FLOAT inner_major_radius =  src_major_radius-border_width_/2.0f;
			FLOAT outer_minor_radius =  src_minor_radius+border_width_/2.0f;
			FLOAT inner_minor_radius =  src_minor_radius-border_width_/2.0f;
			inner_minor_radius = (woodychang0611::max<FLOAT>)(inner_minor_radius,0.0f);

			FLOAT outer_focus_distance = 2*sqrt(pow(outer_major_radius,2)-pow(outer_minor_radius,2));
			FLOAT inner_focus_distance = 2*sqrt(pow(inner_major_radius,2)-pow(inner_minor_radius,2));
			Point2D center_point = (src_focus_point1+src_focus_point2)/2;
			Point2D outer_focus_point1,outer_focus_point2;
			Point2D inner_focus_point1,inner_focus_point2;
			FLOAT y_upper_limit = center_point.y_ - 
				sqrt(pow(2*src_major_radius,2) -pow(src_focus_point1.x_ - src_focus_point2.x_,2))/2 - border_width_/2-0.5f;
			FLOAT y_lower_limit =center_point.y_ + sqrt(pow(2*src_major_radius,2) -pow(src_focus_point1.x_ - src_focus_point2.x_,2))/2 + border_width_/2+0.5f;
			//Check Boundary
			y_upper_limit =(woodychang0611::max<FLOAT>) (y_upper_limit,0.0);
			y_lower_limit = (woodychang0611::min<FLOAT>) (y_lower_limit, static_cast<FLOAT>(img.GetImageHeight()));

			//Focus Distance =0 act as circle 
			if (src_focus_distance==0){
				outer_focus_point1 = center_point;
				outer_focus_point2 = center_point;
				inner_focus_point1 = center_point;
				inner_focus_point2 = center_point;
			}
			else{
				outer_focus_point1 = center_point+
					(src_focus_point1-center_point)*(outer_focus_distance/src_focus_distance);
				outer_focus_point2 = center_point+
					(src_focus_point2-center_point)*(outer_focus_distance/src_focus_distance);
				inner_focus_point1 = center_point+
					(src_focus_point1-center_point)*(inner_focus_distance/src_focus_distance);
				inner_focus_point2 = center_point+
					(src_focus_point2-center_point)*(inner_focus_distance/src_focus_distance);
			}
			Ellipse2D ellipse_outer(outer_focus_point1,outer_focus_point2,outer_major_radius);
			Ellipse2D ellipse_inner(inner_focus_point1,inner_focus_point2,inner_major_radius);
			
			for (FLOAT i=y_upper_limit;i<y_lower_limit;++i){
				FLOAT x1,x2,x3,x4;
				if(GetXValue(ellipse_outer,i,x1,x2)){
					Line2D line;
					x1 = (x1>0?floor(x1+0.501f):floor(x1-0.501f));
					x2 = (x2>0?floor(x2+0.499f):floor(x2-0.499f));
					//  x1  x3  x4  x2
					if (border_width_ ==0){
						fill_obj.Draw(Line2D(x1,i,x2,i),img);
					}else{
						if(GetXValue(ellipse_inner,i,x3,x4)){
							x3 = (x3>0?floor(x3+0.5001f):floor(x3-0.5001f));
							x4 = (x4>0?floor(x4+0.499f):floor(x4-0.499f));
							line =Line2D(Point2D(x1,i),Point2D(x3-1,i));
							border_obj.Draw(line,img);
							line =Line2D(Point2D(x2,i),Point2D(x4+1,i));
							border_obj.Draw(line,img);
							if(fill_)fill_obj.Draw(Line2D(x3,i,x4,i),img);
						}else
						{
							line =Line2D(Point2D(x1,i),Point2D(x2,i));
							border_obj.Draw(line,img);
						}
					}//end of border_width==0 else
				}
			}
		}
		void DrawObject::Draw(const woodychang0611::geometry::Circle2D &src_circle,ImageRGB24 &img)const{
			if(!img.IsValid()) return;
			if(border_alpha_==0x00 && fill_alpha_==0x00) return; //0% 
			if(border_width_==0 && fill_ ==false) return; //0% 
			if(src_circle.GetRadius()==0)return;
			Ellipse2D ellipse(src_circle.GetCenterPoint(),
				src_circle.GetCenterPoint(),
				src_circle.GetRadius());
			Draw(ellipse,img);
		}
		void DrawObject::Draw (const woodychang0611::geometry::Polygon2D &src_polygon,ImageRGB24 &img)const{
			if(!img.IsValid()) return;
			if(border_alpha_==0x00 && fill_alpha_==0x00) return; //0% 
			if(border_width_==0 && fill_ ==false) return; //0%
			if(src_polygon.GetSideCount()<2) return;

			DrawObject border_obj; 
			border_obj.border_color_ = border_color_;
			border_obj.border_alpha_ = border_alpha_;
			border_obj.border_width_=1;

			DrawObject fill_obj; 
			fill_obj.border_color_ = fill_color_;
			fill_obj.border_alpha_ = fill_alpha_;
			fill_obj.border_width_=1;


			//Assign polygon point data to lines and find upper and lower limit of the polygon
			UINT32 side_count =src_polygon.GetSideCount(); 
			Line2D *polygon_side;
			polygon_side = new Line2D[side_count];
			FLOAT y_min = src_polygon.GetPoint(0).y_;
			FLOAT y_max = src_polygon.GetPoint(0).y_;
			FLOAT x_min = src_polygon.GetPoint(0).x_;
			FLOAT x_max = src_polygon.GetPoint(0).x_;
			for (UINT16 i=0;i<side_count;++i){
				Point2D pt1 = src_polygon.GetPoint(i);
				y_min = (woodychang0611::min)( y_min , pt1.y_ );
				y_max = (woodychang0611::max)( y_max , pt1.y_ );
				x_min = (woodychang0611::min)(x_min , pt1.x_ );
				x_max = (woodychang0611::max)( x_max , pt1.x_ );
				//For the last point
				//When i == side_cunt-1===>(i+1)%side_count ==0
				Point2D pt2 = src_polygon.GetPoint((i+1)%side_count);
				polygon_side[i]=Line2D(pt1,pt2);
			}
			x_min = floor(x_min);
			x_max = ceil(x_max);
			y_min = floor(y_min);
			y_max = ceil(y_max);
		//Drawing limit
			INT16 left_limit = (woodychang0611::max<INT16>)(static_cast<INT16>(x_min-border_width_), 0);
			INT16 right_limit = (woodychang0611::min<INT16>)(static_cast<INT16>(x_max+border_width_), img.GetImageWidth());
			INT16 upper_limit = (woodychang0611::max<INT16>)(static_cast<INT16>(y_min-border_width_), 0);
			INT16 lower_limit = (woodychang0611::min<INT16>)(static_cast<INT16>(y_max+border_width_), img.GetImageHeight());
			if (border_width_ <= 1 && border_alpha_==0xFF && fill_alpha_==0xFF){
				if (fill_){
					for (UINT16 i=upper_limit;i<lower_limit;++i){
						std::vector<FLOAT> intersection_x;
						for (UINT16 j=0;j<side_count;++j){
							Line2D scan_line(x_min-1,i,x_max+1,i);
							if (polygon_side[j].start_point_.y_ == i && 
								polygon_side[j].end_point_.y_ == i ){
								intersection_x.push_back(polygon_side[j].start_point_.x_);
								intersection_x.push_back(polygon_side[j].end_point_.x_);
								continue;
							};
							//Ignore if the intersection is the lower part of the point;
							if(i==(woodychang0611::min)(polygon_side[j].start_point_.y_ ,polygon_side[j].end_point_.y_)){
								continue;
							};
							if(IsIntersected(scan_line, polygon_side[j])==1){
								Point2D pt;
								Intersection(scan_line,polygon_side[j],pt);
								intersection_x.push_back(pt.x_);
							}
						}
						std::sort(intersection_x.begin(),intersection_x.end());
						for (UINT16 j=0;j<(intersection_x.size()/2);++j){
//							fill_obj.Draw(Line2D(intersection_x[2*j]+0.5f,i,intersection_x[2*j+1]-0.5f,i),img);
							//Changed by Woody 2009/12/16
							fill_obj.Draw(Line2D(intersection_x[2*j],i,intersection_x[2*j+1],i),img);
						}
					}
				}
				if (border_width_==0) border_obj.border_color_=fill_color_;
				for (UINT16 i=0;i<side_count;++i){
						border_obj.Draw(polygon_side[i],img);
				}
			}else{
				//For border_width>1 or alpha !=0
				ImageGray8 border_mask( right_limit-left_limit+1 , lower_limit-upper_limit+1 );
				ImageGray8 fill_mask( right_limit-left_limit+1 , lower_limit-upper_limit+1 );
				ImageRGB24 border_mask_temp( right_limit-left_limit+1 , lower_limit-upper_limit+1 );
				ImageRGB24 fill_mask_temp( right_limit-left_limit+1 , lower_limit-upper_limit+1 );
				Polygon2D offseted_polygon = src_polygon;
				offseted_polygon.Offset((FLOAT)-left_limit,(FLOAT)-upper_limit);
				DrawObject mask_obj;
				mask_obj.border_color_ = RGB24(0,0,0);
				mask_obj.fill_color_ = RGB24(0,0,0);
				mask_obj.border_alpha_ = 0xFF;
				mask_obj.fill_alpha_ = 0xFF;
				mask_obj.border_width_=1;
				mask_obj.fill_ = false;
				border_mask_temp.Clear(woodychang0611::color::RGB24(0xFF,0xFF,0xFF));
				mask_obj.Draw(offseted_polygon,border_mask_temp);
				mask_obj.border_width_=0;
				mask_obj.fill_ = true;
				fill_mask_temp.Clear(woodychang0611::color::RGB24(0xFF,0xFF,0xFF));
				mask_obj.Draw(offseted_polygon,fill_mask_temp);
				border_mask.CopyFrom(border_mask_temp);
				fill_mask.CopyFrom(fill_mask_temp);
		
				if((border_width_-1)/2>0) ErodeDisk(border_mask,border_mask,(border_width_-1)/2);
				for (UINT16 i=0;i<border_mask.GetImageWidth();++i){
					for (UINT16 j=0;j<border_mask.GetImageHeight();++j){
						if (border_mask.GetPixel(i,j).value_==0x00){
							if(border_width_==0)
								img.GetPixel(i+left_limit,j+upper_limit)=
									MixColor(fill_color_,img.GetPixel(i+left_limit,j+upper_limit),fill_alpha_);
							else
								img.GetPixel(i+left_limit,j+upper_limit)=
								MixColor(border_color_,img.GetPixel(i+left_limit,j+upper_limit),border_alpha_);
						}else if(fill_mask.GetPixel(i,j).value_==0x00)
						img.GetPixel(i+left_limit,j+upper_limit)=
							MixColor(fill_color_,img.GetPixel(i+left_limit,j+upper_limit),fill_alpha_);
					}
				}
			}
			delete []polygon_side;
		}

	}//namespace image
}//namespace woodychang0611

