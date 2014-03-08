#include <iostream>
#include <cmath>
#include <cstring>

#include "MathEngine.h"

using namespace std;

void test_VECTOR2D_ZERO()
{
	VECTOR2D test = 
	{
		10, 
		10
	};
	VECTOR2D_ZERO(&test);

	if (test.x == 0 && test.y == 0)
	{
		cout << "VECTOR2D_ZERO have passed the test" << endl;
		cout << endl;
	}
	else
	{
		cout << "VECTOR2D_ZERO failed to pass the test" << endl;
		cout << endl;
	}
}

void test_VECTOR3D_ZERO()
{
	VECTOR3D test = 
	{
		10, 
		10,
		10
	};
	VECTOR3D_ZERO(&test);

	if (test.x == 0 && test.y == 0 && test.z == 0)
	{
		cout << "VECTOR3D_ZERO have passed the test" << endl;
		cout << endl;
	}
	else
	{
		cout << "VECTOR3D_ZERO failed to pass the test" << endl;
		cout << endl;
	}
}

void test_VECTOR4D_ZERO()
{
	VECTOR4D test = 
	{
		10, 
		10,
		10,
		10
	};
	VECTOR4D_ZERO(&test);

	if (test.x == 0 && test.y == 0 && test.z == 0 && test.w == 1.0)
	{
		cout << "VECTOR4D_ZERO have passed the test" << endl;
		cout << endl;
	}
	else
	{
		cout << "VECTOR4D_ZERO failed to pass the test" << endl;
		cout << endl;
	}
}



void test_VECTOR2D_INITXY();
void test_VECTOR3D_INITXYZ();
void test_VECTOR4D_INITXYZ();


void test_VECTOR2D_INIT();
void test_VECTOR3D_INIT();
void test_VECTOR4D_INIT();
void test_VECTOR2D_COPY();
void test_VECTOR3D_COPY();
void test_VECTOR4D_COPY();


void test_VECTOR4D_DIV_BY_W();
void test_VECTOR4D_DIV_BY_W_VECTOR3D();


void test_POINT2D_INIT();
void test_POINT3D_INIT();
void test_POINT4D_INIT();
void test_POINT2D_COPY();
void test_POINT3D_COPY();
void test_POINT4D_COPY();



void test_MATRIX_ZERO_2X2();
void test_MATRIX_ZERO_3X3();
void test_MATRIX_ZERO_4X4();
void test_MATRIX_ZERO_4X3();



void test_MATRIX_IDENTITY_2X2();
void test_MATRIX_IDENTITY_3X3();
void test_MATRIX_IDENTITY_4X4();
void test_MATRIX_IDENTITY_4X3();



void test_MATRIX_COPY_2X2();
void test_MATRIX_COPY_3X3();
void test_MATRIX_COPY_4X4();
void test_MATRIX_COPY_4X3();



void test_MATRIX_TRANSPOSE_3X3();
void test_MATRIX_TRANSPOSE_3X3();
void test_MATRIX_TRANSPOSE_4X4();
void test_MATRIX_TRANSPOSE_4X4();



void test_MATRIX_COLUMN_SWAP_2X2();
void test_MATRIX_COLUMN_SWAP_3X3();
void test_MATRIX_COLUMN_SWAP_4X4();
void test_MATRIX_COLUMN_SWAP_4X3();



void test_MATRIX_ROW_SWAP_2X2();
void test_MATRIX_ROW_SWAP_3X3();
void test_MATRIX_ROW_SWAP_4X4();
void test_MATRIX_ROW_SWAP_4X3();



void test_QUAT_ZERO();
void test_QUAT_INITWXYZ();
void test_QUAT_INIT_VECTOR3D();
void test_QUAT_INIT();
void test_QUAT_COPY();



void test_Build_Sin_Cos_Tables();
void test_Fast_Sin();
void test_Fast_Cos();
void test_Fast_Distance_2D();
void test_Fast_Distance_3D();




void test_POLAR2D_TO_POINT2D();
void test_POLAR2D_TO_XY();
void test_POINT2D_TO_POLAR2D();
void test_POINT2D_TO_POLAR_RTH();
void test_CYLINDRICAL3D_TO_POINT3D();
void test_CYLINDRICAL3D_TO_XYZ();
void test_POINT3D_TO_CYLINDRICAL3D();
void test_POINT3D_TO_CYLINDRICAL3D_RTHZ();
void test_SPHERICAL3D_TO_POINT3D();
void test_SPHERICAL3D_TO_XYZ();
void test_POINT3D_TO_SPHERICAL3D();
void test_POINT3D_TO_SPHERICAL3D_PTHPI();



void test_VECTOR2D_ADD();
void test_VECTOR2D_ADD();
void test_VECTOR2D_SUB();
void test_VECTOR2D_SUB();



