#pragma once

#include <d3d11.h>
#include "Graphics/Shader.h"
#include "Player.h"
#include "Object.h"

class Tower : public Object
{
public:

	Tower();
	virtual ~Tower() { delete model; }

	void Update(float elapsedTime);

	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//デバックプリミティブ描画
	void DrawDebugPrimitive();

	//タワー行列処理関数
	void UpdateTransform();

	//タワーデバック描画
	void DrawDebugGUI();

	//タワーの当たり判定（Character用）
	bool CheckCollision(const DirectX::XMFLOAT3& charPos, float charRadius, float charHeight);

	//Towerの位置を取得
	DirectX::XMFLOAT3 GetPosition() { return position; }

	//Towerの位置を設定
	DirectX::XMFLOAT3 SetPosition(DirectX::XMFLOAT3 p) { return position = p; }
	
	//Towerの回転を設定
	DirectX::XMFLOAT3 SetRotation(DirectX::XMFLOAT3 r) { return angle = r; }
	
	//Towerの半径を取得
	float GetRadius() { return radius; }
private:
	Model* model = nullptr;

	DirectX::XMFLOAT3	position = { 0,0,0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,	// 右方向
		0,1,0,0,	// 上方向
		0,0,1,0,	// 前方向
		0,0,0,1
	};

	// 半径
	float radius = 100.0f;
	// 高さ
	float height = 280.0f;
};