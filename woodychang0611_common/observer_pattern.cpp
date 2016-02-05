#include "observer_pattern.h"
#include "diagnostics.h"
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif 



namespace woodychang0611{
	namespace design_patterns{

#ifdef _OBSERVER_SAFE_DETACH_CHECK
const UINT64 OBSERVER_DETACH_SLEEP_TIME=1000;
const UINT32 OBSERVER_DETACH_SLEEP_COUNT=100;
#include <windows.h>

#endif //_OBSERVER_SAFE_DETACH_CHECK

		Observer::Observer()
		{
			_notified_count =0;
			_observer_state = OBSERVER_ON;
#ifdef _OBSERVER_SAFE_DETACH_CHECK
			_critical_section = (void*) new CRITICAL_SECTION;
			InitializeCriticalSection((CRITICAL_SECTION*)_critical_section);
#endif //_OBSERVER_SAFE_DETACH_CHECK
		};

		Observer::~Observer()
		{
			WaitNotifiedEnd();
			DetachAllSubject();		
		

#ifdef _OBSERVER_SAFE_DETACH_CHECK
			DeleteCriticalSection((CRITICAL_SECTION*)_critical_section);
			delete _critical_section;
#endif //_OBSERVER_SAFE_DETACH_CHECK
			}

		void Observer::_AttachSubject(Subject* s)
		{
			//avoid attach twice
			std::vector<Subject*>::iterator new_end;
			new_end = std::remove(_subjects.begin(),_subjects.end(),(Subject*)s);
			_subjects.erase(new_end,_subjects.end());
			_subjects.push_back(s);
		}
		void Observer::_DetachSubject(Subject* s)
		{
			std::vector<Subject*>::iterator new_end;
			new_end = std::remove(_subjects.begin(),_subjects.end(),(Subject*)s);
			_subjects.erase(new_end,_subjects.end());
		}

		bool Observer::On(bool state)
		{
			if(state)
				return SetObserverState(OBSERVER_ON);
			else
				return Off();
		};
		bool Observer::Off(bool state)
		{
			if(state)
				return SetObserverState(OBSERVER_OFF);
			else
				return On();
		};
		bool Observer::IsOn()const
		{
			return _observer_state==OBSERVER_ON;
		};
		bool Observer::IsOff()const
		{
			return _observer_state==OBSERVER_OFF;
		};
		bool Observer::SetObserverState(const ObserverState state)
		{
			_observer_state = state;
			return true;
		};
		ObserverState Observer::GetObserverState()const
		{
			return _observer_state;
		};

		UINT32 Observer::GetSubjectCount()
		{
			return _subjects.size();
		};
		
		void Observer::AttachSubject(Subject* subject)
		{
			subject->AttachObserver(this);
		};
		void Observer::DetachSubject(Subject* subject)
		{
			subject->DetachObserver(this);
		};
		void Observer::DetachAllSubject()
		{
			while(_subjects.size() >0){
				_subjects[0]->DetachObserver(this);
			}
		};

		void Observer::WaitNotifiedEnd()
		{
			Off();

#ifdef _OBSERVER_SAFE_DETACH_CHECK
			
			UINT32 sleep_count=0;
			while(_notified_count!=0){
				OutputDebugString(_T("Observer WaitNotifiedEnd い.....\n"));
				Sleep( OBSERVER_DETACH_SLEEP_TIME );
				sleep_count++;
				if(sleep_count>OBSERVER_DETACH_SLEEP_COUNT){//单筁
					OutputDebugString(_T("Observer WaitNotifiedEnd 单び\n"));
					MessageBox(0,_T("Observer WaitNotifiedEnd 单び\n"),0,0);
					break;
				}
			}
			OutputDebugString(_T("observer WaitNotifiedEnd挡!\n"));
	
#endif //_OBSERVER_SAFE_DETACH_CHECK
		};


