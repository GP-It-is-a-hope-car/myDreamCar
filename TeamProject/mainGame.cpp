#include "GameFunc.h"
#include "Intro.h"
#include "Intro2.h"
#include "Intro3.h"
#include "Intro4.h"
#include "Intro5.h"
#include "Explain.h"
#include "MainStage.h"
#include "Ending.h"
#include "Happy_Ending.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;
int g_current_game_phase;

PhaseInterface* game_phases[9];
float g_timestep_s = 1.0f / 60.0f;

int main(int argc, char* argv[]) {

	// Initializing SDL library
	SDL_Init(SDL_INIT_EVERYTHING);
	g_window = SDL_CreateWindow("First Window", 100, 100, 640, 640, 0);

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}
	TTF_Init();
	//rendering context
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);
	InitGame();

	//g_current_game_phase = PHASE_MAINSTAGE;
		
	game_phases[PHASE_INTRO] = new Intro;
	game_phases[PHASE_INTRO2] = new Intro2;
	game_phases[PHASE_INTRO3] = new Intro3;
	game_phases[PHASE_INTRO4] = new Intro4;
	game_phases[PHASE_INTRO5] = new Intro5;
	game_phases[PHASE_EXPLAIN] = new Explain;
	game_phases[PHASE_MAINSTAGE] = new MainStage;
	game_phases[PHASE_ENDING] = new Ending;
	game_phases[PHASE_HAPPY_ENDING] = new Happy_ending;
	g_current_game_phase = PHASE_INTRO;

	g_last_time_ms = SDL_GetTicks();

	bool flag_stage1 = false;

	while (g_flag_running) {

		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < (1000 * g_timestep_s))
			continue;

		game_phases[g_current_game_phase]->HandleEvents();
		game_phases[g_current_game_phase]->Update();
		game_phases[g_current_game_phase]->Render();

		SDL_Delay(30);	// 30 밀리세컨드 기다린다.
		g_last_time_ms = cur_time_ms;
	}

	ClearGame();

	for (int i = 0; i < 9; i++)
	{
		delete game_phases[i];
	}

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	Mix_CloseAudio();
	SDL_Quit();

	return 0;
}

