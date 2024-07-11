#include "common.h"

Scene::Scene() {
	// default scene constructor
	root = new Node;
	root->name = "root";
}

void Scene::set_loaded_model(u32 id, Model* m) {
	if(id < loaded_models.size()) {
		u8 loaded = loaded_models[id] != nullptr;

		if(!loaded && m)	n_loaded_models++;
		if(loaded && !m)	n_loaded_models--;

		loaded_models[id] = m;
	} else {
		loaded_models.resize(id + 1);
		loaded_models[id] = m;
		if(m)
			n_loaded_models++;
	}
}

Model* Scene::get_loaded_model(u32 id) {
	if(id >= loaded_models.size())
		return 0;
	return loaded_models[id];
}

u32 Scene::get_num_loaded_models() {
	return n_loaded_models;
}

Model* Scene::load_model(u32 id, std::string mesh_fpath) {
	Model* m = new Model(mesh_fpath);
	m->model_id = id;
	set_loaded_model(id, m);
	return m;
}

Model* Scene::load_model(u32 id, std::string mesh_fpath, std::string tex_fpath) {
	Model* m = new Model(mesh_fpath, tex_fpath);
	m->model_id = id;
	set_loaded_model(id, m);
	return m;
}
