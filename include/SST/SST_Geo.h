#ifndef __SST_GEO_H__
#define __SST_GEO_H__

#ifdef __cplusplus
extern "C" {
#endif

bool SST_Geo_RayPlaneIntersectionF(const SST_Vec3f* RESTRICT ray, const SST_Vec3f* RESTRICT ray_point, const SST_Vec3f* RESTRICT p0, const SST_Vec3f* RESTRICT p1,const SST_Vec3f* RESTRICT p2, SST_Vec3f* RESTRICT Intersection);
bool SST_Geo_RayPlaneIntersectionD(const SST_Vec3d* RESTRICT ray, const SST_Vec3d* RESTRICT ray_point, const SST_Vec3d* RESTRICT p0, const SST_Vec3d* RESTRICT p1,const SST_Vec3d* RESTRICT p2, SST_Vec3d* RESTRICT Intersection);

#ifdef __cplusplus
}
#endif

#endif
