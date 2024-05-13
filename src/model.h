#ifndef MODEL_H
#define MODEL_H

#include "common.h"

class Model {
	public:
	u32 model_id;
	std::string filepath, tex_filepath;
	GLuint vao, vbo, ibo, tbo;
	u32 n_verts;

	Model();

	void load(std::string fpath);
	void load_textured(std::string fpath, std::string tex_fpath);
};

#endif
