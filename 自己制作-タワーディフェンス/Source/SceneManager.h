#pragma once

#include "Scene.h"

// シーンマネージャー
class SceneManager
{
private:
	SceneManager() {}
	~SceneManager() {}

public:
	// 唯一のインスタンス取得
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画所為
	void Render();

	// シーンのクリア
	void Clear();

	/// <summary>
	/// シーン切り替え
	/// </summary>
	/// <param name="scene">次のシーンになるシーン</param>
	void ChangeScene(Scene* scene);

private:
	Scene* currentScene = nullptr;	// 現在のシーン
	Scene* nextScene = nullptr;		// 次のシーン
};