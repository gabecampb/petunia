#include "common.h"

int main() {
	init_gfx();
	init_render();

	Scene default_scene;
	Model* m = default_scene.load_model(256, "assets/cube.obj", "assets/petunia.jpg");
	Node* n = new Node;
	default_scene.root->children.push_back(n);

	n->model = m;
	n->pos = Vec3(0,0,3);
	n->scale = Vec3(.25,.25,.25);
	n->rot = Vec4(-.001, -.469, .156, -.869);

	while(next_frame())
		render(&default_scene);

	terminate_gfx();
	return 0;
}
