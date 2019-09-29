#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>

class MeshLoader {

public:
	MeshLoader();
	void loadMesh(std::string path);

private:
	Assimp::Importer mAssimp;

};