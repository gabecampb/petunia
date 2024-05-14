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

Vec4 euler_to_quat(Vec3 angles) {
	f32 c1, c2, c3;
	f32 s1, s2, s3;

	if(angles.x >= 0) angles.x = fmod(angles.x, 360.0);
	else angles.x = 360 - fmod(-angles.x, 360);
	if(angles.y >= 0) angles.y = fmod(angles.y, 360.0);
	else angles.y = 360 - fmod(-angles.y, 360);
	if(angles.z >= 0) angles.z = fmod(angles.z, 360.0);
	else angles.z = 360 - fmod(-angles.z, 360);

	angles.x = torad(angles.x);
	angles.y = torad(angles.y);
	angles.z = torad(angles.z);

	c1 = cos(angles.y / 2.0);
	c2 = cos(angles.z / 2.0);
	c3 = cos(angles.x / 2.0);
	s1 = sin(angles.y / 2.0);
	s2 = sin(angles.z / 2.0);
	s3 = sin(angles.x / 2.0);

	Vec4 quat;
	quat.w = c1*c2*c3 - s1*s2*s3;
	quat.x = s1*s2*c3 + c1*c2*s3;
	quat.y = s1*c2*c3 + c1*s2*s3;
	quat.z = c1*s2*c3 - s1*c2*s3;

	float n = sqrt(pow(quat.x, 2) + pow(quat.y, 2) + pow(quat.z, 2) + pow(quat.w, 2));
	if(n)
		quat /= n;

	return quat;
}

Vec4 quat_axis_rotation(Vec3 axis, f32 angle) {
	angle = torad(angle);
	Vec3 v = axis * sin(angle/2.);
	Vec4 quat(v.x, v.y, v.z, cos(angle/2.));
	return quat.norm();
}

Vec3 quat_to_euler(Vec4 q) {
	float sqw = q.w*q.w;
	float sqx = q.x*q.x;
	float sqy = q.y*q.y;
	float sqz = q.z*q.z;
	float unit = sqx + sqy + sqz + sqw;
	float test = q.x*q.y + q.z*q.w;
	if(test > 0.499*unit)
		return Vec3(0, todeg(2 * atan2(q.x,q.w)), todeg(M_PI/2));
	if(test < -0.499*unit)
		return Vec3(0, todeg(-2 * atan2(q.x,q.w)), todeg(M_PI/2));
	return Vec3(
		todeg( atan2(2*q.x*q.w-2*q.y*q.z , -sqx + sqy - sqz + sqw) ),
		todeg( atan2(2*q.y*q.w-2*q.x*q.z , sqx - sqy - sqz + sqw) ),
		todeg( asin(2*test/unit) )
	);
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
