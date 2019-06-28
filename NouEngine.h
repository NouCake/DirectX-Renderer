#pragma once

#include "settings.h"

#include "NouWindow.h"
#include "NouTimer.h"


#include "Cube.h"
#include "Camera.h"
#include "BaseMaterial.h"
#include "Mesh.h"
#include "TextureLoader.h"

class NouEngine
{
public:
	NouEngine();
	int Run();

private:
	NouWindow window;
	NouTimer timer;
	std::vector<Mesh> meshes;
	
	void ExecuteFrame();

};