#include "logo.h"
#include "cprocessing.h"
#include "menu.h"

CP_Image logo;
int fade, logostate = 1;

void logo_init(void)
{
	//image of logo
	logo = CP_Image_Load("./Assets/DigiPen_WHITE.png");
	fade = 0;
}
void logo_update(void)
{
	CP_Settings_Background(CP_Color_Create(0, 0, 0, 255));

	//fade in logo
	if (logostate == 1)
	{
		CP_Image_Draw(logo, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 1200, 325, fade);
		fade += 2;
		if (fade > 255)
			logostate = 2;
	}

	//fade out
	if (logostate == 2)
	{
		CP_Image_Draw(logo, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 1200, 325, fade);
		fade -= 2;
		if (fade < 0)
			logostate = 3;
	}

	//finish fade in and fade out, go to main menu
	if (logostate == 3)
		CP_Engine_SetNextGameState(menu_init, menu_update, menu_exit);
	if (CP_Input_KeyTriggered(KEY_ENTER))
		CP_Engine_SetNextGameState(menu_init, menu_update, menu_exit);
}
void logo_exit(void)
{
	CP_Image_Free(&logo);
}