#ifndef __WOODYCHANG0611_COMMON_TENSOR__
#define __WOODYCHANG0611_COMMON_TENSOR__


#include "base.h"
#include "diagnostics.h"
#include "data_operation.h"
#include <stdarg.h> //不定數量變數時要使用
namespace woodychang0611{
	namespace math{
	//	namespace {
			//產生指標種類
			//例如  
			//PointerType為int, level=1 RecursivePointer:int*
			//PointerType為int, level=2 RecursivePointer:int**
			//PointerType為int, level=3 RecursivePointer:int***
			template <typename PointerType,int level> class RecursivePointer;
			template <typename PointerType,int level> class RecursivePointer{
			public:
				typedef typename RecursivePointer <PointerType,level-1>::Type * Type;
			};
			template <typename PointerType> class RecursivePointer<PointerType,1>{
			public:
				typedef PointerType * Type;
			};


	

			//設定_indirect_pointer的內容
			template <int dimension> inline void CreateIndirectPointer(void*& ptr,int*size,int type_size,void* start_pos){
				int sub_size[dimension-1];
				int offset = type_size;
				for (int i=1;i<dimension;i++){
					sub_size[i-1]=size[i];
					offset*=size[i];
				}
				ptr = new void*[size[0]];
				for (int i=0;i<size[0];i++){
					CreateIndirectPointer<dimension-1>(((void**)ptr)[i],sub_size,type_size,(char*)start_pos+offset*i);
				}
			}
			template <>  inline void CreateIndirectPointer<1>(void*& ptr,int*,int ,void* start_pos){
				ptr=start_pos;
			}
			//清除_indirect_pointer的內容
	
			 template <int dimension> inline void DeleteIndirectPointer(void* ptr,int*size){
				int sub_size[dimension-1];
				for (int i=1;i<dimension;i++){
					sub_size[i-1]=size[i];
				}
				for (int i=0;i<size[0];i++){
					DeleteIndirectPointer<dimension-1>(((void**)ptr)[i],sub_size);
				}
				delete[]ptr;
			}
			template <> inline void DeleteIndirectPointer<2>(void* ptr,int*){
				delete[]ptr;
			}
			template <> inline void DeleteIndirectPointer<1>(void* ,int*){
				//Do nothing no need to delete 
			}
	//	}//anonymous namespace

		template <typename Type,int dimension> class Tensor{
		protected:
			INT32 _size[dimension];//每個維度的尺寸
			INT32 _element_count;//整個張量的總數 例如10X10X10 就有1000個元素
			Type *_data_pointer;
			//透過遞迴產生對應的class
			//dimension =1 _indirect_pointer 類型為Type*
			//dimension =2 _indirect_pointer 類型為Type**
			//dimension =3 _indirect_pointer 類型為Type***
			typename RecursivePointer<Type,dimension>::Type _indirect_pointer; //透過[][][][]方式存取
			template <typename Type2> bool _IsSameSize(Tensor<Type2,dimension>const &);
			template <typename Type2> void _Copy(const Tensor<Type2,dimension> &);
			void _Constructor(); //相同的部份
			void _Copy(const Tensor<Type,dimension> &);
			void _Create();
			void _Destory();
		public:
			//return pointer 
			//ex: dimension=1 return Type* 
			//    dimension=2 return Type**
			//	  dimension=3 return Type***
			inline operator typename RecursivePointer<Type,dimension>::Type ()
			{return _indirect_pointer;};
			//
			inline operator typename RecursivePointer<Type,dimension>::Type ()const
			{return _indirect_pointer;};
			Tensor();
			Tensor(int size0,...);
			template <typename Type2> void MakeSameSize(const Tensor<Type2,dimension> &);
			Tensor(Tensor<Type,dimension>const &);//copy constructor
			template <typename Type2> Tensor(const Tensor<Type2,dimension> &);//copy constructor
			Tensor<Type,dimension>& operator=(const Tensor<Type,dimension> &);

			Tensor<Type,dimension> operator -(void)const;
			void operator +=(Tensor<Type,dimension>const&);
			void operator -=(Tensor<Type,dimension>const&);
			Tensor<Type,dimension> operator+(Tensor<Type,dimension>const&)const;
			Tensor<Type,dimension> operator-(Tensor<Type,dimension>const&)const;
			template <typename Type2> void operator*=(const Type2& A);
			template <typename Type2> void operator/=(const Type2& A);
			template <typename Type2> Tensor<Type,dimension> operator*(const Type2&)const;
			template <typename Type2> Tensor<Type,dimension> operator/(const Type2&)const;

			inline INT32 GetElementCount() const  {return _element_count;};
			inline INT32 GetSize(int i) const  {return _size[i];};
			inline Type* GetDataPointer(void) {return _data_pointer;}; 
			inline const Type* GetDataPointer(void)const {return _data_pointer;}; 
			inline bool IsEmpty(void)const{return (_element_count!=0?false:true);};//! <Test if Tensor is empty
			inline void SetValue(const Type &src ){for (INT32 i=0;i<this->GetElementCount();i++){GetDataPointer()[i]=src;}};//Set all value 
			virtual ~Tensor();
		};

