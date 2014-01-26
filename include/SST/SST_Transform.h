#ifndef __SST_TRANSFORM_H__
#define __SST_TRANSFORM_H__

#ifdef __cplusplus
extern "C" {
#endif

void SST_Math_Mat33fCreateIdentity(SST_Mat33f* Mat);
void SST_Math_Mat44fCreateIdentity(SST_Mat44f* Mat);
void SST_Math_Mat33fCreateTranslation(SST_Mat33f* Mat, const float x, const float y);
void SST_Math_Mat44fCreateTranslation(SST_Mat44f* Mat, const float x, const float y, const float z);
void SST_Math_Mat44fCreateEulerX(SST_Mat44f* Mat, const float angle);
void SST_Math_Mat44fCreateEulerY(SST_Mat44f* Mat, const float angle);
void SST_Math_Mat44fCreateEulerZ(SST_Mat44f* Mat, const float angle);
void SST_Math_Mat44fCreateEulerXC(SST_Mat44f* Mat, const float angle);
void SST_Math_Mat44fCreateEulerYC(SST_Mat44f* Mat, const float angle);
void SST_Math_Mat44fCreateEulerZC(SST_Mat44f* Mat, const float angle);
void SST_Math_Mat44fCreateShear(SST_Mat44f* Mat, const float x_in_y, const float x_in_z, const float y_in_x, const float y_in_z, const float z_in_x, const float z_in_y);
void SST_Math_Mat44fCreateFreeTransform(SST_Mat44f* Mat,const float scale_x, const float scale_y, const float scale_z, const float x_in_y, const float x_in_z, const float y_in_x, const float y_in_z, const float z_in_x, const float z_in_y);
void SST_Math_Mat44fConvertQuaternion(SST_Mat44f* Mat,const SST_Vec4f* q);
void SST_Math_Mat44fCreateRotationFromQuaternion(SST_Mat44f* Mat,const SST_Vec4f* q);
void SST_Math_Mat44fCreateScale(SST_Mat44f* Mat, const float scale_x, const float scale_y, const float scale_z);
void SST_Math_Mat44fCreateLookAt(SST_Mat44f* Mat, const SST_Vec3f* eye,const SST_Vec3f* at, const SST_Vec3f* up);
void SST_Math_Mat44fCreateLookDir(SST_Mat44f* Mat, const SST_Vec3f* eye,const SST_Vec3f* dir, const SST_Vec3f* up);
void SST_Math_Mat33fCreateFromMat44f(const SST_Mat44f* Mat44, SST_Mat33f* Mat33);
void SST_Math_Mat44fCreatePerspective(SST_Mat44f* Mat, const float FOV_in_y, const float aspect_in_x, const float zNear, const float zFar);
void SST_Math_Mat44fCreateOrtho(SST_Mat44f* Mat, const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal);
void SST_Math_Mat33fFromVec3f(SST_Mat33f* Mat, const SST_Vec3f* v, const int column);
void SST_Math_Mat44fFromVec4f(SST_Mat44f* Mat, const SST_Vec4f* v, const int column);
void SST_Math_Mat33dFromVec3d(SST_Mat33d* Mat, const SST_Vec3d* v, const int column);
void SST_Math_Mat44dFromVec4d(SST_Mat44d* Mat, const SST_Vec4d* v, const int column);

#ifdef __cplusplus
}
#endif

#endif
