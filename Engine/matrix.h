//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifndef MATRIX_H
#define MATRIX_H

#include "MathUtil.h"
#include "MathTypes.h"

//MATRIX 4x4
vec3* mat4f_GetAt(const mat4f pMatrix);
vec3* mat4f_GetUp(const mat4f pMatrix);
vec3* mat4f_GetLeft(const mat4f pMatrix);
vec3* mat4f_GetPos(const mat4f pMatrix);

void mat4f_LoadIdentity(mat4f pOut_matrix);
void mat4f_LoadScaleFromVec(mat4f pOut_matrix, const vec3* v);
void mat4f_LoadScaleFromFloats(mat4f pOut_matrix, f32 scaleX, f32 scaleY, f32 scaleZ);
void mat4f_LoadScaleFromVec_IgnoreTranslation(mat4f pOut_matrix, const vec3* v);
void mat4f_LoadScale(mat4f pOut_matrix, f32 s);
void mat4f_LoadScale_IgnoreTranslation(mat4f pOut_matrix, f32 s);
void mat4f_Copy(mat4f pOut_matrix,mat4f m_copyFrom);
void mat4f_CopyRotation(mat4f pOut_matrix,mat4f m_copyFrom);
void mat4f_GetViewAt(vec3* out_viewAt, mat4f pMatrix);
void mat4f_GetViewUp(vec3* out_viewUp, mat4f pMatrix);
void mat4f_GetViewLeft(vec3* out_viewLeft, mat4f pMatrix);
void mat4f_LoadXRotation(mat4f pOut_matrix, f32 radians);
void mat4f_LoadYRotation(mat4f pOut_matrix, f32 radians);
void mat4f_LoadYRotation_IgnoreTranslation(mat4f pOut_matrix, f32 radians);
void mat4f_LoadZRotation(mat4f pOut_matrix, f32 radians);
void mat4f_LoadXRotation_IgnoreTranslation(mat4f pOut_matrix, f32 radians);
void mat4f_LoadZRotation_IgnoreTranslation(mat4f pOut_matrix, f32 radians);
void mat4f_LoadScaledZRotation(mat4f pOut_matrix, f32 radians, f32 scale);
void mat4f_LoadScaledZRotationFromVec_IgnoreTranslation(mat4f pOut_matrix, f32 radians, const vec3* v);
void mat4f_LoadScaledXRotation_IgnoreTranslation(mat4f pOut_matrix,f32 radians,f32 scale);
void mat4f_LoadScaledZRotation_IgnoreTranslation(mat4f pOut_matrix,f32 radians,f32 scale);
void mat4f_LoadScaledXRotation(mat4f pOut_matrix, f32 radians, f32 scale);
void mat4f_LoadScaledYRotation(mat4f pOut_matrix, f32 radians, f32 scale);
void mat4f_LoadScaledYRotation_IgnoreTranslation(mat4f pOut_matrix, f32 radians, f32 scale);
void mat4f_RotateY(mat4f pOut_matrix, f32 radians, bool ignoreTranslation);
void mat4f_LoadTranslation(mat4f pOut_matrix, const vec3* v);
void mat4f_LoadLookAtDir(mat4f pOut_matrix,const vec3* camPos, const vec3* at, const vec3* up);
void mat4f_LoadLookAtTarget(mat4f pOut_matrix,const vec3* camPos, const vec3* target, const vec3* up);
void mat4f_LoadPerspective(mat4f pOut_matrix, f32 fov_radians, f32 aspect, f32 zNear, f32 zFar);
void mat4f_LoadOrtho(mat4f pOut_matrix, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
void mat4f_Multiply_Self_LHS(mat4f a, const mat4f b);
void mat4f_Multiply_Self_RHS(mat4f a, const mat4f b);
void mat4f_Multiply(mat4f pOut_matrix, const mat4f a, const mat4f b);
void mat4f_SetLookAtPos(mat4f pInOut_matrix,const vec3* camPos);
void mat4f_LoadTranspose(mat4f pOut_matrix, const mat4f a);
void mat4f_LoadScaledRotationFromLeftAndUp(f32* pOut_matrix, const vec3* pLeft, const vec3* pUp, const f32 scale);
void mat4f_LoadScaledRotationFromUpAndLeft(f32* pOut_matrix, const vec3* pLeft, const vec3* pUp, const f32 scale);
void mat4f_LoadScaledRotationFromUp(f32* pOut_matrix, const vec3* pUp, const f32 scale);
void mat4f_LoadScaledRotationFromLeftAndUp_IgnoreTranslation(f32* pOut_matrix, const vec3* pLeft, const vec3* pUp, const f32 scale);
void mat4f_LoadScaledRotationFromUpAndLeft_IgnoreTranslation(f32* pOut_matrix, const vec3* pLeft, const vec3* pUp, const f32 scale);
void mat4f_LoadScaledRotationFromUp_IgnoreTranslation(f32* pOut_matrix, const vec3* pUp, const f32 scale);
//MATRIX 3x3
void mat3f_Copy(mat4f pOut_matrix,mat4f m_copyFrom);
void mat3f_Invert(mat4f pOut_matrix,mat4f pIn_Matrix);
void mat3f_CopyMat4fRotation(mat4f pOut_matrix,mat4f m_copyFrom);
void mat3f_InvertMat4fRotation(mat4f pOut_matrix,mat4f pIn_Matrix);
void mat3f_LoadIdentity(mat4f pOut_matrix);
void mat3f_LoadScaleFromVec(mat4f pOut_matrix, const vec3* v);
void mat3f_LoadScale(mat4f pOut_matrix, f32 s);
void mat3f_LoadXRotation(mat4f pOut_matrix, f32 radians);
void mat3f_LoadYRotation(mat4f pOut_matrix, f32 radians);
void mat3f_LoadZRotation(mat4f pOut_matrix, f32 radians);
void mat3f_LoadScaledZRotation(mat4f pOut_matrix, f32 radians, f32 scale);
void mat3f_Multiply_Self_LHS(mat4f a, const mat4f b);
void mat3f_Multiply_Self_RHS(mat4f a, const mat4f b);
void mat3f_Multiply(mat4f pOut_matrix, const mat4f a, const mat4f b);

#endif /* MATRIX_H */
