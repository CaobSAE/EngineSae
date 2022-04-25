#include "Game.h"
#include "SDL.h"

const int THICKNESS = 15;
const float	PADDLEH = 100.0f;

Game::Game() :
    m_Window(nullptr),
    m_IsRunning(true)
{
    
}

bool Game::Initialize(){
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    m_Window = SDL_CreateWindow(
        "Motor SAE",
        100,//coordenadas de x, esquina superior izquierda
        100,//coordenadas de y, esquina superior izquierda
        1024,//Ancho de la pantalla
        768,//Altura de la pantalla
        0//Bandera(0 para una bandera)
    );

    if (!m_Window){
        SDL_Log("Failed to create window %s", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(
        m_Window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    m_BallPos.x = 1024 / 2;
    m_BallPos.y = 768 / 2;
    m_LeftPaddlePos.x = 20.f;
    m_LeftPaddlePos.y = 768 / 2;
    m_RightPaddlePos.x = 1024 - 20.f;
    m_RightPaddlePos.y = 768 / 2;
    m_BallVel.x = -200.0f;
    m_BallVel.y = 235.0f;

    return true;
}

void Game::RunLoop(){
    while (m_IsRunning){
        ProcessingInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown(){
    SDL_DestroyWindow(m_Window);
    SDL_DestroyRenderer(m_Renderer);
    SDL_Quit();
}

void Game::ProcessingInput(){

    //Eventos de mouse

    SDL_Event event;//Almacena todos los inputs
    
    while (SDL_PollEvent(&event)) {//Se revisan todos los eventos que sucedieron entre la última llamada a SDL_PollEvent y esta
        switch (event.type){
            case SDL_QUIT:
                m_IsRunning = false; 
                break;
            default:
                break;
        }
    }

    //Eventos del teclado
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]){
        m_IsRunning = false;
    }

    m_LPaddleDir = 0;
    m_RPaddleDir = 0;
    //Movimiento paleta Izquierda
    if (state[SDL_SCANCODE_W]){
        // En SDL, +y es hacia abajo
        m_LPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S]){
        m_LPaddleDir += 1;
    }
    //Movimiento paleta derecha
    if (state[SDL_SCANCODE_I]) {
        // En SDL, +y es hacia abajo
        m_RPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_K]) {
        m_RPaddleDir += 1;
    }
}

void Game::UpdateGame(){
    // Frame limiting
    // Nuestro objetivo es que el juego corra a 60fps.
    // Esto significa que cada cuadro tiene que durar 16.6ms
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16));

    float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
    m_TicksCount = SDL_GetTicks();

    if (deltaTime > 0.05f){
        deltaTime = 0.05;
    }
    //Movimiento con paleta izquierda
    if (m_LPaddleDir != 0){
        m_LeftPaddlePos.y += m_LPaddleDir * 300.0f * deltaTime;
        if (m_LeftPaddlePos.y < (PADDLEH / 2.0f + THICKNESS)){
            m_LeftPaddlePos.y = PADDLEH / 2.0f + THICKNESS;
        }else if (m_LeftPaddlePos.y > 768.0f - PADDLEH / 2.0 - THICKNESS){
            m_LeftPaddlePos.y = 769.0f - PADDLEH / 2.0 - THICKNESS;
        }
    }
    //Movimiento con paleta derecha
    if (m_RPaddleDir != 0) {
        m_RightPaddlePos.y += m_RPaddleDir * 300.0f * deltaTime;
        if (m_RightPaddlePos.y < (PADDLEH / 2.0f + THICKNESS)) {
            m_RightPaddlePos.y = PADDLEH / 2.0f + THICKNESS;
        }
        else if (m_RightPaddlePos.y > 768.0f - PADDLEH / 2.0 - THICKNESS) {
            m_RightPaddlePos.y = 769.0f - PADDLEH / 2.0 - THICKNESS;
        }
    }

    m_BallPos.x += m_BallVel.x * deltaTime;
    m_BallPos.y += m_BallVel.y * deltaTime;

    // Revisar colisión con paredes
    if ((m_BallPos.y <= THICKNESS && m_BallVel.y < 0.0f) ||
        m_BallPos.y >= 768.0f - THICKNESS && m_BallVel.y > 0.0f)
    {
        m_BallVel.y *= -1;
    }

    // Revisar colisión contra la paleta
    float diffL = m_LeftPaddlePos.y - m_BallPos.y;
    float diffR = m_RightPaddlePos.y + m_BallPos.y;
    diffL = (diffL > 0.0f) ? diffL : -diffL;
    diffR = (diffR < 1024.0f) ? diffL : -diffL;
    //Colision paleta izquierda
    if (
        (diffL <= PADDLEH / 2.0f &&
            m_BallPos.x <= m_LeftPaddlePos.x + THICKNESS / 2.0f &&
            m_BallPos.x >= m_LeftPaddlePos.x - THICKNESS / 2.0f &&
            m_BallVel.x < 0.0f) ||
        ((m_BallPos.x >= (1024 - THICKNESS) && m_BallPos.x > 0.0f))
        )
    {
        m_BallVel.x *= -1.0f;
    }
    else if (m_BallPos.x <= 0.0f)
    {
        m_IsRunning = false;
    }
    //Colision paleta derecha
    if (
        (diffL <= PADDLEH / 2.0f &&
            m_BallPos.x <= m_RightPaddlePos.x + THICKNESS / 2.0f &&
            m_BallPos.x >= m_RightPaddlePos.x - THICKNESS / 2.0f &&
            m_BallVel.x < 0.0f) ||
        ((m_BallPos.x >= (1024 - THICKNESS) && m_BallPos.x > 0.0f))
        )
    {
        m_BallVel.x *= -1.0f;
    }
    else if (m_BallPos.x >= 1024.0f)
    {
        m_IsRunning = false;
    }
}

