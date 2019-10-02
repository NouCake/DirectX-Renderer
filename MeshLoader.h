#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>

#include "Mesh.h"

class MeshLoader {

public:
	MeshLoader();
	Mesh* loadMesh(std::string path);

private:
	Assimp::Importer mAssimp;

};