		template <typename Type,int dimension> void Tensor<Type,dimension>::_Constructor(){
			_data_pointer =NULL;
			_indirect_pointer=NULL;
			_element_count=0;
		}

		template <typename Type,int dimension>Tensor<Type,dimension>::Tensor(){
			this->_Constructor();
			//將各維度尺寸設為0
			for (int i=0;i<dimension;i++){
				this->_size[i]=0;
			}
		}
		template <typename Type,int dimension>Tensor<Type,dimension>::~Tensor(){
			_Destory();
		}




		template <typename Type,int dimension> Tensor<Type,dimension>::Tensor(INT32 size0,...){
			//call default contrustor
			this-> Tensor<Type,dimension>::Tensor();
			va_list argptr; //不定變數用
			va_start(argptr, size0);//不定變數初始化 size0為第一個位置
			_size[0]=size0;
			INT32 i=1;
			while(i<dimension){
				_size[i] = va_arg(argptr,INT32);
				i++;
			}
			
			va_end(argptr);//不定變數結束
			_Create();//初始化
		}
		template <typename Type,int dimension> void Tensor<Type,dimension>::_Create(void){
			_element_count =1;
			for (INT32 i=0;i<dimension;i++){
				if(_size[i]<=0){ //任一維度尺寸小於0 將其他維度尺寸也設成0
					for (INT32 j=0;j<dimension;j++)
						_size[j]=0;
					_element_count=0;
					return;
				}
				_element_count*=_size[i];
			}
			if(_element_count!=0 )
				_data_pointer=new Type[_element_count];
			void *temp = _indirect_pointer;
			CreateIndirectPointer<dimension>(temp,_size,sizeof(Type),_data_pointer);
			_indirect_pointer =(typename RecursivePointer<Type,dimension>::Type) temp;			
		}
		template <typename Type,int dimension> void Tensor<Type,dimension>::_Destory(void){
			if(_data_pointer !=NULL) delete []_data_pointer;
			_data_pointer=NULL;
			for(int i=0;i<dimension;i++)
				_size[i]=0;
			if(_indirect_pointer!=NULL)
				DeleteIndirectPointer<dimension>(_indirect_pointer,_size);
			_indirect_pointer=NULL;
		}


		template <typename Type,int dimension> Tensor<Type,dimension>::Tensor(Tensor<Type,dimension>const & src){
			//call defalut contrustor
			this-> Tensor<Type,dimension>::Tensor();
			this->_Copy(src);
		}

		template <typename Type,int dimension>  template<typename Type2> 
		Tensor<Type,dimension>::Tensor(Tensor<Type2,dimension>const & src){
			//call defalut contrustor
			this-> Tensor<Type,dimension>::Tensor();
			this->_Copy(src);
		}

		template <typename Type,int dimension> 
		Tensor<Type,dimension> & Tensor<Type,dimension>::operator=(const Tensor<Type,dimension> & src){
				_Copy(src);
				return *this;
		}

