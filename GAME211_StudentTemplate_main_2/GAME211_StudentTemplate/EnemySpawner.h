#pragma once

#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include <vector>
#include "GameManager.h"
#include "Enemy.h"
#include "Projectile.h"

using namespace std;

class EnemySpawner
{
private:
    vector<Enemy*> enemies;
    vector<Projectile*> shots;
    GameManager* game;

public:
    EnemySpawner(GameManager* game_) : game(game_) {}
    ~EnemySpawner();

    void SetProjectiles(vector<Projectile*>* projectiles) { shots = *projectiles; }
    void SpawnEnemy(const Vec3& spawnPos);
    void UpdateEnemies(float deltaTime);
    void RenderEnemies(float scale);
    void ClearEnemies();

};

#endif // ENEMY_SPAWNER_H