#pragma once

#include "Graphics/Shader.h"
#include "Collision.h"

// ステージ
class Stage
{
public:
	Stage() {}
	virtual ~Stage() {}

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//レイキャスト
	virtual bool RayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		HitResult& hit) = 0;

private:
	DirectX::XMFLOAT3	position = { 0,0,0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,	// 右方向
		0,1,0,0,	// 上方向
		0,0,1,0,	// 前方向
		0,0,0,1
	};
};