		template <typename Type,int dimension> 	
		Tensor<Type,dimension> Tensor<Type,dimension>::operator -(void)const{
			Tensor<Type,dimension> temp(*this);
			for (INT32 i=0;i<temp.GetElementCount();i++)
				temp.GetDataPointer()[i]=-temp.GetDataPointer()[i];
			return temp;
		}
		template <typename Type,int dimension> 	
		void Tensor<Type,dimension>::operator +=(Tensor<Type,dimension>const& src){
			if(!this->_IsSameSize(src))
			{
				woodychang0611::diagnostics::SendWarning(_T("Tensor operator += size mismatch"));
				return;
			}
			for (INT32 i=0;i<this->GetElementCount();i++)
			this->GetDataPointer()[i]+=src.GetDataPointer()[i];
		}
		template <typename Type,int dimension> 	
		void Tensor<Type,dimension>::operator -=(Tensor<Type,dimension>const&src){
			if(!this->_IsSameSize(src))
			{
				woodychang0611::diagnostics::SendWarning(_T("Tensor operator -= size mismatch"));
				return;
			}
			for (INT32 i=0;i<this->GetElementCount();i++)
			this->GetDataPointer()[i]-=src.GetDataPointer()[i];
		}
		template <typename Type,int dimension> 	template <typename Type2>
		void Tensor<Type,dimension>::operator *=(const Type2 & multiplier ){
			for (INT32 i=0;i<this->GetElementCount();i++)
			this->GetDataPointer()[i]=(Type) (this->GetDataPointer()[i]*multiplier);
		}
		template <typename Type,int dimension> 	template <typename Type2>	
		void Tensor<Type,dimension>::operator /=(const Type2 & divisor){
			for (INT32 i=0;i<this->GetElementCount();i++)
			this->GetDataPointer()[i]=(Type) (this->GetDataPointer()[i]/divisor);

		}

		template <typename Type,int dimension> 	
		Tensor<Type,dimension> Tensor<Type,dimension>::operator+(Tensor<Type,dimension>const&src)const{
			Tensor<Type,dimension> temp(*this);
			temp+=src;
			return temp;
		}
		template <typename Type,int dimension> 	
		Tensor<Type,dimension> Tensor<Type,dimension>::operator-(Tensor<Type,dimension>const&src)const{
			Tensor<Type,dimension> temp(*this);
			temp-=src;
			return temp;
		}

		template <typename Type,int dimension> 	template <typename Type2> 
		Tensor<Type,dimension> Tensor<Type,dimension>::operator*(const Type2& multiplier)const{
			Tensor<Type,dimension> temp(*this);
			temp*=multiplier;
			return temp;
		}
		template <typename Type,int dimension> 	
		template <typename Type2> Tensor<Type,dimension> Tensor<Type,dimension>::operator/(const Type2& divisor )const{
			Tensor<Type,dimension> temp(*this);
			temp/=divisor;
			return temp;
		}


		template <typename Type,int dimension> template <typename Type2>	
		bool Tensor<Type,dimension>::_IsSameSize(Tensor<Type2,dimension>const & src){
			for (int i=0;i<dimension;i++){
				if (this->_size[i] != src.GetSize(i)) return false;
			}
			return true;
		}

		template <typename Type,int dimension> template <typename Type2>
		void Tensor<Type,dimension>::MakeSameSize(const Tensor<Type2,dimension> & src){
			if(this->_IsSameSize<Type2>(src)==false){
				this->_Destory();
				for (int i=0;i<dimension;i++){
					this->_size[i] = src.GetSize(i);
				}
				this->_Create();
			}
		}

		template <typename Type,int dimension> 	template <typename Type2>	
		void Tensor<Type,dimension>::_Copy(const Tensor<Type2,dimension> & src){
			MakeSameSize<Type2>(src);
			for (INT32 i=0;i<src.GetElementCount();i++)
			{
				this->GetDataPointer()[i]=static_cast<Type> (src.GetDataPointer()[i]);
			}
		}

		template <typename Type,int dimension> 		
		void Tensor<Type,dimension>::_Copy(const Tensor<Type,dimension> & src){
			if((void*)*this==src) return;
			MakeSameSize<Type>(src);
			woodychang0611::DatCopy(this->GetDataPointer(),src.GetDataPointer(),this->GetElementCount()*sizeof(Type));
		}

		 template <typename Type,int dimension,typename Type2> 	
		 Tensor<Type,dimension> operator *(Type2 multiplier,const Tensor<Type,dimension>& tensor){
			return tensor*multiplier;
		 }

		
	}//end if namespace math
}//end of namespace woodychang0611





#endif //end of __WOODYCHANG0611_COMMON_TENSOR__
