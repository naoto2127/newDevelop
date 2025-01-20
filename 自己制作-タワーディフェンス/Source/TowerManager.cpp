#include "TowerManager.h"
#include "Collision.h"

#include <imgui.h>

void TowerManager::Update(float elapsedTime)
{
	for (Tower* Tower : towers)
	{
		Tower->Update(elapsedTime);
	}

	// ”jŠüˆ—
	for (Tower* tower : removes)
	{
		std::vector<Tower*>::iterator it = std::find(towers.begin(), towers.end(), tower);

		if (it != towers.end())
		{
			towers.erase(it);
		}

		delete tower;
	}

	removes.clear();
}

void TowerManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (Tower* tower : towers)
	{
		tower->Render(context, shader);
	}
}

void TowerManager::Register(Tower* tower)
{
	towers.emplace_back(tower);
}

void TowerManager::Clear()
{
	for (Tower* tower : towers)
	{
		delete tower;
	}
	towers.clear();
}

void TowerManager::DrawDebugPrimitive()
{
	for (Tower* tower : towers)
	{
		tower->DrawDebugPrimitive();
	}
}

void TowerManager::Remove(Tower* tower)
{
	removes.insert(tower);
}

void TowerManager::DrawDebugGUI()
{
	for (Tower* tower : towers)
	{
		tower->DrawDebugGUI();
	}
}