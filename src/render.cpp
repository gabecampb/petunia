#include "common.h"

GLuint program_id;

void render(Camera &cam, Scene* scene) {
	if(!scene->root)
		return;
	render_node(cam, scene->root);
}

void render_node(Camera &cam, Node* node) {
	if(!node)
		return;

	GLint model_loc	= glGetUniformLocation(program_id, "u_model");
	GLint view_loc	= glGetUniformLocation(program_id, "u_view");
	GLint proj_loc	= glGetUniformLocation(program_id, "u_proj");
	GLint color_loc	= glGetUniformLocation(program_id, "u_color");
	GLint tex_loc	= glGetUniformLocation(program_id, "u_tex");

	if(node->model) {
		Model* m = node->model;
		glBindVertexArray(m->vao);
		glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ibo);

		Mat4 persp = cam.get_persp(get_aspect());
		Mat4 view = cam.get_view();
		f32 color[] = { 1, 1, 1, 1 };

		Mat4 model_mat = node->get_global();
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, (f32*)model_mat.data);
		glUniformMatrix4fv(proj_loc, 1, GL_TRUE, (f32*)persp.data);
		glUniformMatrix4fv(view_loc, 1, GL_TRUE, (f32*)view.data);
		glUniform4fv(color_loc, 1, color);

		if(m->tbo) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m->tbo);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glUniform1i(tex_loc, 0);
		}

		if(!m->ibo)
			glDrawArrays(GL_TRIANGLES, 0, m->n_verts);
	}

	// call render_node for every child
	for(u32 i = 0; i < node->get_child_count(); i++)
		render_node(cam, node->get_child(i));
}

GLuint create_program(const char* vtx_shader_src, const char* pxl_shader_src) {
	GLuint vtx_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint pxl_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vtx_shader,1,&vtx_shader_src,0);
	glShaderSource(pxl_shader,1,&pxl_shader_src,0);
	glCompileShader(vtx_shader);
	GLint success = 0;
	glGetShaderiv(vtx_shader,GL_COMPILE_STATUS,&success);
	if(!success) {
		std::cout << "failed to compile vertex shader" << std::endl;
		GLint max_length = 0;
		glGetShaderiv(vtx_shader, GL_INFO_LOG_LENGTH, &max_length);
		char* info_log = (char*)calloc(1,max_length);
		glGetShaderInfoLog(vtx_shader, max_length, &max_length, &info_log[0]);
		std::cout << info_log << std::endl;
		exit(1);
	}
	glCompileShader(pxl_shader);
	glGetShaderiv(pxl_shader,GL_COMPILE_STATUS,&success);
	if(!success) {
		std::cout << "failed to compile pixel shader" << std::endl;
		GLint max_length = 0;
		glGetShaderiv(pxl_shader, GL_INFO_LOG_LENGTH, &max_length);
		char* info_log = (char*)calloc(1,max_length);
		glGetShaderInfoLog(pxl_shader, max_length, &max_length, &info_log[0]);
		std::cout << info_log << std::endl;
		exit(1);
	}
	GLint program = glCreateProgram();
	glAttachShader(program,vtx_shader);
	glAttachShader(program,pxl_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success) {
		printf("failed to link shaders");
		exit(1);
	}
	glDetachShader(program, vtx_shader);
	glDetachShader(program, pxl_shader);

	return program;
}

const char* vtx_shader =
	"#version 330												\n"
	"layout(location = 0) in vec3 pos;							\n"
	"layout(location = 1) in vec3 normal;						\n"
	"layout(location = 2) in vec2 tex_coord;					\n"
	"out vec2 tcoords;											\n"
	"uniform mat4 u_model, u_view, u_proj;						\n"
	"void main() {												\n"
	"	tcoords = tex_coord;									\n"
	"	gl_Position = u_proj * u_view * u_model * vec4(pos, 1);	\n"
	"}															\n";

const char* frag_shader =
	"#version 330												\n"
	"layout(location = 0) out vec4 final;						\n"
	"in vec2 tcoords;											\n"
	"uniform vec4 u_color;										\n"
	"uniform sampler2D u_tex;									\n"
	"void main() {												\n"
	"	final = texture(u_tex, tcoords);						\n"
	"}															\n";

void init_render() {
	glEnable(GL_DEPTH_TEST);
	program_id = create_program(vtx_shader, frag_shader);
	glUseProgram(program_id);
}
