#pragma once

#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "Graphics/Shader.h"

#include <functional>

class UI
{
public:
	UI();
	~UI();

	void Update(float elapsedTime);
	void Render();

	// 敵生成用のコールバック関数
	void SetSpawnSpiderCallback(std::function<void()> callback);
	void SetSpawnGolemCallback(std::function<void()> callback);
private:

	Sprite* uiSpiderButton = nullptr;
	Sprite* uiGolemButton = nullptr;

	DirectX::XMFLOAT2 uiPositionSpider;
	DirectX::XMFLOAT2 uiPositionGolem;

	std::function<void()> spawnSpiderCallback;
	std::function<void()> spawnGolemCallback;

	float cooldownTimerSpider;  // スパイダーボタンのクールダウンタイマー
	float cooldownTimerGolem;   // ゴーレムボタンのクールダウンタイマー
	const float cooldownDuration; // クールダウンの時間 (秒)

	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
};
