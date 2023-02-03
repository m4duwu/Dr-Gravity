#include "menu.h"
#include "game.h"
#include "cprocessing.h"
#include "credits.h"
#include "controls.h"
#include "selection.h"

static const float halfX = 180.0f;																		//half of buttons's width
static const float halfY = 70.0f;																		//half of button's height
static float centerXbuttonSTART, centerYbuttonSTART, centerXbuttonQUIT, centerYbuttonQUIT;				//centre of button, to get area where mouse clicks
static float centerXbuttonCONTROLS, centerYbuttonCONTROLS, centerXbuttonCREDITS, centerYbuttonCREDITS;	//centre of button, to get area where mouse clicks
static float STARTbuttonLeft, STARTbuttonRight, STARTbuttonTop, STARTbuttonBottom;						//sides of start button
static float QUITbuttonLeft, QUITbuttonRight, QUITbuttonTop, QUITbuttonBottom;							//sides of quit button
static float CREDITSbuttonLeft, CREDITSbuttonRight, CREDITSbuttonTop, CREDITSbuttonBottom;				//sides of credits button
static float CONTROLSbuttonLeft, CONTROLSbuttonRight, CONTROLSbuttonTop, CONTROLSbuttonBottom;			//sides of controls button

CP_Image menuscreen, start1, start2, controls1, controls2, credits1, credits2, quit1, quit2;
CP_Sound click;

void menu_init(void)
{
	click = CP_Sound_Load("./Assets/Sound/unlock.mp3");
	CP_Sound_Play(click);

	//images
	menuscreen = CP_Image_Load("./Assets/Art/Background/menuscreen.png");
	start1 = CP_Image_Load("./Assets/Art/Background/startbutton.png");
	start2 = CP_Image_Load("./Assets/Art/Background/startbuttonhover.png");
	controls1 = CP_Image_Load("./Assets/Art/Background/controlsbutton.png");
	controls2 = CP_Image_Load("./Assets/Art/Background/controlsbuttonhover.png");
	credits1 = CP_Image_Load("./Assets/Art/Background/creditsbutton.png");
	credits2 = CP_Image_Load("./Assets/Art/Background/creditsbuttonhover.png");
	quit1 = CP_Image_Load("./Assets/Art/Background/quitbutton.png");
	quit2 = CP_Image_Load("./Assets/Art/Background/quitbuttonhover.png");



	//to position the center of start button
	centerXbuttonSTART = CP_System_GetWindowWidth() / 2.9f;
	centerYbuttonSTART = CP_System_GetWindowHeight() / 1.8f;

	//get perimeter of start button
	STARTbuttonLeft = centerXbuttonSTART - halfX;
	STARTbuttonRight = centerXbuttonSTART + halfX;
	STARTbuttonTop = centerYbuttonSTART - halfY;
	STARTbuttonBottom = centerYbuttonSTART + halfY;

	//to position the center of controls button
	centerXbuttonCONTROLS = CP_System_GetWindowWidth() / 2.9f;
	centerYbuttonCONTROLS = CP_System_GetWindowHeight() / 1.3f;

	//get perimeter of controls button
	CONTROLSbuttonLeft = centerXbuttonCONTROLS - halfX;
	CONTROLSbuttonRight = centerXbuttonCONTROLS + halfX;
	CONTROLSbuttonTop = centerYbuttonCONTROLS - halfY;
	CONTROLSbuttonBottom = centerYbuttonCONTROLS + halfY;

	//to position the center of credits button
	centerXbuttonCREDITS = CP_System_GetWindowWidth() / 1.5f;
	centerYbuttonCREDITS = CP_System_GetWindowHeight() / 1.8f;

	//get perimeter of credits button
	CREDITSbuttonLeft = centerXbuttonCREDITS - halfX;
	CREDITSbuttonRight = centerXbuttonCREDITS + halfX;
	CREDITSbuttonTop = centerYbuttonCREDITS - halfY;
	CREDITSbuttonBottom = centerYbuttonCREDITS + halfY;

	//position center of quit button
	centerXbuttonQUIT = CP_System_GetWindowWidth() / 1.5f;
	centerYbuttonQUIT = CP_System_GetWindowHeight() / 1.3f;

	//perimeter of quit button
	QUITbuttonLeft = centerXbuttonQUIT - halfX;
	QUITbuttonRight = centerXbuttonQUIT + halfX;
	QUITbuttonTop = centerYbuttonQUIT - halfY;
	QUITbuttonBottom = centerYbuttonQUIT + halfY;
}

