#pragma once

#include "CameraController.h"
#include "Scene.h"
#include "Tower.h"

#include "Graphics/Sprite.h"
#include <DirectXMath.h>// DirectX::XMFLOAT3, XMConvertToRadians

#include "UI.h"


// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

private:
	//エネミーHPゲージ描画
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	void RenderEnemyArrows(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	//UI表示
	void RenderUIButton(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);
	
	//デバック表示（後で消す）
	void DebugDrawGUI();
private:
	enum
	{
		numEnemyTower,
		numPlayerTower,
		numFortFirst,
	};

private:

	CameraController* cameraController = nullptr;

	Sprite* gauge{};

	//進行方向の矢印表示
	Sprite* uiArrows{};

	//砦の初期設定
	static const int numFort = 3;
	Tower* fort[numFort]{};
	Tower* playerTower = {}; // タワーポインタ配列を初期化
	Tower* enemyTower = {}; // タワーポインタ配列を初期化

	UI* ui{};

	DirectX::XMFLOAT3 target;
	float range;


private:
	float positionX; // ボタンのX座標
	float positionY; // ボタンのY座標
};
