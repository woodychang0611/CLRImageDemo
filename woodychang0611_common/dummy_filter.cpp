#include "base.h"
#include "dummy_filter.h"

using namespace woodychang0611;
namespace woodychang0611 {
	namespace video {
		DummyFilter::DummyFilter(void)
		{
			_color = woodychang0611::color::RGB24_BLUE;
			_alpha = 0x80;
			_enable = true;
			_option = _T("All");
		}

		DummyFilter::~DummyFilter(void)
		{
			woodychang0611::diagnostics::SendInfomation(_T("DummyFilter::~DummyFilter"));
		}


		woodychang0611::image::ImageRGB24 *  DummyFilter::GetImage() {
			return &_img;
		}
		void DummyFilter::UpdateSubject(woodychang0611::design_patterns::Subject *the_changed_subject)//subject呼叫後的執行工作
		{
			if (the_changed_subject == this->_frame_graber) {
				_img.CopyFrom(*_frame_graber->GetImage());
				if (_enable) {
					if (_option == _T("All")) {
						for (UINT32 i = 0; i < _img.GetImagePixelCount(); i++)
							_img.GetDataPointer()[i] =  woodychang0611::color::MixColor(_color, _img.GetDataPointer()[i], _alpha);
					}
					else if (_option == _T("Vertical")) {
						for (UINT32 i = 0; i < _img.GetImagePixelCount(); i += 4)
							_img.GetDataPointer()[i] = woodychang0611::color::MixColor(_color, _img.GetDataPointer()[i], _alpha);
					}
					else if (_option == _T("Horizontal")) {
						for (UINT32 i = 0; i < _img.GetImagePixelCount(); i++)
							if (i / _img.GetImageWidth() % 4)
								_img.GetDataPointer()[i] = woodychang0611::color::MixColor(_color, _img.GetDataPointer()[i], _alpha);
					}
				}
				this->NotifyObserver();
			}
		}
	}
}