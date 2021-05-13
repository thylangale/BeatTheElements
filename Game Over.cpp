#include "Game Over.h"

#define Over_WIDTH 11
#define Over_HEIGHT 16

#define Over_ENEMY_COUNT 0

GLuint font2TextureID;

unsigned int Over_data[] =
{
 140, 0, 0, 0, 0, 0, 0, 0, 0, 0, 142,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 160, 0, 0, 0, 0, 0, 0, 0, 0, 0, 162,
 180, 181, 181, 181, 181, 181, 181, 181, 181, 181, 182,
};

void GameOver::Initialize() {

    state.nextScene = -1;
    glClearColor(0.5, 0.8, 0.5, 0); //background color

    GLuint mapTextureID = Util::LoadTexture("platforms.png");
    state.map = new Map(Over_WIDTH, Over_HEIGHT, Over_data, mapTextureID, 1.0f, 20, 16);
    // Move over all of the player and enemy code from initialization.
    // Initialize player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -11, 0.0f);
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
    state.player->jumpPower = 6.0f;

    font2TextureID = Util::LoadTexture("font1.png");
}

void GameOver::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, NULL, Over_ENEMY_COUNT, state.map);
}

void GameOver::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);

    //Game Name
    Util::DrawText(program, font2TextureID, "Game Over", 1, -0.2, glm::vec3(2, -10.75, 0));

    //How to start the game
    Util::DrawText(program, font2TextureID, "Press Enter to Exit", 0.5f, -0.2, glm::vec3(2.25, -12.75, 0));
}