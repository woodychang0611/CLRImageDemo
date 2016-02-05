#ifndef  __WOODYCHANG0611_COMMON_STL_H__
#define __WOODYCHANG0611_COMMON_STL_H__

//disable warnings on extern before template instantiation


#include "base.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#ifdef _WIN32
#pragma warning (disable:4231)
#endif

// Instantiate classes vector<int> and vector<char>
// This does not create an object. It only forces the generation of all
// of the members of classes vector<int> and vector<char>. It exports
// them from the DLL and imports them into the .exe file.

//EXPIMP_TEMPLATE template class WOODYCHANG0611_COMMON_API std::vector<INT8>;
//EXPIMP_TEMPLATE template class WOODYCHANG0611_COMMON_API std::vector<UINT8>;
//EXPIMP_TEMPLATE template class WOODYCHANG0611_COMMON_API std::vector<INT16>;
//EXPIMP_TEMPLATE template class WOODYCHANG0611_COMMON_API std::vector<UINT16>;
//EXPIMP_TEMPLATE template class WOODYCHANG0611_COMMON_API std::vector<INT32>;
//EXPIMP_TEMPLATE template class WOODYCHANG0611_COMMON_API std::vector<UINT32>;
//EXPIMP_TEMPLATE template class WOODYCHANG0611_COMMON_API std::vector<FLOAT32>;
//EXPIMP_TEMPLATE template class WOODYCHANG0611_COMMON_API std::vector<FLOAT64>;
//EXPIMP_TEMPLATE template class WOODYCHANG0611_COMMON_API std::vector<std::vector<std::string>>;
//EXPIMP_TEMPLATE template class WOODYCHANG0611_COMMON_API std::vector<std::string>;

//Unicode Support


namespace std
{
#if defined UNICODE || defined _UNICODE
 
    typedef wstring         tstring;
    typedef wstringbuf      tstringbuf;
    typedef wstringstream   tstringstream;
    typedef wostringstream  tostringstream;
    typedef wistringstream  tistringstream;
 
    typedef wstreambuf      tstreambuf;
 
    typedef wistream        tistream;
    typedef wiostream       tiostream;
 
    typedef wostream        tostream;
 
    typedef wfilebuf        tfilebuf;
    typedef wfstream        tfstream;
    typedef wifstream       tifstream;
    typedef wofstream       tofstream;
 
    typedef wios            tios;
 
#   define tcerr            wcerr
#   define tcin             wcin
#   define tclog            wclog
#   define tcout            wcout
 
#else // defined UNICODE || defined _UNICODE
 
    typedef string          tstring;
 
    typedef stringbuf       tstringbuf;
    typedef stringstream    tstringstream;
    typedef ostringstream   tostringstream;
    typedef istringstream   tistringstream;
 
    typedef streambuf       tstreambuf;
 
    typedef istream         tistream;
    typedef iostream        tiostream;
 
    typedef ostream         tostream;
 
    typedef filebuf         tfilebuf;
    typedef fstream         tfstream;
    typedef ifstream        tifstream;
    typedef ofstream        tofstream;
 
    typedef ios             tios;
 
#   define tcerr            cerr
#   define tcin             cin
#   define tclog            clog
#   define tcout            cout
 
#endif // defined UNICODE || defined _UNICODE
} // namespace std

//template class WOODYCHANG0611_COMMON_API  std::vector <std::tstring>;
//template class WOODYCHANG0611_COMMON_API  std::vector <std::vector <std::tstring> >;


#endif

