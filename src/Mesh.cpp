
#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

Mesh testMesh;

bool Mesh::LoadFromOBJ(OVR::RenderTiny::RenderDevice* device, const void* mem, size_t len)
{
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFileFromMemory(mem, len,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes|aiProcess_MakeLeftHanded, "obj");
	if(scene == NULL)
		return false;

	const aiMesh* mesh = scene->mMeshes[0];

	ZArray<OVR::RenderTiny::Vertex> Vertices;
	ZArray<uint16_t> Indices;

	//Copy vertices
	Vertices.Resize(mesh->mNumVertices);
	for(uint32_t i=0; i<mesh->mNumVertices; i++)
	{
		Vertices[i].Pos.x = mesh->mVertices[i].x;
		Vertices[i].Pos.y = mesh->mVertices[i].y;
		Vertices[i].Pos.z = mesh->mVertices[i].z;

		if(mesh->HasTextureCoords(0)) {

			Vertices[i].U = mesh->mTextureCoords[0]->x;
			Vertices[i].V = mesh->mTextureCoords[0]->y;
		} else {
			Vertices[i].U = 0;
			Vertices[i].V = 0;
		}
	}

	//Copy face data
	Indices.Resize(mesh->mNumFaces*3);
	for(uint32_t i=0; i<mesh->mNumFaces; i++) {

		Indices[i*3+0] = (uint16_t)mesh->mFaces[i].mIndices[0];
		Indices[i*3+1] = (uint16_t)mesh->mFaces[i].mIndices[1];
		Indices[i*3+2] = (uint16_t)mesh->mFaces[i].mIndices[2];
	}

#if 0
	if(scene->HasMaterials()) {

		aiMaterial* material = scene->mMaterials[0];

		aiString s;
		material->Get(AI_MATKEY_NAME,s);

		for(int i=0; i<material->mNumProperties; i++) {

			aiMaterialProperty* prop = material->mProperties[i];

			printf("Key{%s}, length = %u,  value = %s\n", prop->mKey.C_Str(), prop->mDataLength, prop->mData);
		}

	}
#endif

	vb = device->CreateBuffer();
	ib = device->CreateBuffer();

	vb->Data(OVR::RenderTiny::Buffer_Vertex, Vertices.Data(), Vertices.Size()* sizeof(OVR::RenderTiny::Vertex));
	ib->Data(OVR::RenderTiny::Buffer_Index, Indices.Data(), Indices.Size() * sizeof(uint16_t));
	nrFaces = Indices.Size()/3;
	return true;
}
