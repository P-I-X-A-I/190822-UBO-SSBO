#include "stdafx.h"
#include "STL.h"


STL::STL()
{
	file_path_w_string = NULL;
	file_w_path = NULL;
	dataLength = 0;

	isValid = false;

	// set pointer
	header = NULL;
	num_triangles = NULL;
	norm_Ptr = NULL;
	vert_Ptr = NULL;

	obj_Width = 0.0;
	obj_Height = 0.0;
	obj_Depth = 0.0;

	saveSTL_path = NULL;

	trans_x_for_display = 0.0;
	trans_y_for_display = 0.0;
	trans_z_for_display = 0.0;
	scale_x_for_display = 1.0;
	scale_y_for_display = 1.0;
	scale_z_for_display = 1.0;

	rotation_integral = 0.0;
	rotation_integral_x = 0.0;
	rotation_integral_y = 0.0;
	rotation_integral_z = 0.0;
}


STL::~STL()
{
	printf("dealloc STL %ls\n", file_w_path);

	if (file_path_w_string != NULL)
	{
		delete file_path_w_string;
	}

	if (header != NULL)
	{
		free(header);
	}

	if (num_triangles != NULL)
	{
		free(num_triangles);
	}

	if (norm_Ptr != NULL)
	{
		free(norm_Ptr);
	}

	if (vert_Ptr != NULL)
	{
		free(vert_Ptr);
	}
}




void STL::loadSTLfrom_W_Path(const wchar_t* w_stlPath)
{
	FILE* fp;

	const char* mode = "rb";
	wchar_t w_mode[256];
	size_t retNum;

	mbstowcs_s(&retNum, w_mode, 256, mode, _TRUNCATE);

	if ( _wfopen_s(&fp, w_stlPath, w_mode) != 0 )
	{
		printf("wopenf cant read STL file.\n");
		return;
	}
	else
	{
		// set file W_path
		printf("FILE is opened by w_string.\n");
		file_path_w_string = new std::wstring(w_stlPath);
		file_w_path = file_path_w_string->c_str();

		this->readFromFILE(fp);
	}

}


void STL::loadSTLfromPath(const char* stlPath)

