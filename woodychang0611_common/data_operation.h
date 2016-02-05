////////////////////////////////////////////////////////////////////////////////////////////////
// data_operation.h: woodychang0611_common Library Memory Operation header file
//

//

#ifndef __WOODYCHANG0611_COMMON_DATA_OPERATION__
#define __WOODYCHANG0611_COMMON_DATA_OPERATION__


#include "base.h"

namespace woodychang0611{
	
	enum DataCopyMode{
		DATA_1D2D,	//!<src 1D dst 2D
		DATA_2D1D,	//!<src 2D dst 1D
		DATA_2D2D	//!<src 2D dst 2D
	};

	typedef void (DatCopyFunc) (void*dst,const void*src,UINT32 un32Size);
	typedef  void (DatCopy2DFunc) 
		(void*dst,const void*src,UINT16 un16Len, UINT16 unCnt, 
			UINT16 un16Pitch,DataCopyMode eMode);
	//Return the num of byte where src1 and src2 is the same
	typedef UINT32 (CountSameByteFunc)
		(const void *src1,const void *src2, UINT32 un32Count);
	typedef DatCopyFunc *DatCopyFuncPtr;
	typedef DatCopy2DFunc* DatCopy2DFuncPtr;
	typedef CountSameByteFunc* CountSameByteFuncPtr;
	WOODYCHANG0611_COMMON_API extern DatCopyFunc DatCopy_C; //Gerneral C function for DatCopy
	WOODYCHANG0611_COMMON_API extern DatCopy2DFunc DatCopy2D_C; //Gerneral C function for DatCopy2D
	WOODYCHANG0611_COMMON_API extern CountSameByteFunc CountSameByte_C;
	WOODYCHANG0611_COMMON_API extern CountSameByteFunc CountSameByte_SSE2;
	const DatCopyFuncPtr DatCopy=DatCopy_C;
	const DatCopy2DFuncPtr DatCopy2D = DatCopy2D_C;
#ifdef CPU_SSE2
	const CountSameByteFuncPtr CountSameByte = CountSameByte_SSE2;
#else
	const CountSameByteFuncPtr CountSameByte = CountSameByte_C;
#endif
}//end of namespace woodychang0611

#endif //__WOODYCHANG0611_COMMON_DATA_OPERATION__
