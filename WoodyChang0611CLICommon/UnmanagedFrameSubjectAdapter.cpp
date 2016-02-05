#include "Stdafx.h"

#include "..\woodychang0611_common\woodychang0611_common.h"
#include "Imaging.h"
#include <vcclr.h>
#using <mscorlib.dll>
#include <stdio.h>

using namespace System;
using namespace  System::ComponentModel;
using namespace System::Windows::Media;

delegate void ImageSetterFunc(System::Windows::Media::ImageSource ^ src);

namespace{

	class  ManagedFrameObserver : public woodychang0611::video::FrameObserverBase{
	public:
		ManagedFrameObserver();
		gcroot <ImageSetterFunc^ >imageSetterFunc;
		virtual void UpdateSubject(woodychang0611::design_patterns::Subject* target);
	};

	ManagedFrameObserver::ManagedFrameObserver(){
		imageSetterFunc=nullptr;
	}

	void ManagedFrameObserver::UpdateSubject(woodychang0611::design_patterns::Subject * s){
		if (s ==_frame_graber){
			woodychang0611::video::FrameSubjectBase * subject =
				(woodychang0611::video::FrameSubjectBase * )  s;
			if(imageSetterFunc){
				System::Windows::Media::Imaging::BitmapImage ^ resultImage
					= WoodyChang0611::Image::Imaging::GetBitmapImage((System::IntPtr)subject->GetImage());
				imageSetterFunc->Invoke(resultImage);
			}
		}
	}
}



namespace WoodyChang0611{
	namespace Image{
		public ref class ImageReadyEventArgs : System::EventArgs
		{
		private:

		public:
			property System::Windows::Media::ImageSource^ Image;
		};
		public delegate void ImageReadyEventHandler(System::Object^ sender, ImageReadyEventArgs^ e);

		public ref class UnmanagedFrameSubjectAdapter : System::ComponentModel::INotifyPropertyChanged
		{
		private:
			ManagedFrameObserver * _observer;
			woodychang0611::video::FrameSubjectBase * _subject;
			ImageSource ^_image;
		public:

			// value should be type of woodychang0611::video::FrameSubjectBase *
			void SetSubject(System::IntPtr value)
			{
				woodychang0611::video::FrameSubjectBase * temp
					= (woodychang0611::video::FrameSubjectBase *) (void*)value;

				if(_subject != temp)
				{
					_subject = temp;
				//	if(temp!=nullptr)
					_observer->SetFrameGraber(_subject);
				}
			}
			property ImageSource^ Image
			{
				ImageSource^ get()
				{
					return _image;
				}
				void set( ImageSource^ value)
				{
					if(value!=_image){
						_image=value;
						OnPropertyChanged("Image");
						ImageReadyEventArgs ^arg = gcnew ImageReadyEventArgs();
						arg->Image = _image;
						ImageReady(this,arg);
					}
				}
			}
			virtual event ImageReadyEventHandler ^ ImageReady;
			virtual event System::ComponentModel::PropertyChangedEventHandler^ PropertyChanged;

			UnmanagedFrameSubjectAdapter(){
				_observer=nullptr;
				_subject=nullptr;
				_observer =  new ManagedFrameObserver();
				_observer->imageSetterFunc = gcnew ImageSetterFunc(this,&UnmanagedFrameSubjectAdapter::Image::set);
			};

			!UnmanagedFrameSubjectAdapter(){
				if (_observer!=nullptr) delete _observer;
			}

			~UnmanagedFrameSubjectAdapter(){
				this->!UnmanagedFrameSubjectAdapter();
			};

			void OnPropertyChanged(String^ info){
				PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
			}

			void Play(){
				if(_subject!=nullptr)
					_subject->Play();
			}
			void Pause()
			{
				if(_subject!=nullptr)
					_subject->Pause();
			}

			void Stop(){
				if(_subject!=nullptr)
					_subject->Stop();
			}
		};
	}
}