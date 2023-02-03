#include "cprocessing.h"
#include "menu.h"
#include "selection.h"
#include "transition.h"
#include "transition1.h"
#include "transition2.h"
#include "transition3.h"

static const float halfX = 180.0f;																		//half of buttons's width
static const float halfY = 70.0f;																		//half of button's height

static float TUTbuttonLeft, TUTbuttonRight, TUTbuttonTop, TUTbuttonBottom, centerXbuttonTUT, centerYbuttonTUT;			//TUT button
static float buttonLeft1, buttonRight1, buttonTop1, buttonBottom1, centerXbutton1, centerYbutton1;						//1 button
static float buttonLeft2, buttonRight2, buttonTop2, buttonBottom2, centerXbutton2, centerYbutton2;						//2 button
static float buttonLeft3, buttonRight3, buttonTop3, buttonBottom3, centerXbutton3, centerYbutton3;						//3 button
static float buttonLeftMENU, buttonRightMENU, buttonTopMENU, buttonBottomMENU, centerXbuttonMENU, centerYbuttonMENU;	//menu button

CP_Image screen, tut1, tut2, one1, one2, two1, two2, three1, three2, menu1, menu2;
CP_Sound click;

void selection_init(void)
{
	//images
	screen = CP_Image_Load("./Assets/Art/Background/levelselect.png");
	tut1 = CP_Image_Load("./Assets/Art/Background/tutorialbutton.png");
	tut2 = CP_Image_Load("./Assets/Art/Background/tutorialbuttonhover.png");
	one1 = CP_Image_Load("./Assets/Art/Background/level1button.png");
	one2 = CP_Image_Load("./Assets/Art/Background/level1buttonhover.png");
	two1 = CP_Image_Load("./Assets/Art/Background/level2button.png");
	two2 = CP_Image_Load("./Assets/Art/Background/level2buttonhover.png");
	three1 = CP_Image_Load("./Assets/Art/Background/level3button.png");
	three2 = CP_Image_Load("./Assets/Art/Background/level3buttonhover.png");
	menu1 = CP_Image_Load("./Assets/Art/Background/menubutton.png");
	menu2 = CP_Image_Load("./Assets/Art/Background/menubuttonhover.png");

	click = CP_Sound_Load("./Assets/Sound/unlock.mp3");
	CP_Sound_Play(click);
	
	//to position the center of TUT button
	centerXbuttonTUT = CP_System_GetWindowWidth() / 2.9f;
	centerYbuttonTUT = CP_System_GetWindowHeight() / 2.5f;

	//get perimeter of TUT button
	TUTbuttonLeft = centerXbuttonTUT - halfX;
	TUTbuttonRight = centerXbuttonTUT + halfX;
	TUTbuttonTop = centerYbuttonTUT - halfY;
	TUTbuttonBottom = centerYbuttonTUT + halfY;

	//LEVEL 1 BUTTON
	centerXbutton1 = CP_System_GetWindowWidth() / 1.5f;
	centerYbutton1 = CP_System_GetWindowHeight() / 2.5F;

	buttonLeft1 = centerXbutton1 - halfX;
	buttonRight1 = centerXbutton1 + halfX;
	buttonTop1 = centerYbutton1 - halfY;
	buttonBottom1 = centerYbutton1 + halfY;

	//LEVEL 2 BUTTON
	centerXbutton2 = CP_System_GetWindowWidth() / 2.9f;
	centerYbutton2 = CP_System_GetWindowHeight() / 1.6f;

	buttonLeft2 = centerXbutton2 - halfX;
	buttonRight2 = centerXbutton2 + halfX;
	buttonTop2 = centerYbutton2 - halfY;
	buttonBottom2 = centerYbutton2 + halfY;

	// LEVEL 3 BUTTON
	centerXbutton3 = CP_System_GetWindowWidth() / 1.5f;
	centerYbutton3 = CP_System_GetWindowHeight() / 1.6f;

	buttonLeft3 = centerXbutton3 - halfX;
	buttonRight3 = centerXbutton3 + halfX;
	buttonTop3 = centerYbutton3 - halfY;
	buttonBottom3 = centerYbutton3 + halfY;

	//MENU BUTTON
	centerXbuttonMENU = CP_System_GetWindowWidth() / 1.98f;
	centerYbuttonMENU = CP_System_GetWindowHeight() / 1.2f;

	buttonLeftMENU = centerXbuttonMENU - halfX;
	buttonRightMENU = centerXbuttonMENU + halfX;
	buttonTopMENU = centerYbuttonMENU - halfY;
	buttonBottomMENU = centerYbuttonMENU + halfY;
}