{
	
	FILE* fp;

	file_path_string = new std::string(stlPath);
	file_path = file_path_string->c_str();


	// open file
	const char* mode = "rb";
	wchar_t w_mode[256];
	wchar_t w_stlPath[256];
	size_t ret;
	mbstowcs_s(&ret, w_mode, 256, mode, _TRUNCATE);
	mbstowcs_s(&ret, w_stlPath, 256, stlPath, _TRUNCATE);

	if (_wfopen_s(&fp, w_stlPath, w_mode))
	{
		printf("STL file cant be opened\n");
		return;
	}
	else
	{
		// set w path
		printf("FILE is opened by string.\n");
		file_path_w_string = new std::wstring(w_stlPath);
		file_w_path = file_path_w_string->c_str();

		this->readFromFILE(fp);
	}
	

	/*

	// get file size
	struct stat statBuf;
	stat(stlPath, &statBuf);
	dataLength = statBuf.st_size;
	printf("STL file is opened : size %ld\n", dataLength);
	

	// get stl file data

	// header
	header = (char*)malloc(80);
	memset(header, 0, 80);
	fread(header, sizeof(char), 80, fp);
	printf("%s\n", header );

	// num of triangles
	num_triangles = (unsigned int*)malloc(sizeof(unsigned int));
	fread(num_triangles, sizeof(unsigned int), 1, fp);
	printf("%d\n", *num_triangles);

	// normal & vertex data
	int NUM_T = *num_triangles;
	float* vert_origin;
	float* norm_origin;

	// vertex memory of STL class object
	vert_Ptr = (float*)malloc(sizeof(float) * 3 * 3 * NUM_T); // floatsize * xyz * 3vertex * numT
	memset(vert_Ptr, 0, sizeof(float) * 3 * 3 * NUM_T);

	// normal memory of STL class object
	norm_Ptr = (float*)malloc(sizeof(float) * 3 * 3 * NUM_T);
	memset(norm_Ptr, 0, sizeof(float) * 3 * 3 * NUM_T);

	vert_origin = vert_Ptr;
	norm_origin = norm_Ptr;


	// get data
	float* tempPtr = (float*)malloc(sizeof(float));
	float tempN[3];
	float tempV[3][3];
	size_t error;

	for (long i = 0; i < NUM_T; i++)
	{
		// get norm data
		for (int j = 0; j < 3; j++)
		{
			error = fread(tempPtr, sizeof(float), 1, fp);
			tempN[j] = *tempPtr;
		}// j

		// set norm data to norm memory
		*norm_Ptr = tempN[0];	norm_Ptr++;
		*norm_Ptr = tempN[1];	norm_Ptr++;
		*norm_Ptr = tempN[2];	norm_Ptr++;
		*norm_Ptr = tempN[0];	norm_Ptr++;
		*norm_Ptr = tempN[1];	norm_Ptr++;
		*norm_Ptr = tempN[2];	norm_Ptr++;
		*norm_Ptr = tempN[0];	norm_Ptr++;
		*norm_Ptr = tempN[1];	norm_Ptr++;
		*norm_Ptr = tempN[2];	norm_Ptr++;


		// set vert data to vert memory
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				fread(tempPtr, sizeof(float), 1, fp);
				tempV[j][k] = *tempPtr;
			}
		}

		*vert_Ptr = tempV[0][0];	vert_Ptr++;
		*vert_Ptr = tempV[0][1];	vert_Ptr++;
		*vert_Ptr = tempV[0][2];	vert_Ptr++;
		*vert_Ptr = tempV[1][0];	vert_Ptr++;
		*vert_Ptr = tempV[1][1];	vert_Ptr++;
		*vert_Ptr = tempV[1][2];	vert_Ptr++;
		*vert_Ptr = tempV[2][0];	vert_Ptr++;
		*vert_Ptr = tempV[2][1];	vert_Ptr++;
		*vert_Ptr = tempV[2][2];	vert_Ptr++;


		// set bounding box
		if (i == 0)
		{
			bound_maxX = bound_minX = tempV[0][0];
			bound_maxY = bound_minY = tempV[0][1];
			bound_maxZ = bound_minZ = tempV[0][2];

			if (tempV[1][0] > bound_maxX) { bound_maxX = tempV[1][0]; }
			if (tempV[2][0] > bound_maxX) { bound_maxX = tempV[2][0]; }

			if (tempV[1][1] > bound_maxY) { bound_maxY = tempV[1][1]; }
			if (tempV[2][1] > bound_maxY) { bound_maxY = tempV[2][1]; }

			if (tempV[1][2] > bound_maxZ) { bound_maxZ = tempV[1][2]; }
			if (tempV[2][2] > bound_maxZ) { bound_maxZ = tempV[2][2]; }

			if (tempV[1][0] < bound_minX) { bound_minX = tempV[1][0]; }
			if (tempV[2][0] < bound_minX) { bound_minX = tempV[2][0]; }

			if (tempV[1][1] < bound_minY) { bound_minY = tempV[1][1]; }
			if (tempV[2][1] < bound_minY) { bound_minY = tempV[2][1]; }

			if (tempV[1][2] < bound_minZ) { bound_minZ = tempV[1][2]; }
			if (tempV[2][2] < bound_minZ) { bound_minZ = tempV[2][2]; }

		}
		else
		{
			for (int j = 0; j < 3; j++)
			{
				if (tempV[j][0] > bound_maxX) { bound_maxX = tempV[j][0]; }
				if (tempV[j][1] > bound_maxY) { bound_maxY = tempV[j][1]; }
				if (tempV[j][2] > bound_maxZ) { bound_maxZ = tempV[j][2]; }

				if (tempV[j][0] < bound_minX) { bound_minX = tempV[j][0]; }
				if (tempV[j][1] < bound_minY) { bound_minY = tempV[j][1]; }
				if (tempV[j][2] < bound_minZ) { bound_minZ = tempV[j][2]; }
			}
		}

		// skip 2byte ( unused section )
		fseek(fp, 2, SEEK_CUR);

	} // for i < NUM_T



	// restore pointer
	vert_Ptr = vert_origin;
	norm_Ptr = norm_origin;

	// close file
	fclose(fp);


	// center point
	float center[3];
	float shift[3];

	center[0] = (bound_minX + bound_maxX)*0.5;
	center[1] = (bound_minY + bound_maxY)*0.5;
	center[2] = (bound_minZ + bound_maxZ)*0.5;

	shift[0] = -center[0];
	shift[1] = -center[1];
	shift[2] = -bound_minZ;



	// shift vertex data
	float tempX = 0.0;
	float tempY = 0.0;
	float tempZ = 0.0;

	for (int i = 0; i < NUM_T; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// X
			tempX = *vert_Ptr;
			tempX += shift[0];
			*vert_Ptr = tempX;
			vert_Ptr++;
			// Y
			tempY = *vert_Ptr;
			tempY += shift[1];
			*vert_Ptr = tempY;
			vert_Ptr++;
			// Z
			tempZ = *vert_Ptr;
			tempZ += shift[2];
			*vert_Ptr = tempZ;
			vert_Ptr++;
		}// j
	}// i < NUM_T



	// restore pointer
	vert_Ptr = vert_origin;

	bound_minX += shift[0];
	bound_maxX += shift[0];
	bound_minY += shift[1];
	bound_maxY += shift[1];
	bound_minZ += shift[2];
	bound_maxZ += shift[2];

	// get obj width & depth
	obj_Width = bound_maxX - bound_minX;
	obj_Depth = bound_maxY - bound_minY;
	obj_Height = bound_maxZ - bound_minZ;


	printf("bound x : %f : %f\n", bound_minX, bound_maxX);
	printf("bound y : %f : %f\n", bound_minY, bound_maxY);
	printf("bound z : %f : %f\n", bound_minZ, bound_maxZ);
	printf("STL size : %f %f %f\n", obj_Width, obj_Depth, obj_Height);

	// set valid flag
	isValid = true;

	*/
}



