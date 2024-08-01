#ifndef SCENE_H
#define SCENE_H

#include "common.h"

class Scene {
	private:
	std::vector<Model*> loaded_models;
	u32 n_loaded_models = 0;

	public:
	Node* root;

	Scene();
	void set_loaded_model(u32 id, Model* m);
	Model* get_loaded_model(u32 id);
	u32 get_num_loaded_models();
	Model* load_box_model(u32 id);
	Model* load_line_model(u32 id);
	Model* load_model(u32 id, std::string mesh_fpath);
	Model* load_model(u32 id, std::string mesh_fpath, std::string tex_fpath);
	void print_models();
};

#endif
