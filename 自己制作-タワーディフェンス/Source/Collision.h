#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"

//ヒット結果
struct HitResult
{
	DirectX::XMFLOAT3 position = { 0, 0, 0 };//レイとポリゴンの交点
	DirectX::XMFLOAT3 normal = { 0,0,0 };//衝突したポリゴンの法線ベクトル
	float	distance = 0.0f;	//レイの始点から交点までの距離
	int		materialIndex = -1;//衝突したポリゴンのマテリアル番号
};

// コリジョン
class Collision
{
public:
	int test;

public:
	/// <summary>
	/// 球と球の交差判定
	/// </summary>
	/// <param name="postionA">位置A</param>
	/// <param name="radiusA">半径A</param>
	/// <param name="postionB">位置B</param>
	/// <param name="radiusB">半径B</param>
	/// <param name="outPositionB">出力用押し出し後の位置</param>
	/// <returns></returns>
	static bool IntersectSphereVsSphere(
		const DirectX::XMFLOAT3& postionA,
		float radiusA,
		const DirectX::XMFLOAT3& postionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionB
	);

	/// <summary>
	/// 円柱と円柱の交差判定
	/// </summary>
	/// <param name="postionA">位置A</param>
	/// <param name="radiusA">半径A</param>
	/// <param name="heightA">高さA</param>
	/// <param name="postionB">位置B</param>
	/// <param name="radiusB">半径B</param>
	/// <param name="heightB">高さB</param>
	/// <param name="outPositionB">出力用押し出し後の位置</param>
	/// <returns></returns>
	static bool IntersectCylinderVsCylinder(
		const DirectX::XMFLOAT3& postionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& postionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3& outPositionB
	);

	/// <summary>
	/// 球と円柱の衝突判定
	/// </summary>
	/// <param name="spherePosition">球の位置</param>
	/// <param name="shereRadius">球の半径</param>
	/// <param name="cylinderPosition">円柱の位置</param>
	/// <param name="cylinderRadius">円柱の半径</param>
	/// <param name="cylinderHeight">円柱の高さ</param>
	/// <param name="outCylinderPosition"></param>
	/// <returns>出力用押し出し後の位置</returns>
	static bool IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3& spherePosition,
		float shereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition
	);

	//レイとモデルの交差判定
	static bool IntersectRayVsModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& result
	);

	//スケールを考慮した円柱と円柱の交差判定
	static bool Collision::IntersectCylinderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA, float heightA, const DirectX::XMFLOAT3& scaleA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB, float heightB, const DirectX::XMFLOAT3& scaleB,
		DirectX::XMFLOAT3& outPositionB);
};