// 這是主要 DLL 檔案。

#include "stdafx.h"
#include "../woodychang0611_common//woodychang0611_common.h"
#include "CLRLibraryTest.h"
#include <stdio.h>

#using <mscorlib.dll>
#include <vcclr.h>


using namespace System;
using namespace System::Windows;
using namespace System::ComponentModel;
using namespace WoodyChang0611::Image;

namespace{
	class  MyFrameObserver;
}



namespace WoodyChang0611{
	namespace Test{

		public ref class MyTestClass : public System::Windows::FrameworkElement
		{
		protected:
			woodychang0611::video::DummyFrameGraber * graber;
			WoodyChang0611::Image::UnmanagedFrameSubjectAdapter^ imageProvider;
		public:
			MyTestClass();
			!MyTestClass();
			~MyTestClass() {
				MyTestClass::!MyTestClass();
			}
			property System::Windows::Input::ICommand^  StartCommand{
				System::Windows::Input::ICommand^ get()
				{
					WoodyChang0611::Common::RelayCommand^ cmd = gcnew WoodyChang0611::Common::RelayCommand(
						gcnew System::Action<System::Object^>(this,&MyTestClass::Start)
					);
					return cmd;
				}
			}

			property System::Windows::Input::ICommand^  EndCommand{
				System::Windows::Input::ICommand^ get()
				{
					return gcnew WoodyChang0611::Common::RelayCommand(
						gcnew System::Action<System::Object^>(this,&MyTestClass::End)
					);
				}
			}
			void Start(System::Object^){
				this->Start();
			}
			void End(System::Object^){
				this->End();
			}

			void Start();
			void End();
			static void ElementCountPropertyChanged(System::Windows::DependencyObject ^ s,System::Windows::DependencyPropertyChangedEventArgs e)
			{
				MyTestClass ^ temp = (MyTestClass^)s;
				temp->graber->SetElementCount(temp->ElementCount);
				temp->ElementCount= temp->graber->GetElementCount();
			}
	
			void UpdateImage(System::Object^ sender, ImageReadyEventArgs^ e)
			{
				//VideoSource = e->Image;
				VideoSource = imageProvider->Image;
			}

			delegate void SetImageSoruceFunc(System::Windows::Media::ImageSource^ value);
			property  System::Windows::Media::ImageSource^ VideoSource{
				virtual System::Windows::Media::ImageSource^ get() { 
					return (System::Windows::Media::ImageSource^) GetValue(VideoSourceProperty);
				};

				virtual void set(System::Windows::Media::ImageSource^ value)
				{
					if (this->Dispatcher->CheckAccess ()){
					}else{
						SetImageSoruceFunc ^ func = gcnew SetImageSoruceFunc(this,&MyTestClass::VideoSource::set);
						this->Dispatcher->Invoke (  
							System::Windows::Threading::DispatcherPriority::Normal,  
							func,value);
						return;
					} 
					SetValue(VideoSourceProperty, value);
				};
			}
			
			static System::Windows::DependencyProperty ^VideoSourceProperty
				= System::Windows::DependencyProperty::Register(
				"VideoSource",
				System::Windows::Media::ImageSource::typeid,
				MyTestClass::typeid);

			property System::Int32 ElementCount
			{

				System::Int32 get() { 
					return (System::Int32) GetValue(ElementCountProperty);
				};
				void set(System::Int32 value)
				{
					SetValue(ElementCountProperty, value);
				};
			}
			static System::Windows::DependencyProperty ^ElementCountProperty
				= System::Windows::DependencyProperty::Register(
				"ElementCount",
				Int32::typeid,
				MyTestClass::typeid,
				gcnew System::Windows::PropertyMetadata( 
				(Int32)(123),
				gcnew  System::Windows::PropertyChangedCallback(&MyTestClass::ElementCountPropertyChanged)));
		};

	}//end of namespace Test
}//end of Namespace Sunlib

namespace WoodyChang0611{
	namespace Test{

		MyTestClass::MyTestClass(){
			woodychang0611::diagnostics::SendInfomation(_T("Test Class constructor"));
			graber= nullptr;
			graber = new woodychang0611::video::DummyFrameGraber();
		}

		MyTestClass::!MyTestClass(){
			woodychang0611::diagnostics::SendInfomation(_T("Test Class destructor"));
			if (graber!=nullptr)  delete graber;
			graber=nullptr;
		}

		void MyTestClass::Start(){
			woodychang0611::diagnostics::SendInfomation(_T("MyTestClass::Start()"));
			imageProvider = gcnew WoodyChang0611::Image::UnmanagedFrameSubjectAdapter();
			imageProvider->SetSubject(IntPtr(graber));
			imageProvider->ImageReady+=
				gcnew ImageReadyEventHandler(this,&MyTestClass::UpdateImage);


			graber->SetElementCount(this->ElementCount);
			graber->Open();
			graber->Play();

		}
		void MyTestClass::End(){
			woodychang0611::diagnostics::SendInfomation(_T("MyTestClass::End()"));
			graber->Stop();
		}
	}//end of namespace Test

}//end of Namespace Sunlib