void selection_update(void)
{
	CP_Image_Draw(screen, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

	CP_Image_Draw(tut1, centerXbuttonTUT, centerYbuttonTUT, halfX * 2, halfY * 2, 255);			//TUT button
	CP_Image_Draw(one1, centerXbutton1, centerYbutton1, halfX * 2, halfY * 2, 255);				//1 button
	CP_Image_Draw(two1, centerXbutton2, centerYbutton2, halfX * 2, halfY * 2, 255);				//2 button
	CP_Image_Draw(three1, centerXbutton3, centerYbutton3, halfX * 2, halfY * 2, 255);			//3 button
	CP_Image_Draw(menu1, centerXbuttonMENU, centerYbuttonMENU, halfX * 2, halfY * 2, 255);		//MENU button

	//detect mouse_
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	//cursor in range of TUT button
	if (((mouseX >= TUTbuttonLeft) && (mouseX <= TUTbuttonRight)) && ((mouseY >= TUTbuttonTop) && (mouseY <= TUTbuttonBottom)))
	{
		//hover effect
		CP_Image_Draw(tut2, centerXbuttonTUT, centerYbuttonTUT, halfX * 2, halfY * 2, 255);

		//TUT level
		if (CP_Input_MouseClicked())
		{
			CP_Engine_SetNextGameState(transition_Init, transition_Update, transition_Exit);
		}
	}

	//cursor in range of level one button
	if (((mouseX >= buttonLeft1) && (mouseX <= buttonRight1)) && ((mouseY >= buttonTop1) && (mouseY <= buttonBottom1)))
	{
		//hover effect
		CP_Image_Draw(one2, centerXbutton1, centerYbutton1, halfX * 2, halfY * 2, 255);

		//launch level 1
		if (CP_Input_MouseClicked())
		{
			CP_Engine_SetNextGameState(transition1_Init, transition1_Update, transition1_Exit);
		}
	}

	//cursor in range of lvl2 button
	else if (((mouseX >= buttonLeft2) && (mouseX <= buttonRight2)) && ((mouseY >= buttonTop2) && (mouseY <= buttonBottom2)))
	{
		//hover effect
		CP_Image_Draw(two2, centerXbutton2, centerYbutton2, halfX * 2, halfY * 2, 255);

		//launch lvl2
		if (CP_Input_MouseClicked())
		{
			CP_Engine_SetNextGameState(transition2_Init, transition2_Update, transition2_Exit);
		}
	}

	//cursor in range of lvl3 button
	else if (((mouseX >= buttonLeft3) && (mouseX <= buttonRight3)) && ((mouseY >= buttonTop3) && (mouseY <= buttonBottom3)))
	{
		//hover effect
		CP_Image_Draw(three2, centerXbutton3, centerYbutton3, halfX * 2, halfY * 2, 255);

		//launch lvl3
		if (CP_Input_MouseClicked())
		{
			CP_Engine_SetNextGameState(transition3_Init, transition3_Update, transition3_Exit);
		}
	}

	//cursor in range of menu button
	else if (((mouseX >= buttonLeftMENU) && (mouseX <= buttonRightMENU)) && ((mouseY >= buttonTopMENU) && (mouseY <= buttonBottomMENU)))
	{
		//hover effect
		CP_Image_Draw(menu2, centerXbuttonMENU, centerYbuttonMENU, halfX * 2, halfY * 2, 255);

		//launch menu
		if (CP_Input_MouseClicked())
		{
			CP_Engine_SetNextGameState(menu_init, menu_update, menu_exit);
		}
	}
}

void selection_exit(void)
{
	CP_Image_Free(&screen);
	CP_Image_Free(&tut1);
	CP_Image_Free(&tut2);
	CP_Image_Free(&one1);
	CP_Image_Free(&one2);
	CP_Image_Free(&two1);
	CP_Image_Free(&two2);
	CP_Image_Free(&three1);
	CP_Image_Free(&three2);
	CP_Image_Free(&menu1);
	CP_Image_Free(&menu2);

	CP_Sound_Free(click);
}
