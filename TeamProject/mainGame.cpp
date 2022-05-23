#include "GameFunc.h"
#include "MainStage.h"
SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;
int g_current_game_phase;

PhaseInterface* game_phases[3];

int main(int argc, char* argv[]) {

	// Initializing SDL library
	SDL_Init(SDL_INIT_EVERYTHING);
	g_window = SDL_CreateWindow("First Window", 100, 100, 700, 700, 0);

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}
	TTF_Init();
	//rendering context
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);
	InitGame();

	g_current_game_phase = PHASE_MAINSTAGE;

	/*game_phases[PHASE_INTRO] = new Intro;	
	game_phases[PHASE_ENDING] = new Ending;*/
	game_phases[PHASE_MAINSTAGE] = new MainStage;
	//g_current_game_phase = PHASE_INTRO;

	g_last_time_ms = SDL_GetTicks();

	bool flag_stage1 = false;

	while (g_flag_running) {

		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < 33)
			continue;

		game_phases[g_current_game_phase]->HandleEvents();
		game_phases[g_current_game_phase]->Update();
		game_phases[g_current_game_phase]->Render();

		SDL_Delay(30);	// 30 밀리세컨드 기다린다.
		g_last_time_ms = cur_time_ms;
	}

	ClearGame();

	/*for (int i = 0; i < 3; i++)
	{
		delete game_phases[i];
	}*/

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	Mix_CloseAudio();
	SDL_Quit();

	return 0;
}

