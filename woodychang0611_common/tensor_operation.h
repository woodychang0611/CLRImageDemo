#ifndef __WOODYCHANG0611_ARRAY_OPERATION_H__
#define __WOODYCHANG0611_ARRAY_OPERATION_H__

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4251) //Disable C4251
#endif

#include "tensor.h"
#include "geometry2d.h"
//template<typename T> WOODYCHANG0611_COMMON_API void ShowVector(const Array1D<T> &vecSrc,Img & imgDst,const UINT16 un16Resolution);
//template WOODYCHANG0611_COMMON_API void ShowVector <INT8> (const Array1D<INT8> &vecSrc,Img & imgDst,const UINT16 un16Resolution);

// ******************************************************************************************************************
// MovingAverage
//
// Purpose
//		讓vector分布平緩,un16CountingNumber越大,效果越平.
// Useage
//		MovingAverage(vecSrc,vecDst,un16CountingNumber);
// Description of Parameters
//		vecSrc				-	Vector source
//		vecDst				-	Vector destionaton
//		un16CountingNumber	-	參考點都周圍點數,用來平均用

// Return Value
//		None.
// Example
//		Img Simg ;
//		UINT32Array1D vec1,vec2;
//		GetHistogram(Simg,vec1,Lum);
//		MovingAverage(vec1,vec2,3) ;
//
// ******************************************************************************************************************

namespace woodychang0611{
	namespace math{
		template <typename Type> void TrimMatrix(const Tensor<Type,2> &src,Tensor<Type,2> &dst,INT32 rowStart,INT32 colStart,INT32 rowEnd,INT32 colEnd){
			//Check Value
			//If illegal return empty matrix
			if(src.IsEmpty()||
				rowEnd<rowStart||
				rowEnd<rowStart||
				rowStart>src.GetSize(0)-1||
				colStart>src.GetSize(1)-1||
				rowEnd>src.GetSize(0)-1||
				colEnd>src.GetSize(1)-1){
					dst = Tensor<Type,2>();
					return;
			}
			Tensor<Type,2> temp(rowEnd-rowStart+1,colEnd-colStart+1);
			UINT32 col=0;
			UINT32 row=0;
			//Assign Value
			for (INT32 i=rowStart;i<rowEnd+1;i++){
				for (INT32 j=colStart;j<colEnd+1;j++){
					temp[row][col]=src[i][j];
					col++;
				}
				col=0;
				row++;
			}
			dst = temp;
		}
		template<typename Type> Tensor<Type,2>  operator *( Tensor<Type,2> const & src1, Tensor<Type,2> const & src2){
			if(src1.IsEmpty()||src2.IsEmpty()){
				return  Tensor<Type,2>();
			}
			//Check row and col size
			if(src1.GetSize(1) != src2.GetSize(0)){
				return  Tensor<Type,2>();
			}
			Tensor<Type,2> new_array(src1.GetSize(0),src2.GetSize(1));
			for (UINT16 i=0;i<new_array.GetSize(0);++i){
				for (UINT16 j=0;j<new_array.GetSize(1);++j){
					new_array[i][j] =0;
					for (UINT16 k=0;k<src1.GetSize(1) ;++k){
						new_array[i][j] +=(src1[i][k] * src2[k][j]); 
					}
				}
			}
			return new_array;
		}
		//1's on the diagonal and 0's elsewhere
		template <typename Type>  void MakeIdentity(const  Tensor<Type,2>&src){
			for (UINT16 i=0;i<src.GetSize(0);++i){
				for(UINT16 j=0;j<src.GetSize(1);++j){
					if(i==j) src[i][j]=1;
					else src[i][j]=0;
				}
			}
		}


		template<typename Type>	 Tensor<Type,2> SubMatrix(const  Tensor<Type,2>&src, UINT16 const row_position, UINT16 const col_position) {
			Tensor<Type,2> new_array;
			UINT16 i,imax,j,jmax; //for loop
			UINT32 un32WritePos,un32ReadPos;
			//Check if Array is bigger than 1X1 and row_position, col_position is within the Array
			if (src.GetSize(0)>1 && src.GetSize(1)>1 && row_position <= src.GetSize(0) && col_position<=src.GetSize(1)){
				new_array=Tensor<Type,2>(src.GetSize(0)-1,src.GetSize(1)-1);
				imax = src.GetSize(0);
				jmax = src.GetSize(1);
				un32WritePos = 0;
				un32ReadPos =0;
				for(i=0;i<imax;i++){
					for(j=0;j<jmax;j++){
						if(i!=row_position-1 && j!=col_position-1){
							new_array.indirect_pointer_[un32WritePos]=src.indirect_pointer_[un32ReadPos];
							un32WritePos++;
						}
						un32ReadPos++;
					}
				}
			}
			return new_array;
		}

