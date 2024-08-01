#include "common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

f32* load_vdata_from_file(std::string file, u32& size);
GLuint load_texture_from_file(std::string file);
GLuint load_vbo_from_vdata(f32* data, u32 size);
extern f32 box[];
extern f32 line[];

Model::Model() {
	model_id = vao = vbo = ibo = tbo = n_verts = 0;
	mesh_filepath = tex_filepath = "";
}

Model::Model(std::string mesh_fpath) {
	*this = Model();
	load_mesh(mesh_fpath);
}

Model::Model(std::string mesh_fpath, std::string tex_fpath) {
	*this = Model(mesh_fpath);
	load_texture(tex_fpath);
}

void Model::load_vbo_from_vdata(f32* data, u32 size) {
	const u32 stride = 32;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride,0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,stride,(void*)12);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,stride,(void*)24);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	n_verts = size / (8 * sizeof(f32));
}

// load the VBO and IBO; discards previous data
void Model::load_mesh(std::string mesh_fpath) {
	u32 s;
	f32* data = load_vdata_from_file(mesh_fpath, s);
	load_vbo_from_vdata(data, s);
	free(data);

	mesh_filepath = mesh_fpath;
}

void Model::load_texture(std::string tex_fpath) {
	tbo = load_texture_from_file(tex_fpath);
	if(tbo)
		tex_filepath = tex_fpath;
}

void Model::load_box() {
	u32 size = sizeof(f32) * 72 * 8;
	load_vbo_from_vdata(box, size);
}

void Model::load_line() {
	u32 size = sizeof(f32) * 2 * 8;
	load_vbo_from_vdata(line, size);
}

// returns a V3 N3 T2 array describing vertices of model in a file, via assimp.
// Triangulates. Returns size of array in bytes.
f32* load_vdata_from_file(std::string file, u32& size) {
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile(file.c_str(),
	aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_SortByPType);
	
	if(!scene) {
		std::cout << importer.GetErrorString() << std::endl;
		return nullptr;
	}
	
	std::vector<f32> vb;
	
	if(scene->mNumMeshes > 0) {
		for(u32 m = 0; m < scene->mNumMeshes; ++m) {
			aiMesh* mesh = scene->mMeshes[m];
			
			if(!mesh)
				continue;
				
			for(u32 f = 0; f < mesh->mNumFaces; ++f) {
				aiFace face = mesh->mFaces[f];
				
				for(u32 e = 0; e < 3; e += 1) {	
					u32 i = face.mIndices[e];
					
					aiVector3D& pos = mesh->mVertices[i];
					vb.emplace_back( pos.x );
					vb.emplace_back( pos.y );
					vb.emplace_back( pos.z );
					
					if(mesh->mNormals) {
						aiVector3D& n = mesh->mNormals[i];
						vb.emplace_back( n.x );
						vb.emplace_back( n.y );
						vb.emplace_back( n.z );
					} else {
						vb.emplace_back(0.0);
						vb.emplace_back(0.0);
						vb.emplace_back(0.0);
					}
					
					if(mesh->HasTextureCoords(0)) {
						aiVector3D& t = mesh->mTextureCoords[0][i];
						vb.emplace_back( t.x );
						vb.emplace_back( t.y );
					} else {
						vb.emplace_back(0.0);
						vb.emplace_back(0.0);
					}
				}
			}
		}
	}

	size = vb.size() * sizeof(float);
	
	f32* ret = (f32*) malloc(size);
	for(u32 i = 0; i < vb.size(); i++)
		ret[i] = vb.at(i);

	return ret;
}

