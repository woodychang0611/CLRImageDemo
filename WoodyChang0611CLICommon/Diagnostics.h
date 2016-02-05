// SunvisionCLRCommon.h

#pragma once

using namespace System;

namespace WoodyChang0611 {
	namespace Diagnostics{
		//Class used to trace WPF Binding Error 
		public ref  class BindingErrorTraceListener :   System::Diagnostics::DefaultTraceListener
		{
		private:
			static BindingErrorTraceListener ^ _Listener;
			System::Text::StringBuilder ^ _Message;
			BindingErrorTraceListener();
		public:
			static void SetTrace();
			static void SetTrace(System::Diagnostics::SourceLevels level, System::Diagnostics::TraceOptions options);
			static void CloseTrace();
			virtual void Write(System::String ^message) override;
			virtual void WriteLine(System::String ^message) override;
		};
	};
}
