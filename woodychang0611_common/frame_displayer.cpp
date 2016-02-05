#include "frame_displayer.h"
#include "windows_io.h"
#include "draw_object.h"
#include <ctime>
#include <time.h>
#include <sstream>

namespace woodychang0611{
	namespace video{
		Displayer::Displayer()
		{
			average_count=10;
			t1=0;
			counter=0;
			actual_frame_per_secound =0;
		}

		Displayer::~Displayer()
		{

		}

		void Displayer::UpdateSubject(woodychang0611::design_patterns::Subject *the_changed_subject){

			if (the_changed_subject ==  _frame_graber){
				HBITMAP bmp = CreateCompatibleBitmap(_hdc,_width,_height);
				HDC hdc_temp = CreateCompatibleDC(_hdc);
				::SelectObject(hdc_temp,bmp);
				woodychang0611::image::ImageRGB24 * imgPtr = _frame_graber->GetImage();

				if (imgPtr!=NULL){
					//woodychang0611::win::ShowImage(hdc_temp,0,0,_width,_height,*imgPtr);

					UINT32 image_count = _frame_graber->GetImageCount();
					UINT16 new_height = (UINT16) (_height/image_count);
					for(UINT32 i=0;i<image_count;i++){
						woodychang0611::win::ShowImage(hdc_temp,0,i*new_height,_width,new_height,*_frame_graber->GetImage(i));
					}
					//Calculate average  Frame elapse time and FPS (frame per sec.)
					

					counter = counter%average_count;
					
					if(counter == 0){
						actual_frame_per_secound = average_count*1000.0f/(GetTickCount()-t1);
						t1 = GetTickCount();
						average_count = (int) actual_frame_per_secound/3;
						if(average_count<1) average_count=1;
					}
					counter++;
					//SetBkMode(hdc_temp,TRANSPARENT);
					SetBkColor(hdc_temp,RGB(255,255,255));
					SetTextColor(hdc_temp, RGB(0,0,255));
					const FrameInfo * info = _frame_graber->GetFrameInfo();
					if(1){//Temp 2010.1.28 要改回來
					std::tstring str;
					std::tstringstream stream;
					//str +=_T("Frame Time:");
					//str += _tasctime (&info->frame_time_);
					stream <<_T("Width:")<<info->frame_width_<<_T("\n");
					stream <<_T("Height:")<<info->frame_height_<<_T("\n");
					stream <<_T("Frame Per Second(ideal):")<<info->frame_per_second_<<_T("\n");
					stream <<_T("Frame Per Second(actual):")<<actual_frame_per_secound<<_T("\n");
					stream <<_T("Start Frame Index:")<<info->start_frame_<<_T("\n");
					stream <<_T("Total Frame Count:")<<info->frame_length_<<_T("\n");
					stream <<_T("Current Frame Index:")<<info->current_frame_<<_T("\n");
					stream <<_T("Note:")<<info->note_<<_T("\n");

					str+=stream.str();
					RECT rect={10,10,500,500};
					DrawText(hdc_temp,str.c_str(),str.size(),&rect,DT_NOPREFIX );
					}
					::BitBlt(_hdc,_left,_top,_width,_height,hdc_temp,0,0,SRCCOPY);
				}
				DeleteObject(bmp);
				DeleteDC(hdc_temp);
			}
		}
		
	}//end of namespace video
}//end of namespace woodychang0611
