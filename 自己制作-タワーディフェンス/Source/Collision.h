#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"

//�q�b�g����
struct HitResult
{
	DirectX::XMFLOAT3 position = { 0, 0, 0 };//���C�ƃ|���S���̌�_
	DirectX::XMFLOAT3 normal = { 0,0,0 };//�Փ˂����|���S���̖@���x�N�g��
	float	distance = 0.0f;	//���C�̎n�_�����_�܂ł̋���
	int		materialIndex = -1;//�Փ˂����|���S���̃}�e���A���ԍ�
};

// �R���W����
class Collision
{
public:
	int test;

public:
	/// <summary>
	/// ���Ƌ��̌�������
	/// </summary>
	/// <param name="postionA">�ʒuA</param>
	/// <param name="radiusA">���aA</param>
	/// <param name="postionB">�ʒuB</param>
	/// <param name="radiusB">���aB</param>
	/// <param name="outPositionB">�o�͗p�����o����̈ʒu</param>
	/// <returns></returns>
	static bool IntersectSphereVsSphere(
		const DirectX::XMFLOAT3& postionA,
		float radiusA,
		const DirectX::XMFLOAT3& postionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionB
	);

	/// <summary>
	/// �~���Ɖ~���̌�������
	/// </summary>
	/// <param name="postionA">�ʒuA</param>
	/// <param name="radiusA">���aA</param>
	/// <param name="heightA">����A</param>
	/// <param name="postionB">�ʒuB</param>
	/// <param name="radiusB">���aB</param>
	/// <param name="heightB">����B</param>
	/// <param name="outPositionB">�o�͗p�����o����̈ʒu</param>
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
	/// ���Ɖ~���̏Փ˔���
	/// </summary>
	/// <param name="spherePosition">���̈ʒu</param>
	/// <param name="shereRadius">���̔��a</param>
	/// <param name="cylinderPosition">�~���̈ʒu</param>
	/// <param name="cylinderRadius">�~���̔��a</param>
	/// <param name="cylinderHeight">�~���̍���</param>
	/// <param name="outCylinderPosition"></param>
	/// <returns>�o�͗p�����o����̈ʒu</returns>
	static bool IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3& spherePosition,
		float shereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition
	);

	//���C�ƃ��f���̌�������
	static bool IntersectRayVsModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& result
	);

	//�X�P�[�����l�������~���Ɖ~���̌�������
	static bool Collision::IntersectCylinderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA, float heightA, const DirectX::XMFLOAT3& scaleA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB, float heightB, const DirectX::XMFLOAT3& scaleB,
		DirectX::XMFLOAT3& outPositionB);
};