		template<typename Type>	Tensor<Type,2> Transpose(const  Tensor<Type,2>&src) {
			if(src.IsEmpty()) return Tensor<Type,2>();
			Tensor<Type,2> new_array(src.GetSize(1),src.GetSize(0));
			for (UINT16 i=0;i<new_array.GetSize(0);++i){
				for (UINT16 j=0;j<new_array.GetSize(1);++j){
					new_array[i][j] =src[j][i]; 
				}
			}
			return new_array;
		}			

		template<typename Type>	Tensor<Type,2> Inverse(const  Tensor<Type,2>&src){
			if(src.GetSize(1) != src.GetSize(0)) {
				return Tensor<Type,2>();//Square Matrix only
			}
			INT32 size = src.GetSize(0);//Size of the Square Matrix
			Tensor<Type,2> src_temp = src; //Used for Gaussian elimination
			Tensor<Type,2> inverse_temp(size,size); //Used for inversed result
			Tensor<Type,2> inverse_result(size,size); //Used for inversed result

			//1X1 Matrix
			if (size ==1){
				if (src[0][0] ==0){
					return Tensor<Type,2>();
				}
				else {
					inverse_result[0][0] = 1/src[0][0];
					return inverse_result;
				}
			}

			MakeIdentity(inverse_temp);
			std::vector<UINT16> col_index; //Used for rearrange ( undo swaping caused by pivoting)
			for (UINT16 i=0;i<size;++i){
				col_index.push_back(i);
			}

			//Gaussian elimination with Pivoting
			for (UINT16 i=0;i<size-1;++i){
				//Pivoting 

				//Find Max Value
				UINT16 max_value_row =0;
				UINT16 max_value_col =0;
				Type max_value = 0;
				for (UINT16 j=i;j<size;++j){
					for (UINT16 k=i;k<size;++k){
						if(abs(src_temp[j][k])> max_value){
							max_value = static_cast <Type> (abs(src_temp[j][k]));
							max_value_row = j;
							max_value_col =k;
						}
					}
				}

				if (max_value==0){
					return Tensor<Type,2>(); //Singular Matrix has no inverse matrix
				}

				if (i!=max_value_row){
					SwapRow(src_temp,i,max_value_row);
					SwapRow(inverse_temp,i,max_value_row);
				}
				if(i!=max_value_col){
					SwapCol(src_temp,i,max_value_col);
					SwapCol(inverse_temp,i,max_value_col);
					(woodychang0611::swap)(col_index[i],col_index[max_value_col]);
				}
				// Make upper triangle matrix
				for (UINT16 j=i+1;j<size;++j){
					if (src_temp[i][i]!=0){
						Type scale = src_temp[j][i]/src_temp[i][i];
						for (UINT16 k=0;k<size;++k){
							src_temp[j][k]=src_temp[j][k]-src_temp[i][k]*scale;
							inverse_temp[j][k]=inverse_temp[j][k]-inverse_temp[i][k]*scale;
						}
					}
				}
			}
			// Make all element on diagonal equal 1
			for (UINT16 i=0;i<size;++i){
				Type scale = src_temp[i][i];
				for (UINT16 j=0;j<size;++j){
					src_temp[i][j] = src_temp[i][j]/scale;
					inverse_temp[i][j] = inverse_temp[i][j]/scale;
				}
			}

			// Eliminate element upper-right corner to 0
			for (UINT16 i=0;i<size;++i){
				for (UINT16 j=0;j<i;++j){
					Type scale = src_temp[j][i]/src_temp[i][i];
					for (UINT16 k=0;k<size;++k){
						src_temp[j][k] = src_temp[j][k] - src_temp[i][k]*scale;
						inverse_temp[j][k] = inverse_temp[j][k]  - inverse_temp[i][k]*scale;
					}
				}
			}
			//Rearrange ( undo swaping caused by pivoting)
			for (UINT16 i=0;i<size;++i){
				for (UINT16 j=0;j<size;++j){
					inverse_result[(col_index[i])][(col_index[j])] = inverse_temp[i][j];
				}
			}
			return inverse_result;
		}

