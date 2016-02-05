#include <string.h> // for memcpy
#include "data_operation.h"
namespace woodychang0611{

	void DatCopy_C (void*dst,const void*src,UINT32 size){
		memcpy(dst,src,size);
	}


	void DatCopy2D_C
		(void*dst,const void*src,UINT16 len,UINT16 count, 
			UINT16 pitch,DataCopyMode mode)
	{
		UINT32 readOffset=0;
		UINT32 writeOffset=0;
		for (UINT16 i=0;i<count;i++){
			memcpy((UINT8*)dst+writeOffset,(UINT8*)src+readOffset,len);
			switch(mode){
			case DATA_1D2D:
				readOffset+=len;
				writeOffset+=pitch;
				break;
			case DATA_2D1D:
				readOffset+=pitch;
				writeOffset+=len;
				break;
			case DATA_2D2D:
				readOffset+=pitch;
				writeOffset+=pitch;
				break;
			default:
				break;
			}
		}
	}
	UINT32 CountSameByte_C (const void *src1,const void *src2, UINT32 length){
		UINT8 * ptr1 = (UINT8*)src1;
		UINT8 * ptr2 = (UINT8*)src2;
		UINT32 result=0;
		for (UINT32 i=0;i<length;i++){
			if(ptr1[i]==ptr2[i]) result++;
		}
		return result;
	}

	#ifdef _WIN32
		UINT32 CountSameByte_SSE2 (const void *src1,const void *src2, UINT32 length){
		UINT8 * ptr1 = (UINT8*)src1;
		UINT8 * ptr2 = (UINT8*)src2;
		UINT64 result=0;
		UINT32 loopCount = length/8; //Compare 8Bytes once
		__asm{
			mov esi,[ptr1];
			mov edi,[ptr2];
			pxor mm2,mm2;
			pxor mm3,mm3;
			mov ecx,loopCount;
L1:
			movq mm0,[esi]; //Data1
			movq mm1,[edi]; //Data2
			add esi,8h;
			add edi,8h;
			pcmpeqb mm0,mm1; //Compare
			pxor mm2,mm2; //Clear mm2
			PSADBW mm2,mm0; //Add Difference
			paddq mm3,mm2; //Add mm2 to mm3 sse2 Instructions
			Loop L1;
			movq result,mm3;
			emms;
	}
	result = result/0xFF;
		//Compare unaligned bytes
		if(length%8!=0){
			for(UINT32 i=0;i<loopCount%8;i++){
				if(ptr1[((length/8)*8)+i]==ptr2[(length/8)*8]) result++;		
			}
		}
		return (UINT32) result;
	}
	#endif //_WIN32
}//end of namespace woodychang0611
