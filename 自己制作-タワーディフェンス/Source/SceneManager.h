#pragma once

#include "Scene.h"

// �V�[���}�l�[�W���[
class SceneManager
{
private:
	SceneManager() {}
	~SceneManager() {}

public:
	// �B��̃C���X�^���X�擾
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揊��
	void Render();

	// �V�[���̃N���A
	void Clear();

	/// <summary>
	/// �V�[���؂�ւ�
	/// </summary>
	/// <param name="scene">���̃V�[���ɂȂ�V�[��</param>
	void ChangeScene(Scene* scene);

private:
	Scene* currentScene = nullptr;	// ���݂̃V�[��
	Scene* nextScene = nullptr;		// ���̃V�[��
};