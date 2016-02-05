#include "base.h"
#include "stl.h"



#ifndef __WOODYCHANG0611_COMMON_OBSERVER_PATTERN__
#define __WOODYCHANG0611_COMMON_OBSERVER_PATTERN__

#ifdef _OBSERVER_SAFE_DETACH_CHECK

//#include <windows.h>

#endif //_OBSERVER_SAFE_DETACH_CHECK

namespace woodychang0611{
	namespace design_patterns{

		class Subject;
		class Observer;

#ifdef _WIN32
	template class WOODYCHANG0611_COMMON_API std::allocator <Observer*>;
	template class WOODYCHANG0611_COMMON_API std::allocator <Subject*>;
	template class WOODYCHANG0611_COMMON_API std::vector <Observer*>;
	template class WOODYCHANG0611_COMMON_API std::vector <Subject*>;
#endif 


		enum ObserverState{
			OBSERVER_ON,
			OBSERVER_OFF
		};

		class Observer{
		public:
			WOODYCHANG0611_COMMON_API Observer();
			WOODYCHANG0611_COMMON_API virtual ~Observer();

			WOODYCHANG0611_COMMON_API bool On(bool state = true);
			WOODYCHANG0611_COMMON_API bool Off(bool state = true);
			WOODYCHANG0611_COMMON_API bool IsOn()const;
			WOODYCHANG0611_COMMON_API bool IsOff()const;
			WOODYCHANG0611_COMMON_API virtual bool SetObserverState(const ObserverState state);//實作
			WOODYCHANG0611_COMMON_API ObserverState GetObserverState()const;
			WOODYCHANG0611_COMMON_API UINT32 GetSubjectCount();
			WOODYCHANG0611_COMMON_API virtual void AttachSubject(Subject*);
			WOODYCHANG0611_COMMON_API virtual void DetachSubject(Subject*);
			WOODYCHANG0611_COMMON_API virtual void DetachAllSubject();
			WOODYCHANG0611_COMMON_API void WaitNotifiedEnd();
		protected:
			virtual void SubjectClosed(woodychang0611::design_patterns::Subject *the_closed_subject)=0;
			virtual void UpdateSubject(woodychang0611::design_patterns::Subject *the_changed_subject)=0;//subject呼叫後的執行工作
			ObserverState _observer_state;
			friend class Subject;
		private:
			WOODYCHANG0611_COMMON_API void _AttachSubject(Subject*);
			WOODYCHANG0611_COMMON_API void _DetachSubject(Subject*);

			std::vector <Subject*> _subjects;
			UINT32 _notified_count;

#ifdef _OBSERVER_SAFE_DETACH_CHECK
			void* _critical_section;
#endif //_OBSERVER_SAFE_DETACH_CHECK
		};

		class Subject {
		public:
			WOODYCHANG0611_COMMON_API  void Destory();
			WOODYCHANG0611_COMMON_API virtual ~Subject();
			WOODYCHANG0611_COMMON_API void AttachObserver(Observer*);
			WOODYCHANG0611_COMMON_API void DetachObserver(Observer*);
			WOODYCHANG0611_COMMON_API void DetachAllObserver();
			WOODYCHANG0611_COMMON_API void WaitNotifyEnd();
			WOODYCHANG0611_COMMON_API UINT32 GetObserverCount();
		protected:
			WOODYCHANG0611_COMMON_API void NotifyObserver(void);
			WOODYCHANG0611_COMMON_API Subject();
		private:
			std::vector <Observer*> _observers;

#ifdef _OBSERVER_SAFE_DETACH_CHECK
			bool _subject_notify;
			UINT32 _notify_count;
			void* _subject_critical_section;
#endif //_OBSERVER_SAFE_DETACH_CHECK


		};

	}//end of 	namespace design_patterns
}//end of namespace woodychang0611

#endif //end of #ifndef __WOODYCHANG0611_COMMON_OBSERVER_PATTERN__
