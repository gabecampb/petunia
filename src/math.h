#ifndef MATH_H
#define MATH_H

#include "common.h"

void mat_mat(f32* dst, f32* a, f32* b, u32 m, u32 n, u32 p);

class Vec3 {
	public:	
	f32 x, y, z;

	Vec3() {
		x = y = z = 0.;
	}
	Vec3(f32 X, f32 Y, f32 Z) {
		x = X;
		y = Y;
		z = Z;
	}
	Vec3(f32 p[3]) {
		*this = Vec3(p[0], p[1], p[2]);
	}

	Vec3 operator +(Vec3 v) { return Vec3(x + v.x, y + v.y, z + v.z); }
	Vec3 operator -(Vec3 v) { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3 operator *(Vec3 v) { return Vec3(x * v.x, y * v.y, z * v.z); }
	Vec3 operator /(Vec3 v) {
		if(v.x == 0 || v.y == 0 || v.z == 0)
			return Vec3(0,0,0);
		return Vec3(x / v.x, y / v.y, z / v.z);
	}
	Vec3 operator *(f32 f) { return Vec3(x * f, y * f, z * f); }
	Vec3 operator /(f32 f) {
		if(f == 0)
			return Vec3(0,0,0);
		return Vec3(x / f, y / f, z / f);
	}

	Vec3 operator +=(Vec3 v) { *this = *this + v; return *this; }
	Vec3 operator -=(Vec3 v) { *this = *this - v; return *this; }
	Vec3 operator *=(Vec3 v) { *this = *this * v; return *this; }
	Vec3 operator /=(Vec3 v) { *this = *this / v; return *this; }
	Vec3 operator *=(f32 f)  { *this = *this * f; return *this; }
	Vec3 operator /=(f32 f)  { *this = *this / f; return *this; }

	f32 mag() {
		return sqrt(x*x + y*y + z*z);
	}
	Vec3 norm() {
		return *this / mag();
	}
	Vec3 cross(Vec3 v) {
		return Vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	f32 dot(Vec3 v) {
		return x*v.x + y*v.y + z*v.z;
	}
};

class Vec4 {
	public:
	f32 x, y, z, w;

	Vec4() {
		x = y = z = w = 0;
	}
	Vec4(f32 X, f32 Y, f32 Z, f32 W) {
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
	Vec4(f32 p[4]) {
		*this = Vec4(p[0], p[1], p[2], p[3]);
	}

	Vec4 operator +(Vec4 v) { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	Vec4 operator -(Vec4 v) { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vec4 operator *(Vec4 v) { return Vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
	Vec4 operator /(Vec4 v) {
		if(v.x == 0 || v.y == 0 || v.z == 0 || v.w == 0)
			return Vec4(0,0,0,0);
		return Vec4(x / v.x, y / v.y, z / v.z, w / v.w);
	}
	Vec4 operator *(f32 f)  { return Vec4(x * f, y * f, z * f, w * f); }
	Vec4 operator /(f32 f)  {
		if(f == 0)
			return Vec4(0,0,0,0);
		return Vec4(x / f, y / f, z / f, w / f);
	}

	Vec4 operator +=(Vec4 v) { *this = *this + v; return *this; }
	Vec4 operator -=(Vec4 v) { *this = *this - v; return *this; }
	Vec4 operator *=(Vec4 v) { *this = *this * v; return *this; }
	Vec4 operator /=(Vec4 v) { *this = *this / v; return *this; }
	Vec4 operator *=(f32 f)  { *this = *this * f; return *this; }
	Vec4 operator /=(f32 f)  { *this = *this / f; return *this; }

	f32 mag() {
		return sqrt(x*x + y*y + z*z + w*w);
	}
	Vec4 norm() {
		return *this / mag();
	}
};

class Mat4 {
	public:
	f32 data[4][4];		// [row][col]
	void clear() {
		memset(data, 0, sizeof(f32)*16);
	}

	Mat4() {
		clear();
		for(u32 i = 0; i < 4; i++)
			data[i][i] = 1;
	}

	Vec4 operator *(Vec4 v) {
		f32 prod[4], b[4] = { v.x, v.y, v.z, v.w };
		mat_mat(prod, (f32*)data, b, 4, 4, 1);
		return Vec4(prod[0], prod[1], prod[2], prod[3]);
	}
	Mat4 operator *(Mat4 m) {
		Mat4 p;
		mat_mat((f32*)p.data, (f32*)data, (f32*)m.data, 4, 4, 4);
		return p;
	}

	Mat4 operator *=(Mat4 m) { *this = *this * m; return *this; }
};

f32 torad(f32 a);
f32 todeg(f32 a);
Mat4 look_at(Vec3 eye, Vec3 center, Vec3 up);
Mat4 perspective(f32 fovy, f32 aspect, f32 near, f32 far);
Mat4 translate(Vec3 t);
Mat4 scale(Vec3 s);
Mat4 rotate(Vec3 axis, f32 angle);

Vec4 quat_quat(Vec4 a, Vec4 b);
Vec4 euler_to_quat(Vec3 angles);
Vec3 quat_to_euler(Vec4 q);
Mat4 quat_to_mat4(Vec4 q);

#endif
