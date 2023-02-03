#include "transition.h"
#include "cprocessing.h"
#include "game.h"

CP_Image transition;
float fade;
int statetransition = 1;

void transition_Init(void)
{
	transition = CP_Image_Load("./Assets/Art/Background/transition.png");
	fade = 0;

}

void transition_Update(void)
{
	if (statetransition == 1)
	{
		CP_Image_Draw(transition, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 1600, 960, (int)fade);
		fade += (float)0.8;

		if ((int)fade > 60)
			statetransition = 2;
	}

	if (statetransition == 2)
	{
		CP_Engine_SetNextGameState(game_init, game_update, game_exit);
		statetransition = 1;
	}

}

void transition_Exit(void)
{
	CP_Image_Free(&transition);
}