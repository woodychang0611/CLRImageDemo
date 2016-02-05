
#pragma once

#ifndef __WOODYCHANG0611_COMMON_DIAGNOSTIC__
#define __WOODYCHANG0611_COMMON_DIAGNOSTIC__

#include "base.h"


namespace woodychang0611{
	namespace diagnostics{
		enum NOTIFY_TYPE{
			NOTIFY_INFOMATION,
			NOTIFY_WARNING,
			NOTIFY_ERROR
		};
		const int MAX_LOG_FILE_PATH_LENGTH =260; //log file 最長的路徑長度
		typedef void (NotifyOutputFunc) (const TCHAR*, NOTIFY_TYPE type);
		WOODYCHANG0611_COMMON_API NotifyOutputFunc NotifyOutputDefault;
		WOODYCHANG0611_COMMON_API NotifyOutputFunc NotifyOutputMessageBox;
		WOODYCHANG0611_COMMON_API NotifyOutputFunc NotifyOutputWriteLog;
		WOODYCHANG0611_COMMON_API void SetLogFilePath(const TCHAR* path);

		WOODYCHANG0611_COMMON_API extern  NotifyOutputFunc*   NotifyOutput;

		inline void SendNotify (const TCHAR* msg, NOTIFY_TYPE type){
			if(NotifyOutput!=0){
				NotifyOutput(msg,type);
			}else{
				//Default Ouput
				NotifyOutputDefault(msg,type);
			};
		}
		inline void SendInfomation(const TCHAR* msg){
			SendNotify(msg,NOTIFY_INFOMATION);
		}
		inline void SendWarning(const TCHAR* msg){
			SendNotify(msg,NOTIFY_WARNING);
		}
		inline void SendError(const TCHAR* msg){
			SendNotify(msg,NOTIFY_ERROR);
		}

	}
}

#endif //__WOODYCHANG0611_COMMON_DIAGNOSTIC__
