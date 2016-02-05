#pragma once

#ifndef __WOODYCHANG0611_COMMON__VIDEO_H__
#define __WOODYCHANG0611_COMMON__VIDEO_H__

#include "base.h"
#include "image.h"
#include "observer_pattern.h"
namespace woodychang0611{
	namespace video{
		enum FrameSubjectStatus{
			FRAME_SUBJECT_PLAY,
			FRAME_SUBJECT_PAUSE,
			FRAME_SUBJECT_STOP
		};

		struct FrameInfo{
			std::tm frame_time_;
			UINT16 frame_width_;
			UINT16 frame_height_;
			INT32 frame_length_;
			INT32 start_frame_;
			INT32 current_frame_;
			FLOAT32 frame_per_second_;
			std::tstring note_;
		};

		class FrameSubjectBase:public woodychang0611::design_patterns::Subject{
		public:
			virtual ~FrameSubjectBase(){};
			virtual woodychang0611::image::ImageRGB24 * GetImage()=0;
			virtual woodychang0611::image::ImageRGB24 * GetImage(UINT32){return GetImage();};
			inline  const FrameInfo* GetFrameInfo(void) {return &frame_info_;};
			virtual UINT32 GetImageCount(void){return 1;};
			WOODYCHANG0611_COMMON_API virtual bool Play();
			WOODYCHANG0611_COMMON_API virtual bool Pause();
			WOODYCHANG0611_COMMON_API virtual bool Stop();
			WOODYCHANG0611_COMMON_API virtual bool IsPlay();
			WOODYCHANG0611_COMMON_API virtual bool IsPause();
			WOODYCHANG0611_COMMON_API virtual bool IsStop();
		protected:
			FrameSubjectBase(){};
			FrameInfo frame_info_;
			FrameSubjectStatus _status;
		};


	}//namespace video
}//namespace woodychang0611
#endif //#ifndef __WOODYCHANG0611_COMMON__VIDEO_H__
