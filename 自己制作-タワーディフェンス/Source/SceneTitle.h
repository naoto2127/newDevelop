#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"

class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

	// ‰Šú‰»
	virtual void Initialize() override;
	// I—¹ˆ—
	virtual void Finalize() override;
	// XVˆ—
	virtual void Update(float elapsedTime) override;
	// •`‰æˆ—
	virtual void Render() override;

private:
	Sprite* sprite = nullptr;
};