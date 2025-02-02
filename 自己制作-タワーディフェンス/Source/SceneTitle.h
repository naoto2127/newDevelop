#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"

class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

	// 初期化
	virtual void Initialize() override;
	// 終了処理
	virtual void Finalize() override;
	// 更新処理
	virtual void Update(float elapsedTime) override;
	// 描画処理
	virtual void Render() override;

private:
	Sprite* sprite = nullptr;
};