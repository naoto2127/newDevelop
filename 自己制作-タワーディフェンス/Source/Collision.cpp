#include "Collision.h"

// ���Ƌ��̌�������
bool Collision::IntersectSphereVsSphere(
	const DirectX::XMFLOAT3& postionA, 
	float radiusA, 
	const DirectX::XMFLOAT3& postionB, 
	float radiusB,
	DirectX::XMFLOAT3& outPositionB)
{
	// positionA �� XMVECTOR �ɕϊ�
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&postionA);
	// positionB �� XMVECTOR �ɕϊ�
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&postionB);
	// positionA ���� positionB �ւ̃x�N�g�����v�Z(positionB - positionA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec �̒������v�Z(XMVector3LengthSq �𗘗p���Ă��ǂ�)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// radiusA �� radiusB �̒���
	float range = radiusA + radiusB;
	// lengthSq �� range ���傫���Ȃ� true �ł������Ă��Ȃ� 
	if (lengthSq > range)
	{
		// �������Ă��Ȃ�
		return false;
	}

	// �������Ă���ꍇ
	// Vec �����̒P�ʃx�N�g���iNormalize�j���擾
	Vec = DirectX::XMVector3Normalize(Vec);
	// ��L�̃x�N�g���� range ���X�P�[��
	Vec = DirectX::XMVectorScale(Vec, range);
	// ���̃x�N�g�����ʒu A �iPositionA�j����̑������ʒu�Ɉړ�
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// �o�͗p�̈ʒu�ioutPositionB�j�ɑ������
	DirectX::XMStoreFloat3(&outPositionB, Vec);

	return true;
}

// �~���Ɖ~���̌�������
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA, 
	float heightA, 
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB)
{
	// A �̑���(positionA.y) �� B �̓��ipositionB.y + height�j����Ȃ�
	if (positionA.y > positionB.y + heightB)
	{
		// �������ĂȂ�
		return false;
	}
	// A �̓��� B �̑�����艺�Ȃ�
	if (positionA.y + heightA < positionB.y)
	{
		// �������ĂȂ�
		return false;
	}




	// XZ ���ʂł͈̔̓`�F�b�N
	// positionB - positionA �̋�����
	DirectX::XMFLOAT2 posA(positionA.x, positionA.z);
	DirectX::XMFLOAT2 posB(positionB.x, positionB.z);

	// posA �� XMVECTOR �ɕϊ�
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
	// posB �� XMVECTOR �ɕϊ�
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
	// posA ���� posB �ւ̃x�N�g�����v�Z(posB - posA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec �̒������v�Z(XMVector2LengthSq �𗘗p���Ă��ǂ�)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// radiusA �� radiusB �̒���
	float range = radiusA + radiusB;

	// radisuA + radisuB ���傫��������
	if (lengthSq > range * range)
	{
		// �������ĂȂ�
		return false;
	}

	// A �� B �������o���iB �͉��ɉ����o���Ă͂����Ȃ��j

	// �������Ă���ꍇ

	// Vec �����̒P�ʃx�N�g���iNormalize�j���擾
	Vec = DirectX::XMVector2Normalize(Vec);
	// ��L�̃x�N�g���� range ���X�P�[��
	Vec = DirectX::XMVectorScale(Vec, range);
	// ���̃x�N�g�����ʒu A �iPositionA�j����̑������ʒu�Ɉړ�
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// �o�͗p�̈ʒu�ioutPositionB�j�ɑ������
	DirectX::XMFLOAT2 resultPos;
	DirectX::XMStoreFloat2(&resultPos, Vec);

	outPositionB.x = resultPos.x;
	outPositionB.y = positionB.y;
	outPositionB.z = resultPos.y;

	return true;
}

