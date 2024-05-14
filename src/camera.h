#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class Camera {
	public:
	Vec3 pos;
	Vec4 rot;
	f32 fovy, near, far;

	Camera(f32 fov, f32 n, f32 f);
	Mat4 get_persp(f32 asp_ratio);
	Mat4 get_view();
	Vec3 get_fwd_dir();
	Vec3 get_right_dir();
};

#endif
