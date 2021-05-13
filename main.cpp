#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"


#include "Entity.h"
#include "Map.h"
#include "Util.h"
#include "Scene.h"
#include "Menu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Game Won.h"
#include "Game Over.h"
#include "Lives.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

Mix_Music* music;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[7];

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Thy-Lan Gale - Project 5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("Newer Wave.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(1, 1, 1, 0); //background color
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Initialize Scenes
    sceneList[0] = new Level1();
    sceneList[1] = new Level2();
    sceneList[2] = new Level3();
    sceneList[3] = new Menu();
    sceneList[4] = new GameWon();
    sceneList[5] = new GameOver();
    sceneList[6] = new Lives();
    SwitchToScene(sceneList[3]); //to debug but remember to switch back to 3

}

void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                // Some sort of action
                if (currentScene->state.player->collidedBottom) {
                    currentScene->state.player->jump = true;
                }
                break;
            case SDLK_RETURN:
                if (currentScene == sceneList[3]) {
                    currentScene->state.nextScene = 0;
                }
                else if (currentScene == sceneList[4] || currentScene == sceneList[5]) {
                    gameIsRunning = false;

                }
                else if (currentScene == sceneList[6]) {
                    if (LIVES == 0) { //Game Over
                        currentScene->state.nextScene = 5;
                    }
                    else currentScene->state.nextScene = prevScene; //restart the current level
                }
            }

            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }


    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.y > -3.25) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.25, 0));
    }
    else if (currentScene->state.player->position.y > -11.75 && (currentScene != sceneList[3] && currentScene != sceneList[4] && currentScene != sceneList[5])) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, -currentScene->state.player->position.y, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 11.75, 0));
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);

    currentScene->Render(&program);

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();

        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        Render();
    }

    Shutdown();
    return 0;
}