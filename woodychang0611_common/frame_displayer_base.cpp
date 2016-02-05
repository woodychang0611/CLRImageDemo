#include "frame_displayer_base.h"


namespace woodychang0611{
	namespace video{

		DisplayerBase::DisplayerBase()
		{
			_top=0;
			_left=0;
			_width=720;
			_height=240;

			
			_hdc=NULL;
		}

		DisplayerBase::~DisplayerBase()
		{

		}
		void DisplayerBase::Init(HDC hdc,const UINT16 top,const UINT16 left,const UINT16 width,const UINT16 height){
			SetHdc(hdc);
			SetPosition(top,left);
			SetSize(width,height);
		};
		inline void DisplayerBase::SetHdc(HDC hdc){
			Hdc(hdc);
		};
		inline void DisplayerBase::SetPosition(const UINT16 top,const UINT16 left){
			Top(top);
			Left(left);
		};
		inline void DisplayerBase::SetSize(const UINT16 width,const UINT16 height){
			Width(width);
			Height(height);
		};
		
		inline HDC DisplayerBase::Hdc()const{
			return _hdc;
		};
		inline UINT16 DisplayerBase::Top()const{
			return _top;
		};
		inline UINT16 DisplayerBase::Left()const{
			return _left;
		};
		inline UINT16 DisplayerBase::Width()const{
			return _width;
		};
		inline UINT16 DisplayerBase::Height()const{
			return _height;
		};
		inline void DisplayerBase::Hdc(HDC hdc){
			_hdc=hdc;
		};
		inline void DisplayerBase::Top(UINT16 top){
			_top=top;
		};
		inline void DisplayerBase::Left(UINT16 left){
			_left=left;
		};
		inline void DisplayerBase::Width(UINT16 width){
			_width=width;
		};
		inline void DisplayerBase::Height(UINT16 height){
			_height=height;
		};

		

	}//end of namespace video
}//end of namespace woodychang0611