		void Subject::AttachObserver (Observer* o){
			//avoid attach twice
			std::vector<Observer*>::iterator new_end;
			new_end = std::remove(_observers.begin(),_observers.end(),(Observer*)o);
			_observers.erase(new_end,_observers.end());
			_observers.push_back(o);
			o->_AttachSubject(this);
		}
		void Subject::DetachObserver (Observer* o){
			std::vector<Observer*>::iterator new_end;
			new_end = std::remove(_observers.begin(),_observers.end(),(Observer*)o);
			_observers.erase(new_end,_observers.end());
			o->_DetachSubject(this);
		}
		void Subject::DetachAllObserver(){
			while(_observers.size() >0){
				DetachObserver(_observers[0]);
			}
		};
		void Subject::WaitNotifyEnd()
		{
#ifdef _OBSERVER_SAFE_DETACH_CHECK
			_subject_notify=false;
			UINT32 sleep_count=0;
			while(_notify_count!=0){
				OutputDebugString(_T("Subject WaitNotifyEnd い.....\n"));
				Sleep( OBSERVER_DETACH_SLEEP_TIME );
				sleep_count++;
				if(sleep_count>OBSERVER_DETACH_SLEEP_COUNT){//单筁
					OutputDebugString(_T("Subject WaitNotifyEnd 单び\n"));
					MessageBox(0,_T("Subject WaitNotifyEnd 单び\n"),0,0);
					break;
				}
			}
			OutputDebugString(_T("Subject WaitNotifyEnd挡!\n"));			
#endif //_OBSERVER_SAFE_DETACH_CHECK
		};

		UINT32 Subject::GetObserverCount()
		{
			UINT32 observer_count = 0;
			for(UINT32 i=0;i<_observers.size();i++){
				if(_observers[i]->IsOn())
					observer_count++;
			}
			return observer_count;
		};

		void Subject::NotifyObserver(void)
		{
#ifdef _OBSERVER_SAFE_DETACH_CHECK
			if(_subject_notify)
			{
				EnterCriticalSection((CRITICAL_SECTION*)_subject_critical_section);
				_notify_count++;
				LeaveCriticalSection((CRITICAL_SECTION*)_subject_critical_section);
#endif //_OBSERVER_SAFE_DETACH_CHECK

				const std::vector <Observer*>observers_copy=_observers;
				for (UINT32 i=0;i<observers_copy.size();++i){
					if(observers_copy[i]->IsOn()){

	#ifdef _OBSERVER_SAFE_DETACH_CHECK
						EnterCriticalSection((CRITICAL_SECTION*)observers_copy[i]->_critical_section);
						observers_copy[i]->_notified_count++;
						LeaveCriticalSection((CRITICAL_SECTION*)observers_copy[i]->_critical_section);
	#endif //_OBSERVER_SAFE_DETACH_CHECK

						observers_copy[i]->UpdateSubject(this);

	#ifdef _OBSERVER_SAFE_DETACH_CHECK
						EnterCriticalSection((CRITICAL_SECTION*)observers_copy[i]->_critical_section);
						observers_copy[i]->_notified_count--;
						LeaveCriticalSection((CRITICAL_SECTION*)observers_copy[i]->_critical_section);
	#endif //_OBSERVER_SAFE_DETACH_CHECK
					}
				}

#ifdef _OBSERVER_SAFE_DETACH_CHECK
			
				EnterCriticalSection((CRITICAL_SECTION*)_subject_critical_section);
				_notify_count--;
				LeaveCriticalSection((CRITICAL_SECTION*)_subject_critical_section);
			}
#endif //_OBSERVER_SAFE_DETACH_CHECK
		}

		void Subject::Destory()
		{
			for (UINT32 i=0;i<_observers.size();++i)
			{
				_observers[i]->SubjectClosed(this);
				_observers[i]->_DetachSubject(this);
			}
		}

		Subject::Subject()
		{

#ifdef _OBSERVER_SAFE_DETACH_CHECK
			_subject_notify=true;
			_notify_count=0;
			_subject_critical_section=(void*) new CRITICAL_SECTION;;
			InitializeCriticalSection((CRITICAL_SECTION*)_subject_critical_section);
#endif //_OBSERVER_SAFE_DETACH_CHECK

		};

		Subject::~Subject()
		{
			this->Destory();
			WaitNotifyEnd();

#ifdef _OBSERVER_SAFE_DETACH_CHECK			
			DeleteCriticalSection((CRITICAL_SECTION*)_subject_critical_section);
			delete _subject_critical_section;
#endif //_OBSERVER_SAFE_DETACH_CHECK
		}
	}//end of 	namespace design_patterns
}//end of namespace woodychang0611
