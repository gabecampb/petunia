#ifndef RENDER_H
#define RENDER_H

void render(Scene* scene);
void render_node(Node* node);
GLuint create_program(const char* vtx_shader_src, const char* pxl_shader_src);
void init_render();

#endif
