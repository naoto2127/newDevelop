#pragma once
#include <vector>
#include "PlayerMonster.h"
#include <set>
// プレイヤーのモンスター管理
class PlayerMonsterManager
{
private:
	PlayerMonsterManager() {}
	~PlayerMonsterManager() {}
public:
	// 唯一のインスタンス取得
	static PlayerMonsterManager& Instance()
	{
		static PlayerMonsterManager instance;
		return instance;
	}
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);
	// プレイヤーのモンスター登録
	void Register(PlayerMonster* monster);
	// プレイヤーのモンスター全削除
	void Clear();
	// デバッグプリミティブ描画
	void DrawDebugPrimitive();
	// プレイヤーのモンスター数取得
	int GetMonsterCount() const { return static_cast<int>(monsters.size()); }
	// プレイヤーのモンスター取得
	PlayerMonster* GetMonster(int index) { return monsters.at(index); }
	void Attack();
	// 弾丸削除
	void Remove(PlayerMonster* monster);
private:
	// プレイヤーのモンスター同士の衝突処理
	void CollisionMonsterVsMonsters();
private:
	std::vector<PlayerMonster*>	monsters;
	std::set<PlayerMonster*> removes;
};