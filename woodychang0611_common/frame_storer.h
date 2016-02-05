#pragma once
#ifndef __SUNLIB_FRAME_STORER2__
#define __SUNLIB_FRAME_STORER2__
#include "frame_storer_base.h"
#include "image_storer.h"

namespace sunlib{
	namespace video{
		struct FrameStorerParameter{
			UINT64 cycle_time_ms;
			UINT64 max_cycle_save_count;
			sunlib::win::ImageStorerParameter image_storer_parameter;
			bool on;
		};

		class FrameStorer:public sunlib::video::FrameStorerBase
		{
		public:
			SUNLIB_API FrameStorer();
			sunlib::win::ImageStorer image_storer_;
			SUNLIB_API virtual void UpdateSubject(sunlib::design_patterns::Subject *the_changed_subject);

			SUNLIB_API FrameStorerParameter GetParameter();//因為要同步內部物件的參數 所以不是const;
			SUNLIB_API void SetParameter(const FrameStorerParameter parameter);

			SUNLIB_API void CycleTime(const UINT64 &cycle_time);
			SUNLIB_API UINT64 CycleTime()const;
			SUNLIB_API void MaxCycleSaveCount(const UINT64 &max_cycle_save_count);
			SUNLIB_API UINT64 MaxCycleSaveCount()const;

		protected:
			FrameStorerParameter _parameter;
			UINT64 _save_count;
			UINT64 _zero_time;
		};
	}
}

#endif//__SUNLIB_FRAME_STORER__