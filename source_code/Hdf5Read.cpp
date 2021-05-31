#include "Hdf5Read.h"
#include "qDebug"

const H5std_string FILE_NAME("C:\\Users\\LJ\\Desktop\\example.fast5");

void Hdf5Read::save_file(double* data, int row, int col, const char* file_path, const char* db_name) {
	//先创建一个文件写入
	const int data_rank = 2;
	hid_t file_id;
	file_id = H5Fcreate(file_path, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

	hsize_t dims[2];
	dims[0] = row;
	dims[1] = col;

	hid_t dataspace_id, dataset_id;
	dataspace_id = H5Screate_simple(data_rank, dims, NULL);
	dataset_id = H5Dcreate2(file_id, db_name, H5T_IEEE_F64BE, dataspace_id, H5P_DEFAULT,
		H5P_DEFAULT, H5P_DEFAULT);
	herr_t status;
	status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

	status = H5Sclose(dataspace_id);
	status = H5Sclose(dataset_id);
	status = H5Sclose(file_id);

	return;
}


bool Hdf5Read::read_file(double* data, int& row, int& col, const char* file_path, const char* db_name) {
	//H5File file(FILE_NAME, H5F_ACC_RDONLY);
	
	//打开HDF5文件
	hid_t file_id;
	herr_t status;
	file_id = H5Fopen(file_path,H5F_ACC_RDWR,H5P_DEFAULT);

	// 创建数据集中的数据本身
	hid_t dataset_id;    // 数据集本身的id
	// dataset_id = H5Dopen(group位置id,
	//                 const char *name, 数据集名
	//                    数据集访问性质)
	dataset_id = H5Dopen(file_id, db_name, H5P_DEFAULT);


	// 将数据写入数据集
	// herr_t 读取状态 = H5Dread(写入目标数据集id,
	//                              内存数据类型,
	//                       memory_dataspace_id, 定义内存dataspace和其中的选择
	//                          - H5S_ALL: 文件中dataspace用做内存dataspace，file_dataspace_id中的选择作为内存dataspace的选择
	//                         file_dataspace_id, 定义文件中dataspace的选择
	//                          - H5S_ALL: 文件中datasapce的全部，定义为数据集中dataspace定义的全部维度数据
	//                        本次IO操作的转换性质,
	//                          const void * buf, 内存中接受数据的位置
	status = H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

	hid_t dspace = H5Dget_space(dataset_id);/** 获取数据集大小信息 */
	hsize_t dims[2];
	H5Sget_simple_extent_dims(dspace, dims, NULL);
	row = dims[0];
	col = dims[1];
	
	// 关闭dataset相关对象
	status = H5Dclose(dataset_id);
	//       status = H5Sclose(dataspace_id);

		   // 关闭文件对象
	status = H5Fclose(file_id);
	return true;
	
}

double* Hdf5Read::read_ont_file(int& dim, const char* file_path, const char* db_name) {

	hid_t file, group,dataset,datatype;
	herr_t status;
	H5G_info_t ginfo;
	ssize_t size;
	hsize_t i,dims;
	char* name;

	//获取group下的subgroup

	file = H5Fopen(file_path, H5F_ACC_RDONLY, H5P_DEFAULT);
	group = H5Gopen(file, "/Raw/Reads", H5P_DEFAULT);
	status = H5Gget_info(group, &ginfo);

	i = 0;
	size = 1 + H5Lget_name_by_idx(group, ".", H5_INDEX_NAME, H5_ITER_INC, i, NULL, 0, H5P_DEFAULT);
	name = (char*)malloc(size);
	size = 1 + H5Lget_name_by_idx(group, ".", H5_INDEX_NAME, H5_ITER_INC, i, name, (size_t)size, H5P_DEFAULT);

	group = H5Gopen(group, name, H5P_DEFAULT);
	//获取datatype dims
	dataset=H5Dopen(group,db_name, H5P_DEFAULT);
	hid_t dataspace = H5Dget_space(dataset);
	datatype = H5Dget_type(dataset);
	H5Sget_simple_extent_dims(dataspace, &dims, NULL);

	dim = dims;
	double *data = (double*)malloc(sizeof(double) * dims);

	//double* data = (double*)malloc(sizeof(double) * dims);
	H5Dread(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
	
	H5Dclose(dataset);
	H5Gclose(group);
	H5Fclose(file);

	return data;
}
