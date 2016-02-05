
#include "windows_io.h"
#include "mem_block.h"
using std::vector;
using std::tstring;
namespace woodychang0611{
	namespace win{
		
			void ShowImage(HDC hdc,INT32 left, INT32 top, INT32 width, INT32 height,const woodychang0611::image::ImageRGB24& image){
				SetStretchBltMode(hdc,COLORONCOLOR);
					if(!image.IsValid()) {
						return;
				}
				void *bitmap_bits = (void*)image.GetDataPointer();
				BITMAPINFO bitmap_info;
				BITMAPINFO * bitmap_info_pointer = &bitmap_info;
				bitmap_info.bmiHeader.biBitCount=0;
				bitmap_info.bmiHeader.biSize = 40;
				bitmap_info.bmiHeader.biWidth = image.GetImageWidth();
				bitmap_info.bmiHeader.biHeight = -image.GetImageHeight(); 
				bitmap_info.bmiHeader.biPlanes = 1;
				bitmap_info.bmiHeader.biBitCount = image.GetBytePerPixel() *8;
				bitmap_info.bmiHeader.biCompression = 0;
				bitmap_info.bmiHeader.biSizeImage =image.GetImageByteCount();
				bitmap_info.bmiHeader.biXPelsPerMeter = 0;
				bitmap_info.bmiHeader.biYPelsPerMeter = 0;
				bitmap_info.bmiHeader.biClrUsed = 0;
				bitmap_info.bmiHeader.biClrImportant = 0;
				// Special Process for image if width is not multiple of 4
				// Bitmap must have multiple of 4 bytes per line
				MemBlock dst_mem;
				if(image.GetImageWidth()%4!=0)
				{
					if(!EncodeBMP(image,dst_mem)) return;
					bitmap_bits = dst_mem.GetDataPointer()+54 ;
					bitmap_info_pointer =(BITMAPINFO*) (dst_mem.GetDataPointer()+14);
				}

				StretchDIBits(
				hdc, 
				left, 
				top, 
				width, 
				height,
				0, 
				0, 
				image.GetImageWidth(),
				image.GetImageHeight(),
				bitmap_bits,
				bitmap_info_pointer,
				DIB_RGB_COLORS, 
				SRCCOPY);
			}
	
