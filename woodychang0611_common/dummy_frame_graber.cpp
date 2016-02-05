#include "dummy_frame_graber.h"
#include "diagnostics.h"
#include <ctime>
#include <time.h>
#include <sstream>
#include "draw_object.h"
#include <cstdlib> //for random

namespace woodychang0611{
	namespace video{

		DummyFrameGraber::DummyFrameGraber(){
			thread_handle_=NULL;
			//亂數
			//避免time取得相同結果 亂數出使相同
			Sleep(1);
			srand((INT32) time(NULL));
			_bg_graylevel =rand()%50+200;
			SetElementCount(30);
		}

		DummyFrameGraber::~DummyFrameGraber(){
			_Close();
			woodychang0611::diagnostics::SendInfomation(_T("DummyFrameGraber::~DummyFrameGraber"));
		}

		bool DummyFrameGraber::_Open(void){
			//thread_handle_ !=NULL means it is already opened
			if (thread_handle_!=NULL)	return false;
			thread_handle_ =CreateThread(NULL ,0,this->_ThreadFunc,this,0,NULL);
			frame_info_.start_frame_=0;
			frame_info_.frame_length_ =0; 
			frame_info_.frame_per_second_=30;
			frame_info_.frame_width_=_width;
			frame_info_.frame_height_=_height;
			frame_info_.current_frame_=0;
			return true;
		}
		void DummyFrameGraber::_Close(void){
			_status=FRAME_SUBJECT_STOP;
			//! \todo test 
			//!  \todo set timeout time as a variable instead const
			WaitForSingleObject(thread_handle_,1000);
			if (thread_handle_!=NULL){
				thread_handle_=NULL;
			}
		}

		void DummyFrameGraber::SetElementCount(INT16 count){
				if(count >MAX_ELEMENT_COUNT) count =MAX_ELEMENT_COUNT;
				if(count <0) count =0;
				this->_elementCount =count;
			woodychang0611::color::RGB24 colors[7]={
				woodychang0611::color::RGB24_RED,
				woodychang0611::color::RGB24_YELLOW,
				woodychang0611::color::RGB24_LIME,
				woodychang0611::color::RGB24_BLUE,
				woodychang0611::color::RGB24_CYAN,
				woodychang0611::color::RGB24_MAGENTA,
				woodychang0611::color::RGB24_WHITE
			};

			for (INT16 i=0;i<_elementCount;i++){
				_color[i]=colors[rand()%7];
				_x[i]=rand()%_width;
				_y[i]=rand()%_height;
				_x_speed[i]=5+rand()%10;
				_y_speed[i]=5+rand()%10;
			}

		}


		DWORD WINAPI DummyFrameGraber::_ThreadFunc (LPVOID lpParameter){
			DummyFrameGraber* dummy_frame_graber;
			dummy_frame_graber = (DummyFrameGraber*) lpParameter;
			woodychang0611::image::ImageRGB24 image(dummy_frame_graber->frame_info_.frame_width_,dummy_frame_graber->frame_info_.frame_height_);
			dummy_frame_graber->image_pointer_ = &image;
	//		srand((INT32)time(NULL)+(INT32) dummy_frame_graber->thread_handle_);

			//the thread will end itself if status is FRAME_SUBJECT_STOP
			woodychang0611::geometry::Circle2D circle;
			woodychang0611::image::DrawObject obj;



			while(dummy_frame_graber->GetStatus() !=FRAME_SUBJECT_STOP){	
				switch(dummy_frame_graber->GetStatus()){ 
				case FRAME_SUBJECT_PLAY:
					dummy_frame_graber->frame_info_.current_frame_++;
					image.Clear(woodychang0611::color::RGB24((UINT8)dummy_frame_graber->_bg_graylevel,(UINT8)dummy_frame_graber->_bg_graylevel,(UINT8)dummy_frame_graber->_bg_graylevel));
	
					for (INT16 i =0;i<dummy_frame_graber->GetElementCount();i++){
					obj.fill_color_=dummy_frame_graber->_color[i];
					obj.fill_=true;
					obj.border_width_=0;
					dummy_frame_graber->_x[i] += dummy_frame_graber->_x_speed[i];
					dummy_frame_graber->_y[i] += dummy_frame_graber->_y_speed[i];

					//檢查邊緣 超過邊界 速度變反向
					if(dummy_frame_graber->_x[i]<0){
						dummy_frame_graber->_x[i]=0;
						dummy_frame_graber->_x_speed[i]=-dummy_frame_graber->_x_speed[i];
					}else if(dummy_frame_graber->_x[i]>dummy_frame_graber->_width){
						dummy_frame_graber->_x[i]=dummy_frame_graber->_width;
						dummy_frame_graber->_x_speed[i]=-dummy_frame_graber->_x_speed[i];
					}
					if(dummy_frame_graber->_y[i]<0){
						dummy_frame_graber->_y[i]=0;
						dummy_frame_graber->_y_speed[i]=-dummy_frame_graber->_y_speed[i];
					}else if(dummy_frame_graber->_y[i]>dummy_frame_graber->_height){
						dummy_frame_graber->_y[i]=dummy_frame_graber->_height;
						dummy_frame_graber->_y_speed[i]=-dummy_frame_graber->_y_speed[i];
					}
					//circle=circle;
					circle = woodychang0611::geometry::Circle2D(dummy_frame_graber->_x[i],dummy_frame_graber->_y[i],5);
					obj.Draw(circle,image);

					}

					dummy_frame_graber->NotifyObserver();
					Sleep(1);
					break;
				default:
					break;
				}//end of switch(avi_frame_graber->GetStatus())
			}//end of while(avi_frame_graber->GetStatus() !=FRAME_SUBJECT_STOP)
			dummy_frame_graber->thread_handle_=NULL;
			return 0;
		}

	}//end of namespace video
}//end of namespace woodychang0611

