#pragma once
#ifndef __WOODYCHANG0611_COMMON_FRAME_PROCESSOR_BASE__
#define __WOODYCHANG0611_COMMON_FRAME_PROCESSOR_BASE__
#include "frame_observer_base.h"

namespace woodychang0611{
	namespace video{

		class FrameProcessorBase:public woodychang0611::video::FrameObserverBase,public woodychang0611::design_patterns::Subject
		{
		};
	}
}

#endif