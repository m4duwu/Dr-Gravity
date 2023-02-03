#include "controls.h"
#include "menu.h"
#include "cprocessing.h"

static const float halfX = 60.0f;			//half of buttons's width
static const float halfY = 60.0f;			//half of button's height
static float centerXbutton, centerYbutton;
static float left, right, top, bottom;

CP_Image controls, button1, button2;
CP_Sound click;

void controls_init(void)
{
	click = CP_Sound_Load("./Assets/Sound/unlock.mp3");
	CP_Sound_Play(click);

	//images
	controls = CP_Image_Load("./Assets/Art/Background/howtoplay.png");
	button1 = CP_Image_Load("./Assets/Art/Background/returnbutton.png");
	button2 = CP_Image_Load("./Assets/Art/Background/returnbuttonhover.png");

	//to position the center of button
	centerXbutton = CP_System_GetWindowWidth() / 8.0f;
	centerYbutton = CP_System_GetWindowHeight() / 7.0f;

	//get perimeter of button
	left = centerXbutton - halfX;
	right = centerXbutton + halfX;
	top = centerYbutton - halfY;
	bottom = centerYbutton + halfY;
}

void controls_update(void)
{
	CP_Settings_Background(CP_Color_Create(255, 255, 255, 255));
	CP_Image_Draw(controls, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

	//draw button
	CP_Image_Draw(button1, centerXbutton, centerYbutton, halfX * 2, halfY * 2, 255);

	//detect mouse
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	//cursor in range of button
	if (((mouseX >= left) && (mouseX <= right)) && ((mouseY >= top) && (mouseY <= bottom)))
	{
		//hover effect
		CP_Image_Draw(button2, centerXbutton, centerYbutton, halfX * 2, halfY * 2, 255);

		//return to menu
		if (CP_Input_MouseClicked())
		{
			CP_Engine_SetNextGameState(menu_init, menu_update, menu_exit);
		}
	}
}

void controls_exit(void)
{
	CP_Image_Free(&controls);
	CP_Image_Free(&button1);
	CP_Image_Free(&button2);

	CP_Sound_Free(click);
}