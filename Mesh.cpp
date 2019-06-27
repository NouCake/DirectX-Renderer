#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Mesh::Mesh()
{
	Assimp::Importer imp;
	imp.ReadFile("sponza/sponza.obj",
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
}