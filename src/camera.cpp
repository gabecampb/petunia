#include "common.h"

Camera::Camera(f32 fov, f32 n, f32 f) {
	type = NodeType::TYPE_CAMERA;
	fovy = fov;
	near = n;
	far = f;
}

Mat4 Camera::get_persp(f32 aspect_ratio) {
	return perspective(fovy, get_aspect(), near, far);
}

Mat4 Camera::get_view() {
	return look_at(pos, pos + get_fwd_dir(), Vec3(0,1,0));
}

Vec3 Camera::get_fwd_dir() {
	Vec4 c(0,0,1,0);
	c = quat_to_mat4(rot) * c;
	return Vec3(c.x, c.y, c.z);
}

Vec3 Camera::get_right_dir() {
	Vec4 c(1,0,0,0);
	c = quat_to_mat4(rot) * c;
	return Vec3(c.x, c.y, c.z);
}
