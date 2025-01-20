#include "Collision.h"

// 球と球の交差判定
bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& postionA, 
	float radiusA, 
	const DirectX::XMFLOAT3& postionB, 
	float radiusB,
	DirectX::XMFLOAT3& outPositionB)
{
	// positionA を XMVECTOR に変換
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&postionA);
	// positionB を XMVECTOR に変換
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&postionB);
	// positionA から positionB へのベクトルを計算(positionB - positionA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec の長さを計算(XMVector3LengthSq を利用しても良い)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// radiusA と radiusB の長さ
	float range = radiusA + radiusB;
	// lengthSq が range より大きいなら true であたっていない 
	if (lengthSq > range)
	{
		// あたっていない
		return false;
	}

	// あたっている場合
	// Vec 方向の単位ベクトル（Normalize）を取得
	Vec = DirectX::XMVector3Normalize(Vec);
	// 上記のベクトルを range 分スケール
	Vec = DirectX::XMVectorScale(Vec, range);
	// そのベクトルを位置 A （PositionA）からの足した位置に移動
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// 出力用の位置（outPositionB）に代入する
	DirectX::XMStoreFloat3(&outPositionB, Vec);

	return true;
}

// 円柱と円柱の交差判定
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA, 
	float heightA, 
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB)
{
	// A の足元(positionA.y) が B の頭（positionB.y + height）より上なら
	if (positionA.y > positionB.y + heightB)
	{
		// 当たってない
		return false;
	}
	// A の頭が B の足もより下なら
	if (positionA.y + heightA < positionB.y)
	{
		// 当たってない
		return false;
	}




	// XZ 平面での範囲チェック
	// positionB - positionA の距離が
	DirectX::XMFLOAT2 posA(positionA.x, positionA.z);
	DirectX::XMFLOAT2 posB(positionB.x, positionB.z);

	// posA を XMVECTOR に変換
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
	// posB を XMVECTOR に変換
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
	// posA から posB へのベクトルを計算(posB - posA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec の長さを計算(XMVector2LengthSq を利用しても良い)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// radiusA と radiusB の長さ
	float range = radiusA + radiusB;

	// radisuA + radisuB より大きかったら
	if (lengthSq > range * range)
	{
		// 当たってない
		return false;
	}

	// A が B を押し出す（B は下に押し出してはいけない）

	// あたっている場合

	// Vec 方向の単位ベクトル（Normalize）を取得
	Vec = DirectX::XMVector2Normalize(Vec);
	// 上記のベクトルを range 分スケール
	Vec = DirectX::XMVectorScale(Vec, range);
	// そのベクトルを位置 A （PositionA）からの足した位置に移動
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// 出力用の位置（outPositionB）に代入する
	DirectX::XMFLOAT2 resultPos;
	DirectX::XMStoreFloat2(&resultPos, Vec);

	outPositionB.x = resultPos.x;
	outPositionB.y = positionB.y;
	outPositionB.z = resultPos.y;

	return true;
}

// 球と円柱の交差判定
bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition, 
	float sphereRadius, 
	const DirectX::XMFLOAT3& cylinderPosition, 
	float cylinderRadius, 
	float cylinderHeight, 
	DirectX::XMFLOAT3& outCylinderPosition)
{
	// 球の頭(.y + radius )が円柱の足元(.y)より下なら false
	if (spherePosition.y + sphereRadius < cylinderPosition.y) 
		return false;

	// 球の足元( .y - radius)が円柱の頭(.y + Height)より上なら false
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderRadius)
		return false;

	// XZ 平面での範囲チェック
	// positionB - positionA の距離が
	DirectX::XMFLOAT2 posA(spherePosition.x, spherePosition.z);
	DirectX::XMFLOAT2 posB(cylinderPosition.x, cylinderPosition.z);

	// posA を XMVECTOR に変換
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
	// posB を XMVECTOR に変換
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
	// posA から posB へのベクトルを計算(posB - posA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec の長さを計算(XMVector2LengthSq を利用しても良い)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// radiusA と radiusB の長さ
	float range = sphereRadius + cylinderRadius;

	// radisuA + radisuB より大きかったら
	if (lengthSq > range)
	{
		// 当たってない
		return false;
	}

	// A が B を押し出す（B は下に押し出してはいけない）

	// あたっている場合
	// Vec 方向の単位ベクトル（Normalize）を取得
	Vec = DirectX::XMVector2Normalize(Vec);
	// 上記のベクトルを range 分スケール
	Vec = DirectX::XMVectorScale(Vec, range);
	// そのベクトルを位置 A （PositionA）からの足した位置に移動
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// 出力用の位置（outPositionB）に代入する
	DirectX::XMFLOAT2 resultPos;
	DirectX::XMStoreFloat2(&resultPos, Vec);


	outCylinderPosition.x = spherePosition.x + resultPos.x;
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = spherePosition.z + resultPos.y;

	return true;
}

