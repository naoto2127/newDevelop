#include "SceneManager.h"

void SceneManager::Update(float elapsedTime)
{
	// 入れ替えるべき次のシーンがあったら
	if (nextScene != nullptr)
	{
		// 古いシーン(つまり現在のシーン)の終了処理を行う
		Clear();

		// 新しいシーン(入れ替えるべき次のシーン)を設定
		currentScene = nextScene;
		nextScene = nullptr;

		// 入れ替わった新しい現在のシーンの初期化処理
		if (currentScene->IsReady() == false)
		{
			currentScene->Initialize();
		}
	}

	if (currentScene != nullptr)
	{
		currentScene->Update(elapsedTime);
	}
}

void SceneManager::Render()
{
	if (currentScene != nullptr)
	{
		currentScene->Render();
	}
}

void SceneManager::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
	}
}

// シーン切り替え
void SceneManager::ChangeScene(Scene* scene)
{
	// 新しいシーンを設定
	nextScene = scene;
}