void STL::readFromFILE(FILE* fp)
{
	// get file size
	fpos_t pos;
	if (fseek(fp, 0, SEEK_END) == 0)
	{
		if (fgetpos(fp, &pos) == 0)
		{
			printf("STL file is opened : size %ld\n", pos);
			dataLength = pos;
		}
	}

	// reset file posiiton
	fseek(fp, 0, SEEK_SET);



	// get stl file data

	// header
	header = (char*)malloc(80);
	memset(header, 0, 80);
	fread(header, sizeof(char), 80, fp);
	printf("%s\n", header);


	// check if it ASCII or not
	std::string* headString = new std::string(header);
	std::string* solidString = new std::string("solid");
	std::string* kaigyoString = new std::string("\n");
	std::string* facetString = new std::string("facet");
	std::string* outerString = new std::string("outer");

	int compareRes = headString->compare(0, 5, *solidString);
	
	if (compareRes == 0)// header starts "solid"
	{
		// find first "\0"
		int next[10] = {0};
		int tempCount = 0;
		for (int k = 0; k < 80; k++)
		{
			compareRes = headString->compare(k, 1, *kaigyoString);
			if (compareRes == 0)
			{
				next[tempCount] = k;
				printf("next %d\n", next[tempCount]);
				tempCount++;
			}
		}

		// check next line ( if it contain "facet" or "outer" )
		int range = next[1] - next[0] - 5; // f, a, c, e, t
		bool isAsciiSTL = false;

		// "facet"
		for (int k = next[0]; k < next[0] + range; k++)
		{
			compareRes = headString->compare(k, 5, *facetString);
			if (compareRes == 0)
			{
				isAsciiSTL = true;
				break;
			}
		}

		// "outer"
		for (int k = next[0]; k < next[0] + range; k++)
		{
			compareRes = headString->compare(k, 5, *outerString);
			if (compareRes == 0)
			{
				isAsciiSTL = true;
				break;
			}
		}


		if (isAsciiSTL)
		{
			MessageBox(NULL, TEXT("This STL file may be ASCII format, It should be Binary format."), NULL, MB_OK);

			fclose(fp);
			delete headString;
			delete solidString;
			delete kaigyoString;
			delete facetString;
			delete outerString;
			return;
		}

	}


	delete headString;
	delete solidString;
	delete kaigyoString;
	delete facetString;
	delete outerString;



	// num of triangles
	num_triangles = (unsigned int*)malloc(sizeof(unsigned int));
	fread(num_triangles, sizeof(unsigned int), 1, fp);
	printf("%d\n", *num_triangles);

	// normal & vertex data
	int NUM_T = *num_triangles;
	float* vert_origin;
	float* norm_origin;

	// vertex memory of STL class object
	vert_Ptr = (float*)malloc(sizeof(float) * 3 * 3 * NUM_T); // floatsize * xyz * 3vertex * numT
	memset(vert_Ptr, 0, sizeof(float) * 3 * 3 * NUM_T);

	// normal memory of STL class object
	norm_Ptr = (float*)malloc(sizeof(float) * 3 * 3 * NUM_T);
	memset(norm_Ptr, 0, sizeof(float) * 3 * 3 * NUM_T);

	vert_origin = vert_Ptr;
	norm_origin = norm_Ptr;


	// get data
	float* tempPtr = (float*)malloc(sizeof(float));
	float tempN[3];
	float tempV[3][3];
	size_t error;

	for (long i = 0; i < NUM_T; i++)
	{
		// get norm data
		for (int j = 0; j < 3; j++)
		{
			error = fread(tempPtr, sizeof(float), 1, fp);
			tempN[j] = *tempPtr;
		}// j

		// set norm data to norm memory
		*norm_Ptr = tempN[0];	norm_Ptr++;
		*norm_Ptr = tempN[1];	norm_Ptr++;
		*norm_Ptr = tempN[2];	norm_Ptr++;
		*norm_Ptr = tempN[0];	norm_Ptr++;
		*norm_Ptr = tempN[1];	norm_Ptr++;
		*norm_Ptr = tempN[2];	norm_Ptr++;
		*norm_Ptr = tempN[0];	norm_Ptr++;
		*norm_Ptr = tempN[1];	norm_Ptr++;
		*norm_Ptr = tempN[2];	norm_Ptr++;


		// set vert data to vert memory
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				fread(tempPtr, sizeof(float), 1, fp);
				tempV[j][k] = *tempPtr;
			}
		}

		*vert_Ptr = tempV[0][0];	vert_Ptr++;
		*vert_Ptr = tempV[0][1];	vert_Ptr++;
		*vert_Ptr = tempV[0][2];	vert_Ptr++;
		*vert_Ptr = tempV[1][0];	vert_Ptr++;
		*vert_Ptr = tempV[1][1];	vert_Ptr++;
		*vert_Ptr = tempV[1][2];	vert_Ptr++;
		*vert_Ptr = tempV[2][0];	vert_Ptr++;
		*vert_Ptr = tempV[2][1];	vert_Ptr++;
		*vert_Ptr = tempV[2][2];	vert_Ptr++;


		// set bounding box
		if (i == 0)
		{
			bound_maxX = bound_minX = tempV[0][0];
			bound_maxY = bound_minY = tempV[0][1];
			bound_maxZ = bound_minZ = tempV[0][2];

			if (tempV[1][0] > bound_maxX) { bound_maxX = tempV[1][0]; }
			if (tempV[2][0] > bound_maxX) { bound_maxX = tempV[2][0]; }

			if (tempV[1][1] > bound_maxY) { bound_maxY = tempV[1][1]; }
			if (tempV[2][1] > bound_maxY) { bound_maxY = tempV[2][1]; }

			if (tempV[1][2] > bound_maxZ) { bound_maxZ = tempV[1][2]; }
			if (tempV[2][2] > bound_maxZ) { bound_maxZ = tempV[2][2]; }

			if (tempV[1][0] < bound_minX) { bound_minX = tempV[1][0]; }
			if (tempV[2][0] < bound_minX) { bound_minX = tempV[2][0]; }

			if (tempV[1][1] < bound_minY) { bound_minY = tempV[1][1]; }
			if (tempV[2][1] < bound_minY) { bound_minY = tempV[2][1]; }

			if (tempV[1][2] < bound_minZ) { bound_minZ = tempV[1][2]; }
			if (tempV[2][2] < bound_minZ) { bound_minZ = tempV[2][2]; }

		}
		else
		{
			for (int j = 0; j < 3; j++)
			{
				if (tempV[j][0] > bound_maxX) { bound_maxX = tempV[j][0]; }
				if (tempV[j][1] > bound_maxY) { bound_maxY = tempV[j][1]; }
				if (tempV[j][2] > bound_maxZ) { bound_maxZ = tempV[j][2]; }

				if (tempV[j][0] < bound_minX) { bound_minX = tempV[j][0]; }
				if (tempV[j][1] < bound_minY) { bound_minY = tempV[j][1]; }
				if (tempV[j][2] < bound_minZ) { bound_minZ = tempV[j][2]; }
			}
		}

		// skip 2byte ( unused section )
		fseek(fp, 2, SEEK_CUR);

	} // for i < NUM_T



	// restore pointer
	vert_Ptr = vert_origin;
	norm_Ptr = norm_origin;

	// close file
	fclose(fp);


	// center point
	float center[3];
	float shift[3];

	center[0] = (bound_minX + bound_maxX)*0.5;
	center[1] = (bound_minY + bound_maxY)*0.5;
	center[2] = (bound_minZ + bound_maxZ)*0.5;

	shift[0] = -center[0];
	shift[1] = -center[1];
	shift[2] = -bound_minZ;



	// shift vertex data
	float tempX = 0.0;
	float tempY = 0.0;
	float tempZ = 0.0;

	for (int i = 0; i < NUM_T; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// X
			tempX = *vert_Ptr;
			tempX += shift[0];
			*vert_Ptr = tempX;
			vert_Ptr++;
			// Y
			tempY = *vert_Ptr;
			tempY += shift[1];
			*vert_Ptr = tempY;
			vert_Ptr++;
			// Z
			tempZ = *vert_Ptr;
			tempZ += shift[2];
			*vert_Ptr = tempZ;
			vert_Ptr++;
		}// j
	}// i < NUM_T



	// restore pointer
	vert_Ptr = vert_origin;

	bound_minX += shift[0];
	bound_maxX += shift[0];
	bound_minY += shift[1];
	bound_maxY += shift[1];
	bound_minZ += shift[2];
	bound_maxZ += shift[2];

	// get obj width & depth
	obj_Width = bound_maxX - bound_minX;
	obj_Depth = bound_maxY - bound_minY;
	obj_Height = bound_maxZ - bound_minZ;


	printf("bound x : %f : %f\n", bound_minX, bound_maxX);
	printf("bound y : %f : %f\n", bound_minY, bound_maxY);
	printf("bound z : %f : %f\n", bound_minZ, bound_maxZ);
	printf("STL size : %f %f %f\n", obj_Width, obj_Depth, obj_Height);

	// set valid flag
	isValid = true;


}