//レイとモデルの交差判定
bool Collision::IntersectRayVsModel(
	const DirectX::XMFLOAT3& start, 
	const DirectX::XMFLOAT3& end, 
	const Model* model, 
	HitResult& result)
{
	//ワールド座標dのレイの始点	
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
	//ワールド座標のレイの終点
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
	//ワールド座標のレイのベクトル
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	//ワールド座標のレイの長さ
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

	//ワールド空間のレイの長さ
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		//メッシュノード取得
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

		//レイをワールド空間からローカル空間へ変換
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

		//ローカル空間のレイの始点
		DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		//ローカル空間のレイの終点
		DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		//ローカル空間のレイのベクトル
		DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
		//ローカル空間の正規化されたレイのベクトル
		DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
		//ローカル空間のレイの長さ
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

		//レイの長さ
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		//三角形（面）との交差判定
		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		const std::vector<UINT> indices = mesh.indices;

		int materialIndex = -1;
		DirectX::XMVECTOR HitPosition;
		DirectX::XMVECTOR HitNormal;
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.indexCount; i += 3)
			{
				UINT index = subset.startIndex + i;

				//三角形の頂点を抽出
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));
				
				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				//三角形の三辺ベクトルを算出
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				//三角形の法線ベクトルを算出
				DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

				//内席の結果がプラスならば裏向き（Dot が教材のd2）
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
				float d;
				DirectX::XMStoreFloat(&d, Dot);
				if (d >= 0)continue;

				//レイの平面の交点を算出
				DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
				//DirectX::XMVector3Dot(N,SA) が教材の d1
				DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), DirectX::XMVector3Dot(N, V));
				//DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), DirectX::XMVec);
				
				float x;
				DirectX::XMStoreFloat(&x, X);
				if (x < 0 || x > neart) continue;
				//交点までの距離が今までに計算した最近距離より
				//大きいときはスキップ

				DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V,X), S);
			
				//交点が三角形の内側んいあるか判定
				//１つめ
				DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f)continue;

				//２つめ
				DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f)continue;

				//３つめ
				DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f)continue;

				//最近距離を更新
				neart = x;

				//交点と法線を更新
				HitPosition = P;
				HitNormal = N;
				materialIndex = subset.materialIndex;
			}
		}
		if (materialIndex >= 0)
		{
			// ローカル空間からワールド空間に変換
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(
				HitPosition, WorldTransform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(
				WorldPosition, WorldStart);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(
				WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			//ヒット情報保存
			if (result.distance > distance)
			{
				DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(
					HitNormal, WorldTransform);

				result.distance = distance;
				result.materialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.position, WorldPosition);
				DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
				hit = true;
			}

		}
	}
	return hit;


	////以前の処理が正しく動くように仮の実装
	//if (end.y < 0.0f)
	//{
	//	result.position.x = end.x;
	//	result.position.y = 0.0f;
	//	result.position.z = end.z;
	//	result.normal.x = 0.0f;
	//	result.normal.y = 1.0f;
	//	result.normal.z = 0.0f;
	//	return true;
	//}
	//return false;
}

/// <summary>
/// スケールを考慮した円柱と円柱の交差判定
/// </summary>
/// <param name="positionA">円柱Aの位置</param>
/// <param name="radiusA">円柱Aの半径</param>
/// <param name="heightA">円柱Aの高さ</param>
/// <param name="scaleA">円柱Aの大きさ</param>
/// <param name="positionB">円柱Bの位置</param>
/// <param name="radiusB">円柱Bの半径</param>
/// <param name="heightB">円柱Bの高さ</param>
/// <param name="scaleB">円柱Bの大きさ</param>
/// <param name="outPositionB">円柱Bの判定後の押し出し位置</param>
/// <returns></returns>
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA, float heightA, const DirectX::XMFLOAT3& scaleA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB, float heightB, const DirectX::XMFLOAT3& scaleB,
	DirectX::XMFLOAT3& outPositionB)
{
	// スケールを適用した半径と高さを計算
	float scaledRadiusA = radiusA * scaleA.x; // 半径はX軸スケール
	float scaledHeightA = heightA * scaleA.y; // 高さはY軸スケール
	float scaledRadiusB = radiusB * scaleB.x; // 半径はX軸スケール
	float scaledHeightB = heightB * scaleB.y; // 高さはY軸スケール

	// 高さ方向の交差判定
	if (positionA.y > positionB.y + scaledHeightB || positionA.y + scaledHeightA < positionB.y)
	{
		// 高さで交差していない
		return false;
	}

	// XZ 平面での範囲チェック
	DirectX::XMFLOAT2 posA(positionA.x, positionA.z);
	DirectX::XMFLOAT2 posB(positionB.x, positionB.z);

	// posA を XMVECTOR に変換
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
	// posB を XMVECTOR に変換
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
	// posA から posB へのベクトルを計算(posB - posA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec の長さを計算(XMVector2LengthSq を利用)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// スケール適用後の合計半径
	float range = scaledRadiusA + scaledRadiusB;

	// 距離が range を超える場合は交差していない
	if (lengthSq > range * range)
	{
		return false;
	}

	// 当たっている場合
	// Vec 方向の単位ベクトル（Normalize）を取得
	Vec = DirectX::XMVector2Normalize(Vec);
	// 上記のベクトルを range 分スケール
	Vec = DirectX::XMVectorScale(Vec, range);
	// そのベクトルを位置 A （PositionA）から足した位置に移動
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// 出力用の位置（outPositionB）に代入する
	DirectX::XMFLOAT2 resultPos;
	DirectX::XMStoreFloat2(&resultPos, Vec);

	// Y座標は変更せずに、XZ平面で計算した位置を結果として返す
	outPositionB.x = resultPos.x;
	outPositionB.y = positionB.y; // Yは変更なし
	outPositionB.z = resultPos.y;

	return true;
}
