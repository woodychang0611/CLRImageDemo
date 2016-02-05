#include "diagnostics.h"

#ifdef _WIN32
#include <time.h>
#include <Windows.h>
#include <stdio.h>
namespace woodychang0611{
	namespace diagnostics{
		NotifyOutputFunc*   NotifyOutput;
		TCHAR LogFilePath[MAX_LOG_FILE_PATH_LENGTH];
		//注意 預設檔名長度不可超過MAX_LOG_FILE_PATH_LENGTH
		const TCHAR *DefaultLogFilePath = _T("default.log");
		void SetLogFilePath(const TCHAR* path){
			//如果長度過長 使用預設值
			if (_tcslen(path) > MAX_LOG_FILE_PATH_LENGTH && _tcscmp(LogFilePath,DefaultLogFilePath)!=0){
				TCHAR * str = new TCHAR[_tcslen(path)+100];
				_stprintf_s(str,_tcslen(path)+100,_T("Log file \"%s\"has %d characters is longer than limit %d"),path,_tcslen(path),MAX_LOG_FILE_PATH_LENGTH);
				SetLogFilePath(DefaultLogFilePath);
				woodychang0611::diagnostics::SendError(str);
				delete []str;
			}else{
				_stprintf_s(LogFilePath,_T("%s"),path);
			}
		}

		void NotifyOutputDefault(const TCHAR* msg, NOTIFY_TYPE type){
			NotifyOutputWriteLog(msg,type);
		}
		void NotifyOutputMessageBox(const TCHAR* msg,NOTIFY_TYPE type){
			TCHAR str[256];
			switch (type){
			case NOTIFY_ERROR:
				_stprintf_s(str,_T("%s:%s"),_T("Error"),msg);
				MessageBox(0,str,0,MB_ICONERROR);
				break;
			case NOTIFY_INFOMATION:
				_stprintf_s(str,_T("%s:%s"),_T("Infomation"),msg);
				MessageBox(0,str,0,MB_ICONINFORMATION);
				break;
			case NOTIFY_WARNING:
				_stprintf_s(str,_T("%s:%s"),_T("Warning"),msg);
				MessageBox(0,str,0,MB_ICONWARNING);
				break;
			default:
				break;
			}
			OutputDebugString(str);
		}

		void NotifyOutputWriteLog(const TCHAR* msg,NOTIFY_TYPE type){
			//檢查LogFilePath是否是空的
			//全域變數未設定時 會是空的
			if(_tcslen(LogFilePath)==0){
				_stprintf_s(LogFilePath,_T("%s"),DefaultLogFilePath);
			}
			FILE * log_file;
			//未完成 先測試檔案是否存在 再決定用a或是w
			if(_tfopen_s (&log_file,LogFilePath,_T("r, ccs= UNICODE"))==0){
				if(log_file!=NULL) fclose(log_file);
				_tfopen_s (&log_file,LogFilePath,_T("a+, ccs= UNICODE"));
			}else{
				_tfopen_s (&log_file,LogFilePath,_T("w, ccs= UNICODE"));
			}
			//使用非預設檔名開檔失敗 改使用預設檔名
			if(log_file==NULL && _tcscmp(LogFilePath,DefaultLogFilePath)!=0){
				TCHAR * str = new TCHAR[_tcslen(LogFilePath)+100];
				_stprintf_s(str,_tcslen(LogFilePath)+100,_T("Log file \"%s\" open failed use default path \"%s\" instead"),LogFilePath,DefaultLogFilePath);
				SetLogFilePath(DefaultLogFilePath);
				SendError(str);
				//重送訊息
				NotifyOutputWriteLog(msg,type);
				//MessageBox(0,str,0,MB_ICONERROR);
				delete []str;
			}


			TCHAR type_name[20];
			switch (type){
				case NOTIFY_ERROR:
					 _stprintf_s(type_name,_T("%s"),_T("Error\t"));
					break;
				case NOTIFY_INFOMATION:
					_stprintf_s(type_name,_T("%s"),_T("Infomation"));
					break;
				case NOTIFY_WARNING:
					_stprintf_s(type_name,_T("%s"),_T("Warning"));
					break;
				default:
					break;
			}
			if(log_file!=NULL){
				TCHAR date[9],time[9]; //放日期和時間
				_tstrdate_s(date);
				_tstrtime_s(time);
				_ftprintf_s(log_file,_T("%s %s %s\t%s\n"),date,time,type_name,msg);
			}
			if(log_file!=NULL) fclose(log_file);			
		}
	}
}

#else //end of _WIN32

namespace woodychang0611{
	namespace exception{


		NotifyOutputFunc*   NotifyOutput;
		void NotifyOutputDefault(TCHAR* msg, NOTIFY_TYPE type){
			// For not Win32 platform 
		}
	}

}
#endif 










