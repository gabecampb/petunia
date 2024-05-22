#include "common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

Model::Model() {
	model_id = vao = vbo = ibo = tbo = n_verts = 0;
	filepath = tex_filepath = "";
}

// returns a V3 N3 T2 array describing vertices of model in a file, via assimp.
// Triangulates. Returns size of array in bytes.
f32* load_vbo_from_file(std::string file, u32& size) {
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

// load the VBO and IBO; discards previous data
void Model::load(std::string fpath) {
	u32 stride = 32;
	u32 s;
	f32* data = load_vbo_from_file(fpath, s);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, s, data, GL_STATIC_DRAW);
	free(data);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,stride,0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,stride,(void*)12);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,stride,(void*)24);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	filepath = fpath;
	n_verts = s / (8 * sizeof(f32));
}

void Model::load_textured(std::string fpath, std::string tex_fpath) {
	load(fpath);
	tbo = load_texture_from_file(tex_fpath);
	if(tbo)
		tex_filepath = tex_fpath;
}
