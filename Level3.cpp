#include "Level3.h"
//Electricity Level

#define LEVEL3_WIDTH 11
#define LEVEL3_HEIGHT 16

#define LEVEL3_ENEMY_COUNT 3
Mix_Chunk* bounce3;
Mix_Chunk* lose3;

unsigned int level3_data[] =
{
    148, 0, 0, 0, 0, 0, 0, 277, 278, 0, 150,
    168, 0, 0, 0, 0, 0, 0, 317, 318, 0, 170,
    168, 0, 0, 0, 0, 0, 0, 275,276, 0, 170,
    168, 0, 0, 0, 0, 178, 179, 0, 0, 0, 170,
    168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170,
    168, 178, 179, 0, 0, 0, 0, 0, 0, 0, 170,
    168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170,
    168, 0, 0, 0, 198, 0, 0, 0, 0, 0, 170,
    168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170,
    168, 0, 0, 0, 0, 198, 0, 0, 0, 0, 170,
    168, 0, 0, 0, 0, 0, 198, 0, 0, 0, 170,
    168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 170,
    168, 0, 0, 0, 0, 0, 0, 0, 0, 198, 170,
    168, 0, 178, 179, 0, 0, 0, 0, 0, 0, 170,
    168, 0, 0, 0, 0, 0, 0, 178, 179, 0, 170,
    188, 0, 0, 0, 0, 189, 0, 0, 0, 0, 190,
};

void Level3::Initialize() {

    state.nextScene = -1;
    glClearColor(1, 0.75, 0, 0); //background color

    GLuint mapTextureID = Util::LoadTexture("platforms.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 20, 16);

    bounce3 = Mix_LoadWAV("GameBeep.wav");
    lose3 = Mix_LoadWAV("LoseLife.wav");

    // Move over all of the player and enemy code from initialization.
    // Initialize player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -13, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("Leaf_Dude.png");

    state.player->animRight = new int[2]{ 14, 15 };
    state.player->animLeft = new int[2]{ 0, 1 };
    state.player->animUp = new int[2]{ 2, 3 };
    state.player->animDown = new int[2]{ 9,10 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 8;
    state.player->animRows = 2;

    state.player->height = 0.8f;
    state.player->width = 0.8f;
    state.player->jumpPower = 7.0f;


    //Initialize Enemies
    
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("Lightning_Dude.png"); //add enemy

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].aiType = JUMPER;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].acceleration = glm::vec3(0, -9.81f, 0);
        state.enemies[i].jumpPower = 4.0f;
    }

    state.enemies[0].position = glm::vec3(2, -10, 0);

    state.enemies[1].position = glm::vec3(7.75, -13, 0);
    
    state.enemies[2].position = glm::vec3(5.75, -2, 0);
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.enemies, state.player, 1, state.map);
    }

    //Switch scenes
    if (state.player->position.y > -2 && state.player->position.x > 6 && state.player->position.x < 9) {
        state.nextScene = 4;
    }
    
    if (state.player->killedEnemy && (state.player->lastCollision == &state.enemies[0] || state.player->lastCollision == &state.enemies[1] || state.player->lastCollision == &state.enemies[2])) { //player beat enemy
        Mix_PlayChannel(-1, bounce3, 0);
        if (state.player->lastCollision == &state.enemies[0]) {
            state.enemies[0].isActive = false;
        }
        else if (state.player->lastCollision == &state.enemies[1]) {
            state.enemies[1].isActive = false;
        }
        else state.enemies[2].isActive = false;
        state.player->killedEnemy = false;
    }
    else if (state.player->lastCollision == &state.enemies[0] || state.player->lastCollision == &state.enemies[1] || state.player->lastCollision == &state.enemies[2] || state.player->position.y < -15.5) { //lose a life

        LIVES -= 1;
        prevScene = 2;
        Mix_PlayChannel(-1, lose3, 0);

        if (LIVES == 0) state.nextScene = 5;
        else state.nextScene = 6;
    }
    
}

void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}