GLuint load_texture_from_file(std::string file) {
	int w, h, comp;
	u8* image = stbi_load(file.c_str(), &w, &h, &comp, 0);
	if(!image) {
		std::cout << "failed to load texture " << file << std::endl;
		return 0;
	}
	// create texture object
	GLuint tbo_id;
	glGenTextures(1,&tbo_id);
	glBindTexture(GL_TEXTURE_2D, tbo_id);
	// upload image to TBO
	if(comp == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else if(comp == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);
	return tbo_id;
}

f32 box[] = {
	1, 1, -1, -0, 1, -0, 0.625, 0.5,
	-1, 1, -1, -0, 1, -0, 0.875, 0.5,
	-1, 1, 1, -0, 1, -0, 0.875, 0.75,
	1, 1, -1, -0, 1, -0, 0.625, 0.5,
	-1, 1, 1, -0, 1, -0, 0.875, 0.75,
	1, 1, 1, -0, 1, -0, 0.625, 0.75,
	1, -1, 1, -0, -0, 1, 0.375, 0.75,
	1, 1, 1, -0, -0, 1, 0.625, 0.75,
	-1, 1, 1, -0, -0, 1, 0.625, 1,
	1, -1, 1, -0, -0, 1, 0.375, 0.75,
	-1, 1, 1, -0, -0, 1, 0.625, 1,
	-1, -1, 1, -0, -0, 1, 0.375, 1,
	-1, -1, 1, -1, -0, -0, 0.375, 0,
	-1, 1, 1, -1, -0, -0, 0.625, 0,
	-1, 1, -1, -1, -0, -0, 0.625, 0.25,
	-1, -1, 1, -1, -0, -0, 0.375, 0,
	-1, 1, -1, -1, -0, -0, 0.625, 0.25,
	-1, -1, -1, -1, -0, -0, 0.375, 0.25,
	-1, -1, -1, -0, -1, -0, 0.125, 0.5,
	1, -1, -1, -0, -1, -0, 0.375, 0.5,
	1, -1, 1, -0, -1, -0, 0.375, 0.75,
	-1, -1, -1, -0, -1, -0, 0.125, 0.5,
	1, -1, 1, -0, -1, -0, 0.375, 0.75,
	-1, -1, 1, -0, -1, -0, 0.125, 0.75,
	1, -1, -1, 1, -0, -0, 0.375, 0.5,
	1, 1, -1, 1, -0, -0, 0.625, 0.5,
	1, 1, 1, 1, -0, -0, 0.625, 0.75,
	1, -1, -1, 1, -0, -0, 0.375, 0.5,
	1, 1, 1, 1, -0, -0, 0.625, 0.75,
	1, -1, 1, 1, -0, -0, 0.375, 0.75,
	-1, -1, -1, -0, -0, -1, 0.375, 0.25,
	-1, 1, -1, -0, -0, -1, 0.625, 0.25,
	1, 1, -1, -0, -0, -1, 0.625, 0.5,
	-1, -1, -1, -0, -0, -1, 0.375, 0.25,
	1, 1, -1, -0, -0, -1, 0.625, 0.5,
	1, -1, -1, -0, -0, -1, 0.375, 0.5,
	1, 1, -1, -0, 1, -0, 0, 0,
	-1, 1, -1, -0, 1, -0, 1, 0,
	-1, 1, 1, -0, 1, -0, 1, 1,
	1, 1, -1, -0, 1, -0, 0, 0,
	-1, 1, 1, -0, 1, -0, 1, 1,
	1, 1, 1, -0, 1, -0, 0, 1,
	1, -1, 1, -0, -0, 1, 0, 0,
	1, 1, 1, -0, -0, 1, 1, 0,
	-1, 1, 1, -0, -0, 1, 1, 1,
	1, -1, 1, -0, -0, 1, 0, 0,
	-1, 1, 1, -0, -0, 1, 1, 1,
	-1, -1, 1, -0, -0, 1, 0, 1,
	-1, -1, 1, -1, -0, -0, 0, 0,
	-1, 1, 1, -1, -0, -0, 1, 0,
	-1, 1, -1, -1, -0, -0, 1, 1,
	-1, -1, 1, -1, -0, -0, 0, 0,
	-1, 1, -1, -1, -0, -0, 1, 1,
	-1, -1, -1, -1, -0, -0, 0, 1,
	-1, -1, -1, -0, -1, -0, 0, 0,
	1, -1, -1, -0, -1, -0, 1, 0,
	1, -1, 1, -0, -1, -0, 1, 1,
	-1, -1, -1, -0, -1, -0, 0, 0,
	1, -1, 1, -0, -1, -0, 1, 1,
	-1, -1, 1, -0, -1, -0, 0, 1,
	1, -1, -1, 1, -0, -0, 0, 0,
	1, 1, -1, 1, -0, -0, 1, 0,
	1, 1, 1, 1, -0, -0, 1, 1,
	1, -1, -1, 1, -0, -0, 0, 0,
	1, 1, 1, 1, -0, -0, 1, 1,
	1, -1, 1, 1, -0, -0, 0, 1,
	-1, -1, -1, -0, -0, -1, 0, 0,
	-1, 1, -1, -0, -0, -1, 1, 0,
	1, 1, -1, -0, -0, -1, 1, 1,
	-1, -1, -1, -0, -0, -1, 0, 0,
	1, 1, -1, -0, -0, -1, 1, 1,
	1, -1, -1, -0, -0, -1, 0, 1
};

f32 line[] = {
	0, 0, 0, 0, -1, 0, 0, 0,
	0, 1, 0, 0,  1, 0, 0, 0,
};
