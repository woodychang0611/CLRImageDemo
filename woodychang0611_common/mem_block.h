////////////////////////////////////////////////////////////////////////////////////////////////////
// mem_block.h: MemBlock header file
//
// MemBlock is a  library providing dynamic memory block
//
// Note:
// To improve efficiency, memory block doesn't make error check
// ex: MemBlock <int> A(5);   A.data_pointer_[5] = 10; // ERROR! this will result a run-time error

//

#ifndef __WOODYCHANG0611_COMMON_MEM_BLOCK__
#define __WOODYCHANG0611_COMMON_MEM_BLOCK__

#include "base.h"
#include "data_operation.h"


namespace woodychang0611{
// ******************************************************************************************************************
// MemBlock
//
// Purpose
//		Providing dynamic memory block
// Useage
//		MemBlock()
//		MemBlock(size)
// Description of Parameters
//		size	-	The size to be created.
// Return Value
//		None.
// Example
//		MemBlock Smb1 ;
//		MemBlock Smb2(256) ;
//
// Member function
//		Purpose
//			Set new size to MenBlock object.
//		Useage
//			SetSize(size)
//		Description of Parameters
//			size	-	The size to be created.
//		Return Value
//			None.
//		Example
//			MemBlock Smb ;
//			Smb.SetSize(256);  
//
// Member function
//		Purpose
//			Get size of MenBlock object.
//		Useage
//			GtSize()
//		Description of Parameters
//			None.
//		Return Value
//			Size of MenBlock object.
//		Example
//			MemBlock Smb ;
//			Smb.SetSize(256);  
//			UINT32 size = Smb.GetSize() ;
//
// ******************************************************************************************************************





class MemBlock
{
public:
	WOODYCHANG0611_COMMON_API MemBlock(void);
	WOODYCHANG0611_COMMON_API MemBlock(const UINT32 size);
	WOODYCHANG0611_COMMON_API void SetSize(const UINT32 size);
	WOODYCHANG0611_COMMON_API inline UINT32 GetSize(void) const {return size_;} ;
	WOODYCHANG0611_COMMON_API ~MemBlock(void);
	WOODYCHANG0611_COMMON_API MemBlock& operator = (const MemBlock& src);
	WOODYCHANG0611_COMMON_API inline UINT8* GetDataPointer(void){return data_pointer_;};
	WOODYCHANG0611_COMMON_API inline const UINT8* GetDataPointer(void)const{return data_pointer_;};
//	inline WOODYCHANG0611_COMMON_API const UINT8* GetDataPointer(void){return data_pointer_;}const;
private:
	UINT8 * data_pointer_;
	UINT32 size_; //Length of the memory block
};
}//end of namespace woodychang0611

#endif //__WOODYCHANG0611_COMMON_MEM_BLOCK__


