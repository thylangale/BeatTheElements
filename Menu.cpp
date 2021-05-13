#include "Menu.h"

#define Menu_WIDTH 11
#define Menu_HEIGHT 16

#define Menu_ENEMY_COUNT 0

GLuint fontTextureID;
int LIVES;

unsigned int Menu_data[] =
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

void Menu::Initialize() {

    state.nextScene = -1;
    LIVES = 3;
    glClearColor(0.5, 0.8, 0.5, 0); //background color

    GLuint mapTextureID = Util::LoadTexture("platforms.png");
    state.map = new Map(Menu_WIDTH, Menu_HEIGHT, Menu_data, mapTextureID, 1.0f, 20, 16);
    // Move over all of the player and enemy code from initialization.
    // Initialize player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -11, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("Leaf_Dude.png");

    state.player->animRight = new int[2]{ 14, 15};
    state.player->animLeft = new int[2]{ 0, 1};
    state.player->animUp = new int[2]{ 2, 3};
    state.player->animDown = new int[2]{ 9,10};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 2;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 8;
    state.player->animRows = 2;
    

    state.player->height = 0.8f;
    state.player->width = 0.8f;
    state.player->jumpPower = 6.0f;

    fontTextureID = Util::LoadTexture("font1.png");
}

void Menu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, NULL, Menu_ENEMY_COUNT, state.map);
}

void Menu::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);

    //Game Name
    Util::DrawText(program, fontTextureID, "Welcome to", 0.5f, -0.2, glm::vec3(3.75, -8.5, 0));
    Util::DrawText(program, fontTextureID, "Beat the Elements!", 0.5f, -0.2, glm::vec3(2.75, -9.5, 0));

    //How to play Instructions
    Util::DrawText(program, fontTextureID, "How To Play:", 0.3f, 0, glm::vec3(1, -10.75, 0));
    Util::DrawText(program, fontTextureID, "Move with arrow keys", 0.3f, 0, glm::vec3(1, -11.25, 0));
    Util::DrawText(program, fontTextureID, "Jump with space bar", 0.3f, 0, glm::vec3(1, -11.75, 0));

    //How to start the game
    Util::DrawText(program, fontTextureID, "Press Enter to Play", 0.5f, -0.2, glm::vec3(2.5, -13, 0));
}