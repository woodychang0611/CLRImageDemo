///////////////////////////////////////////////////////////////////////////////////////////////////
// base.h: woodychang0611_common Gerneral library header file
//



#ifndef __WOODYCHANG0611_COMMON_LIBRARY_BASE_H__

#define __WOODYCHANG0611_COMMON_LIBRARY_BASE_H__
//Under Windows Platform
#ifdef _WIN32
#ifdef  WOODYCHANG0611_COMMON_LIBRARY_EXPORTS

//#define WOODYCHANG0611_COMMON_API __declspec(dllexport)
#define WOODYCHANG0611_COMMON_API

#define WOODYCHANG0611_COMMON_GLOBAL_VARIABLE 
#else
//#define WOODYCHANG0611_COMMON_API __declspec(dllimport)
#define WOODYCHANG0611_COMMON_API 

#define WOODYCHANG0611_COMMON_GLOBAL_VARIABLE extern
#endif //WOODYCHANG0611_COMMON_LIBRARY_EXPORTS

#ifdef WOODYCHANG0611_COMMON_LIBRARY_EXPORTS
#    define EXPIMP_TEMPLATE
#else
#    define EXPIMP_TEMPLATE extern
#endif //WOODYCHANG0611_COMMON_LIBRARY_EXPORTS

#else //NOT _WIN32
#define WOODYCHANG0611_COMMON_API
#    define EXPIMP_TEMPLATE
#define NO_SUPPORT_EXCEPTION
#endif //_WIN32


//CPU instruction support
#define CPU_MMX     /*       mmx : pentiumMMX,k6 */
#define CPU_SSE     /*       sse : pentium3, athlonXP */
#define CPU_SSE2	/*       sse2: pentium4, k8 */

typedef int BOOL;
typedef void *HANDLE;
typedef unsigned long DWORD;
typedef void *LPVOID;

//WOODYCHANG0611_COMMON_API bool __stdcall DllMain( HANDLE hModule, 
//                       DWORD  ul_reason_for_call, 
//                       LPVOID lpReserved
//					 );
//#define TRUE 1
//#define DLL_PROCESS_ATTACH   1    
//#define DLL_THREAD_ATTACH    2    
//#define DLL_THREAD_DETACH    3    
//#define DLL_PROCESS_DETACH   0    
//#define DLL_PROCESS_VERIFIER 4



//Since Different Complier int, double, long have different length 
//Define for the same data length
//Basic data types
#ifdef _WIN32
#include <tchar.h>
#endif

#if defined (_TMS320C6400)
typedef char TCHAR;
#define _T
#endif

namespace woodychang0611{

#ifdef _WIN32
	//Type size for WIN32 Platform
	typedef int BOOL;
	typedef signed char INT8;
	typedef unsigned char UINT8;
	typedef signed short INT16;
	typedef unsigned short UINT16;
	typedef signed int INT32;
	typedef unsigned int UINT32;
	typedef float FLOAT32;
	typedef double FLOAT64;
	typedef long long INT64;
	typedef unsigned long long UINT64;
#endif //_WIN32

#if defined (_TMS320C6400)
	typedef int BOOL;
	typedef signed char INT8;
	typedef unsigned char UINT8;
	typedef signed short INT16;
	typedef unsigned short UINT16;
	typedef signed int INT32;
	typedef unsigned int UINT32;
	typedef float FLOAT32;
	typedef double FLOAT64;
	typedef long long INT64;
	typedef unsigned long long UINT64;
#endif
	//Basic template function

	template <typename T> inline T (min)(const T& a, const T &b){
		return (a<b?a:b);
	}
	template <typename T> inline T (max)(const T& a, const T &b){
		return (a>b?a:b);
	}
	template <typename T> inline void swap(T& a, T &b){
		T temp;
		temp =a;
		a = b;
		b=temp;
	}
	const UINT16 IMAGE_MAX_WIDTH =8000; //!< Maximum image width
	const UINT16 IMAGE_MAX_HEIGHT =8000;//!< Maximum image height
	const UINT16 IMAGE_MORPHOLOGY_MAX_RADIUS =1000;//!< Maximum image morphology radius

} //end of namespace woodychang0611


#if !defined TRUE
#define TRUE 1
#define FALSE 0
#endif

#if !defined NULL
#define NULL 0
#endif


#endif // __WOODYCHANG0611_COMMON_LIBRARY_BASE_H__


#ifdef _WIN32

#ifndef _OBSERVER_SAFE_DETACH_CHECK 
#define _OBSERVER_SAFE_DETACH_CHECK //自動檢查機制
#endif // _OBSERVER_SAFE_DETACH_CHECK 

#endif// _WIN32


