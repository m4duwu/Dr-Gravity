#include "transition2.h"
#include "cprocessing.h"
#include "game2.h"


CP_Image transition2;
float fade2;
int statetransition2 = 1;


void transition2_Init(void)
{
	transition2 = CP_Image_Load("./Assets/Art/Background/transition.png");
	fade2 = 0;

}

void transition2_Update(void)
{
	//CP_Settings_Background(CP_Color_Create(0, 0, 0, 255));

	if (statetransition2 == 1)
	{
		CP_Image_Draw(transition2, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 1600, 960, (int)fade2);
		fade2 += (float)0.8;

		if ((int)fade2 > 60)
		{
			statetransition2 = 2;
		}
	}

	if (statetransition2 == 2)
	{
		//Set to level 2 or terminate
		CP_Engine_SetNextGameState(game2_init, game2_update, game2_exit);
		//CP_Engine_Terminate();
	}

}

void transition2_Exit(void)
{
	CP_Image_Free(&transition2);
}