void test_VECTOR2D_SCALE();
void test_VECTOR2D_SCALE();
void test_VECTOR2D_DOT();
void test_VECTOR2D_LENGTH();
void test_VECTOR2D_LENGTH_FAST();
void test_VECTOR2D_NORMALIZE();
void test_VECTOR2D_NORMALIZE();
void test_VECTOR2D_BUILD();
void test_VECTOR2D_COSTH();
void test_VECTOR2D_PRINT();



void test_VECTOR3D_ADD();
void test_VECTOR3D_ADD();
void test_VECTOR3D_SUB();
void test_VECTOR3D_SUB();
void test_VECTOR3D_SCALE();
void test_VECTOR3D_SCALE(); 
void test_VECTOR3D_DOT();
void test_VECTOR3D_CROSS();
void test_VECTOR3D_CROSS();
void test_VECTOR3D_LENGTH();
void test_VECTOR3D_LENGTH_FAST();
void test_VECTOR3D_NORMALIZE();
void test_VECTOR3D_NORMALIZE();
void test_VECTOR3D_BUILD();
void test_VECTOR3D_COSTH();
void test_VECTOR3D_PRINT();




void test_VECTOR4D_ADD();
void test_VECTOR4D_ADD();
void test_VECTOR4D_SUB();
void test_VECTOR4D_SUB();
void test_VECTOR4D_SCALE();
void test_VECTOR4D_SCALE();
void test_VECTOR4D_DOT();
void test_VECTOR4D_CROSS();
void test_VECTOR4D_CROSS();
void test_VECTOR4D_LENGTH();
void test_VECTOR4D_LENGTH_FAST();
void test_VECTOR4D_NORMALIZE();
void test_VECTOR4D_NORMALIZE();
void test_VECTOR4D_BUILD();
void test_VECTOR4D_COSTH();
void test_VECTOR4D_PRINT();



void test_MATRIX_INIT_2X2();
void test_MATRIX_PRINT_2X2();
void test_MATRIX_DET_2X2();
void test_MATRIX_ADD_2X2();
void test_MATRIX_MUL_2X2();
void test_MATRIX_MUL_2X2_2X1();
void test_MATRIX_INVERSE_2X2();
void test_SOLVE_SYSTEM_2X2();


void test_MATRIX_INIT_3X3();
void test_MATRIX_PRINT_3X3();
void test_MATRIX_DET_3X3();
void test_MATRIX_ADD_3X3();
void test_MATRIX_MUL_1X2_3X2();
void test_MATRIX_MUL_1X3_3X3();
void test_MATRIX_MUL_3X3();
void test_MATRIX_INVERSE_3X3();
void test_SOLVE_SYSTEM_3X3();

void test_MATRIX_INIT_4X4();			
void test_MATRIX_PRINT_4X4();
void test_MATRIX_DET_4X4();
void test_MATRIX_ADD_4X4();
void test_MATRIX_MUL_4X4();
void test_MATRIX_MUL_1X4_4X4();
void test_MATRIX_MUL_1X4_4X4();
void test_MATRIX_MUL_1X3_4X4();
void test_MATRIX_MUL_1X3_4X4();
void test_MATRIX_INVERSE_4X4();
void test_SOLVE_SYSTEM_4X4();