// ���Ɖ~���̌�������
bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition, 
	float sphereRadius, 
	const DirectX::XMFLOAT3& cylinderPosition, 
	float cylinderRadius, 
	float cylinderHeight, 
	DirectX::XMFLOAT3& outCylinderPosition)
{
	// ���̓�(.y + radius )���~���̑���(.y)��艺�Ȃ� false
	if (spherePosition.y + sphereRadius < cylinderPosition.y) 
		return false;

	// ���̑���( .y - radius)���~���̓�(.y + Height)����Ȃ� false
	if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderRadius)
		return false;

	// XZ ���ʂł͈̔̓`�F�b�N
	// positionB - positionA �̋�����
	DirectX::XMFLOAT2 posA(spherePosition.x, spherePosition.z);
	DirectX::XMFLOAT2 posB(cylinderPosition.x, cylinderPosition.z);

	// posA �� XMVECTOR �ɕϊ�
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
	// posB �� XMVECTOR �ɕϊ�
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
	// posA ���� posB �ւ̃x�N�g�����v�Z(posB - posA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec �̒������v�Z(XMVector2LengthSq �𗘗p���Ă��ǂ�)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// radiusA �� radiusB �̒���
	float range = sphereRadius + cylinderRadius;

	// radisuA + radisuB ���傫��������
	if (lengthSq > range)
	{
		// �������ĂȂ�
		return false;
	}

	// A �� B �������o���iB �͉��ɉ����o���Ă͂����Ȃ��j

	// �������Ă���ꍇ
	// Vec �����̒P�ʃx�N�g���iNormalize�j���擾
	Vec = DirectX::XMVector2Normalize(Vec);
	// ��L�̃x�N�g���� range ���X�P�[��
	Vec = DirectX::XMVectorScale(Vec, range);
	// ���̃x�N�g�����ʒu A �iPositionA�j����̑������ʒu�Ɉړ�
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// �o�͗p�̈ʒu�ioutPositionB�j�ɑ������
	DirectX::XMFLOAT2 resultPos;
	DirectX::XMStoreFloat2(&resultPos, Vec);


	outCylinderPosition.x = spherePosition.x + resultPos.x;
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = spherePosition.z + resultPos.y;

	return true;
}

