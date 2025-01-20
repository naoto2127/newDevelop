#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <thread>



// ローディングシーン
class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
	~SceneLoading() override {}

	// 初期化
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

private:
	static void LoadingThread(SceneLoading* scene);

private:
	Sprite* sprite = nullptr;
	float	angle = 0.0f;

	// 次のシーン
	Scene* nextScene = nullptr;
	// 次のシーンを作成してくれるスレッド
	std::thread* thread = nullptr;
};