int main()
{
	test_VECTOR2D_ZERO();
	test_VECTOR3D_ZERO();
	test_VECTOR4D_ZERO();

	test_VECTOR2D_INITXY();
	test_VECTOR3D_INITXYZ();
	test_VECTOR4D_INITXYZ();

	test_VECTOR2D_INIT();
	test_VECTOR3D_INIT();
	test_VECTOR4D_INIT();
	test_VECTOR2D_COPY();
	test_VECTOR3D_COPY();
	test_VECTOR4D_COPY();

	test_VECTOR4D_DIV_BY_W();
	test_VECTOR4D_DIV_BY_W_VECTOR3D();

	test_POINT2D_INIT();
	test_POINT3D_INIT();
	test_POINT4D_INIT();
	test_POINT2D_COPY();
	test_POINT3D_COPY();
	test_POINT4D_COPY();

	test_MATRIX_ZERO_2X2();
	test_MATRIX_ZERO_3X3();
	test_MATRIX_ZERO_4X4();
	test_MATRIX_ZERO_4X3();

	test_MATRIX_IDENTITY_2X2();
	test_MATRIX_IDENTITY_3X3();
	test_MATRIX_IDENTITY_4X4();
	test_MATRIX_IDENTITY_4X3();

	test_MATRIX_COPY_2X2();
	test_MATRIX_COPY_3X3();
	test_MATRIX_COPY_4X4();
	test_MATRIX_COPY_4X3();

	test_MATRIX_TRANSPOSE_3X3();
	test_MATRIX_TRANSPOSE_3X3();
	test_MATRIX_TRANSPOSE_4X4();
	test_MATRIX_TRANSPOSE_4X4();

	test_MATRIX_COLUMN_SWAP_2X2();
	test_MATRIX_COLUMN_SWAP_3X3();
	test_MATRIX_COLUMN_SWAP_4X4();
	test_MATRIX_COLUMN_SWAP_4X3();

	test_MATRIX_ROW_SWAP_2X2();
	test_MATRIX_ROW_SWAP_3X3();
	test_MATRIX_ROW_SWAP_4X4();
	test_MATRIX_ROW_SWAP_4X3();

	test_QUAT_ZERO();
	test_QUAT_INITWXYZ();
	test_QUAT_INIT_VECTOR3D();
	test_QUAT_INIT();
	test_QUAT_COPY();

	test_Build_Sin_Cos_Tables();
	test_Fast_Sin();
	test_Fast_Cos();
	test_Fast_Distance_2D();
	test_Fast_Distance_3D();

	test_POLAR2D_TO_POINT2D();
	test_POLAR2D_TO_XY();
	test_POINT2D_TO_POLAR2D();
	test_POINT2D_TO_POLAR_RTH();
	test_CYLINDRICAL3D_TO_POINT3D();
	test_CYLINDRICAL3D_TO_XYZ();
	test_POINT3D_TO_CYLINDRICAL3D();
	test_POINT3D_TO_CYLINDRICAL3D_RTHZ();
	test_SPHERICAL3D_TO_POINT3D();
	test_SPHERICAL3D_TO_XYZ();
	test_POINT3D_TO_SPHERICAL3D();
	test_POINT3D_TO_SPHERICAL3D_PTHPI();

	test_VECTOR2D_ADD();
	test_VECTOR2D_ADD();
	test_VECTOR2D_SUB();
	test_VECTOR2D_SUB();

	test_VECTOR2D_SCALE();
	test_VECTOR2D_SCALE();
	test_VECTOR2D_DOT();
	test_VECTOR2D_LENGTH();
	test_VECTOR2D_LENGTH_FAST();
	test_VECTOR2D_NORMALIZE();
	test_VECTOR2D_NORMALIZE();
	test_VECTOR2D_BUILD();
	test_VECTOR2D_COSTH();
	test_VECTOR2D_PRINT();


	test_VECTOR3D_ADD();
	test_VECTOR3D_ADD();
	test_VECTOR3D_SUB();
	test_VECTOR3D_SUB();
	test_VECTOR3D_SCALE();
	test_VECTOR3D_SCALE(); 
	test_VECTOR3D_DOT();
	test_VECTOR3D_CROSS();
	test_VECTOR3D_CROSS();
	test_VECTOR3D_LENGTH();
	test_VECTOR3D_LENGTH_FAST();
	test_VECTOR3D_NORMALIZE();
	test_VECTOR3D_NORMALIZE();
	test_VECTOR3D_BUILD();
	test_VECTOR3D_COSTH();
	test_VECTOR3D_PRINT();




	test_VECTOR4D_ADD();
	test_VECTOR4D_ADD();
	test_VECTOR4D_SUB();
	test_VECTOR4D_SUB();
	test_VECTOR4D_SCALE();
	test_VECTOR4D_SCALE();
	test_VECTOR4D_DOT();
	test_VECTOR4D_CROSS();
	test_VECTOR4D_CROSS();
	test_VECTOR4D_LENGTH();
	test_VECTOR4D_LENGTH_FAST();
	test_VECTOR4D_NORMALIZE();
	test_VECTOR4D_NORMALIZE();
	test_VECTOR4D_BUILD();
	test_VECTOR4D_COSTH();
	test_VECTOR4D_PRINT();




	test_MATRIX_INIT_2X2();
	test_MATRIX_PRINT_2X2();
	test_MATRIX_DET_2X2();
	test_MATRIX_ADD_2X2();
	test_MATRIX_MUL_2X2();
	test_MATRIX_MUL_2X2_2X1();
	test_MATRIX_INVERSE_2X2();
	test_SOLVE_SYSTEM_2X2();


	test_MATRIX_INIT_3X3();
	test_MATRIX_PRINT_3X3();
	test_MATRIX_DET_3X3();
	test_MATRIX_ADD_3X3();
	test_MATRIX_MUL_1X2_3X2();
	test_MATRIX_MUL_1X3_3X3();
	test_MATRIX_MUL_3X3();
	test_MATRIX_INVERSE_3X3();
	test_SOLVE_SYSTEM_3X3();

	test_MATRIX_INIT_4X4();			
	test_MATRIX_PRINT_4X4();
	test_MATRIX_DET_4X4();
	test_MATRIX_ADD_4X4();
	test_MATRIX_MUL_4X4();
	test_MATRIX_MUL_1X4_4X4();
	test_MATRIX_MUL_1X4_4X4();
	test_MATRIX_MUL_1X3_4X4();
	test_MATRIX_MUL_1X3_4X4();
	test_MATRIX_INVERSE_4X4();
	test_SOLVE_SYSTEM_4X4();
	
	
	
}	
	