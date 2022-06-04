/*  Gaurav Laxmandas
 *  Nathani
 *  gauravla
 */

#ifndef A0_HPP
#define A0_HPP

#include <vector>

void filter_2d(int n, int m, const std::vector<float>& K, std::vector<float>& A) {
	int i,j,x=0;
	// Creating temporary vector R to store results.
	std::vector<float> R(n*m);
	// First loop for each row. Second loop for each element of row. 
	// 2 loops used for cache reuse.
#pragma omp parallel for default(none) shared(A,K,m,n,i,R) schedule(dynamic)
	for(i=m;i<m*(n-1);i=i+m) {
#pragma omp parallel for default(none) shared(A,K,m,n,i,R) schedule(dynamic)
		for(j=i;j<i+m;j++) {
			if(j%m!=0 && (j+1)%m!=0) {
				R[j] = K[0]*A[j-m-1] + K[3]*A[j-m] + K[6]*A[j-m+1] + \
				       K[1]*A[j-m-1] + K[4]*A[j-m] + K[7]*A[j-m+1] + \
					   K[2]*A[j-m-1] + K[5]*A[j-m] + K[8]*A[j-m+1];
				R[j] += K[0]*A[j-1] + K[3]*A[j] + K[6]*A[j+1] + \
					K[1]*A[j-1] + K[4]*A[j] + K[7]*A[j+1] + \
					K[2]*A[j-1] + K[5]*A[j] + K[8]*A[j+1];
				R[j] += K[0]*A[j+m-1] + K[3]*A[j+m] + K[6]*A[j+m+1] + \
					K[1]*A[j+m-1] + K[4]*A[j+m] + K[7]*A[j+m+1] + \
					K[2]*A[j+m-1] + K[5]*A[j+m] + K[8]*A[j+m+1];
			}
			else {
				R[j]=A[j];
			}
		}
	}
	// Updating A from temporary vector.
	for (i=m; i<m*(n-1); i++) A[i]=R[i];
} // filter_2D
#endif // A0_HPP
