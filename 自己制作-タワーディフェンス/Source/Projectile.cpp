#include "Projectile.h"
#include "ProjectileManager.h"
#include "Graphics/Graphics.h"

Projectile::Projectile(ProjectileManager* manager)
	: manager(manager)
{
	manager->Register(this);
}

// デバッグプリミティブの描画
void Projectile::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用デバッグ球を描画
	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void Projectile::Destroy()
{
	manager->Remove(this);
}

// 行列更新処理
void Projectile::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	// 前ベクトルを算出
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	// 仮の上のベクトル算出
	Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
	Up = DirectX::XMVector3Normalize(Up);

	// 右ベクトルを算出
	Right = DirectX::XMVector3Cross(Up, Front);
	Right = DirectX::XMVector3Normalize(Right);

	// 上ベクトルを算出
	Up = DirectX::XMVector3Cross(Front, Right);

	// 計算結果を取り出し
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	// 算出した軸ベクトルから行列を作成
	transform._11 = right.x * scale.x;		// 右方向ベクトルの x（Xx）
	transform._12 = right.y * scale.x;		// 右方向ベクトルの y（Xy）
	transform._13 = right.z * scale.x;		// 右方向ベクトルの z（Xz）
	transform._14 = 0.0f;		
	transform._21 = up.x * scale.y;		// 上方向ベクトルの x（Yx）
	transform._22 = up.y * scale.y;		// 上方向ベクトルの y（Yy）
	transform._23 = up.z * scale.y;		// 上方向ベクトルの z（Yz）
	transform._24 = 0.0f;
	transform._31 = front.x * scale.z;		// 前方向ベクトルの x（Zx）
	transform._32 = front.y * scale.z;		// 前方向ベクトルの y（Zy）
	transform._33 = front.z * scale.z;		// 前方向ベクトルの z（Zz）
	transform._34 = 0.0f;
	transform._41 = position.x;	// 位置 x（Px）
	transform._42 = position.y;	// 位置 y（Py）
	transform._43 = position.z;	// 位置 z（Pz）
	transform._44 = 1.0f;

	// 発射方向
	this->direction = front;
}
