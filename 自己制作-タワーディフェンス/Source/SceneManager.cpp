#include "SceneManager.h"

void SceneManager::Update(float elapsedTime)
{
	// ����ւ���ׂ����̃V�[������������
	if (nextScene != nullptr)
	{
		// �Â��V�[��(�܂茻�݂̃V�[��)�̏I���������s��
		Clear();

		// �V�����V�[��(����ւ���ׂ����̃V�[��)��ݒ�
		currentScene = nextScene;
		nextScene = nullptr;

		// ����ւ�����V�������݂̃V�[���̏���������
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

// �V�[���؂�ւ�
void SceneManager::ChangeScene(Scene* scene)
{
	// �V�����V�[����ݒ�
	nextScene = scene;
}
