#pragma once

#include "Scene.h"

class Lives : public Scene {

public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
};
