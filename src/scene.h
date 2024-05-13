#include "common.h"

class Scene {
	public:
	Node* root;
	std::vector<Model*> loaded_models;

	Scene() {
		// default scene constructor
		root = new Node;
	}

	Model* load_model(u32 id, std::string fpath, std::string tex_fpath) {
		Model* m = new Model;
		m->model_id = id;
		if(tex_fpath == "")
			m->load(fpath);
		else
			m->load_textured(fpath, tex_fpath);
		loaded_models.push_back(m);
		return m;
	}
};
