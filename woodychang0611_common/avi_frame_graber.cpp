#include "avi_frame_graber.h"
#include "diagnostics.h"
#include <ctime>
#include <time.h>
#include <sstream>

namespace woodychang0611{
	namespace video{

		AviFrameGraber::AviFrameGraber(){
			frame_=NULL;
			avi_file_=NULL;
			stream_=NULL;
			AVIFileInit();
		}

		AviFrameGraber::~AviFrameGraber(){
			_Close();
			AVIFileExit();
		}
		
		bool AviFrameGraber::_Open(void){
			//thread_handle_ !=NULL means it is already opened
			if (thread_handle_!=NULL)	return false;

			int res=AVIFileOpen(&avi_file_, file_path_.c_str(), OF_READ, NULL);
			if (res!=AVIERR_OK){
				woodychang0611::diagnostics::SendError(_T("AviFrameGraber Open File Fail"));
				_Close();
				return false;
			}
			res=AVIFileGetStream(avi_file_, &stream_, streamtypeVIDEO, 0/*first stream*/);
			if (res!=AVIERR_OK){
				woodychang0611::diagnostics::SendError(_T("AviFrameGraber Get Stream Fail"));
				_Close();
				return false;
			}
			if (AVIStreamStart(stream_)==-1 || AVIStreamLength(stream_)==-1){
				woodychang0611::diagnostics::SendError(_T("AviFrameGraber Stream Start or Length no correct"));
				_Close();
				return false;								
			}
			AVIFileInfo(avi_file_, &avi_info_, sizeof(AVIFILEINFO));
			BITMAPINFOHEADER bih;
			bih.biSize = sizeof(BITMAPINFOHEADER);
			bih.biWidth = avi_info_.dwWidth;
			bih.biHeight = avi_info_.dwHeight;
			bih.biPlanes = 1;
			bih.biBitCount = 24;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = 0;
			bih.biXPelsPerMeter = 0;
			bih.biYPelsPerMeter = 0;
			bih.biClrUsed = 0;
			bih.biClrImportant = 0;
			frame_=AVIStreamGetFrameOpen(stream_, (LPBITMAPINFOHEADER) &bih);
			if (frame_ !=NULL){
				start_frame_ = AVIStreamStart(stream_);
				frame_length_ = AVIStreamLength(stream_);
				current_frame_ = start_frame_;

				//Set Frame info
				frame_info_.start_frame_=start_frame_;
				frame_info_.frame_length_ =frame_length_; 
				frame_info_.frame_per_second_=(FLOAT32)avi_info_.dwRate/avi_info_.dwScale;
				frame_info_.frame_width_=(UINT16) avi_info_.dwWidth;
				frame_info_.frame_height_=(UINT16)  avi_info_.dwHeight;
				_status = FRAME_SUBJECT_PAUSE;
				thread_handle_ =CreateThread(NULL ,0,this->_ThreadFunc,this,0,NULL);
				return true;
			}else{
				woodychang0611::diagnostics::SendError(_T("AviFrameGraber Get Frame Failed"));
			}
			return false;
		}
		void AviFrameGraber::_Close(void){
			_status=FRAME_SUBJECT_STOP;
			//! \todo test 
			//!  \todo set timeout time as a variable instead const
			WaitForSingleObject(thread_handle_,1000);
			if (thread_handle_!=NULL){
				thread_handle_=NULL;
			}
			if(frame_!=NULL){
				AVIStreamGetFrameClose(frame_);
				frame_ = NULL;
			}
			if (stream_!=NULL){
				AVIStreamRelease(stream_);
				stream_=NULL;
			}
			if(avi_file_!=NULL){
				AVIFileRelease(avi_file_);
				avi_file_=NULL;
			}
		}

		DWORD WINAPI AviFrameGraber::_ThreadFunc (LPVOID lpParameter){
			AviFrameGraber* avi_frame_graber;
			avi_frame_graber = (AviFrameGraber*) lpParameter;
			woodychang0611::image::ImageRGB24	image ((UINT16)avi_frame_graber->avi_info_.dwWidth,(UINT16)avi_frame_graber->avi_info_.dwHeight);
			avi_frame_graber->image_pointer_ = &image;

			//the thread will end itself if status is FRAME_SUBJECT_STOP
			while(avi_frame_graber->GetStatus() !=FRAME_SUBJECT_STOP){
				unsigned char* dib_pointer;
				switch(avi_frame_graber->GetStatus()){ 
					case FRAME_SUBJECT_PLAY:
						dib_pointer = (unsigned char*) AVIStreamGetFrame(avi_frame_graber->frame_,avi_frame_graber->current_frame_);
						if (avi_frame_graber->GetStatus()==FRAME_SUBJECT_STOP) return 0;
						if (dib_pointer){
							//Copy the from source DIB to woodychang0611::ImageRGB24
							for (UINT16 j=0;j<image.GetImageHeight();++j){
								for (UINT16 i=0;i<image.GetImageWidth();++i){
									UINT32 pos = (image.GetImageHeight()-j-1)*
										image.GetBytePerLine()+i*3+40;
									image.GetPixel(i,j).b_ = dib_pointer[pos];
									image.GetPixel(i,j).g_ = dib_pointer[pos+1];
									image.GetPixel(i,j).r_ = dib_pointer[pos+2];
								}
							}
							// Update frame info
							avi_frame_graber->frame_info_.current_frame_ = avi_frame_graber->current_frame_;
							std::time_t frame_time = mktime(&avi_frame_graber->initial_time_)+(INT32)
								((avi_frame_graber->current_frame_-avi_frame_graber->start_frame_)/
								avi_frame_graber->frame_info_.frame_per_second_);
							tm tm_frame;
							localtime_s(&tm_frame,&frame_time);
							avi_frame_graber->frame_info_.frame_time_=tm_frame;
							avi_frame_graber->NotifyObserver();
						}
						avi_frame_graber->current_frame_++;
						//Check end
						if (avi_frame_graber->current_frame_ >=
							avi_frame_graber-> start_frame_+avi_frame_graber->frame_length_){
								if(avi_frame_graber->auto_replay_){
									avi_frame_graber->current_frame_=avi_frame_graber->start_frame_;
								}
								else{
									avi_frame_graber->current_frame_ = avi_frame_graber->start_frame_;
									avi_frame_graber->SetStatus(FRAME_SUBJECT_PAUSE);
								}
						}
						break;
					default:
						break;
				}//end of switch(avi_frame_graber->GetStatus())
			}//end of while(avi_frame_graber->GetStatus() !=FRAME_SUBJECT_STOP)
			return 0;
		}

	}//end of namespace video
}//end of namespace woodychang0611