void menu_update(void)
{
	//background
	CP_Settings_Background(CP_Color_Create(255, 255, 255, 255));

	//logo
	CP_Image_Draw(menuscreen, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

	CP_Image_Draw(start1, centerXbuttonSTART, centerYbuttonSTART, halfX * 2, halfY * 2, 255);			//start button
	CP_Image_Draw(controls1, centerXbuttonCONTROLS, centerYbuttonCONTROLS, halfX * 2, halfY * 2, 255);	//controls button
	CP_Image_Draw(credits1, centerXbuttonCREDITS, centerYbuttonCREDITS, halfX * 2, halfY * 2, 255);		//credits button
	CP_Image_Draw(quit1, centerXbuttonQUIT, centerYbuttonQUIT, halfX * 2, halfY * 2, 255);				//quit button

	//detect mouse
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	//cursor in range of start button
	if (((mouseX >= STARTbuttonLeft) && (mouseX <= STARTbuttonRight)) && ((mouseY >= STARTbuttonTop) && (mouseY <= STARTbuttonBottom)))
	{
		//hover effect
		CP_Image_Draw(start2, centerXbuttonSTART, centerYbuttonSTART, halfX * 2, halfY * 2, 255);

		//launch game
		if (CP_Input_MouseClicked())
		{
			//CP_Engine_SetNextGameState(game_init, game_update, game_exit);
			CP_Engine_SetNextGameState(selection_init, selection_update, selection_exit);
		}
	}

	//cursor in range of controls button
	if (((mouseX >= CONTROLSbuttonLeft) && (mouseX <= CONTROLSbuttonRight)) && ((mouseY >= CONTROLSbuttonTop) && (mouseY <= CONTROLSbuttonBottom)))
	{
		//hover effect
		CP_Image_Draw(controls2, centerXbuttonCONTROLS, centerYbuttonCONTROLS, halfX * 2, halfY * 2, 255);

		//launch controls screen
		if (CP_Input_MouseClicked())
		{
			CP_Engine_SetNextGameState(controls_init, controls_update, controls_exit);
		}
	}

	//cursor in range of credits button
	else if (((mouseX >= CREDITSbuttonLeft) && (mouseX <= CREDITSbuttonRight)) && ((mouseY >= CREDITSbuttonTop) && (mouseY <= CREDITSbuttonBottom)))
	{
		//hover effect
		CP_Image_Draw(credits2, centerXbuttonCREDITS, centerYbuttonCREDITS, halfX * 2, halfY * 2, 255);

		//credit screen
		if (CP_Input_MouseClicked())
		{
			CP_Engine_SetNextGameState(credits_init, credits_update, credits_exit);
		}
	}

	//cursor in range of quit button
	else if (((mouseX >= QUITbuttonLeft) && (mouseX <= QUITbuttonRight)) && ((mouseY >= QUITbuttonTop) && (mouseY <= QUITbuttonBottom)))
	{
		//hover effect
		CP_Image_Draw(quit2, centerXbuttonQUIT, centerYbuttonQUIT, halfX * 2, halfY * 2, 255);

		//quit game
		if (CP_Input_MouseClicked())
		{
			CP_Engine_Terminate();
		}
	}
}

void menu_exit(void)
{
	CP_Image_Free(&menuscreen);
	CP_Image_Free(&start1);
	CP_Image_Free(&start2);
	CP_Image_Free(&controls1);
	CP_Image_Free(&controls2);
	CP_Image_Free(&credits1);
	CP_Image_Free(&credits2);
	CP_Image_Free(&quit1);
	CP_Image_Free(&quit2);

	CP_Sound_Free(click);
}
