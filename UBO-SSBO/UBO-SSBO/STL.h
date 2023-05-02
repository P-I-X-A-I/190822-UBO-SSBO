#pragma once
#include <sys/stat.h>
#include <string>

class STL
{
public:
	STL();
	~STL();

	std::string* file_path_string;
	std::wstring* file_path_w_string;
	const char* file_path;
	const wchar_t* file_w_path;

	const char* saveSTL_path;

	long dataLength;
	char* header;

	unsigned int* num_triangles;
	float* vert_Ptr;
	float* norm_Ptr;

	float obj_Width;
	float obj_Depth;
	float obj_Height;

	float bound_minX;
	float bound_minY;
	float bound_minZ;
	float bound_maxX;
	float bound_maxY;
	float bound_maxZ;

	bool isValid;

	float trans_x_for_display;
	float trans_y_for_display;
	float trans_z_for_display;
	float scale_x_for_display;
	float scale_y_for_display;
	float scale_z_for_display;
	float rotation_integral;
	float rotation_integral_x;
	float rotation_integral_y;
	float rotation_integral_z;


	void loadSTLfromPath(const char* stlPath);
	void loadSTLfrom_W_Path(const wchar_t* w_stlPath);
	void readFromFILE(FILE* fp);

	void rotateXYZ(double xDeg, double yDeg, double zDeg);
	void scaleXYZ(double scaleX, double scaleY, double scaleZ);
	void translateXYZ(double transX, double transY, double transZ);

	void saveSTL(const char* savePath);
	void w_saveSTL(const wchar_t* w_savePath);

	void check_Width_and_Depth_ratio();
};

