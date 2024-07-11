#ifndef MODEL_H
#define MODEL_H

#include "common.h"

class Model {
	public:
	u32 model_id;
	std::string mesh_filepath, tex_filepath;
	GLuint vao, vbo, ibo, tbo;
	u32 n_verts;

	Model();
	Model(std::string mesh_fpath);
	Model(std::string mesh_fpath, std::string tex_fpath);

	void load_mesh(std::string mesh_fpath);
	void load_texture(std::string tex_fpath);
};

#endif
