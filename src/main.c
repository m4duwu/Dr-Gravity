//---------------------------------------------------------
// file:	main.c
// author:	[NAME]
// email:	[DIGIPEN EMAIL ADDRESS]
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "logo.h"
//#include "game3.h"
#include <stdio.h>


// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int main(void)
{
	//CP_System_Fullscreen;
	CP_System_SetWindowTitle("Dr Gravity");//Setting the window title
	//CP_System_Fullscreen();
	CP_System_SetWindowSize(1600, 960);
	CP_System_SetFrameRate(60);//Setting Frame Rate
	CP_System_ShowConsole();//Show console
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	CP_Engine_SetNextGameState(logo_init, logo_update, logo_exit);
	//CP_Engine_SetNextGameState(game3_init, game3_update, game3_exit);
	CP_Engine_Run();
	return 0;
}
