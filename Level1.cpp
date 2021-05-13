#include "Level1.h"
//Ice Level

#define LEVEL1_WIDTH 11
#define LEVEL1_HEIGHT 16

#define LEVEL1_ENEMY_COUNT 1
Mix_Chunk* bounce;
Mix_Chunk* lose;

int prevScene;

unsigned int level1_data[] =
    {
    
    88, 0, 0, 0, 0, 0, 0, 217, 218, 0, 90,
    108, 0, 0, 0, 0, 0, 0, 257, 258, 0, 110,
    108, 118, 119, 0, 0, 0, 0, 255,256, 0, 110,
    108, 0, 0, 0, 0, 0, 138, 0, 0, 0, 110,
    108, 0, 0, 138, 0, 0, 0, 0, 0, 0, 110,
    108, 0, 0, 0, 0, 138, 0, 0, 0, 0, 110,
    108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110,
    108, 0, 0, 0, 0, 0, 0, 118, 119, 0, 110,
    108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110,
    108, 0, 0, 0, 118, 119, 0, 0, 0, 0, 110,
    108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110,
    108, 138, 0, 0, 0, 0, 0, 138, 0, 0, 110,
    108, 0, 0, 0, 0, 0, 0, 0, 0, 138, 110,
    108, 0, 118, 119, 0, 0, 0, 0, 0, 0, 110,
    108, 0, 0, 0, 0, 0, 0, 0, 118, 119, 110,
    128, 0, 0, 0, 0, 129, 0, 0, 0, 0, 130,
    
    };

void Level1::Initialize() {
    
    state.nextScene = -1;
    glClearColor(0, 0.5, 1, 0); //background color

	GLuint mapTextureID = Util::LoadTexture("platforms.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 20, 16);

    bounce = Mix_LoadWAV("GameBeep.wav");
    lose = Mix_LoadWAV("LoseLife.wav");

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
    state.enemies = new Entity();
    GLuint enemyTextureID = Util::LoadTexture("Ice_Dude.png"); //add enemy

    state.enemies->entityType = ENEMY;
    state.enemies->textureID = enemyTextureID;

    state.enemies->position = glm::vec3(7, -3, 0);
    state.enemies->movement = glm::vec3(1.0f, 0, 0);
    state.enemies->speed = 0.75;
    state.enemies->acceleration = glm::vec3(0, -9.81f, 0);

    state.enemies->aiType = WALKER;
    
}

void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    state.enemies->Update(deltaTime, state.enemies, state.player, 1, state.map);
    
    //Switch scenes
    if (state.player->position.y > -2 && state.player->position.x > 6 && state.player->position.x < 9) {
        state.nextScene = 1; //change to 1
    }   
    if (state.player->killedEnemy) { //player beat enemy
        Mix_PlayChannel(-1, bounce, 0);
        state.enemies->isActive = false;
        state.player->killedEnemy = false; //reset for other enemies
    }
    else if (state.enemies->lastCollision == state.player || state.player->position.y < -15.5) { //lose a life
        LIVES -= 1;
        Mix_PlayChannel(-1, lose, 0);
        prevScene = 0;

        if (LIVES == 0) state.nextScene = 5;
        else state.nextScene = 6;
        
    }
    
}

void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
    state.enemies->Render(program);
}