void STL::check_Width_and_Depth_ratio()
{
	if (isValid)
	{
		if (obj_Width > obj_Depth)
		{
			this->rotateXYZ(0.0, 0.0, 90.0);
		}
		else
		{
			printf("this STL object is not valid\n");
		}
	}
}




void STL::rotateXYZ(double xDeg, double yDeg, double zDeg)
{
	if (!isValid)
	{
		printf("this STL object is not valid. return\n");
		return;
	}


	// reset boundint box
	bool setBoundBox_flag = true;
	bound_maxX = 0.0;
	bound_minX = 0.0;
	bound_maxY = 0.0;
	bound_minY = 0.0;
	bound_maxZ = 0.0;
	bound_minZ = 0.0;

	// convert to radian
	double radX = xDeg * 0.0174532925;
	double radY = yDeg * 0.0174532925;
	double radZ = zDeg * 0.0174532925;


	unsigned int NUM_T = *num_triangles;

	float* vert_origin = vert_Ptr;
	float* norm_origin = norm_Ptr;


	float matX[9];
	float matY[9];
	float matZ[9];
	float mat[9];
	float rmat[9];

	matX[0] = 1.0;          matX[1] = 0.0;              matX[2] = 0.0;
	matX[3] = 0.0;          matX[4] = cos(radX);        matX[5] = sin(radX);
	matX[6] = 0.0;          matX[7] = -sin(radX);       matX[8] = cos(radX);

	matY[0] = cos(radY);        matY[1] = 0.0;      matY[2] = -sin(radY);
	matY[3] = 0.0;              matY[4] = 1.0;      matY[5] = 0.0;
	matY[6] = sin(radY);        matY[7] = 0.0;      matY[8] = cos(radY);

	matZ[0] = cos(radZ);        matZ[1] = sin(radZ);    matZ[2] = 0.0;
	matZ[3] = -sin(radZ);       matZ[4] = cos(radZ);    matZ[5] = 0.0;
	matZ[6] = 0.0;              matZ[7] = 0.0;          matZ[8] = 1.0;


	// matX * matY = mat
	mat[0] = matX[0] * matY[0] + matX[1] * matY[3] + matX[2] * matY[6];
	mat[1] = matX[0] * matY[1] + matX[1] * matY[4] + matX[2] * matY[7];
	mat[2] = matX[0] * matY[2] + matX[1] * matY[5] + matX[2] * matY[8];

	mat[3] = matX[3] * matY[0] + matX[4] * matY[3] + matX[5] * matY[6];
	mat[4] = matX[3] * matY[1] + matX[4] * matY[4] + matX[5] * matY[7];
	mat[5] = matX[3] * matY[2] + matX[4] * matY[5] + matX[5] * matY[8];

	mat[6] = matX[6] * matY[0] + matX[7] * matY[3] + matX[8] * matY[6];
	mat[7] = matX[6] * matY[1] + matX[7] * matY[4] + matX[8] * matY[7];
	mat[8] = matX[6] * matY[2] + matX[7] * matY[5] + matX[8] * matY[8];


	// mat * matZ = rmat
	rmat[0] = mat[0] * matZ[0] + mat[1] * matZ[3] + mat[2] * matZ[6];
	rmat[1] = mat[0] * matZ[1] + mat[1] * matZ[4] + mat[2] * matZ[7];
	rmat[2] = mat[0] * matZ[2] + mat[1] * matZ[5] + mat[2] * matZ[8];

	rmat[3] = mat[3] * matZ[0] + mat[4] * matZ[3] + mat[5] * matZ[6];
	rmat[4] = mat[3] * matZ[1] + mat[4] * matZ[4] + mat[5] * matZ[7];
	rmat[5] = mat[3] * matZ[2] + mat[4] * matZ[5] + mat[5] * matZ[8];

	rmat[6] = mat[6] * matZ[0] + mat[7] * matZ[3] + mat[8] * matZ[6];
	rmat[7] = mat[6] * matZ[1] + mat[7] * matZ[4] + mat[8] * matZ[7];
	rmat[8] = mat[6] * matZ[2] + mat[7] * matZ[5] + mat[8] * matZ[8];



	for (int i = 0; i < NUM_T; i++)
	{
		float tempValue[3][3];
		float convertValue[3][3];
		float* tempRestore_V = vert_Ptr;
		float* tempRestore_N = norm_Ptr;
		tempValue[0][0] = *vert_Ptr; vert_Ptr++;
		tempValue[0][1] = *vert_Ptr; vert_Ptr++;
		tempValue[0][2] = *vert_Ptr; vert_Ptr++;
		tempValue[1][0] = *vert_Ptr; vert_Ptr++;
		tempValue[1][1] = *vert_Ptr; vert_Ptr++;
		tempValue[1][2] = *vert_Ptr; vert_Ptr++;
		tempValue[2][0] = *vert_Ptr; vert_Ptr++;
		tempValue[2][1] = *vert_Ptr; vert_Ptr++;
		tempValue[2][2] = *vert_Ptr; vert_Ptr++;

		vert_Ptr = tempRestore_V;


		// rotate
		for (int j = 0; j < 3; j++)
		{
			convertValue[j][0] = rmat[0] * tempValue[j][0] + rmat[1] * tempValue[j][1] + rmat[2] * tempValue[j][2];
			convertValue[j][1] = rmat[3] * tempValue[j][0] + rmat[4] * tempValue[j][1] + rmat[5] * tempValue[j][2];
			convertValue[j][2] = rmat[6] * tempValue[j][0] + rmat[7] * tempValue[j][1] + rmat[8] * tempValue[j][2];
		}

		// reset value
		*vert_Ptr = convertValue[0][0]; vert_Ptr++;
		*vert_Ptr = convertValue[0][1]; vert_Ptr++;
		*vert_Ptr = convertValue[0][2]; vert_Ptr++;
		*vert_Ptr = convertValue[1][0]; vert_Ptr++;
		*vert_Ptr = convertValue[1][1]; vert_Ptr++;
		*vert_Ptr = convertValue[1][2]; vert_Ptr++;
		*vert_Ptr = convertValue[2][0]; vert_Ptr++;
		*vert_Ptr = convertValue[2][1]; vert_Ptr++;
		*vert_Ptr = convertValue[2][2]; vert_Ptr++;

		// reset bounding box once
		if (setBoundBox_flag)
		{
			bound_maxX = convertValue[0][0];
			bound_minX = convertValue[0][0];
			bound_maxY = convertValue[0][1];
			bound_minY = convertValue[0][1];
			bound_maxZ = convertValue[0][2];
			bound_minZ = convertValue[0][2];

			for (int k = 1; k < 3; k++)
			{
				if (convertValue[k][0] > bound_maxX)
				{
					bound_maxX = convertValue[k][0];
				}
				if (convertValue[k][0] < bound_minX)
				{
					bound_minX = convertValue[k][0];
				}

				if (convertValue[k][1] > bound_maxY)
				{
					bound_maxY = convertValue[k][1];
				}
				if (convertValue[k][1] < bound_minY)
				{
					bound_minY = convertValue[k][1];
				}

				if (convertValue[k][2] > bound_maxZ)
				{
					bound_maxZ = convertValue[k][2];
				}
				if (convertValue[k][2] < bound_minZ)
				{
					bound_minZ = convertValue[k][2];
				}
			}

			setBoundBox_flag = false;
		} // setBoundBox_flag


		  // override bounding box
		for (int j = 0; j < 3; j++)
		{
			if (convertValue[j][0] < bound_minX)
			{
				bound_minX = convertValue[j][0];
			}
			else if (convertValue[j][0] > bound_maxX)
			{
				bound_maxX = convertValue[j][0];
			}

			if (convertValue[j][1] < bound_minY)
			{
				bound_minY = convertValue[j][1];
			}
			else if (convertValue[j][1] > bound_maxY)
			{
				bound_maxY = convertValue[j][1];
			}

			if (convertValue[j][2] < bound_minZ)
			{
				bound_minZ = convertValue[j][2];
			}
			else if (convertValue[j][2] > bound_maxZ)
			{
				bound_maxZ = convertValue[j][2];
			}
		}


		//******************************************

		// normal
		tempValue[0][0] = *norm_Ptr;    norm_Ptr++;
		tempValue[0][1] = *norm_Ptr;    norm_Ptr++;
		tempValue[0][2] = *norm_Ptr;    norm_Ptr++;
		tempValue[1][0] = *norm_Ptr;    norm_Ptr++;
		tempValue[1][1] = *norm_Ptr;    norm_Ptr++;
		tempValue[1][2] = *norm_Ptr;    norm_Ptr++;
		tempValue[2][0] = *norm_Ptr;    norm_Ptr++;
		tempValue[2][1] = *norm_Ptr;    norm_Ptr++;
		tempValue[2][2] = *norm_Ptr;    norm_Ptr++;

		norm_Ptr = tempRestore_N;

		// rotate norm
		for (int j = 0; j < 3; j++)
		{
			convertValue[j][0] = rmat[0] * tempValue[j][0] + rmat[1] * tempValue[j][1] + rmat[2] * tempValue[j][2];
			convertValue[j][1] = rmat[3] * tempValue[j][0] + rmat[4] * tempValue[j][1] + rmat[5] * tempValue[j][2];
			convertValue[j][2] = rmat[6] * tempValue[j][0] + rmat[7] * tempValue[j][1] + rmat[8] * tempValue[j][2];
		}

		*norm_Ptr = convertValue[0][0]; norm_Ptr++;
		*norm_Ptr = convertValue[0][1]; norm_Ptr++;
		*norm_Ptr = convertValue[0][2]; norm_Ptr++;
		*norm_Ptr = convertValue[1][0]; norm_Ptr++;
		*norm_Ptr = convertValue[1][1]; norm_Ptr++;
		*norm_Ptr = convertValue[1][2]; norm_Ptr++;
		*norm_Ptr = convertValue[2][0]; norm_Ptr++;
		*norm_Ptr = convertValue[2][1]; norm_Ptr++;
		*norm_Ptr = convertValue[2][2]; norm_Ptr++;

	}// num T



	 // restore pointer
	vert_Ptr = vert_origin;
	norm_Ptr = norm_origin;


	// make it touch to ground and centerize
	float center_x = (bound_maxX + bound_minX) / 2.0;
	float center_y = (bound_maxY + bound_minY) / 2.0;

	this->translateXYZ(-center_x, -center_y, -bound_minZ);
}




