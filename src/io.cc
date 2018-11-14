#include "io.h"
#include <cstdio>
#include <cstring>
#include <vector>
#include <matio.h>
#include <string>
using namespace std;

template<typename T>
void ReadEdgesFromFile(const std::string&filepath, 
		unsigned vertex_count, unsigned int& edge_count, T**data){
	mat_t* mat = Mat_Open(mat_path.c_str(), MAT_ACC_RDONLY);
	if(!mat){
		printf("Open mat file%s error!\n", mat_path.c_str());
		return;
	}
	matvar_t* stru = 0;
	stru = Mat_VarReadNext(mat);
	while(stru){
		//the sparse matrix is at index 2
		matvar_t* var = 0;
		var = Mat_VarGetStructFieldByName(stru, "A", 0);
		uint64_t rank = var->rank;
		const char*var_name = var->name;
		//check if var is matrix
		if(rank != 2){
			printf("var:%s is not a matrix, which has rank:%d\n", var_name, rank);
		}
		rows = var->dims[0];
		columns = var->dims[1];
		uint64_t data_cnt = rows*columns;
		const T* var_data = static_cast<const T*>(var->data);
#define DEBUG
#ifdef DEBUG
		unsigned int nnz = 0;
		for(int i=0; i<data_cnt; i+=1){
			if(var_data[i]!=0){
				nnz+=1;
			}
		}
		printf("nnz=%d\n", nnz);
#endif
		//alocate memory
		*data = static_cast<T*>(malloc(sizeof(T)*data_cnt));
		if(!(*data)){
			printf("allocate memory failed\n");
			return;
		}
		memcpy(*data, var_data, sizeof(T)*data_cnt);
		stru = Mat_VarReadNext(mat);
#ifdef DEBUG
		printf("read var%s, rank=%d, size=%d, rows=%d, columns=%d\n", var_name, rank, data_cnt, rows, columns);
#endif
	}
	Mat_Close(mat);
}

//instantiate explicitly
template
void ReadMatrixFromMat<double>(const std::string&mat_path, 
		unsigned int& rows, unsigned int& colomns, double **data);

/*
int main(const int argc, const char*argv[])
{
	string mat_path = string(argv[1]);
	double* data = nullptr;
	unsigned int row = 0;
	unsigned int column = 0;
	ReadMatrixFromMat(mat_path, row, column, &data);

	free(data);
	return 0;
}
*/
