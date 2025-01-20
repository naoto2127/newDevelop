#pragma once

#include <vector>
#include "Tower.h"
#include <set>

// エネミー管理
class TowerManager
{
private:
	TowerManager() {}
	~TowerManager() {}

public:
	// 唯一のインスタンス取得
	static TowerManager& Instance()
	{
		static TowerManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// エネミー登録
	void Register(Tower* tower);

	// エネミー全削除
	void Clear();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	//タワーデバック描画
	void DrawDebugGUI();

	// タワー数取得
	int GetTowerCount() const { return static_cast<int>(towers.size()); }

	// タワー取得
	Tower* GetTower(int index) { 
		if (index < 0 || index >= towers.size()) return nullptr; // 範囲外チェック
		return towers[index]; 
	}

	//// タワー一覧を取得
	//const std::vector<Tower*>& GetAllTowers() const { return towers; }


	// 弾丸削除
	void Remove(Tower* tower);

private:
	std::vector<Tower*>	towers;

	std::set<Tower*> removes;
};