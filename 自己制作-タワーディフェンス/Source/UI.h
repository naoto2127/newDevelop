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

	void DrawDebugGUI();
	
	void SetCooldownTimerSpider(float t) { cooldownTimerSpider = t; }
	void SetCooldownTimerGolem(float t) { cooldownTimerGolem = t; }
	void SetButtonSpider(bool s) { isSpiderButtonEnabled = s; }
	void SetButtonGolem(bool g) { isGolemButtonEnabled = g; }

	bool GetButtonSpider() { return isSpiderButtonEnabled; }
	bool GetButtonGolem() { return isGolemButtonEnabled; }

private:

	Sprite* uiSpiderButton = nullptr;
	Sprite* uiGolemButton = nullptr;

	DirectX::XMFLOAT2 uiPositionSpider;
	DirectX::XMFLOAT2 uiPositionGolem;

	std::function<void()> spawnSpiderCallback;
	std::function<void()> spawnGolemCallback;

	float cooldownTimerSpider;  // スパイダーボタンのクールダウンタイマー
	float cooldownTimerGolem;   // ゴーレムボタンのクールダウンタイマー
	const float SPAWN_COOLDOWN_TIME; // クールダウンの時間 (秒)

	// ボタンが有効かどうかを管理するフラグ
	bool isSpiderButtonEnabled = true;
	bool isGolemButtonEnabled = true;

	Graphics& graphics = Graphics::Instance();
	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
};
