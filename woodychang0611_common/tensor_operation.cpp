#include "tensor_operation.h"

namespace woodychang0611{
	namespace math{
	//template <typename T> WOODYCHANG0611_COMMON_API void MovingAverage(const Array1D<T> &src,Array1D<T> &dst, const UINT16 un16CountingNumber){
	//	UINT16 un16Size=src.GetSize();
	//	Array1D<T> vecTemp (un16Size);
	//	for (UINT16 i=un16CountingNumber-1;i<un16Size;i++){
	//		FLOAT64 f64temp =0;
	//		for (UINT32 j =0;j<un16CountingNumber;j++){
	//		f64temp += src[i-j];
	//		}
	//		vecTemp[i] =(T)f64temp /un16CountingNumber ;
	//	}
	//	dst =vecTemp;
	//}
		bool LeastSquareFitting(const std::vector<woodychang0611::geometry::Point2D>& src,UINT16 degree,std::vector<FLOAT64>& out){
			UINT16 point_count = static_cast  <UINT16> (src.end()-src.begin());
			out.clear();
			for (UINT16 i=0;i<degree+1;i++){
				out.push_back(0);
			}
			if (point_count <= 0)return false;

			Tensor<FLOAT64,2> x(point_count,1);
			Tensor<FLOAT64,2>y(point_count,1);
			Tensor<FLOAT64,2>X(point_count,degree+1);

			for (UINT16 i=0;i<point_count;i++){
				x[i][0] =src[i].x_;
				y[i][0]=src[i].y_;
			}

			// Xmn = xm^n
			for (UINT16 i=0;i<point_count;i++){
				for (UINT16 j=0;j<degree+1;j++){
					X[i][j] = pow(x[i][0],j);
				}
			}	
			//Least squares Equation
			woodychang0611::math::Tensor<FLOAT64,2> result =Inverse((Transpose(X)*X)) *Transpose(X)*y;

			if (result.GetSize(0)!=degree+1){
				return false;
			}else{
				for (UINT16 i=0;i<degree+1;i++){
					out[i] =result[i][0];
				}
			}
			return true;
		}
	}
}//end of namespace woodychang0611

