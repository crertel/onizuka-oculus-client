/* 
File: SST_VectorN.h

Author: Charles Lena <cmlena@762studios.com>

Purpose: Function prototypes for general N size

LICENSE: DWTF 1.0


*/
#ifndef __SST_VECTOR_N__
#define __SST_VECTOR_N__

float  SST_VectorNf_Magnitude(const int N, const float* _in);
float  SST_VectorNf_MagnitudeSquared(const int N, const float* _in);
float  SST_VectorNf_L1Norm(const int N, const float* _in);
float  SST_VectorNf_Dot(const int N, const float* _a, const float* _b);
float* SST_VectorNf_Normalize(const int N, const float* _in, float* _out);
float* SST_VectorNf_NormalizeLocal(const int N,  float* _inout);
float* SST_VectorNf_Add(const int N, const float* _a, const float* _b, float* _out);
float* SST_VectorNf_Subtract(const int N, const float* _a, const float* _b, float* _out);
float* SST_VectorNf_AddLocal(const int N, float* _a, const float* _b);
float* SST_VectorNf_Cross(const int N, const float* _a, const float* _b, float* _out);
float* SST_VectorNf_CrossLocal(const int N, float* _a, const float* _b);
float* SST_VectorNf_Scale(const int N, const float* _a, const float _scale, float* _out); 
float* SST_VectorNf_ScaleLocal(const int N, float* _a, const float _scale);
float* SST_VectorNf_RotateAbout(const int N, const float* _a, const float* _about,float _theta, float* _out);
float* SST_VectorNf_RotateAboutLocal(const int N, float* _a, const float* _about,float _theta);

double*   SST_VectorNd_Normalize(const int N, const double* _in, double* _out);
double*   SST_VectorNd_NormalizeLocal(const int N,  double* _inout);
double    SST_VectorNd_Magnitude(const int N, const double* _in);
double    SST_VectorNd_MagnitudeSquared(const int N, const double* _in);
double    SST_VectorNd_L1Norm(const int N, const double* _in);
double    SST_VectorNd_Dot(const int N, const double* _a, const double* _b);
double*   SST_VectorNd_Add(const int N, const double* _a, const double* _b, double* _out);
double*   SST_VectorNd_Subtract(const int N, const double* _a, const double* _b, double* _out);
double*   SST_VectorNd_AddLocal(const int N, double* _a, const double* _b);
double*   SST_VectorNd_Cross(const int N, const double* _a, const double* _b, double* _out);
double*   SST_VectorNd_CrossLocal(const int N, double* _a, const double* _b);
double*   SST_VectorNd_Scale(const int N, const double* _a, const double _scale, double* _out); 
double*   SST_VectorNd_ScaleLocal(const int N, double* _a, const double _scale);
double*   SST_VectorNd_RotateAbout(const int N, const double* _a, const double* _about,double _theta, double* _out);
double*   SST_VectorNd_RotateAboutLocal(const int N, double* _a, const double* _about,double _theta);

int  SST_VectorNi_MagnitudeSquared(const int N, const int* _in);
int  SST_VectorNi_L1Norm(const int N, const int* _in);
int  SST_VectorNi_Dot(const int N, const int* _a, const int* _b);
int*  SST_VectorNi_Add(const int N, const int* _a, const int* _b, int* _out);
int*  SST_VectorNi_Subtract(const int N, const int* _a, const int* _b, int* _out);
int*  SST_VectorNi_AddLocal(const int N, int* _a, const int* _b);
int*  SST_VectorNi_Cross(const int N, const int* _a, const int* _b, int* _out);
int*  SST_VectorNi_CrossLocal(const int N, int* _a, const int* _b);
int*  SST_VectorNi_Scale(const int N, const int* _a, const int _scale, int* _out); 
int*  SST_VectorNi_ScaleLocal(const int N, int* _a, const int _scale);

#endif