void STL::scaleXYZ(double scaleX, double scaleY, double scaleZ)
{
	if (!isValid)
	{
		printf("this STL object is not valid. return\n");
		return;
	}

	// reset bounding box
	bool setBoundBox_flag = true;
	bound_maxX = 0.0;
	bound_minX = 0.0;
	bound_maxY = 0.0;
	bound_minY = 0.0;
	bound_maxZ = 0.0;
	bound_minZ = 0.0;


	unsigned int NUM_T = *num_triangles;
	float* vert_origin = vert_Ptr;
	float* norm_origin = norm_Ptr;

	for (int i = 0; i < NUM_T; i++)
	{
		float tempVert[3][3];

		// get triangle vertices
		for (int j = 0; j < 3; j++)
		{
			tempVert[j][0] = (*vert_Ptr)*scaleX;
			*vert_Ptr = tempVert[j][0];
			vert_Ptr++;

			tempVert[j][1] = (*vert_Ptr)*scaleY;
			*vert_Ptr = tempVert[j][1];
			vert_Ptr++;

			tempVert[j][2] = (*vert_Ptr)*scaleZ;
			*vert_Ptr = tempVert[j][2];
			vert_Ptr++;



			// set bounding box once
			if (setBoundBox_flag)
			{
				bound_maxX = tempVert[0][0];
				bound_minX = tempVert[0][0];
				bound_maxY = tempVert[0][1];
				bound_minY = tempVert[0][1];
				bound_maxZ = tempVert[0][2];
				bound_minZ = tempVert[0][2];

				setBoundBox_flag = false;
			}// set bounding box flag



			 // check bounding box
			 // X
			if (tempVert[j][0] < bound_minX)
			{
				bound_minX = tempVert[j][0];
			}
			else if (tempVert[j][0] > bound_maxX)
			{
				bound_maxX = tempVert[j][0];
			}
			// Y
			if (tempVert[j][1] < bound_minY)
			{
				bound_minY = tempVert[j][1];
			}
			else if (tempVert[j][1] > bound_maxY)
			{
				bound_maxY = tempVert[j][1];
			}
			// Z
			if (tempVert[j][2] < bound_minZ)
			{
				bound_minZ = tempVert[j][2];
			}
			else if (tempVert[j][2] > bound_maxZ)
			{
				bound_maxZ = tempVert[j][2];
			}
		}


		// re-calculate normal
		float vector_A[3];
		float vector_B[3];
		float crossVec[3];

		// vector A ( from p0 to p1 )
		vector_A[0] = tempVert[1][0] - tempVert[0][0];
		vector_A[1] = tempVert[1][1] - tempVert[0][1];
		vector_A[2] = tempVert[1][2] - tempVert[0][2];

		// vector B ( from p0 to p2 )
		vector_B[0] = tempVert[2][0] - tempVert[0][0];
		vector_B[1] = tempVert[2][1] - tempVert[0][1];
		vector_B[2] = tempVert[2][2] - tempVert[0][2];

		// get cross vec
		crossVec[0] = vector_A[1] * vector_B[2] - vector_A[2] * vector_B[1];
		crossVec[1] = vector_A[2] * vector_B[0] - vector_A[0] * vector_B[2];
		crossVec[2] = vector_A[0] * vector_B[1] - vector_A[1] * vector_B[0];

		// normalize cross vec
		double distance = sqrt(crossVec[0] * crossVec[0] + crossVec[1] * crossVec[1] + crossVec[2] * crossVec[2]);
		crossVec[0] /= distance;
		crossVec[1] /= distance;
		crossVec[2] /= distance;


		// set normal to pointer
		*norm_Ptr = crossVec[0]; norm_Ptr++;
		*norm_Ptr = crossVec[1]; norm_Ptr++;
		*norm_Ptr = crossVec[2]; norm_Ptr++;
		*norm_Ptr = crossVec[0]; norm_Ptr++;
		*norm_Ptr = crossVec[1]; norm_Ptr++;
		*norm_Ptr = crossVec[2]; norm_Ptr++;
		*norm_Ptr = crossVec[0]; norm_Ptr++;
		*norm_Ptr = crossVec[1]; norm_Ptr++;
		*norm_Ptr = crossVec[2]; norm_Ptr++;


	}// for i


	 // restore pointer
	vert_Ptr = vert_origin;
	norm_Ptr = norm_origin;
}



