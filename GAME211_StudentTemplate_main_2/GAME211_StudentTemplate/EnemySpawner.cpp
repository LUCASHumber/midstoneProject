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
    vector<Enemy*> enemiesToRemove;

    for (auto enemyit = enemies.begin(); enemyit != enemies.end();) {
        Enemy* enemy = *enemyit;
        enemy->Update(deltaTime);

        bool isDestroyed = false;

        // Check collision with projectiles
        auto& shots = game->getShots();
        for (auto projectileit = shots.begin(); projectileit != shots.end();) {
            Projectile* projectile = *projectileit;
          
            if (enemy->IsHitByProjectile(*projectile, 0.5f)) {
                std::cout << "HIT.................................................." << std::endl;

                //// Destroy enemy
                //enemy->OnDestroy();
                //delete enemy;
                //enemyit = enemies.erase(enemyit);
                //isDestroyed = true;

                //// Destroy projectile
                //projectile->OnDestroy();
                //delete projectile;
                //projectileit = shots.erase(projectileit);

                //// Spawn two new enemies outside the view screen
                //
                //SpawnEnemy(GetRandomSpawnPosition()); // Left spawn
                //SpawnEnemy(GetRandomSpawnPosition()); // Right spawn

                enemiesToRemove.push_back(enemy);
                projectile->OnDestroy();
                delete projectile;
                projectileit = shots.erase(projectileit);

                // Spawn new enemies at random positions
                SpawnEnemy(GetRandomSpawnPosition());
                SpawnEnemy(GetRandomSpawnPosition());

                isDestroyed = true;

                break; // Exit the inner loop since the enemy is already destroyed
            }
            else {
                ++projectileit; // Move to the next projectile
            }
        }

        // If the enemy was destroyed, continue with the next enemy
        if (isDestroyed) {
            //continue;
            enemyit = enemies.erase(enemyit);
        }
        else {
            ++enemyit; // Move to the next enemy
        }
    }

    for (Enemy* enemy : enemiesToRemove) {
        enemy->OnDestroy();
        delete enemy;
    }
    enemiesToRemove.clear();

}

Vec3 EnemySpawner::GetRandomSpawnPosition()
{
    // Randomly select one of the four sides outside the view screen
    int side = rand() % 4; // 0: left, 1: right, 2: top, 3: bottom
    float x{}, y{};

    switch (side)
    {
    case 0: // Left
        x = -1.0f;
        y = static_cast<float>(rand() % 16); // Random y between 0 and 15
        break;
    case 1: // Right
        x = 26.0f;
        y = static_cast<float>(rand() % 16); // Random y between 0 and 15
        break;
    case 2: // Top
        x = static_cast<float>(rand() % 26); // Random x between 0 and 25
        y = 16.0f;
        break;
    case 3: // Bottom
        x = static_cast<float>(rand() % 26); // Random x between 0 and 25
        y = -1.0f;
        break;
    }

    return Vec3(x, y, 0.0f);
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
