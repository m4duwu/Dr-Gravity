#include "transition2.h"
#include "cprocessing.h"
#include "game3.h"


CP_Image transition3;
float fade3;
int statetransition3 = 1;


void transition3_Init(void)
{
	transition3 = CP_Image_Load("./Assets/Art/Background/transition.png");
	fade3 = 0;

}

void transition3_Update(void)
{
	//CP_Settings_Background(CP_Color_Create(0, 0, 0, 255));

	if (statetransition3 == 1)
	{
		CP_Image_Draw(transition3, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 1600, 960, (int)fade3);
		fade3 += (float)0.8;

		if ((int)fade3 > 60)
		{
			statetransition3 = 2;
		}
	}

	if (statetransition3 == 2)
	{
		//Set to level 2 or terminate
		CP_Engine_SetNextGameState(game3_init, game3_update, game3_exit);
		//CP_Engine_Terminate();
	}

}

void transition3_Exit(void)
{
	CP_Image_Free(&transition3);
}


