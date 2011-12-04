//Author: Jody McAdams
//Website: http://www.jodymcadams.com
//Company Website: http://www.gamejellyonline.com

#ifndef MATRIX_H
#define MATRIX_H

#include "MathUtil.h"
#include "MathTypes.h"

//MATRIX 4x4
vec3* mat4f_GetAt(f32* pMatrix);
vec3* mat4f_GetUp(f32* pMatrix);
vec3* mat4f_GetLeft(f32* pMatrix);
vec3* mat4f_GetPos(f32* pMatrix);

void mat4f_LoadIdentity(f32* pOut_matrix);
void mat4f_LoadScaleFromVec(f32* pOut_matrix, const vec3* v);
void mat4f_LoadScaleFromVec_IgnoreTranslation(f32* pOut_matrix, const vec3* v);
void mat4f_LoadScale(f32* pOut_matrix, f32 s);
void mat4f_LoadScale_IgnoreTranslation(f32* pOut_matrix, f32 s);
void mat4f_Copy(f32* pOut_matrix,f32* m_copyFrom);
void mat4f_CopyRotation(f32* pOut_matrix,f32* m_copyFrom);
void mat4f_GetViewAt(vec3* out_viewAt, f32* pMatrix);
void mat4f_GetViewUp(vec3* out_viewUp, f32* pMatrix);
void mat4f_GetViewLeft(vec3* out_viewLeft, f32* pMatrix);
void mat4f_LoadXRotation(f32* pOut_matrix, f32 radians);
void mat4f_LoadYRotation(f32* pOut_matrix, f32 radians);
void mat4f_LoadZRotation(f32* pOut_matrix, f32 radians);
void mat4f_LoadXRotation_IgnoreTranslation(f32* pOut_matrix, f32 radians);
void mat4f_LoadZRotation_IgnoreTranslation(f32* pOut_matrix, f32 radians);
void mat4f_LoadScaledZRotation(f32* pOut_matrix, f32 radians, f32 scale);
void mat4f_LoadScaledZRotationFromVec_IgnoreTranslation(f32* pOut_matrix, f32 radians, const vec3* v);
void mat4f_LoadScaledXRotation_IgnoreTranslation(f32* pOut_matrix,f32 radians,f32 scale);
void mat4f_LoadScaledZRotation_IgnoreTranslation(f32* pOut_matrix,f32 radians,f32 scale);
void mat4f_LoadScaledXRotation(f32* pOut_matrix, f32 radians, f32 scale);
void mat4f_LoadScaledYRotation(f32* pOut_matrix, f32 radians, f32 scale);
void mat4f_RotateY(f32* pOut_matrix, f32 radians, bool ignoreTranslation);
void mat4f_LoadTranslation(f32* pOut_matrix, const vec3* v);
void mat4f_LoadLookAtDir(f32* pOut_matrix,const vec3* camPos, const vec3* at, const vec3* up);
void mat4f_LoadLookAtTarget(f32* pOut_matrix,const vec3* camPos, const vec3* target, const vec3* up);
void mat4f_LoadPerspective(f32* pOut_matrix, f32 fov_radians, f32 aspect, f32 zNear, f32 zFar);
void mat4f_LoadOrtho(f32* pOut_matrix, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
void mat4f_Multiply_Self_LHS(f32* a, const f32* b);
void mat4f_Multiply_Self_RHS(f32* a, const f32* b);
void mat4f_Multiply(f32* pOut_matrix, const f32* a, const f32* b);
void mat4f_SetLookAtPos(f32* pInOut_matrix,const vec3* camPos);

//MATRIX 3x3
void mat3f_Copy(f32* pOut_matrix,f32* m_copyFrom);
void mat3f_Invert(f32* pOut_matrix,f32* pIn_Matrix);
void mat3f_CopyMat4fRotation(f32* pOut_matrix,f32* m_copyFrom);
void mat3f_InvertMat4fRotation(f32* pOut_matrix,f32* pIn_Matrix);
void mat3f_LoadIdentity(f32* pOut_matrix);
void mat3f_LoadScaleFromVec(f32* pOut_matrix, const vec3* v);
void mat3f_LoadScale(f32* pOut_matrix, f32 s);
void mat3f_LoadXRotation(f32* pOut_matrix, f32 radians);
void mat3f_LoadYRotation(f32* pOut_matrix, f32 radians);
void mat3f_LoadZRotation(f32* pOut_matrix, f32 radians);
void mat3f_LoadScaledZRotation(f32* pOut_matrix, f32 radians, f32 scale);
void mat3f_Multiply_Self_LHS(f32* a, const f32* b);
void mat3f_Multiply_Self_RHS(f32* a, const f32* b);
void mat3f_Multiply(f32* pOut_matrix, const f32* a, const f32* b);

#endif /* MATRIX_H */
