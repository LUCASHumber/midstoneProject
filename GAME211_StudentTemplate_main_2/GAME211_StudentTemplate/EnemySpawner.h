#pragma once

#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include <vector>
#include "GameManager.h"
#include "Enemy.h"

using namespace std;

class EnemySpawner
{
private:
    vector<Enemy*> enemies;
    GameManager* game;

public:
    EnemySpawner(GameManager* game_) : game(game_) {}
    ~EnemySpawner();

    void SpawnEnemy(const Vec3& spawnPos);
    void UpdateEnemies(float deltaTime);
    void RenderEnemies();
    void ClearEnemies();

};

#endif // ENEMY_SPAWNER_H