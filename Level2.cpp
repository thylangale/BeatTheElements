#include "Level2.h"
//Fire Level

#define LEVEL2_WIDTH 11
#define LEVEL2_HEIGHT 16

#define LEVEL2_ENEMY_COUNT 2
Mix_Chunk* bounce2;
Mix_Chunk* lose2;

unsigned int level2_data[] = 
{
    80, 0, 0, 0, 0, 0, 0, 213, 214, 0, 82,
    100, 0, 0, 0, 0, 0, 0, 253, 254, 0, 102,
    100, 0, 0, 0, 0, 0, 0, 251,252, 0, 102,
    100, 0, 0, 136, 0, 0, 0, 0, 0, 0, 102,
    100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102,
    100, 0, 0, 0, 0, 136, 0, 0, 0, 0, 102,
    100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102,
    100, 0, 0, 136, 0, 0, 0, 116, 117, 0, 102,
    100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102,
    100, 202, 203, 204, 205, 0, 0, 0, 0, 0, 102,
    100, 0, 0, 0, 0, 0, 0, 136, 0, 0, 102,
    100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102,
    100, 0, 0, 0, 0, 0, 202, 203, 204, 205, 102,
    100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102,
    100, 0, 0, 220, 221, 0, 0, 0, 0, 0, 102,
    120, 0, 0, 240, 241, 121, 0, 0, 0, 0, 122,
 };

void Level2::Initialize() {

    state.nextScene = -1;
    glClearColor(1, 0.25, 0, 0); //background color
    
    GLuint mapTextureID = Util::LoadTexture("platforms.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 20, 16);

    bounce2 = Mix_LoadWAV("GameBeep.wav");
    lose2 = Mix_LoadWAV("LoseLife.wav");

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
    
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureLeftID = Util::LoadTexture("Fire_Dude_Left.png"); //add enemy
    GLuint enemyTextureRightID = Util::LoadTexture("Fire_Dude_Right.png"); //add enemy

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].speed = 0.75;
        state.enemies[i].acceleration = glm::vec3(0, -9.81f, 0);
        state.enemies[i].aiType = WAITANDGO;
        state.enemies[i].aiState = IDLE;
    }
    
    state.enemies[0].position = glm::vec3(1, -5, 0);
    state.enemies[0].textureID = enemyTextureRightID;

    state.enemies[1].position = glm::vec3(8.75, -10, 0);
    state.enemies[1].textureID = enemyTextureLeftID;
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.enemies, state.player, 1, state.map);
    }
    

    //Switch scenes
    
    if (state.player->position.y > -2 && state.player->position.x > 6 && state.player->position.x < 9) {
        state.nextScene = 2; 
    } 
    if (state.player->killedEnemy && (state.player->lastCollision == &state.enemies[0] || state.player->lastCollision == &state.enemies[1])) { //player beat enemy
        Mix_PlayChannel(-1, bounce2, 0);
        if (state.player->lastCollision == &state.enemies[0]) {
            state.enemies[0].isActive = false;
        }
        else state.enemies[1].isActive = false;
        state.player->killedEnemy = false;
    }
    else if (state.player->lastCollision == &state.enemies[0] || state.player->lastCollision == &state.enemies[1] || state.player->position.y < -15.5) { //lose a life
        
        LIVES -= 1;
        prevScene = 1;
        Mix_PlayChannel(-1, lose2, 0);

        if (LIVES == 0) state.nextScene = 5;
        else state.nextScene = 6;
    }
    
}

void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
}