#include "EnemySpawner.h"
#include <iostream>

EnemySpawner::~EnemySpawner()
{
	ClearEnemies();
}

void EnemySpawner::SpawnEnemy(const Vec3& spawnPos)
{
    Enemy* enemy = new Enemy(spawnPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f),
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, game);
    if (enemy->OnCreate()) {
        enemies.push_back(enemy);
    }
    else {
        cerr << "Failed to create enemy" << endl;
        delete enemy;
    }
}

void EnemySpawner::UpdateEnemies(float deltaTime)
{
    for (auto& enemy : enemies) {
        if (enemy != nullptr) {
            enemy->Update(deltaTime);
        }
    }
}

void EnemySpawner::RenderEnemies()
{
    for (auto& enemy : enemies) {
        if (enemy != nullptr) {
            enemy->Render(1.0f); // Adjust scale if needed
        }
    }
}

void EnemySpawner::ClearEnemies()
{
    for (Enemy* enemy : enemies) {
        if (enemy != nullptr) {
            enemy->OnDestroy();
            delete enemy;
        }
    }
    enemies.clear();
}
