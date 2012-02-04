//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#include <string.h>
#include <math.h>
#include "matrix.h"


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadIdentity(mat4f pOut_matrix)
{
	pOut_matrix[0] = 1.0f;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = 1.0f;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = 1.0f;
	pOut_matrix[11] = 0.0f;	

	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	pOut_matrix[14] = 0.0f;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadTranspose(mat4f pOut_matrix, const mat4f a)
{
	pOut_matrix[0] = a[0];
	pOut_matrix[1] = a[4];
	pOut_matrix[2] = a[8];
	pOut_matrix[3] = a[12];
	
	pOut_matrix[4] = a[1];
	pOut_matrix[5] = a[5];
	pOut_matrix[6] = a[9];
	pOut_matrix[7] = a[13];
	
	pOut_matrix[8] = a[2];
	pOut_matrix[9] = a[6];
	pOut_matrix[10] = a[10];
	pOut_matrix[11] = a[15];	
	
	pOut_matrix[12] = a[3];
	pOut_matrix[13] = a[7];
	pOut_matrix[14] = a[11];
	pOut_matrix[15] = a[15];
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaleFromFloats(mat4f pOut_matrix, f32 scaleX, f32 scaleY, f32 scaleZ)
{
	pOut_matrix[0] = scaleX;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = scaleY;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = scaleZ;
	pOut_matrix[11] = 0.0f;	
	
	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	pOut_matrix[14] = 0.0f;
	pOut_matrix[15] = 1.0f;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaleFromVec(mat4f pOut_matrix, const vec3* v)
{
	pOut_matrix[0] = v->x;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = v->y;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = v->z;
	pOut_matrix[11] = 0.0f;	
	
	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	pOut_matrix[14] = 0.0f;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaleFromVec_IgnoreTranslation(mat4f pOut_matrix, const vec3* v)
{
	pOut_matrix[0] = v->x;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = v->y;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = v->z;
	pOut_matrix[11] = 0.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScale(mat4f pOut_matrix, f32 s)
{
	pOut_matrix[0] = s;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = s;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = s;
	pOut_matrix[11] = 0.0f;	
	
	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	pOut_matrix[14] = 0.0f;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScale_IgnoreTranslation(mat4f pOut_matrix, f32 s)
{
	pOut_matrix[0] = s;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = s;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = s;
	pOut_matrix[11] = 0.0f;	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadXRotation(mat4f pOut_matrix, f32 radians)
{
	f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = 1.0f;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = cosrad;
	pOut_matrix[6] = sinrad;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = -sinrad;
	pOut_matrix[10] = cosrad;
	pOut_matrix[11] = 0.0f;	
	
	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	pOut_matrix[14] = 0.0f;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadXRotation_IgnoreTranslation(mat4f pOut_matrix, f32 radians)
{
    f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = 1.0f;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = cosrad;
	pOut_matrix[6] = sinrad;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = -sinrad;
	pOut_matrix[10] = cosrad;
	pOut_matrix[11] = 0.0f;	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadYRotation(mat4f pOut_matrix, f32 radians)
{
	f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = cosrad;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = -sinrad;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = 1.0f;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = sinrad;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = cosrad;
	pOut_matrix[11] = 0.0f;	
	
	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	pOut_matrix[14] = 0.0f;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadYRotation_IgnoreTranslation(mat4f pOut_matrix, f32 radians)
{
	f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = cosrad;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = -sinrad;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = 1.0f;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = sinrad;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = cosrad;
	pOut_matrix[11] = 0.0f;	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_RotateY(mat4f pInOut_Matrix, f32 radians, bool ignoreTranslation)
{
	f32 tempMat[16];
	mat4f_LoadYRotation(tempMat, radians);
	
	vec3 oldPos;
	if (ignoreTranslation)
	{
		CopyVec3(&oldPos, mat4f_GetPos(pInOut_Matrix));
		SetVec3(mat4f_GetPos(pInOut_Matrix), 0.0f, 0.0f, 0.0f);
	}
	
	mat4f_Multiply_Self_LHS(pInOut_Matrix, tempMat);
	
	if (ignoreTranslation)
	{
		CopyVec3(mat4f_GetPos(pInOut_Matrix), &oldPos);
	}
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadZRotation(mat4f pOut_matrix, f32 radians)
{
	f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = cosrad;
	pOut_matrix[1] = sinrad;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = -sinrad;
	pOut_matrix[5] = cosrad;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = 1.0f;
	pOut_matrix[11] = 0.0f;	
	
	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	pOut_matrix[14] = 0.0f;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_SetLookAtPos(mat4f pInOut_matrix,const vec3* camPos)
{
    vec3 mat_left;
    vec3 mat_up;
    vec3 mat_at;
    
    mat4f_GetViewAt(&mat_at, pInOut_matrix);
    mat4f_GetViewUp(&mat_up, pInOut_matrix);
    mat4f_GetViewLeft(&mat_left, pInOut_matrix);
    
    //Create view matrix
	const f32 dotLeft = DotVec3(&mat_left, camPos);
	const f32 dotUp = DotVec3(&mat_up, camPos);
	const f32 dotAt = DotVec3(&mat_at, camPos);
    
	pInOut_matrix[12]= -dotLeft;		pInOut_matrix[13]= -dotUp;		pInOut_matrix[14] = -dotAt;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadZRotation_IgnoreTranslation(mat4f pOut_matrix, f32 radians)
{
    f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = cosrad;
	pOut_matrix[1] = sinrad;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = -sinrad;
	pOut_matrix[5] = cosrad;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = 1.0f;
	pOut_matrix[11] = 0.0f;	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaledZRotation(mat4f pOut_matrix, f32 radians, f32 scale)
{
	f32 cosrad = cosf(radians)*scale;
	f32 sinrad = sinf(radians)*scale;
	
	pOut_matrix[0] = cosrad;
	pOut_matrix[1] = sinrad;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = -sinrad;
	pOut_matrix[5] = cosrad;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = scale;
	pOut_matrix[11] = 0.0f;	
	
	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	pOut_matrix[14] = 0.0f;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaledYRotation(mat4f pOut_matrix, f32 radians, f32 scale)
{
	f32 cosrad = cosf(radians)*scale;
	f32 sinrad = sinf(radians)*scale;
	
	pOut_matrix[0] = cosrad;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = -sinrad;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = scale;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = sinrad;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = cosrad;
	pOut_matrix[11] = 0.0f;	
	
	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	pOut_matrix[14] = 0.0f;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaledYRotation_IgnoreTranslation(mat4f pOut_matrix, f32 radians, f32 scale)
{
	f32 cosrad = cosf(radians)*scale;
	f32 sinrad = sinf(radians)*scale;
	
	pOut_matrix[0] = cosrad;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = -sinrad;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = scale;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = sinrad;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = cosrad;
	pOut_matrix[11] = 0.0f;	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaledXRotation(mat4f pOut_matrix, f32 radians, f32 scale)
{
	f32 cosrad = cosf(radians)*scale;
	f32 sinrad = sinf(radians)*scale;
	
	pOut_matrix[0] = scale;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = cosrad;
	pOut_matrix[6] = sinrad;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = -sinrad;
	pOut_matrix[10] = cosrad;
	pOut_matrix[11] = 0.0f;	
	
	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	pOut_matrix[14] = 0.0f;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaledXRotation_IgnoreTranslation(mat4f pOut_matrix,f32 radians,f32 scale)
{
    f32 cosrad = cosf(radians)*scale;
	f32 sinrad = sinf(radians)*scale;
	
	pOut_matrix[0] = scale;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = cosrad;
	pOut_matrix[6] = sinrad;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = -sinrad;
	pOut_matrix[10] = cosrad;
	pOut_matrix[11] = 0.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaledZRotationFromVec_IgnoreTranslation(mat4f pOut_matrix, f32 radians, const vec3* v)
{
	f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = cosrad*v->x;
	pOut_matrix[1] = sinrad*v->x;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = -sinrad*v->y;
	pOut_matrix[5] = cosrad*v->y;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = v->z;
	pOut_matrix[11] = 0.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaledZRotation_IgnoreTranslation(mat4f pOut_matrix, f32 radians, f32 scale)
{
    f32 cosrad = cosf(radians)*scale;
	f32 sinrad = sinf(radians)*scale;
	
	pOut_matrix[0] = cosrad;
	pOut_matrix[1] = sinrad;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = -sinrad;
	pOut_matrix[5] = cosrad;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = scale;
	pOut_matrix[11] = 0.0f;	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadTranslation(mat4f pOut_matrix, const vec3* v)
{
	pOut_matrix[0] = 1.0f;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = 1.0f;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = 1.0f;
	pOut_matrix[11] = 0.0f;	
	
	pOut_matrix[12] = v->x;
	pOut_matrix[13] = v->y;
	pOut_matrix[14] = v->z;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaledRotationFromLeftAndUp_IgnoreTranslation(f32* pOut_matrix, const vec3* pLeft, const vec3* pUp, const f32 scale)
{
	vec3 mat_up;
	CopyVec3(&mat_up,pUp);
	
	vec3 mat_left;
	CopyVec3(&mat_left,pLeft);
	
	//Create at
	vec3 mat_at;
	CrossVec3(&mat_at, pUp, &mat_left);
	NormalizeVec3_Self(&mat_at);
	
	//Create up
	CrossVec3(&mat_up, &mat_left, &mat_at);
	
	ScaleVec3_Self(&mat_up,scale);
	ScaleVec3_Self(&mat_at,scale);
	ScaleVec3_Self(&mat_left,scale);
	
	pOut_matrix[0] =  mat_left.x;	pOut_matrix[1] =  mat_up.x;	pOut_matrix[2]  =  mat_at.x;	pOut_matrix[3]  = 0.0f;	
	pOut_matrix[4] =  mat_left.y;	pOut_matrix[5] =  mat_up.y;	pOut_matrix[6]  =  mat_at.y;	pOut_matrix[7]  = 0.0f;
	pOut_matrix[8] =  mat_left.z;	pOut_matrix[9] =  mat_up.z;	pOut_matrix[10] =  mat_at.z;	pOut_matrix[11] = 0.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadScaledRotationFromUp_IgnoreTranslation(f32* pOut_matrix, const vec3* pUp, const f32 scale)
{
	vec3 mat_up;
	CopyVec3(&mat_up,pUp);
	
	vec3 mat_left;
	CopyVec3(&mat_left,&vec3_left);
	
	//Create at
	vec3 mat_at;
	CrossVec3(&mat_at, pUp, &mat_left);
	NormalizeVec3_Self(&mat_at);
	
	//Create left
	CrossVec3(&mat_left, &mat_up, &mat_at);
	
	ScaleVec3_Self(&mat_up,scale);
	ScaleVec3_Self(&mat_at,scale);
	ScaleVec3_Self(&mat_left,scale);
	
	pOut_matrix[0] =  mat_left.x;	pOut_matrix[1] =  mat_up.x;	pOut_matrix[2]  =  mat_at.x;	pOut_matrix[3]  = 0.0f;	
	pOut_matrix[4] =  mat_left.y;	pOut_matrix[5] =  mat_up.y;	pOut_matrix[6]  =  mat_at.y;	pOut_matrix[7]  = 0.0f;
	pOut_matrix[8] =  mat_left.z;	pOut_matrix[9] =  mat_up.z;	pOut_matrix[10] =  mat_at.z;	pOut_matrix[11] = 0.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
vec3* mat4f_GetLeft(const mat4f pMatrix)
{
	return (vec3*)&pMatrix[0];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
vec3* mat4f_GetUp(const mat4f pMatrix)
{
	return (vec3*)&pMatrix[4];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
vec3* mat4f_GetAt(const mat4f pMatrix)
{
	return (vec3*)&pMatrix[8];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
vec3* mat4f_GetPos(const mat4f pMatrix)
{
	return (vec3*)&pMatrix[12];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_GetViewLeft(vec3* out_viewLeft, mat4f pMatrix)
{
	out_viewLeft->x = pMatrix[0];
	out_viewLeft->y = pMatrix[4];
	out_viewLeft->z = pMatrix[8];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_GetViewUp(vec3* out_viewUp, mat4f pMatrix)
{
	out_viewUp->x = pMatrix[1];
	out_viewUp->y = pMatrix[5];
	out_viewUp->z = pMatrix[9];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_GetViewAt(vec3* out_viewAt, mat4f pMatrix)
{
	out_viewAt->x = pMatrix[2];
	out_viewAt->y = pMatrix[6];
	out_viewAt->z = pMatrix[10];
}


//----------------------------------------------------------------------------
//Create view matrix
// mout[0] = mat_left.x;
// mout[1] = mat_up.x;
// mout[2] = mat_at.x;
// mout[3] = 0.0f;
 
// mout[4] = mat_left.y;
// mout[5] = mat_up.y;
// mout[6] = mat_at.y;
// mout[7] = 0.0f;
 
// mout[8] = mat_left.z;
// mout[9] = mat_up.z;
// mout[10] = mat_at.z;
// mout[11] = 0.0f;
 
// mout[12] = -DotVec3(&mat_left, camPos);
// mout[13] = -DotVec3(&mat_up, camPos);
// mout[14] = -DotVec3(&mat_at, camPos);
// mout[15] = 1.0f;
//----------------------------------------------------------------------------
void mat4f_LoadLookAtDir(mat4f pOut_matrix, const vec3* camPos, const vec3* at, const vec3* up)
{
	vec3 mat_at;
	CopyVec3(&mat_at,at);
	//ScaleVec3_Self(&mat_at, -1.0f);
	
	NormalizeVec3_Self(&mat_at);
	
	vec3 mat_up;
	vec3 mat_left;
	
	//Create left
	CrossVec3(&mat_left, up, &mat_at);
	NormalizeVec3_Self(&mat_left);
	
	//Create up
	CrossVec3(&mat_up, &mat_at, &mat_left);
	 
	//Create view matrix
	f32 dotLeft = DotVec3(&mat_left, camPos);
	f32 dotUp = DotVec3(&mat_up, camPos);
	f32 dotAt = DotVec3(&mat_at, camPos);

	pOut_matrix[0] =  mat_left.x;	pOut_matrix[1] =  mat_up.x;	pOut_matrix[2]  =  mat_at.x;	pOut_matrix[3]  = 0.0f;	
	pOut_matrix[4] =  mat_left.y;	pOut_matrix[5] =  mat_up.y;	pOut_matrix[6]  =  mat_at.y;	pOut_matrix[7]  = 0.0f;
	pOut_matrix[8] =  mat_left.z;	pOut_matrix[9] =  mat_up.z;	pOut_matrix[10] =  mat_at.z;	pOut_matrix[11] = 0.0f;
	pOut_matrix[12]= -dotLeft;		pOut_matrix[13]= -dotUp;		pOut_matrix[14] = -dotAt;		pOut_matrix[15] = 1.0f;
	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadLookAtTarget(mat4f pOut_matrix, const vec3* camPos, const vec3* target, const vec3* up)
{
	vec3 mat_at;

	//Create at
	SubVec3(&mat_at,camPos,target);
	
	return mat4f_LoadLookAtDir(pOut_matrix, camPos,&mat_at,up);
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadPerspective(mat4f pOut_matrix, f32 fov_radians, f32 aspect, f32 zNear, f32 zFar)
{
	f32 f = 1.0f / tanf(fov_radians/2.0f);
	
	pOut_matrix[0] = f / aspect;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = f;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	
	pOut_matrix[10] = zFar/(zNear-zFar);
	pOut_matrix[11] = -1.0f;
	
	pOut_matrix[12] = 0.0f;
	pOut_matrix[13] = 0.0f;
	
	pOut_matrix[14] = zNear*zFar/(zNear-zFar);
	pOut_matrix[15] = 0.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_LoadOrtho(mat4f pOut_matrix, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
	f32 r_l = right - left;
	f32 t_b = top - bottom;
	f32 f_n = far - near;
	f32 tx = - (right + left) / (right - left);
	f32 ty = - (top + bottom) / (top - bottom);
	f32 tz = - (far + near) / (far - near);

	pOut_matrix[0] = 2.0f / r_l;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	pOut_matrix[3] = 0.0f;
	
	pOut_matrix[4] = 0.0f;
	pOut_matrix[5] = 2.0f / t_b;
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	
	pOut_matrix[8] = 0.0f;
	pOut_matrix[9] = 0.0f;
	pOut_matrix[10] = -2.0f / f_n;
	pOut_matrix[11] = 0.0f;
	
	pOut_matrix[12] = tx;
	pOut_matrix[13] = ty;
	pOut_matrix[14] = tz;
	pOut_matrix[15] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_Multiply(mat4f pOut_matrix, const mat4f a, const mat4f b)
{
#if DEBUG
    if(pOut_matrix == a || pOut_matrix == b)
    {
        printf("INSANE ERROR!  You are using mat4f_Multiply incorrectly! pOut_matrix is overwriting itself!\n");
    }
#endif
    
    pOut_matrix[0] = a[0] * b[0]  + a[4] * b[1]  + a[8] * b[2]   + a[12] * b[3];
	pOut_matrix[1] = a[1] * b[0]  + a[5] * b[1]  + a[9] * b[2]   + a[13] * b[3];
	pOut_matrix[2] = a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3];
	pOut_matrix[3] = a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3];
	pOut_matrix[4] = a[0] * b[4]  + a[4] * b[5]  + a[8] * b[6]   + a[12] * b[7];
	pOut_matrix[5] = a[1] * b[4]  + a[5] * b[5]  + a[9] * b[6]   + a[13] * b[7];
	pOut_matrix[6] = a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7];
	pOut_matrix[7] = a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7];
	pOut_matrix[8] = a[0] * b[8]  + a[4] * b[9]  + a[8] * b[10]  + a[12] * b[11];
	pOut_matrix[9] = a[1] * b[8]  + a[5] * b[9]  + a[9] * b[10]  + a[13] * b[11];
	pOut_matrix[10] = a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11];
	pOut_matrix[11] = a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11];
	pOut_matrix[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14]  + a[12] * b[15];
	pOut_matrix[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14]  + a[13] * b[15];
	pOut_matrix[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
	pOut_matrix[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_Multiply_Self_LHS(mat4f a, const mat4f b)
{
    mat4f tempMat;
    mat4f_Multiply(tempMat,a,b);
    mat4f_Copy(a,tempMat);	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_Multiply_Self_RHS(mat4f a, const mat4f b)
{
    mat4f tempMat;
    mat4f_Multiply(tempMat,b,a);
    mat4f_Copy(a,tempMat);	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_Copy(mat4f pOut_matrix,mat4f m_copyFrom)
{
	for(int i=0; i<16; ++i)
	{
		pOut_matrix[i] = m_copyFrom[i];
	}
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat4f_CopyRotation(mat4f pOut_matrix,mat4f m_copyFrom)
{
	for(int i=0; i<12; ++i)
	{
		pOut_matrix[i] = m_copyFrom[i];
	}
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_Copy(mat4f pOut_matrix,mat4f m_copyFrom)
{
	for(int i=0; i<9; ++i)
	{
		pOut_matrix[i] = m_copyFrom[i];
	}
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_CopyMat4fRotation(mat4f pOut_matrix,mat4f m_copyFrom)
{
	pOut_matrix[0] = m_copyFrom[0];
	pOut_matrix[1] = m_copyFrom[1];
	pOut_matrix[2] = m_copyFrom[2];
    
	pOut_matrix[3] = m_copyFrom[4];
	pOut_matrix[4] = m_copyFrom[5];
	pOut_matrix[5] = m_copyFrom[6];
    
	pOut_matrix[6] = m_copyFrom[8];
	pOut_matrix[7] = m_copyFrom[9];
	pOut_matrix[8] = m_copyFrom[10];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_InvertMat4fRotation(mat4f pOut_matrix,mat4f pIn_Matrix)
{
    pOut_matrix[0] = pIn_Matrix[0];
	pOut_matrix[1] = pIn_Matrix[4];
	pOut_matrix[2] = pIn_Matrix[8];
    
	pOut_matrix[3] = pIn_Matrix[1];
	pOut_matrix[4] = pIn_Matrix[5];
	pOut_matrix[5] = pIn_Matrix[9];
    
	pOut_matrix[6] = pIn_Matrix[2];
	pOut_matrix[7] = pIn_Matrix[6];
	pOut_matrix[8] = pIn_Matrix[10];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_Invert(mat4f pOut_matrix,mat4f pIn_Matrix)
{
    pOut_matrix[0] = pIn_Matrix[0];
	pOut_matrix[1] = pIn_Matrix[3];
	pOut_matrix[2] = pIn_Matrix[6];
    
	pOut_matrix[3] = pIn_Matrix[1];
	pOut_matrix[4] = pIn_Matrix[4];
	pOut_matrix[5] = pIn_Matrix[7];
    
	pOut_matrix[6] = pIn_Matrix[2];
	pOut_matrix[7] = pIn_Matrix[5];
	pOut_matrix[8] = pIn_Matrix[8];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_LoadIdentity(mat4f pOut_matrix)
{
    pOut_matrix[0] = 1.0f;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	
	pOut_matrix[3] = 0.0f;
	pOut_matrix[4] = 1.0f;
	pOut_matrix[5] = 0.0f;
	
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	pOut_matrix[8] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_LoadScaleFromVec(mat4f pOut_matrix, const vec3* v)
{
    pOut_matrix[0] = v->x;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;

	pOut_matrix[3] = 0.0f;
	pOut_matrix[4] = v->y;
	pOut_matrix[5] = 0.0f;

	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	pOut_matrix[8] = v->z;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_LoadScale(mat4f pOut_matrix, f32 s)
{
    pOut_matrix[0] = s;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;
	
	pOut_matrix[3] = 0.0f;
	pOut_matrix[4] = s;
	pOut_matrix[5] = 0.0f;
	
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	pOut_matrix[8] = s;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_LoadXRotation(mat4f pOut_matrix, f32 radians)
{
    f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = 1.0f;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = 0.0f;

	pOut_matrix[3] = 0.0f;
	pOut_matrix[4] = cosrad;
	pOut_matrix[5] = sinrad;

	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = -sinrad;
	pOut_matrix[8] = cosrad;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_LoadYRotation(mat4f pOut_matrix, f32 radians)
{
    f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = cosrad;
	pOut_matrix[1] = 0.0f;
	pOut_matrix[2] = -sinrad;
	
	pOut_matrix[3] = 0.0f;
	pOut_matrix[4] = 1.0f;
	pOut_matrix[5] = 0.0f;
	
	pOut_matrix[6] = sinrad;
	pOut_matrix[7] = 0.0f;
	pOut_matrix[8] = cosrad;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_LoadZRotation(mat4f pOut_matrix, f32 radians)
{
    f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = cosrad;
	pOut_matrix[1] = sinrad;
	pOut_matrix[2] = 0.0f;
	
	pOut_matrix[3] = -sinrad;
	pOut_matrix[4] = cosrad;
	pOut_matrix[5] = 0.0f;
	
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	pOut_matrix[8] = 1.0f;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_LoadScaledZRotation(mat4f pOut_matrix, f32 radians, f32 scale)
{
    f32 cosrad = cosf(radians);
	f32 sinrad = sinf(radians);
	
	pOut_matrix[0] = cosrad*scale;
	pOut_matrix[1] = sinrad*scale;
	pOut_matrix[2] = 0.0f;
	
	pOut_matrix[3] = -sinrad*scale;
	pOut_matrix[4] = cosrad*scale;
	pOut_matrix[5] = 0.0f;
	
	pOut_matrix[6] = 0.0f;
	pOut_matrix[7] = 0.0f;
	pOut_matrix[8] = scale;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_Multiply(mat4f pOut_matrix, const mat4f a, const mat4f b)
{
    #if DEBUG
        if(pOut_matrix == a || pOut_matrix == b)
        {
            printf("INSANE ERROR!  You are using mat3f_Multiply incorrectly! pOut_matrix is overwriting itself!\n");
        }
    #endif
    
    pOut_matrix[0] = a[0] * b[0]  + a[3] * b[1]  + a[6] * b[2];
	pOut_matrix[1] = a[1] * b[0]  + a[4] * b[1]  + a[7] * b[2];
	pOut_matrix[2] = a[2] * b[0]  + a[5] * b[1]  + a[8] * b[2];
	pOut_matrix[3] = a[0] * b[3]  + a[3] * b[4]  + a[6] * b[5];
	pOut_matrix[4] = a[1] * b[3]  + a[4] * b[4]  + a[7] * b[5];
	pOut_matrix[5] = a[2] * b[3]  + a[5] * b[4]  + a[8] * b[5];
	pOut_matrix[6] = a[0] * b[6]  + a[3] * b[7]  + a[6] * b[8];
	pOut_matrix[7] = a[1] * b[6]  + a[4] * b[7]  + a[7] * b[8];
	pOut_matrix[8] = a[2] * b[6]  + a[5] * b[7]  + a[8] * b[8];
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_Multiply_Self_LHS(mat4f a, const mat4f b)
{
    mat3f tempMat;
    mat3f_Multiply(tempMat,a,b);
    mat3f_Copy(a,tempMat);	
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void mat3f_Multiply_Self_RHS(mat4f a, const mat4f b)
{
    mat3f tempMat;
    mat3f_Multiply(tempMat,b,a);
    mat3f_Copy(a,tempMat);	
}
