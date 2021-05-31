#pragma once

#include <hdf5.h>
#include <H5Cpp.h>
#include <string>

using namespace H5;

class Hdf5Read
{
public:
	void save_file(double* data, int row, int col, const char* file_path, const char* db_name);
	bool read_file(double* data, int& row, int& col, const char* file_path, const char* db_name);
	double* read_ont_file(int& dim,const char* file_path,const char* db_name);
};

