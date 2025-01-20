#pragma once

// シーン
class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}
	// 初期化
	virtual void Initialize() = 0;
	// 終了処理
	virtual void Finalize() = 0;
	// 更新処理
	virtual void Update(float elapsedTime) = 0;
	// 描画処理
	virtual void Render() = 0;

	// 準備が完了しているかどうか
	bool IsReady() { return ready; }

	// 準備完了設定
	void SetReady() { ready = true; }


private:
	bool ready = false;
};