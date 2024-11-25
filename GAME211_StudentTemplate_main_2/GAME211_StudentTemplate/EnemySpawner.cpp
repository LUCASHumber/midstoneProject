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

        bool isDestroyed = false;

        // Check collision with projectiles
        for (auto projectileIit = shots.begin(); projectileIit != shots.end();) {
            Projectile* projectile = *projectileIit;

            if (projectile->getActive() && enemy->IsHitByProjectile(*projectile, 1.0f)) {
                std::cout << "HIT.................................................." << std::endl;

                // Destroy enemy
                enemy->OnDestroy();
                delete enemy;
                enemyit = enemies.erase(enemyit);
                isDestroyed = true;

                // Destroy projectile
                projectile->OnDestroy();
                delete projectile;
                projectileIit = shots.erase(projectileIit);

                break; // Exit the inner loop since the enemy is already destroyed
            }
            else {
                ++projectileIit; // Move to the next projectile
            }
        }

        // If the enemy was destroyed, continue with the next enemy
        if (isDestroyed) {
            continue;
        }
        else {
            ++enemyit; // Move to the next enemy
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
