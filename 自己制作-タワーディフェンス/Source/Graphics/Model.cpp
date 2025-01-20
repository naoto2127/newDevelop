#include "Graphics/Graphics.h"
#include "Graphics/Model.h"

// �R���X�g���N�^
Model::Model(const char* filename)
{
	// ���\�[�X�ǂݍ���
	resource = std::make_shared<ModelResource>();
	resource->Load(Graphics::Instance().GetDevice(), filename);

	// �m�[�h
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// �s��v�Z
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// �ϊ��s��v�Z
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

//�A�j���[�V�����X�V����
void Model::UpdateAnimation(float elapsedTime)
{
	//�Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation())return;

	//�u�����h���̌v�Z
	float blendRate = 1.0f;
	if (animationBlendTime < animationBlendSeconds)
	{
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
			blendRate = animationBlendTime / animationBlendSeconds;
			blendRate *= blendRate;
		}
	}


	//�w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSecounds >= keyframe0.seconds &&
			currentAnimationSecounds < keyframe1.seconds)
		{
			//�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (currentAnimationSecounds - keyframe0.seconds) /
				(keyframe1.seconds - keyframe0.seconds);
			float nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//�Q�̃L�[�t���[���Ԃ̕⊮����
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				//�u�����h�⊮����
				if (blendRate < 1.0f)
				{
					//���݂̎p���Ǝ��̃L�[�t���[���̎p���̕⊮�i�j
					//node �� key1 �� scale �� XMVECTOR �ɕϊ�
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					//node �� key1 �� rotate �� XMVECTOR �ɕϊ�
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					//node �� key1 �� translate �� XMVECTOR �ɕϊ�
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);
					//�g��k���̐��`��Ԃ� rate �Ōv�Z
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					//��]���̋��ʐ��`��Ԃ� rate �Ōv�Z
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					//�ʒu�̐��`��Ԃ� rate �Ōv�Z
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);
					//�v�Z���ʂ��{�[���Ɋi�[�inode �� scale rotate translate �� XMFLOAT3 or 4 �ɕϊ��j
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				//�ʏ�̌v�Z
				else
				{
					//key0��key1��scale��XMVECTOR�ɕϊ�
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					//key0 �� key1 �� rotate �� XMVECTOR �ɕϊ�
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					//key0 �� key1 �� translate �� XMVECTOR �ɕϊ�
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					//�g��k���̐��`��Ԃ�rate�Ōv�Z
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					//��]���̋��ʐ��`��Ԃ�rate�Ōv�Z
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					//�ʒu�̐��`��Ԃ�rate�Ōv�Z
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);
					//�v�Z���ʂ��{�[���Ɋi�[�inode �� scale rotate translate �� XMVECTOR4 �ɕϊ��j
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

			}
			break;
		}
	}
	//�ŏI�t���[������
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		return;
	}

	//���Ԍo��
	currentAnimationSecounds += elapsedTime;

	//�Đ����Ԃ��T�Z���Ԃ𒴂�����
	if (currentAnimationSecounds >= animation.secondsLength)
	{
		if (animationLoopFlag)
		{
			//�Đ����Ԃ������߂�
			currentAnimationSecounds -= animation.secondsLength;
		}
		//�Đ��I�����Ԃɂ���
		else
		{
			currentAnimationSecounds = animation.secondsLength;
			animationEndFlag = true;
		}
	}
}

//�A�j���[�V�����Đ�
void Model::PlayAnimation(int index, bool loop, float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationSecounds = 0.0f;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

//�A�j���[�V�����Đ�����
bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0)return false;
	if (currentAnimationIndex >= resource->GetAnimations().size())return false;
	return true;
}

Model::Node* Model::FindNode(const char* name)
{
	// �S�Ẵm�[�h�𑍓�����Ŗ��O��r����
	for (Node& node : nodes)  // nodes�x�N�^�[���̊eNode�����[�v
	{
		// �����̖��O�ƃm�[�h�̖��O���r����v���邩�`�F�b�N
		if (strcmp(node.name, name) == 0)  // �m�[�h�̖��O�������̖��O�ƈ�v���邩�m�F
		{
			// ��v�����炻�̃m�[�h�̃|�C���^�����^�[��
			return &node;  // ��v�����m�[�h���|�C���^�ŕԂ�
		}
	}

	// ������Ȃ������ꍇ��nullptr��Ԃ�
	return nullptr;
}