void Game::GenerateOutput(){
    SDL_SetRenderDrawColor(
        m_Renderer,
        0,
        0,
        255,
        255
    );

    //Resetear el búfer de color indicado
    SDL_RenderClear(m_Renderer);

    //Pintar paredes

    //Elegir color de los objetos
    SDL_SetRenderDrawColor(
        m_Renderer,
        255,
        255,
        0,
        255
    );
    SDL_Rect wall{
        0,
        0,
        1024,
        THICKNESS
    };

    SDL_RenderFillRect(m_Renderer, &wall);

    wall.y = 768 - THICKNESS;
    SDL_RenderFillRect(m_Renderer, &wall);

    wall.x = 1024 - THICKNESS;
    wall.y = 0;
    wall.w = THICKNESS;
    wall.h = 768;
    SDL_RenderFillRect(m_Renderer, &wall);

    SDL_SetRenderDrawColor(
        m_Renderer,
        255,
        0,
        255,
        255
    );

    SDL_Rect ball{
        static_cast<int>(m_BallPos.x - THICKNESS / 2),
        static_cast<int>(m_BallPos.y - THICKNESS / 2),
        THICKNESS,
        THICKNESS
    };

    SDL_RenderFillRect(m_Renderer, &ball);

    SDL_Rect leftPaddle{
        static_cast<int>(m_LeftPaddlePos.x - THICKNESS / 2),
        static_cast<int>(m_LeftPaddlePos.y - PADDLEH / 2),
        THICKNESS,
        static_cast<int>(PADDLEH)
    };
    SDL_RenderFillRect(m_Renderer, &leftPaddle);

    SDL_Rect rightPaddle{
        static_cast<int>(m_RightPaddlePos.x - THICKNESS / 2),
        static_cast<int>(m_RightPaddlePos.y - PADDLEH / 2),
        THICKNESS,
        static_cast<int>(PADDLEH)
    };
    SDL_RenderFillRect(m_Renderer, &rightPaddle);

    //Cambiar el búfer de reserva por el búfer frontal
    SDL_RenderPresent(m_Renderer);
}