void STL::translateXYZ(double transX, double transY, double transZ)
{
	if (!isValid)
	{
		printf("this STL object is not valid. return\n");
		return;
	}

	unsigned int NUM_T = *num_triangles;
	float* vert_origin = vert_Ptr;


	// shift bounding box
	bound_maxX += transX;
	bound_minX += transX;
	bound_maxY += transY;
	bound_minY += transY;
	bound_maxZ += transZ;
	bound_minZ += transZ;


	for (int i = 0; i < NUM_T; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			*vert_Ptr = *vert_Ptr + transX;     vert_Ptr++;
			*vert_Ptr = *vert_Ptr + transY;     vert_Ptr++;
			*vert_Ptr = *vert_Ptr + transZ;     vert_Ptr++;
		}// for j
	}// for i



	 // normal is not changed


	 // restor pointer
	vert_Ptr = vert_origin;

}



void STL::saveSTL(const char* savePath)
{
	if (isValid)
	{
		FILE* fp;
		if (fopen_s(&fp, savePath, "wb") != 0)
		{
			printf("can't open save file.\n");
			return;
		}


		float* vert_origin = vert_Ptr;
		float* norm_origin = norm_Ptr;


		// header data
		char headerString[80] = "This STL file is saved as temporary file.";
		fwrite(headerString, sizeof(char), 80, fp);
		
		// num of triangles
		unsigned int NUM_T = *num_triangles;
		fwrite(&NUM_T, sizeof(unsigned int), 1, fp);


		// normal & vertex data
		float tempNorm[3];
		float tempVert[3];
		char unusedArea[2] = { 0,0 };
		for (int i = 0; i < NUM_T; i++)
		{
			tempNorm[0] = *norm_Ptr; norm_Ptr++;
			tempNorm[1] = *norm_Ptr; norm_Ptr++;
			tempNorm[2] = *norm_Ptr; norm_Ptr++;
			norm_Ptr++;
			norm_Ptr++;
			norm_Ptr++;
			norm_Ptr++;
			norm_Ptr++;
			norm_Ptr++;

			fwrite(tempNorm, sizeof(float), 3, fp);

			// vertex
			tempVert[0] = *vert_Ptr; vert_Ptr++;
			tempVert[1] = *vert_Ptr; vert_Ptr++;
			tempVert[2] = *vert_Ptr; vert_Ptr++;

			fwrite(tempVert, sizeof(float), 3, fp);

			tempVert[0] = *vert_Ptr; vert_Ptr++;
			tempVert[1] = *vert_Ptr; vert_Ptr++;
			tempVert[2] = *vert_Ptr; vert_Ptr++;

			fwrite(tempVert, sizeof(float), 3, fp);

			tempVert[0] = *vert_Ptr; vert_Ptr++;
			tempVert[1] = *vert_Ptr; vert_Ptr++;
			tempVert[2] = *vert_Ptr; vert_Ptr++;

			fwrite(tempVert, sizeof(float), 3, fp);

			// skip 2byte
			fwrite(unusedArea, 1, 2, fp);

		}




		// close file
		fclose(fp);

		// restore pointer
		vert_Ptr = vert_origin;
		norm_Ptr = norm_origin;

	}// isValid
}

