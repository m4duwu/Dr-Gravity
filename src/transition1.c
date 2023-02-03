#include "transition1.h"
#include "cprocessing.h"
#include "game1.h"

CP_Image transition;
float fade;
int statetransition1 = 1;

void transition1_Init(void)
{
	transition = CP_Image_Load("./Assets/Art/Background/transition.png");
	fade = 0;

}

void transition1_Update(void)
{
	if (statetransition1 == 1)
	{
		CP_Image_Draw(transition, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 1600, 960, (int)fade);
		fade += (float)0.8;

		if ((int)fade > 60)
			statetransition1 = 2;
	}

	if (statetransition1 == 2)
	{
		CP_Engine_SetNextGameState(game1_init, game1_update, game1_exit);
		statetransition1 = 1;
	}

}

void transition1_Exit(void)
{
	CP_Image_Free(&transition);
}