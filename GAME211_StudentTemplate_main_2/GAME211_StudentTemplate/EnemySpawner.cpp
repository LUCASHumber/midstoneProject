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

const vector<Enemy*>& EnemySpawner::GetEnemies() const {
    return enemies;
}

void EnemySpawner::UpdateEnemies(float deltaTime)
{
    vector<Enemy*> enemiesToDelete; // Store enemies to delete
    vector<Projectile*> projectilesToDelete;
    vector<Vec3> spawnPositions; // To store new spawn positions

    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
        Enemy* enemy = *enemyIt;
        enemy->Update(deltaTime);

        bool isDestroyed = false;

        // Check collision with projectiles
        auto& shots = game->getShots();
        for (auto projectileIt = shots.begin(); projectileIt != shots.end(); ) {
            Projectile* projectile = *projectileIt;

            if (enemy->IsHitByProjectile(*projectile, 0.5f)) {
                std::cout << "HIT.................................................." << std::endl;

                // Mark both the enemy and projectile for deletion
                enemiesToDelete.push_back(enemy);
                projectilesToDelete.push_back(projectile);

                // Determine new spawn positions
                spawnPositions.push_back(GetRandomSpawnPosition());
                spawnPositions.push_back(GetRandomSpawnPosition());

                isDestroyed = true;
                projectileIt = shots.erase(projectileIt); // Safely erase the projectile
            }
            else {
                ++projectileIt;
            }
        }

        if (isDestroyed) {
            enemyIt = enemies.erase(enemyIt); // Safely erase the enemy from the vector
        }
        else {
            ++enemyIt;
        }
    }

    // Clean up all marked enemies
    for (Enemy* enemy : enemiesToDelete) {
        enemy->OnDestroy();
        delete enemy;
    }

    // Clean up all marked projectiles
    for (Projectile* projectile : projectilesToDelete) {
        projectile->OnDestroy();
        delete projectile;
    }

    // Spawn new enemies after the loop
    for (const Vec3& spawnPos : spawnPositions) {
        SpawnEnemy(spawnPos);
    }

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
