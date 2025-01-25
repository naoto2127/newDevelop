#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"

class SceneClear : public Scene
{
public:
	SceneClear() {}
	~SceneClear() override {}

	// ������
	virtual void Initialize() override;
	// �I������
	virtual void Finalize() override;
	// �X�V����
	virtual void Update(float elapsedTime) override;
	// �`�揈��
	virtual void Render() override;

private:
	Sprite* sprite = nullptr;
};