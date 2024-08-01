#ifndef MODEL_H
#define MODEL_H

#include "common.h"

class Model {
	void load_vbo_from_vdata(f32* data, u32 size);

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
	void load_box();
	void load_line();
};

#endif
