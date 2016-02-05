////////////////////////////////////////////////////////////////////////////////////////////////////
// Array.h: MemBlock header file
//
// MemBlock is a  library providing dynamic memory block
//
// Note:
// To improve efficiency, memory block doesn't make error check
// ex: MemBlock <int> A(5);   A.data_pointer_[5] = 10; // ERROR! this will result a run time error

//
#include "mem_block.h"



namespace woodychang0611{

MemBlock::MemBlock(void)
{
	data_pointer_=NULL;
	size_=0;
}

MemBlock::MemBlock(const UINT32 size)
{
	size_=0;
	data_pointer_=NULL;
	SetSize(size);
}
void MemBlock::SetSize(const UINT32 size)
{
	if(size_!=size){
		size_=size;
		if(data_pointer_) delete data_pointer_;
		if(size_)
			data_pointer_=new UINT8[size];
		else //size_ ==0
			data_pointer_ = NULL;
	}
}

MemBlock::~MemBlock(void)
{
	if(data_pointer_) delete data_pointer_;
}

MemBlock& MemBlock::operator = (const MemBlock& src){
	if(this==&src) return *this;
	SetSize(src.GetSize());
	if(src.GetSize())
	DatCopy (data_pointer_,src.data_pointer_,src.GetSize());
	return *this;
}


}//end of namespace woodychang0611
