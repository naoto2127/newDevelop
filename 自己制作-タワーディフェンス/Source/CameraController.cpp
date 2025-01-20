#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

void CameraController::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	// �J�����̉�]���x
	float speed = rollSpeed * elapsedTime;

	// �X�e�B�b�N�̓��͒l�ɍ��킹�� X ���� Y ������]
	angle.x += ay * speed;	// X ��
	angle.y += ax * speed;	// Y ��

	// X ���̃J������]�𐧌�
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}

	// Y ���̉�]�l�� -3.14 �` 3.14 �Ɏ��܂�悤�ɂ���
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	//�J�����̃Y�[���C���A�A�E�g�̐ݒ�
	if (gamePad.GetButton() & GamePad::BTN_A)
	{
		range += 0.01f;
	}
	if (gamePad.GetButton() & GamePad::BTN_B)
	{
		range -= 0.01f;
	}

	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	// ��]�s�񂩂�O���x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;		// x
	eye.y = target.y - front.y * range;		// y
	eye.z = target.z - front.z * range;		// z

	// �J�����̎��_�ƒ����X��ݒ�
	Camera::Instance().SetLookAt(eye, target, { 0,1,0 });
}