//���C�ƃ��f���̌�������
bool Collision::IntersectRayVsModel(
	const DirectX::XMFLOAT3& start, 
	const DirectX::XMFLOAT3& end, 
	const Model* model, 
	HitResult& result)
{
	//���[���h���Wd�̃��C�̎n�_	
	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
	//���[���h���W�̃��C�̏I�_
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
	//���[���h���W�̃��C�̃x�N�g��
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	//���[���h���W�̃��C�̒���
	DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

	//���[���h��Ԃ̃��C�̒���
	DirectX::XMStoreFloat(&result.distance, WorldRayLength);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		//���b�V���m�[�h�擾
		const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

		//���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

		//���[�J����Ԃ̃��C�̎n�_
		DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		//���[�J����Ԃ̃��C�̏I�_
		DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		//���[�J����Ԃ̃��C�̃x�N�g��
		DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
		//���[�J����Ԃ̐��K�����ꂽ���C�̃x�N�g��
		DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
		//���[�J����Ԃ̃��C�̒���
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

		//���C�̒���
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		//�O�p�`�i�ʁj�Ƃ̌�������
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

				//�O�p�`�̒��_�𒊏o
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
				const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));
				
				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				//�O�p�`�̎O�Ӄx�N�g�����Z�o
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				//�O�p�`�̖@���x�N�g�����Z�o
				DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

				//���Ȃ̌��ʂ��v���X�Ȃ�Η������iDot �����ނ�d2�j
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
				float d;
				DirectX::XMStoreFloat(&d, Dot);
				if (d >= 0)continue;

				//���C�̕��ʂ̌�_���Z�o
				DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
				//DirectX::XMVector3Dot(N,SA) �����ނ� d1
				DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), DirectX::XMVector3Dot(N, V));
				//DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), DirectX::XMVec);
				
				float x;
				DirectX::XMStoreFloat(&x, X);
				if (x < 0 || x > neart) continue;
				//��_�܂ł̋��������܂łɌv�Z�����ŋߋ������
				//�傫���Ƃ��̓X�L�b�v

				DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V,X), S);
			
				//��_���O�p�`�̓����񂢂��邩����
				//�P��
				DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f)continue;

				//�Q��
				DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f)continue;

				//�R��
				DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f)continue;

				//�ŋߋ������X�V
				neart = x;

				//��_�Ɩ@�����X�V
				HitPosition = P;
				HitNormal = N;
				materialIndex = subset.materialIndex;
			}
		}
		if (materialIndex >= 0)
		{
			// ���[�J����Ԃ��烏�[���h��Ԃɕϊ�
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(
				HitPosition, WorldTransform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(
				WorldPosition, WorldStart);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(
				WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			//�q�b�g���ۑ�
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


	////�ȑO�̏����������������悤�ɉ��̎���
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
/// �X�P�[�����l�������~���Ɖ~���̌�������
/// </summary>
/// <param name="positionA">�~��A�̈ʒu</param>
/// <param name="radiusA">�~��A�̔��a</param>
/// <param name="heightA">�~��A�̍���</param>
/// <param name="scaleA">�~��A�̑傫��</param>
/// <param name="positionB">�~��B�̈ʒu</param>
/// <param name="radiusB">�~��B�̔��a</param>
/// <param name="heightB">�~��B�̍���</param>
/// <param name="scaleB">�~��B�̑傫��</param>
/// <param name="outPositionB">�~��B�̔����̉����o���ʒu</param>
/// <returns></returns>
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA, float heightA, const DirectX::XMFLOAT3& scaleA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB, float heightB, const DirectX::XMFLOAT3& scaleB,
	DirectX::XMFLOAT3& outPositionB)
{
	// �X�P�[����K�p�������a�ƍ������v�Z
	float scaledRadiusA = radiusA * scaleA.x; // ���a��X���X�P�[��
	float scaledHeightA = heightA * scaleA.y; // ������Y���X�P�[��
	float scaledRadiusB = radiusB * scaleB.x; // ���a��X���X�P�[��
	float scaledHeightB = heightB * scaleB.y; // ������Y���X�P�[��

	// ���������̌�������
	if (positionA.y > positionB.y + scaledHeightB || positionA.y + scaledHeightA < positionB.y)
	{
		// �����Ō������Ă��Ȃ�
		return false;
	}

	// XZ ���ʂł͈̔̓`�F�b�N
	DirectX::XMFLOAT2 posA(positionA.x, positionA.z);
	DirectX::XMFLOAT2 posB(positionB.x, positionB.z);

	// posA �� XMVECTOR �ɕϊ�
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat2(&posA);
	// posB �� XMVECTOR �ɕϊ�
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat2(&posB);
	// posA ���� posB �ւ̃x�N�g�����v�Z(posB - posA)
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	// Vec �̒������v�Z(XMVector2LengthSq �𗘗p)
	DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// �X�P�[���K�p��̍��v���a
	float range = scaledRadiusA + scaledRadiusB;

	// ������ range �𒴂���ꍇ�͌������Ă��Ȃ�
	if (lengthSq > range * range)
	{
		return false;
	}

	// �������Ă���ꍇ
	// Vec �����̒P�ʃx�N�g���iNormalize�j���擾
	Vec = DirectX::XMVector2Normalize(Vec);
	// ��L�̃x�N�g���� range ���X�P�[��
	Vec = DirectX::XMVectorScale(Vec, range);
	// ���̃x�N�g�����ʒu A �iPositionA�j���瑫�����ʒu�Ɉړ�
	Vec = DirectX::XMVectorAdd(PositionA, Vec);
	// �o�͗p�̈ʒu�ioutPositionB�j�ɑ������
	DirectX::XMFLOAT2 resultPos;
	DirectX::XMStoreFloat2(&resultPos, Vec);

	// Y���W�͕ύX�����ɁAXZ���ʂŌv�Z�����ʒu�����ʂƂ��ĕԂ�
	outPositionB.x = resultPos.x;
	outPositionB.y = positionB.y; // Y�͕ύX�Ȃ�
	outPositionB.z = resultPos.y;

	return true;
}
