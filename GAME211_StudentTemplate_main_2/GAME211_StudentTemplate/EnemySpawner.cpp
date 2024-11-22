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
        std::cout << "Enemy spawned at: " << spawnPos.x << ", " << spawnPos.y << std::endl;
    }
    else {
        std::cerr << "Failed to create enemy at: " << spawnPos.x << ", " << spawnPos.y << std::endl;
        delete enemy;
    }
}

void EnemySpawner::UpdateEnemies(float deltaTime)
{
    for (auto enemyit = enemies.begin(); enemyit != enemies.end();) {
        Enemy* enemy = *enemyit;
        enemy->Update(deltaTime);


        bool isDestroy = false;
        

        for (auto projectileit = shots.begin(); projectileit != shots.end();) {
            Projectile* projectile = *projectileit;

            if (projectile->getActive() && enemy->IsHitByProjectile(*projectile, 1.0f)) {
                cout << "HIT.................................................." << endl;
                enemy->OnDestroy();
                delete enemy;
                enemyit = enemies.erase(enemyit);
                isDestroy = true;

                projectile->OnDestroy();
                delete projectile;
                projectileit = shots.erase(projectileit);
                break; // Exit inner loop after destroying projectile
            }
            else {
                ++projectileit;
            }
        }

        if (!isDestroy) {
            ++enemyit;
        }

    }

}

void EnemySpawner::RenderEnemies(float scale)
{
    for (auto& enemy : enemies) {
        if (enemy != nullptr) {
            enemy->Render(scale); 
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
