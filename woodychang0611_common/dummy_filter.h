#ifndef __DUMMY_FILTER__H_____
#define __DUMMY_FILTER__H_____


#include "base.h"
#include "frame_observer_base.h"
#include "frame_subject_base.h"
//#include "..\ParameterLib\parameter_lib.h"
namespace woodychang0611{
	namespace video{
		class DummyFilter :
			public woodychang0611::video::FrameObserverBase, public woodychang0611::video::FrameSubjectBase
		{
		public:
			woodychang0611::image::ImageRGB24 _img;

			woodychang0611::color::RGB24 _color;
			bool _enable;
			UINT8 _alpha; //透明度
			std::tstring _option;
		public:
			WOODYCHANG0611_COMMON_API	virtual woodychang0611::image::ImageRGB24 *  GetImage();
			WOODYCHANG0611_COMMON_API virtual void UpdateSubject(woodychang0611::design_patterns::Subject *the_changed_subject);//subject呼叫後的執行工作
			//	virtual void ExportParameter(woodychang0611::parameter::ParameterItemSet &)const;
			//	virtual void ImportParameter(const woodychang0611::parameter::ParameterItemSet &);
			WOODYCHANG0611_COMMON_API	DummyFilter(void);
			WOODYCHANG0611_COMMON_API	~DummyFilter(void);
		};

	}
}
#endif 