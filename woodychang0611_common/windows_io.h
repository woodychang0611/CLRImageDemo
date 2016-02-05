////////////////////////////////////////////////////////////////////////////////////////////////////
// WindowsIO.h: WindowsIO header file
//
// WindowsIO is a  library providing IO between woodychang0611_common and Microsoft Windows API
//


#ifdef _WIN32 //windows platform only

#ifndef __WOODYCHANG0611_COMMON_WINDOWS_IO_H__
#define __WOODYCHANG0611_COMMON_WINDOWS_IO_H__
#include "windows.h"
#include "tchar.h"
#include "image_operation.h"
#include "stl.h"

// ******************************************************************************************************************
// ShowImg
//
// Purpose
//		Drawing image to screen. 
// Useage
//		ShowImg(hdc,n32Left, n32Top, n32Width, n32Height,src_image)
// Description of Parameters
//		hdc			-	Handle to a screen device context 
//		n32Left		-	X coordinate of TopLeft point
//		n32Top		-	Y coordinate of TopLeft point
//		n32Width	-	Img width to drawing
//		n32Height	-	Img height to drawing
//		src_image		-	Img source to drawing
// Return Value
//		None.
// Example
//		HWND hWnd = GetSafeHwnd();
//		HDC hdc = ::GetDC( hWnd );
//		MemBlock mem;
//		ReadFile("A.bmp",mem);
//		DecodeBMP(Simg,mem);
//		ShowImg(hdc,0 ,0,512,512,Simg);
//
//
//
// ReadFile
//
// Purpose
//		Read bmp file to memory block we defined.
// Useage
//		ReadFile(pszPathName,pMemBlock)
// Description of Parameters
//		pszPathName		-	File path name
//		pMemBlock	-	Memory block to save
// Return Value
//		1 if success.
//		0 if invalid file path.
// Example
//		MemBlock mem;
//		ReadFile("A.bmp",mem);
//
//
// WriteFile
//
// Purpose
//		Write memory data to bmp file .
// Useage
//		WriteFile(pszPathName,pMemBlock)
// Description of Parameters
//		pszPathName		-	Destination file path name to write
//		pMemBlock	-	Memory block source
// Return Value
//		1 if success.
//		0 if invalid file path.
// Example
//		MemBlock mem;
//		ReadFile("A.bmp",mem);
//		WriteFile("B.bmp",mem);
//
// ******************************************************************************************************************


namespace woodychang0611{
	namespace win{
	WOODYCHANG0611_COMMON_API void ShowImage(HDC hdc,INT32 i32Left, INT32 i32Top, INT32 i32Width, INT32 i32Height,const woodychang0611::image::ImageRGB24& image);
	WOODYCHANG0611_COMMON_API int ReadFile(const TCHAR* strPathName,MemBlock& pMemBlock);
	WOODYCHANG0611_COMMON_API int WriteFile(const TCHAR* strPathName,const MemBlock& pMemBlock);
	WOODYCHANG0611_COMMON_API int ReadData(const TCHAR* strFileName, void* data_, const int len_);
	WOODYCHANG0611_COMMON_API int WriteData(const TCHAR* strFileName, const void* data_, const int len_);
	WOODYCHANG0611_COMMON_API int ReadImageFile(const TCHAR* strFileName,std::vector <std::vector <std::tstring> >& dst);
//«Ý¬å
//	WOODYCHANG0611_COMMON_API int ReadCSVFile(const std::tstring &strPathName,std::vector <std::vector <std::tstring> >& dst);
//	WOODYCHANG0611_COMMON_API int WriteCSVFile(const std::tstring &strPathName,const std::vector <std::vector <woodychang0611::FLOAT64> >&src);
//	WOODYCHANG0611_COMMON_API int WriteCSVFile(const std::tstring &strPathName,const std::vector <std::vector <std::tstring> >&src);
//	WOODYCHANG0611_COMMON_API int WriteCSVFile(const std::tstring  &strPathName,const std::vector <std::tstring>&src);
	WOODYCHANG0611_COMMON_API int ReadBMPFile(const TCHAR* strFileName,woodychang0611::image::ImageRGB24 &dst);
	WOODYCHANG0611_COMMON_API int WriteBMPFile(const TCHAR* strFileName,const woodychang0611::image::ImageRGB24 &src);
	WOODYCHANG0611_COMMON_API std::string toNarrowString( const wchar_t* pStr , int len );
	}//namespace win
}//end of namespace woodychang0611


#endif //__WOODYCHANG0611_COMMON_WINDOWS_IO_H__
#endif //_WIN32

