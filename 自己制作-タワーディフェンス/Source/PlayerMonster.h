#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
// プレイヤーのモンスター
class PlayerMonster : public Character
{
public:
	PlayerMonster() {}
	~PlayerMonster() override {}
	// 更新処理
	virtual void Update(float elapsedTime) = 0;
	// 描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
	virtual void Attack() = 0;
	void Destroy();
	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();
	// その他のメンバ関数（位置、半径）
	DirectX::XMFLOAT3 GetPosition() const { return position; }
	float GetRadius() const { return radius; }
};