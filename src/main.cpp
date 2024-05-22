#include "common.h"

int main() {
	init_gfx();
	init_render();

	Scene default_scene;
	Model* m = default_scene.load_model(256, "assets/cube.obj", "assets/petunia.jpg");
	Node* n = new Node;
	default_scene.root->add_child(n);

	n->model = m;
	n->set_pos(Vec3(0,0,3));
	n->set_scale(Vec3(.25,.25,.25));
	n->set_rot(Vec4(-.001, -.469, .156, -.869));

	Player* p = new Player("example_player");
	default_scene.root->add_child(p);
	set_player(p);

	Camera cam(torad(60.), .01, 50);

	while(next_frame()) {
		render(cam, &default_scene);
		node_cleanup();
	}

	terminate_gfx();
	return 0;
}
