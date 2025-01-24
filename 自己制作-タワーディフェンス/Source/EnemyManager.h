#pragma once

#include <vector>
#include "Enemy.h"
#include <set>
#include"Tower.h"

// エネミー管理
class EnemyManager
{
private:
	EnemyManager() {}
	~EnemyManager() {}

public:
	// 唯一のインスタンス取得
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// エネミー登録
	void Register(Enemy* enemy);

	// エネミー全削除
	void Clear();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// エネミー取得
	Enemy* GetEnemy(int index) { return enemies.at(index); }

	/*void Attack();*/

	// 弾丸削除
	void Remove(Enemy* enemy);

	void SpawnEnemy(Tower* target,Tower*spawnTower);



private:
	// エネミー同士の衝突処理
	void CollisionEnemyVsEnemies();

private:
	std::vector<Enemy*>	enemies;

	std::set<Enemy*> removes;
};