		//! 2D convolution of A and B
		//! C will be convolution of A and B
		//! If A is maxna Matrix B is mbxnb
		//! C will be (ma+mb-1)x(na+nb-1) Matrix
		//! Either A or B is empty C will be empty
		//! If bSame=true Trim C same size as A
		template <typename Type> void Convolution2D(const Tensor<Type,2> &A,const Tensor<Type,2> &B,Tensor<Type,2> &C,bool makeSameSize){
			if(A.IsEmpty() || B.IsEmpty()) {
				C = Tensor<Type,2>();	
				return;
			}
			//C will be (ma+mb-1)x(na+nb-1) Matrix 
			Tensor<Type,2> temp (A.GetSize(0)+B.GetSize(0)-1,A.GetSize(1)+B.GetSize(1)-1);
			INT32 rowOffset = (B.GetSize(0)-1)/2;
			INT32 colOffset = (B.GetSize(1)-1)/2;
			INT32 rowA = A.GetSize(0);
			INT32 colA = A.GetSize(1);
			INT32 rowB = B.GetSize(0);
			INT32 colB = B.GetSize(1);
			INT32 rowtemp = temp.GetSize(0);
			INT32 coltemp = temp.GetSize(1);
			for (INT32 i=0;i<rowtemp;i++){
				for (INT32 j=0;j<coltemp;j++){
					Type sum=0;
					INT32 row=0;
					INT32 col=0;
					for (INT32 k=0;k<rowB;k++){
						row = i+k-rowB+1;
						for (UINT16 l=0;l<colB;l++){
							col = j+l-colB+1;
							if( row >=0 && row <rowA &&
								col >=0 && col <colA)
								sum+=(B[k][l]*A[row][col]);
						}
					}
					temp[i][j]=sum;
				}
			}
			if (makeSameSize){
				//Trim C same size as A
				TrimMatrix(temp,C,rowOffset,colOffset,
					rowOffset+rowA-1,colOffset+colA-1);
			}else	C=temp;
		}
		template <typename Type> Tensor<Type,2> Multiply (const Tensor<Type,2> &src1,const Tensor<Type,2> &src2){
			Tensor<Type,2> temp;
			if(src1.GetSize(1)!=src2.GetSize(0)){
			}
			else{
				temp = Tensor<Type,2> (src1.GetSize(0),src2.GetSize(1));
				for (UINT16 i=0;i<temp.GetSize(0);i++){
					for (UINT16 j=0;j<temp.GetSize(1);j++){
						temp[i][j]=0;
						for (UINT16 k=0;k<src1.GetSize(1);k++){
							temp[i][j]+= src1[i][k]*src2[k][j];
						}
					}
				}
			}
			return temp;
		}

		template <typename Type> void SwapRow(const Tensor<Type,2> &src, UINT16 row1,UINT16 row2){
			//check boundary
			if (row1 ==row2 ||
				row1 > src.GetSize(0) ||
				row2 > src.GetSize(0)) return;
			for (UINT16 i=0;i<src.GetSize(1);++i){
				Type temp;
				temp = src[row1][i];
				src[row1][i] = src[row2][i];
				src[row2][i] = temp;
			}
		}

		template <typename Type>  void SwapCol(const Tensor<Type,2> &src,UINT16 col1,UINT16 col2){
			//check boundary
			if (col1 ==col2 ||
				col1 > src.GetSize(1) ||
				col2 > src.GetSize(1)) return;
			for (UINT16 i=0;i<src.GetSize(0);++i){
				Type temp;
				temp = src[i][col1];
				src[i][col1] = src[i][col2];
				src[i][col2] = temp;
			}
		}

		//! \todo Not Ready	
		//! \author Woody
		//! \warning Only floating point array will get correct answer
		template <typename Type> Type Det(const Tensor<Type,2> &src){
			if(src.GetSize(1) != src.GetSize(0)) return 0;//Square Matrix only
			if(src.GetSize(1) ==1 ) return src[0][0]; //1X1 Array

			Tensor<Type,2> temp = src;

			double sign=1; //Change sign after swap

			for (UINT16 i=0;i<temp.GetSize(0)-1;++i){
				//Find Max Value
				UINT16 max_value_row =0;
				UINT16 max_value_col =0;
				Type max_value = 0;
				//Pivoting 
				for (UINT16 j=i;j<temp.GetSize(0);++j){
					for (UINT16 k=i;k<temp.GetSize(1);++k){
						if(abs(temp[j][k])> max_value){
							max_value = static_cast <Type>(abs(temp[j][k]));
							max_value_row = j;
							max_value_col =k;
						}
					}
				}
				if (i!=max_value_row&&max_value!=0){
					sign*=-1;
					SwapRow(temp,i,max_value_row);
				}
				if(i!=max_value_col&&max_value!=0){
					sign*=-1;				
					SwapCol(temp,i,max_value_col);
				}
				for (UINT16 j=i+1;j<temp.GetSize(0);++j){
					if (temp[i][i]!=0){
						Type scale = temp[j][i]/temp[i][i];
						for (UINT16 k=0;k<temp.GetSize(1);++k){
							temp[j][k]=temp[j][k]-temp[i][k]*scale;
						}
					}
				}
			}
			Type det = static_cast<Type> (sign);
			for (UINT16 i=0;i<temp.GetSize(0);++i){
				det *=temp[i][i];
			}
			return det;
		}
		WOODYCHANG0611_COMMON_API bool LeastSquareFitting(const std::vector<woodychang0611::geometry::Point2D>& src,UINT16 degree,std::vector<FLOAT64>& out);

	}//end of namespace math
}//end of namespace woodychang0611


#ifdef _WIN32
#pragma warning(pop)
#endif


#endif //__WOODYCHANG0611_ARRAY_OPERATION_H__
