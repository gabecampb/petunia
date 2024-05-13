#include "common.h"

// matrix a has size mxn, matrix b has size nxp (row x col)
void mat_mat(f32* dst, f32* a, f32* b, u32 m, u32 n, u32 p) {
	f32 A[m][n];
	f32 B[n][p];
	f32 C[m][p];
	memmove(A, a, sizeof(f32) * m * n);
	memmove(B, b, sizeof(f32) * n * p);
	memset(C, 0, sizeof(f32) * m * p);
	for(u32 i = 0; i < m; i++)
		for(u32 j = 0; j < p; j++)
			for(u32 k = 0; k < n; k++)
				C[i][j] += A[i][k] * B[k][j];
	memmove(dst, C, sizeof(f32) * m * p);
}

// calculate a LookAt matrix
Mat4 look_at(Vec3 eye, Vec3 center, Vec3 up) {
	Vec3 f = (eye - center).norm();
	Vec3 u = up.norm();
	Vec3 s = u.cross(f);
	u = f.cross(s);
	
	Mat4 m;
	m.data[0][0] = s.x;	m.data[0][1] = s.y;	m.data[0][2] = s.z;
	m.data[1][0] = u.x;	m.data[1][1] = u.y;	m.data[1][2] = u.z;
	m.data[2][0] = f.x;	m.data[2][1] = f.y;	m.data[2][2] = f.z;
	m.data[0][3] = -s.dot(eye);
	m.data[1][3] = -u.dot(eye);
	m.data[2][3] = -f.dot(eye);
	return m;
}

// calculate a symmetrical-frustum projection matrix
Mat4 perspective(f32 fovy, f32 aspect, f32 near, f32 far) {
	Mat4 m;
	m.data[0][0] = 1. / (aspect * tan(fovy / 2));
	m.data[1][1] = 1. / tan(fovy / 2);
	m.data[2][2] = -((far + near) / (far - near));
	m.data[2][3] = -((2 * far * near) / (far - near));
	m.data[3][3] = 0;
	m.data[3][2] = -1;
	return m;
}

Mat4 translate(Vec3 t) {
	Mat4 m;
	m.data[0][3] = t.x;
	m.data[1][3] = t.y;
	m.data[2][3] = t.z;
	return m;
}

Mat4 scale(Vec3 s) {
	Mat4 m;
	m.data[0][0] = s.x;
	m.data[1][1] = s.y;
	m.data[2][2] = s.z;
	return m;
}

Mat4 quat_to_mat4(Vec4 q) {
	float xx = q.x * q.x;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float xw = q.x * q.w;
	float yy = q.y * q.y;
	float yz = q.y * q.z;
	float yw = q.y * q.w;
	float zz = q.z * q.z;
	float zw = q.z * q.w;
	
	Mat4 r;
	r.data[0][0] = 1.0 - 2.0*yy - 2.0*zz;
	r.data[0][1] = 2.0 * xy - 2.0 * zw;
	r.data[0][2] = 2.0 * xz + 2.0 * yw;
	r.data[1][0] = 2.0 * xy + 2.0 * zw;
	r.data[1][1] = 1.0 - 2.0*xx - 2.0*zz;
	r.data[1][2] = 2.0 * yz - 2.0 * xw;
	r.data[2][0] = 2.0 * xz - 2.0 * yw;
	r.data[2][1] = 2.0 * yz + 2.0 * xw;
	r.data[2][2] = 1.0 - 2.0*xx - 2.0*yy;
	return r;
}