void STL::w_saveSTL(const wchar_t* w_savePath)
{
	if (isValid)
	{
		FILE* fp;

		if (_wfopen_s(&fp, w_savePath, L"wb") != 0 )
		{
			printf("can't open save file.\n");
			return;
		}

		float* vert_origin = vert_Ptr;
		float* norm_origin = norm_Ptr;


		// header data
		char headerString[80] = "This STL file is saved as temporary file.";
		fwrite(headerString, sizeof(char), 80, fp);

		// num of triangles
		unsigned int NUM_T = *num_triangles;
		fwrite(&NUM_T, sizeof(unsigned int), 1, fp);


		// normal & vertex data
		float tempNorm[3];
		float tempVert[3];
		char unusedArea[2] = { 0,0 };
		for (int i = 0; i < NUM_T; i++)
		{
			tempNorm[0] = *norm_Ptr; norm_Ptr++;
			tempNorm[1] = *norm_Ptr; norm_Ptr++;
			tempNorm[2] = *norm_Ptr; norm_Ptr++;
			norm_Ptr++;
			norm_Ptr++;
			norm_Ptr++;
			norm_Ptr++;
			norm_Ptr++;
			norm_Ptr++;

			fwrite(tempNorm, sizeof(float), 3, fp);

			// vertex
			tempVert[0] = *vert_Ptr; vert_Ptr++;
			tempVert[1] = *vert_Ptr; vert_Ptr++;
			tempVert[2] = *vert_Ptr; vert_Ptr++;

			fwrite(tempVert, sizeof(float), 3, fp);

			tempVert[0] = *vert_Ptr; vert_Ptr++;
			tempVert[1] = *vert_Ptr; vert_Ptr++;
			tempVert[2] = *vert_Ptr; vert_Ptr++;

			fwrite(tempVert, sizeof(float), 3, fp);

			tempVert[0] = *vert_Ptr; vert_Ptr++;
			tempVert[1] = *vert_Ptr; vert_Ptr++;
			tempVert[2] = *vert_Ptr; vert_Ptr++;

			fwrite(tempVert, sizeof(float), 3, fp);

			// skip 2byte
			fwrite(unusedArea, 1, 2, fp);

		}




		// close file
		fclose(fp);

		// restore pointer
		vert_Ptr = vert_origin;
		norm_Ptr = norm_origin;
	}// if isValid
}