		int ReadFile(const TCHAR* filePath,MemBlock& pMemBlock){

			HANDLE hFile = CreateFile((TCHAR*) filePath, GENERIC_READ, FILE_SHARE_READ, NULL,
										OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (hFile != INVALID_HANDLE_VALUE){
				//Maximum File Size 4GB (2^32);
				UINT32 un32FileSize;
				UINT32 un32NumberOfBytesRead ;
				un32FileSize =GetFileSize(hFile,NULL);
				if(un32FileSize){
					pMemBlock.SetSize(un32FileSize);
				}
				BOOL ret = ::ReadFile(hFile,(LPVOID) pMemBlock.GetDataPointer(), un32FileSize,(LPDWORD) &un32NumberOfBytesRead,0);
				//
				if(ret==0) woodychang0611::diagnostics::SendWarning(_T("File Read Failed!"));
				CloseHandle(hFile);// Close the file
				return 0;
			}
			return -1;
		}

		int WriteFile(const TCHAR* filePath,const MemBlock& pMemBlock){
			HANDLE hFile = CreateFile((TCHAR*) filePath, GENERIC_WRITE, FILE_SHARE_READ, NULL,
										CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			UINT32 un32NumberOfBytesWritten;
			if (hFile != INVALID_HANDLE_VALUE){
				// Write the bitmap header and bitmap bits to the file
				BOOL ret = ::WriteFile(hFile, (LPCVOID) pMemBlock.GetDataPointer(), pMemBlock.GetSize(), (LPDWORD)&un32NumberOfBytesWritten, 0);
				if(ret==0) woodychang0611::diagnostics::SendWarning(_T("File Write Failed!"));
				CloseHandle(hFile);// Close the file
			}
			return 0;
		}
		int ReadData(const TCHAR* strFileName, void* data_, const int len_)
		{
			HANDLE hFile = CreateFile((TCHAR*) strFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
									OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (hFile != INVALID_HANDLE_VALUE){
				UINT32 un32NumberOfBytesRead ;
				BOOL ret = ::ReadFile(hFile,(LPVOID) data_, len_,(LPDWORD) &un32NumberOfBytesRead,0);
				if(ret==0) woodychang0611::diagnostics::SendWarning(_T("ReadData Failed!"));
				CloseHandle(hFile);// Close the file
				return 0;
			}
			return -1;
		}

		int WriteData(const TCHAR* filePath, const void* data_, const int len_){
			HANDLE hFile = CreateFile((TCHAR*) filePath, GENERIC_WRITE, FILE_SHARE_READ, NULL,
									CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (hFile != INVALID_HANDLE_VALUE){
				UINT32 un32NumberOfBytesWritten;
				::WriteFile(hFile, (LPCVOID) data_, len_, (LPDWORD)&un32NumberOfBytesWritten, 0);
				CloseHandle(hFile);// Close the file
			}
			return 0;
		}

		int ReadImageFile(const TCHAR* filePath,vector <vector <tstring>>& dst)
		{
			dst.clear();
			TCHAR line[1000];
			FILE *file;
			_tfopen_s(&file,(TCHAR*)filePath,_T("r"));
//			_tfopen_s(&file,(TCHAR*)strPathName.c_str(),_T("r"));
			if (file!=NULL){
				while(_fgetts(line,1000,file)!=NULL){
					if (line[0] == ';') //此行為註解
						continue;

					vector <tstring> vs;
					TCHAR seps[]   = _T(" \n\t,");
					TCHAR *token1 = NULL;
					TCHAR *next_token1 = NULL;
					token1 = _tcstok_s(line, seps, &next_token1);
					bool bHasElement = false;
					while ((token1 != NULL))
					{
						// Get next token:
						tstring str = token1;
						vs.push_back(str);
						token1 = _tcstok_s( NULL, seps, &next_token1);
						bHasElement = true;
					}
					if (bHasElement)
						dst.push_back(vs);
				}
				fclose(file);
			}			
			return 0;
		}

		//! \bug Problem with Unicode
		int ReadCSVFile(const TCHAR* filePath,vector <vector <tstring>>& dst){
			dst.clear();
			TCHAR line[1000];
			FILE *file;
//			file= _tfopen((TCHAR*)filePath,_T("r"));
			_tfopen_s(&file,(TCHAR*)filePath,_T("r"));
			if (file!=NULL){
				while(_fgetts(line,1000,file)!=NULL){
					vector <tstring> vs;
					TCHAR seps[]   = _T("\n\t,");
					TCHAR *token1 = NULL;
					TCHAR *next_token1 = NULL;
					token1 = _tcstok_s(line, seps, &next_token1);
					while ((token1 != NULL))
					{
						// Get next token:
						tstring str = token1;
						vs.push_back(str);
						token1 = _tcstok_s( NULL, seps, &next_token1);
					}
					dst.push_back(vs);
				}
				fclose(file);
			}
			return 0;
		}

		//int WriteCSVFile(const std::tstring &strPathName,const std::vector <std::vector <woodychang0611::FLOAT64> >&src)
		//{
		//	vector <vector <tstring>> temp;
		//	temp.resize(src.size());
		//	TCHAR buff[1000];
		//	for(UINT32 i=0;i<src.size();i++)
		//	{
		//		temp[i].resize(src[i].size());
		//		for(UINT32 j=0;j<src[i].size();j++)
		//		{
		//			_stprintf(buff,_T("%f"),src[i][j]);
		//			temp[i][j]=std::tstring(buff);
		//		}
		//	}
		//	return WriteCSVFile(strPathName,temp);
		//};
		//! \bug Problem with Unicode
		//int WriteCSVFile(const std::tstring &strPathName,const vector <vector <tstring>>&src){
		//	FILE *file;
		//	_tfopen_s(&file, (TCHAR*)strPathName.c_str(),_T("w"));	
		//	if(file !=NULL) fclose(file);
		//	for (UINT32 i=0;i<src.size();i++){
		//		WriteCSVFile(strPathName,src[i]);
		//	}
		//	return 0;
		//}

		//! \bug Problem with Unicode
		//int WriteCSVFile(const std::tstring &strPathName,const vector <tstring>&src){
		//	FILE *file;
		//	_tfopen_s(&file, (TCHAR*)strPathName.c_str(),_T("a"));
		//	if (file!=NULL){
		//		tstring str;
		//		for (UINT32 i=0;i<src.size();i++){
		//			str = str +src[i]+_T(",");
		//		}
		//		str = str+_T("\n");
		//		_fputts((TCHAR*)str.c_str(),file);
		//		fclose(file);
		//	}
		//	return 0;
		//}

		int ReadBMPFile(const TCHAR* filePath,woodychang0611::image::ImageRGB24 &dst){
			MemBlock mem;
			if (ReadFile(filePath,mem)>=0)
				DecodeBMP(dst,mem);
			return 0;
		}
		
		int WriteBMPFile(const TCHAR* filePath,const woodychang0611::image::ImageRGB24 &src){
			MemBlock mem;
			EncodeBMP(src,mem);
			WriteFile(filePath,mem);
			return 0;
		}

		std::string toNarrowString( const wchar_t* pStr , int len )
		{
		     //ASSERT_PTR( pStr ) ;
		     //ASSERT( len >= 0 || len == -1 , _T("Invalid string length: ") << len ) ;
		     // figure out how many narrow characters we are going to get
		     int nChars = WideCharToMultiByte( CP_ACP , 0 ,
			      pStr , len , NULL , 0 , NULL , NULL ) ;
		     if ( len == -1 )
			-- nChars ;
		     if ( nChars == 0 )
			return "" ;
		    // convert the wide string to a narrow string
		    // nb: slightly naughty to write directly into the string like this
		    std::string buf ;
		    buf.resize( nChars ) ;
		    WideCharToMultiByte( CP_ACP , 0 , pStr , len ,
			   const_cast<char*>(buf.c_str()) , nChars , NULL , NULL ) ;
		    return buf ;
		}

	}//namespace win
}//end of namespace woodychang0611
