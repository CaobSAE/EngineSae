#pragma once
#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};

class Game{
	public:
		Game();

		bool Initialize();
		void RunLoop();
		void Shutdown();
	private:
		void ProcessingInput();
		void UpdateGame();
		void GenerateOutput();

		SDL_Window* m_Window;
		SDL_Renderer* m_Renderer;

		bool m_IsRunning;
		Uint32 m_TicksCount;

		//Game specific
		Vector2 m_BallPos;
		Vector2 m_BallVel;
		Vector2 m_LeftPaddlePos;
		Vector2 m_RightPaddlePos;
		int m_LPaddleDir;
		int m_RPaddleDir;
};

