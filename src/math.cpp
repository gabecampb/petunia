#include "common.h"

f32 torad(f32 a) {
	return a * 0.0174533;
}

f32 todeg(f32 a) {
	return a * 57.2958;
}

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

Mat4 rotate(Vec3 axis, f32 angle) {
	if(angle >= 0) angle = fmod(angle, 360.);
	else angle = 360 - fmod(-angle, 360);
	angle = torad(angle);

	f32 x = axis.x, y = axis.y, z = axis.z;
	f32 c = cos(angle), s = sin(angle);

	f32 one_sub_c = 1.0 - c;
	f32 zs = z*s;
	f32 ys = y*s;
	f32 xs = x*s;
	f32 xz = x*z;
	f32 yz = y*z;

	Mat4 mrot;
	mrot.data[0][0] = x*x*(one_sub_c)+c;
	mrot.data[0][1] = x*y*(one_sub_c)-zs;
	mrot.data[0][2] = xz *(one_sub_c)+ys;
	mrot.data[1][0] = y*x*(one_sub_c)+zs;
	mrot.data[1][1] = y*y*(one_sub_c)+c;
	mrot.data[1][2] = yz *(one_sub_c)-xs;
	mrot.data[2][0] = xz *(one_sub_c)-ys;
	mrot.data[2][1] = yz *(one_sub_c)+xs;
	mrot.data[2][2] = z*z*(one_sub_c)+c;
	return mrot;
}

// heading		= y
// attitude		= z
// bank			= x
// perform rotation about y,z then x

Vec4 quat_quat(Vec4 a, Vec4 b) {
	return Vec4(
		 a.x * b.w + a.y * b.z - a.z * b.y + a.w * b.x,
		-a.x * b.z + a.y * b.w + a.z * b.x + a.w * b.y,
		 a.x * b.y - a.y * b.x + a.z * b.w + a.w * b.z,
		-a.x * b.x - a.y * b.y - a.z * b.z + a.w * b.w
	);
}

Vec4 euler_to_quat(Vec3 angles) {
	f64 c1 = cos(angles.y / 2.);
	f64 c2 = cos(angles.z / 2.);
	f64 c3 = cos(angles.x / 2.);
	f64 s1 = sin(angles.y / 2.);
	f64 s2 = sin(angles.z / 2.);
	f64 s3 = sin(angles.x / 2.);

	Vec4 q;
	q.w = c1 * c2 * c3 - s1 * s2 * s3;
	q.x = s1 * s2 * c3 + c1 * c2 * s3;
	q.y = s1 * c2 * c3 + c1 * s2 * s3;
	q.z = c1 * s2 * c3 - s1 * c2 * s3;
	return q;
}

Vec3 quat_to_euler(Vec4 q) {
	Vec3 e;
	q = q.norm();

	f64 test = q.x * q.y + q.z * q.w;
	if(test > .4995) {		// north pole singularity
		e.y = 2 * atan2(q.x, q.w);
		e.z = M_PI / 2.;
		e.x = 0;
		return e;
	}

	if(test < -.4995) {		// south pole singularity
		e.y = -2. * atan2(q.x, q.w);
		e.z = -M_PI / 2.;
		e.x = 0;
	}

	f64 sqx = q.x * q.x;
	f64 sqy = q.y * q.y;
	f64 sqz = q.z * q.z;

	e.y = atan2(2*q.y*q.w-2*q.x*q.z , 1 - 2*sqy - 2*sqz);
	e.z = asin(2*test);
	e.x = atan2(2*q.x*q.w-2*q.y*q.z , 1 - 2*sqx - 2*sqz);
	return e;
}

Mat4 quat_to_mat4(Vec4 q) {
	f64 sqx = q.x*q.x;
	f64 sqy = q.y*q.y;
	f64 sqz = q.z*q.z;

	Mat4 m;
	m.data[0][0] = 1 - 2*sqy - 2*sqz;
	m.data[1][0] = 2*q.x*q.y + 2*q.z*q.w;
	m.data[2][0] = 2*q.x*q.z - 2*q.y*q.w;

	m.data[0][1] = 2*q.x*q.y - 2*q.z*q.w;
	m.data[1][1] = 1-2*sqx-2*sqz;
	m.data[2][1] = 2*q.y*q.z + 2*q.x*q.w;

	m.data[0][2] = 2*q.x*q.z + 2*q.y*q.w;
	m.data[1][2] = 2*q.y*q.z - 2*q.x*q.w;
	m.data[2][2] = 1 - 2*sqx - 2*sqy;

	return m;
}
