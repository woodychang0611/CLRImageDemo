// 這是主要 DLL 檔案。

#include "stdafx.h"
#include "..\woodychang0611_common\woodychang0611_common.h"
#include "Diagnostics.h"
#include<vcclr.h>
#using <System.dll>
#using <WindowsBase.dll>
#using <PresentationFramework.dll>

using namespace System::Diagnostics;
using namespace System::Windows;
using namespace System;



namespace WoodyChang0611 {
	namespace Diagnostics{
		void BindingErrorTraceListener::SetTrace()
		{ SetTrace(SourceLevels::Error, TraceOptions::None ); }

		void BindingErrorTraceListener::SetTrace(SourceLevels level, TraceOptions options)
		{
			if (_Listener == nullptr)
			{
				_Listener = gcnew BindingErrorTraceListener();
				PresentationTraceSources::DataBindingSource->Listeners->Add(_Listener);
			}

			_Listener->TraceOutputOptions = options;
			PresentationTraceSources::DataBindingSource->Switch->Level = level;
		}

		void BindingErrorTraceListener::CloseTrace()
		{
			if (_Listener == nullptr)
			{ return; }

			_Listener->Flush();
			_Listener->Close();
			PresentationTraceSources::DataBindingSource->Listeners->Remove(_Listener);
			_Listener = nullptr;
		}





		BindingErrorTraceListener::BindingErrorTraceListener()
		{ 
			_Message = gcnew System::Text::StringBuilder();
		}

		void BindingErrorTraceListener::Write(System::String ^message)
		{ _Message->Append(message); }

		void BindingErrorTraceListener::WriteLine(System::String ^message)
		{
			_Message->Append(message);

			String^ final = _Message->ToString();
			_Message->Length = 0;

			//System::Windows::MessageBox::Show(final, "Binding Error", MessageBoxButton::OK,
			//	MessageBoxImage::Error);
			pin_ptr<const TCHAR> ptr  = PtrToStringChars(final);
			woodychang0611::diagnostics::SendInfomation(ptr);
		}
	}
}