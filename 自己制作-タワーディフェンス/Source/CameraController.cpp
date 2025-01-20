#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

void CameraController::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	// カメラの回転速度
	float speed = rollSpeed * elapsedTime;

	// スティックの入力値に合わせて X 軸と Y 軸を回転
	angle.x += ay * speed;	// X 軸
	angle.y += ax * speed;	// Y 軸

	// X 軸のカメラ回転を制限
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}

	// Y 軸の回転値を -3.14 ～ 3.14 に収まるようにする
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	//カメラのズームイン、アウトの設定
	if (gamePad.GetButton() & GamePad::BTN_A)
	{
		range += 0.01f;
	}
	if (gamePad.GetButton() & GamePad::BTN_B)
	{
		range -= 0.01f;
	}

	// カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	// 回転行列から前方ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;		// x
	eye.y = target.y - front.y * range;		// y
	eye.z = target.z - front.z * range;		// z

	// カメラの視点と注視店を設定
	Camera::Instance().SetLookAt(eye, target, { 0,1,0 });
}
