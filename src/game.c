#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cprocessing.h"
#include "transition.h"
#include "transition1.h"
#include "selection.h"

#define GRID_SIZE 80 //grid size
#define GRID_OFFSET 40 //grid offset, to center the sprite in  
#define GRAVITY_TOP 0.2f
#define GRAVITY_BOTTOM -0.2f
#define spritey_half 80
#define spritex_half 40
#define TRUE 1
#define FALSE 0
#define TOP 1
#define BOTTOM 0
#define CHARACTER_BUFFER_SIZE 40 //for string
#define GAMEW 22
#define GAMEH 17

int window_width, window_height, GW, GH; //for window size
int jump = FALSE, gravity = FALSE, stand = TRUE, state = BOTTOM, coll = FALSE, yold1 = 0;
int xspeed = 1, checkinit = 0, keytaken = 0, bpill = 0, rpill = 0, rp = 0, bluetime = 0;
float ttime = 0, gravitytimer = 7.5f;
float velX, velY, velYobj;//for velocity (Sprite movement)

int pushchk = 0, pause = 2;

float directionX = -1;	//1 is left, -1 is right
float directionY = 1;	//1 is on ground, -1 is on ceiling
//for camera
static const float SCALE_QUANTUM = 1.5f;
static const float ROTATION_QUANTUM = 5.0f;
static const float PAN_QUANTUM = 100.0f;
static const float FONT_SIZE = 50.0f;
static float currentScale;
static float currentRotation;
static CP_Vector currentPosition, centerOffset;
static CP_Matrix scaleMatrix, rotationMatrix, translationMatrix;
//for pause menu
static const float halfX = 180.0f;																		//half of buttons's width
static const float halfY = 70.0f;																		//half of button's height
static float centerXbuttonRESTART, centerYbuttonRESTART, centerXbuttonQUIT, centerYbuttonQUIT;			//centre of button, to get area where mouse clicks
static float centerXbuttonLEVELS, centerYbuttonLEVELS, centerXbuttonCONT, centerYbuttonCONT;			//centre of button, to get area where mouse clicks
static float RESTARTbuttonLeft, RESTARTbuttonRight, RESTARTbuttonTop, RESTARTbuttonBottom;				//sides of restart button
static float QUITbuttonLeft, QUITbuttonRight, QUITbuttonTop, QUITbuttonBottom;							//sides of quit button
static float CONTbuttonLeft, CONTbuttonRight, CONTbuttonTop, CONTbuttonBottom;							//sides of continue button
static float LEVELSbuttonLeft, LEVELSbuttonRight, LEVELSbuttonTop, LEVELSbuttonBottom;					//sides of level select button

CP_Image pausescreen, restart1, restart2, levels1, levels2, resume1, resume2, quit1, quit2;
CP_Image sprite;
CP_Image docIdle;
CP_Image docWalk1;
CP_Image docWalk2;
CP_Image pushLeft1;
CP_Image pushLeft2;
CP_Image keycard;
CP_Image floor;
CP_Image pillRed;
CP_Image pillBlue;
CP_Image windowTHIC;
CP_Image windowlong;
CP_Image box;
CP_Image bg;
CP_Image roof;
CP_Image doors;
CP_Image walls;
CP_Image bluespeechbubble;
CP_Image redspeechbubble;
CP_Image keyspeechbubble;
CP_Image movespeechbubble;
CP_Image pausespeechbubble;

CP_Sound bgm;
CP_Sound timerbeep;
CP_Sound unlock;

typedef struct coordinate {
	int x;//x-axis (Values are in terms of GRID NUMBER)
	int y;//y-axis (Values are in terms of GRID NUMBER)
}coordinate;
coordinate dr, key, door, ground[100], platform[100], window[100], red, blue, wallb[100], sides[100];
coordinate obj1max, obj1min, obj2max, obj2min, pen; //for collision
coordinate bluebubble, redbubble, keybubble, movebubble, pausebubble; //for speechbubbles

void game_init(void)
{
	scaleMatrix = rotationMatrix = CP_Matrix_Identity();
	currentScale = 1.0f;
	currentRotation = 0.0f;
	//initialize struct array
	for (int i = 0; i < 50; ++i)
	{
		window[i].x = ((-5 * GRID_SIZE) - GRID_OFFSET);
		window[i].y = ((-5 * GRID_SIZE) - GRID_OFFSET);

		wallb[i].x = ((-5 * GRID_SIZE) - GRID_OFFSET);
		wallb[i].y = ((-5 * GRID_SIZE) - GRID_OFFSET);

		sides[i].x = ((-5 * GRID_SIZE) - GRID_OFFSET);
		sides[i].y = ((-5 * GRID_SIZE) - GRID_OFFSET);
	}

	//window
	window_width = 1760;			//get window width
	window_height = 1360;			//get window height
	GW = window_width / GRID_SIZE;	//get X-grid
	GH = window_height / GRID_SIZE; //get Y-grid 

	float centerX = window_width / 2.0f;
	float centerY = window_height / 2.0f;
	centerOffset = CP_Vector_Set(centerX, centerY);
	currentPosition = CP_Vector_Zero();
	translationMatrix = CP_Matrix_Translate(currentPosition);

	//images
	sprite = CP_Image_Load("./Assets/Art/Sprite/surgeon.png");
	docIdle = CP_Image_Load("./Assets/Art/Sprite/surgeonIdle.png");
	docWalk1 = CP_Image_Load("./Assets/Art/Sprite/walkingleft1.png");
	docWalk2 = CP_Image_Load("./Assets/Art/Sprite/walkingleft2.png");
	pushLeft1 = CP_Image_Load("./Assets/Art/Sprite/pushleft1.png");
	pushLeft2 = CP_Image_Load("./Assets/Art/Sprite/pushleft2.png");
	keycard = CP_Image_Load("./Assets/Art/Sprite/keycard.png");
	pillRed = CP_Image_Load("./Assets/Art/Sprite/redpill.png");
	pillBlue = CP_Image_Load("./Assets/Art/Sprite/bluepill.png");
	box = CP_Image_Load("./Assets/Art/Sprite/box.png");
	windowTHIC = CP_Image_Load("./Assets/Art/Sprite/widewindow.png");
	windowlong = CP_Image_Load("./Assets/Art/Sprite/longwindow.png");
	doors = CP_Image_Load("./Assets/Art/Sprite/doors.png");

	floor = CP_Image_Load("./Assets/Art/Wall/floor.png");		//floor(border)
	bg = CP_Image_Load("./Assets/Art/Wall/bg.png");
	roof = CP_Image_Load("./Assets/Art/Wall/roof.png");
	walls = CP_Image_Load("./Assets/Art/Wall/walls.png");

	bluespeechbubble = CP_Image_Load("./Assets/Art/Background/bluebubble.png");
	redspeechbubble = CP_Image_Load("./Assets/Art/Background/redbubble.png");
	movespeechbubble = CP_Image_Load("./Assets/Art/Background/movebubble.png");
	keyspeechbubble = CP_Image_Load("./Assets/Art/Background/keybubble.png");
	pausespeechbubble = CP_Image_Load("./Assets/Art/Background/pausebubble.png");

	bgm = CP_Sound_LoadMusic("./Assets/Sound/DocGravBGM.mp3");	//Load music
	CP_Sound_PlayMusic(bgm);	//Play music
	timerbeep = CP_Sound_Load("./Assets/Sound/timerbeep.mp3");

	//sprite coordinates
	dr.x = 2 * GRID_SIZE;
	dr.y = (GH - 4) * GRID_SIZE;

	//box
	ground[0].x = (10 * GRID_SIZE - GRID_OFFSET);
	ground[0].y = (((GH - 10) * GRID_SIZE) - GRID_OFFSET);

	//horizontal windows
	window[2].x = ((9 * GRID_SIZE) - GRID_OFFSET);
	window[2].y = (((GH - 9) * GRID_SIZE) - GRID_OFFSET);

	window[3].x = ((12 * GRID_SIZE) - GRID_OFFSET);
	window[3].y = (((GH - 9) * GRID_SIZE) - GRID_OFFSET);

	window[4].x = ((15 * GRID_SIZE) - GRID_OFFSET);
	window[4].y = (((GH - 10) * GRID_SIZE) - GRID_OFFSET);

	window[5].x = ((15 * GRID_SIZE) - GRID_OFFSET);
	window[5].y = (((GH - 11) * GRID_SIZE) - GRID_OFFSET);

	window[6].x = ((18 * GRID_SIZE) - GRID_OFFSET);
	window[6].y = (((GH - 10) * GRID_SIZE) - GRID_OFFSET);

	window[7].x = ((18 * GRID_SIZE) - GRID_OFFSET);
	window[7].y = (((GH - 11) * GRID_SIZE) - GRID_OFFSET);

	window[8].x = ((9 * GRID_SIZE) - GRID_OFFSET);
	window[8].y = (((GH - 5) * GRID_SIZE) - GRID_OFFSET);

	//vertical windows
	wallb[1].x = ((7 * GRID_SIZE) - GRID_OFFSET);
	wallb[1].y = (((GH - 2) * GRID_SIZE) - GRID_OFFSET);

	wallb[2].x = ((7 * GRID_SIZE) - GRID_OFFSET);
	wallb[2].y = (((GH - 5) * GRID_SIZE) - GRID_OFFSET);

	wallb[3].x = ((7 * GRID_SIZE) - GRID_OFFSET);
	wallb[3].y = (((GH - 8) * GRID_SIZE) - GRID_OFFSET);

	wallb[4].x = ((10 * GRID_SIZE) - GRID_OFFSET);
	wallb[4].y = (((GH - 14) * GRID_SIZE) - GRID_OFFSET);

	//items
	key.x = 9 * GRID_SIZE - GRID_OFFSET;
	key.y = 10 * GRID_SIZE - GRID_OFFSET;
	door.x = (10 * GRID_SIZE);
	door.y = (15 * GRID_SIZE) - GRID_OFFSET;
	red.x = 16 * GRID_SIZE - GRID_OFFSET;
	red.y = 5 * GRID_SIZE - GRID_OFFSET;
	blue.x = 5 * GRID_SIZE - GRID_OFFSET;
	blue.y = 15 * GRID_SIZE - GRID_OFFSET;

	bluebubble.x = 5 * GRID_SIZE;
	bluebubble.y = 4 * GRID_SIZE;
	redbubble.x = 17 * GRID_SIZE;
	redbubble.y = 3 * GRID_SIZE - GRID_OFFSET;
	keybubble.x = 16 * GRID_SIZE;
	keybubble.y = 14 * GRID_SIZE;
	movebubble.x = 4 * GRID_SIZE - GRID_OFFSET;
	movebubble.y = 11 * GRID_SIZE;
	pausebubble.x = 17 * GRID_SIZE - GRID_OFFSET;
	pausebubble.y = 10 * GRID_SIZE - GRID_OFFSET;

	velX = 0.0f;
	velY = 0.0f;
	velYobj = 0.0f;

	//images
	pausescreen = CP_Image_Load("./Assets/Art/Background/pause.png");
	restart1 = CP_Image_Load("./Assets/Art/Background/restartbutton.png");
	restart2 = CP_Image_Load("./Assets/Art/Background/restartbuttonhover.png");
	levels1 = CP_Image_Load("./Assets/Art/Background/levelsbutton.png");
	levels2 = CP_Image_Load("./Assets/Art/Background/levelsbuttonhover.png");
	resume1 = CP_Image_Load("./Assets/Art/Background/resumebutton.png");
	resume2 = CP_Image_Load("./Assets/Art/Background/resumebuttonhover.png");
	quit1 = CP_Image_Load("./Assets/Art/Background/quitbutton.png");
	quit2 = CP_Image_Load("./Assets/Art/Background/quitbuttonhover.png");

	//to position the center of restart button
	centerXbuttonRESTART = CP_System_GetWindowWidth() / 2.9f;
	centerYbuttonRESTART = CP_System_GetWindowHeight() / 2.0f;

	//get perimeter of restart button
	RESTARTbuttonLeft = centerXbuttonRESTART - halfX;
	RESTARTbuttonRight = centerXbuttonRESTART + halfX;
	RESTARTbuttonTop = centerYbuttonRESTART - halfY;
	RESTARTbuttonBottom = centerYbuttonRESTART + halfY;

	//to position the center of controls button
	centerXbuttonLEVELS = CP_System_GetWindowWidth() / 2.9f;
	centerYbuttonLEVELS = CP_System_GetWindowHeight() / 1.3f;

	//get perimeter of controls button
	LEVELSbuttonLeft = centerXbuttonLEVELS - halfX;
	LEVELSbuttonRight = centerXbuttonLEVELS + halfX;
	LEVELSbuttonTop = centerYbuttonLEVELS - halfY;
	LEVELSbuttonBottom = centerYbuttonLEVELS + halfY;

	//to position the center of continue button
	centerXbuttonCONT = CP_System_GetWindowWidth() / 1.5f;
	centerYbuttonCONT = CP_System_GetWindowHeight() / 2.0f;

	//get perimeter of credits button
	CONTbuttonLeft = centerXbuttonCONT - halfX;
	CONTbuttonRight = centerXbuttonCONT + halfX;
	CONTbuttonTop = centerYbuttonCONT - halfY;
	CONTbuttonBottom = centerYbuttonCONT + halfY;

	//position center of quit button
	centerXbuttonQUIT = CP_System_GetWindowWidth() / 1.5f;
	centerYbuttonQUIT = CP_System_GetWindowHeight() / 1.3f;

	//perimeter of quit button
	QUITbuttonLeft = centerXbuttonQUIT - halfX;
	QUITbuttonRight = centerXbuttonQUIT + halfX;
	QUITbuttonTop = centerYbuttonQUIT - halfY;
	QUITbuttonBottom = centerYbuttonQUIT + halfY;
}
/*collision codes for pick up item*/
int pickup_item(coordinate obj1, coordinate obj2)
{
	//set target box
	obj1max.x = obj1.x + 40;
	obj1max.y = obj1.y + 80;
	obj1min.x = obj1.x - 40;
	obj1min.y = obj1.y - 80;

	obj2max.x = obj2.x + 20;
	obj2max.y = obj2.y + 20;
	obj2min.x = obj2.x - 20;
	obj2min.y = obj2.y - 20;

	if (((obj1min.y < obj2max.y) && (obj1max.y > obj2min.y)) && ((obj1max.x > obj2min.x) && (obj1min.x < obj2max.x)))
		return 1;
	else
		return 0;
}
/*collision codes for everything else*/
void collision(coordinate* obj1, coordinate* obj2, int z)
{
	pen.x = 0;
	pen.y = 0;
	//set target box
	if (obj1 == &dr)
	{
		obj1max.x = obj1->x + 40;		//doc
		obj1max.y = obj1->y + 80;
		obj1min.x = obj1->x - 40;
		obj1min.y = obj1->y - 80;
	}
	else if (obj1 == &ground[0])
	{
		obj1max.x = obj1->x + 40;		//box
		obj1max.y = obj1->y + 40;
		obj1min.x = obj1->x - 40;
		obj1min.y = obj1->y - 40;
	}
	if (obj2 == &window[z])
	{
		obj2max.x = obj2->x + 120;		//window horizontal
		obj2max.y = obj2->y + 40;
		obj2min.x = obj2->x - 120;
		obj2min.y = obj2->y - 40;
	}

	else if (obj2 == &wallb[z])
	{
		obj2max.x = obj2->x + 40;		//window vertical
		obj2max.y = obj2->y + 120;
		obj2min.x = obj2->x - 40;
		obj2min.y = obj2->y - 120;
	}

	else if (obj2 == &sides[z])
	{
		obj2max.x = obj2->x + 40;		//sides
		obj2max.y = obj2->y + 120;
		obj2min.x = obj2->x - 40;
		obj2min.y = obj2->y - 120;
	}

	else if (obj2 == &ground[0])
	{
		obj2max.x = obj2->x + 40;		//box
		obj2max.y = obj2->y + 40;
		obj2min.x = obj2->x - 40;
		obj2min.y = obj2->y - 40;
	}

	if (obj1->x <= obj2->x)
		pen.x = (obj1max.x - obj2min.x);
	if (obj1->x > obj2->x)
		pen.x = (obj2max.x - obj1min.x);
	if (obj1->y >= obj2->y)
		pen.y = (obj2max.y - obj1min.y);
	if (obj1->y < obj2->y)
		pen.y = (obj1max.y - obj2min.y);

	//checking for collision
	if ((pen.x >= 0) && (pen.y >= 0))
	{
		//check for left or right penetration
		if (pen.x < pen.y)
		{
			//left penetration
			if (obj1->x < obj2->x)
			{
				if (((obj1 == &dr) || (obj1 == &ground[0])) && ((obj2 == &window[z]) || (obj2 == &wallb[z]) || (obj2 == &sides[z])))
				{
					obj1->x = obj1->x - (obj1max.x - obj2min.x - 2);
					obj1->x -= (int)velX;
				}
				if ((obj1 == &dr || obj1 == &ground[0]) && (obj2 == &ground[0]))
				{
					obj1->x = obj1->x - (obj1max.x - obj2min.x - 2);

					obj2->x += 6;

					pushchk = 1;
				}
			}
			//right penetration
			else if (obj1->x > obj2->x)
			{
				//check whether objects can be pushed or not
				if (((obj1 == &dr) || (obj1 == &ground[0])) && ((obj2 == &window[z]) || (obj2 == &wallb[z]) || (obj2 == &sides[z])))
				{
					obj1->x = obj1->x + (obj2max.x - obj1min.x - 2);
					obj1->x -= (int)velX;
				}
				if (((obj1 == &dr) || (obj1 == &ground[0])) && (obj2 == &ground[0]))
				{
					obj1->x = obj1->x + (obj2max.x - obj1min.x - 2);

					obj2->x -= 6;

					pushchk = 1;
				}

			}
		}
		//check for up down penetration
		else if (pen.y < pen.x)
		{
			//check for up penetration
			if (obj1->y < obj2->y)
			{
				obj1->y = obj1->y - 2 - (obj1max.y - obj2min.y);
				//check object type
				if (obj1 == &dr)
				{
					velY = 0;
					if (state == BOTTOM)
					{
						stand = TRUE;
						jump = FALSE;
					}
				}
				//check object type
				else if (obj1 == &ground[0])
					velYobj = 0;
			}
			//check for bottom penetration		
			else if (obj1->y > obj2->y)
			{
				//check object type
				if (obj1 == &dr)
				{
					velY = 0;
					if (state == TOP)
					{
						stand = TRUE;
						jump = FALSE;
					}
					//check for gravity
					if (gravity == TRUE)
					{
						jump = FALSE;
						gravity = TRUE;
						stand = TRUE;
						state = TOP;
					}
				}
				//check object type
				else if (obj1 == &ground[0])
					velYobj = 0;
				obj1->y = obj1->y + 2 + (obj2max.y - obj1min.y);


			}
		}
	}
}

void collision_border(coordinate* obj1)
{
	int w = 0;
	//check for object type
	if (obj1 == &dr)
	{
		//set target box
		obj1max.x = obj1->x + 40;			//doc
		obj1max.y = obj1->y + 80;
		obj1min.x = obj1->x - 40;
		obj1min.y = obj1->y - 80;
		w = 80;

		if (obj1max.y > ((GH - 1) * 80))
		{
			obj1->y = ((GH - 1) * 80) - w;
			if (obj1 == &dr)
			{
				stand = TRUE;
				jump = FALSE;
				velY = 0;
			}
		}
		if (obj1min.y < GRID_SIZE)
		{
			obj1->y = w * 2;
			stand = TRUE;
			state = BOTTOM;
			jump = FALSE;
			velY = 0;
			if (gravity == TRUE)
			{
				jump = FALSE;
				gravity = TRUE;
				stand = TRUE;
				state = TOP;
			}
		}
	}
	//check for object type
	else if (obj1 == &ground[0])
	{
		obj1max.x = obj1->x + 40;		//box object
		obj1max.y = obj1->y + 40;
		obj1min.x = obj1->x - 40;
		obj1min.y = obj1->y - 30;
		w = 40;
		if (obj1max.y > ((GH - 1) * 80))
		{
			obj1->y = ((GH - 1) * 80) - w;
			velYobj = 0;
		}

		if (obj1min.y < GRID_SIZE)
		{
			obj1->y = w * 2 + w;
			velYobj = 0;
		}

	}

}

void move(void) {
	if (checkinit == 0 && (
		CP_Input_KeyDown(KEY_D) ||
		CP_Input_KeyDown(KEY_A) ||
		CP_Input_KeyTriggered(KEY_W) ||
		CP_Input_KeyTriggered(KEY_SPACE)))
		checkinit++;
	if (CP_Input_KeyDown(KEY_D))
		velX = 6.0f;
	else if (CP_Input_KeyDown(KEY_A))
		velX = -6.0f;
	else
		velX = 0.0f;

	if (CP_Input_KeyTriggered(KEY_W))
	{
		if (jump == FALSE && stand == TRUE && state == BOTTOM)
		{
			stand = FALSE;
			jump = TRUE;
			velY = -7.0f;
		}
		if (state == TOP && stand == TRUE && jump == FALSE)
		{
			stand = FALSE;
			jump = TRUE;
			velY = +7.0f;
		}
	}

	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		if (rpill > 0)//check for redpill status
		{
			if (state == BOTTOM && gravity == FALSE && stand == TRUE)
			{
				gravity = TRUE;
				stand = FALSE;
				state = TOP;
				velY -= 5.0f;
				velYobj -= 5.0f;
			}
			else if (state == TOP && gravity == TRUE && stand == TRUE)
			{
				gravity = FALSE;
				stand = FALSE;
				state = BOTTOM;
				velY += 5.0f;
				velYobj += 5.0f;
			}
		}
	}
	dr.x += (int)velX;
	dr.y += (int)velY;
	if (yold1 != dr.y)
		stand = FALSE;
	ground[0].y += (int)velYobj;

	if ((dr.y > window_height / 2) && (dr.y <= (((GAMEH + 5) * 80) - (window_height / 2))))
		currentPosition.y = (float)-dr.y + window_height / 2;

	if ((dr.x >= window_width / 2) && (dr.x <= (((GAMEW + 2) * 80) - (window_width / 2))))
		currentPosition.x = (float)-dr.x + (window_width / 2);

	// FOR CAMERA MOVEMENT
	translationMatrix = CP_Matrix_Translate(currentPosition);
	CP_Vector offsetOrigin = CP_Vector_Scale(currentPosition, -1.0f);
	CP_Vector offsetVector = CP_Vector_Add(offsetOrigin, centerOffset);
	CP_Matrix offsetMatrix = CP_Matrix_Translate(CP_Vector_Scale(offsetVector, -1.0f));

	//translate all objects by the world space position of the current screen center
	CP_Matrix offsetUndoMatrix = CP_Matrix_Translate(offsetVector);

	//now all objects are within the screen center's local coord system
	//so we apply the scale
	CP_Matrix transform = CP_Matrix_Multiply(scaleMatrix, offsetMatrix);

	//then the rotation
	transform = CP_Matrix_Multiply(rotationMatrix, transform);
	//then we return all objects to the world space
	transform = CP_Matrix_Multiply(offsetUndoMatrix, transform);
	//translate objects in world space based on the current camera panning
	transform = CP_Matrix_Multiply(translationMatrix, transform);

	CP_Settings_ApplyMatrix(transform);

	//check for gravity
	if (gravity == TRUE)
	{
		velY += GRAVITY_BOTTOM;
		velYobj += GRAVITY_BOTTOM;
	}
	//collision for border
	collision_border(&dr);
	collision_border(&ground[0]);

	//collision for windows
	for (int z = 1; z < 13; ++z)
	{
		collision(&dr, &window[z], z);
		collision(&ground[0], &window[z], z);
	}
	//collision for walls
	for (int i = 0; i < 5; ++i)
	{
		collision(&dr, &wallb[i], i);
		collision(&ground[0], &wallb[i], i);
	}

	for (int i = 0; i < 100; ++i)
	{
		collision(&dr, &sides[i], i);
		collision(&ground[0], &sides[i], i);
	}

	//collision for dr and boxes
	collision(&dr, &ground[0], 0);
	//check for gravity
	if (gravity == FALSE)
	{
		velY += GRAVITY_TOP;
		velYobj += GRAVITY_TOP;
	}
	//check pick up key
	if (pickup_item(dr, key) == 1)
	{
		keytaken++;
	}
	//check pick up blue pill
	if ((pickup_item(dr, blue) == 1) && (bluetime == 0))
	{
		CP_Sound_Play(timerbeep);
		//timer for gravity shift
		gravitytimer = 7.5f;
		bpill++;
		jump = TRUE;
		stand = FALSE;
		bluetime = 1;
		if (bpill == 1)
		{

			gravity = TRUE;
			state = TOP;
		}
	}
	//timer for gravity shift
	if (gravitytimer > 0 && bpill > 0)
	{
		gravitytimer -= CP_System_GetDt();//counter
		if (gravitytimer < 0)
		{
			//stop gravity shift
			jump = TRUE;
			stand = FALSE;
			gravity = FALSE;
			state = BOTTOM;
			bluetime = 0;
			if (rpill < 1)
				bpill = 0;
			if (rpill > 0)
				rp++; //to use gravity shift after timer is finished
		}
	}
	//check pick up red pill
	if (pickup_item(dr, red) == 1)
	{
		if (bpill == 0)
			rp++;
		bpill++;
		bluetime = 2;
		rpill++;
	}

	yold1 = dr.y;//to check if doctor is in air or not
	if ((pickup_item(dr, door) == 1) && (keytaken > 0))
		CP_Engine_SetNextGameState(transition1_Init, transition1_Update, transition1_Exit);

}
void border(void)
{
	int y = 1;
	for (int x = 1; x <= 100; ++x)
	{
		if (x <= 30) //left
		{
			sides[x].x = (1 * GRID_SIZE) - GRID_OFFSET;
			sides[x].y = (x * GRID_SIZE) - GRID_OFFSET;
			CP_Image_Draw(walls, (float)sides[x].x, (float)sides[x].y, GRID_SIZE, GRID_SIZE, 255);
		}
		else if (x > 30)//right
		{
			sides[x].x = (GAMEW * GRID_SIZE) - GRID_OFFSET;
			sides[x].y = (y * GRID_SIZE) - GRID_OFFSET;
			CP_Image_Draw(walls, (float)sides[x].x, (float)sides[x].y, GRID_SIZE, GRID_SIZE, 255);
			++y;
		}
	}

	y = 1;
	for (int x = 1; x <= 80; ++x)
	{
		if (x <= 30) //bottom
		{
			ground[x].x = (x * GRID_SIZE) - GRID_OFFSET;
			ground[x].y = (GH * GRID_SIZE) - GRID_OFFSET;
			CP_Image_Draw(floor, (float)ground[x].x, (float)ground[x].y, GRID_SIZE, GRID_SIZE, 255);
		}
		else if (x > 30)//top
		{
			ground[x].x = (y * GRID_SIZE) - GRID_OFFSET;
			ground[x].y = (GRID_SIZE)-GRID_OFFSET;
			y++;
			CP_Image_Draw(roof, (float)ground[x].x, (float)ground[x].y, GRID_SIZE, GRID_SIZE, 255);
		}
	}

	//draw boxes
	CP_Image_Draw(box, (float)ground[0].x, (float)ground[0].y, GRID_SIZE, GRID_SIZE, 255);
	for (int i = 0; i < 50; ++i)		//print windows
	{
		CP_Image_Draw(windowTHIC, (float)window[i].x, (float)window[i].y, GRID_SIZE * 3, GRID_SIZE, 255);
		CP_Image_Draw(windowlong, (float)wallb[i].x, (float)wallb[i].y, GRID_SIZE, GRID_SIZE * 3, 255);
	}
}

//takes in TWO sprites to make anim, coords for the sprite, time for each frame, and direction/size of the sprite
void anim(CP_Image img1, CP_Image img2, coordinate c, float t1, float t2, float dirX, float dirY)
{
	ttime += CP_System_GetDt();

	if (ttime <= t1)	//for t1 time, 
	{	//draw FIRST image at the pos, for what dir
		CP_Image_Draw(img1, (float)c.x, (float)c.y, GRID_SIZE * dirX, GRID_SIZE * dirY, 255);
	}
	else if ((ttime >= t1) && (ttime < t2))	//for t1 to t2 time,
	{	//draw SECOND image at the pos, for what dir
		CP_Image_Draw(img2, (float)c.x, (float)c.y, GRID_SIZE * dirX, GRID_SIZE * dirY, 255);
	}
	else if (ttime >= t2)
	{
		ttime -= t2;
		CP_Image_Draw(img1, (float)c.x, (float)c.y, GRID_SIZE * dirX, GRID_SIZE * dirY, 255);
	}
}

void draw(void)
{
	if (rpill == 0)
		CP_Image_Draw(pillRed, (float)red.x, (float)red.y, GRID_SIZE, GRID_SIZE, 255);			//red
	if (bpill == 0)
		CP_Image_Draw(pillBlue, (float)blue.x, (float)blue.y, GRID_SIZE, GRID_SIZE, 255);			//blue
	CP_Image_Draw(doors, (float)door.x, (float)door.y, GRID_SIZE * 3, GRID_SIZE * 3, 255);			//door
	CP_Image_Draw(bluespeechbubble, (float)bluebubble.x, (float)bluebubble.y, GRID_SIZE * 7, GRID_SIZE * 2, 255); //blue speech bubble
	CP_Image_Draw(redspeechbubble, (float)redbubble.x, (float)redbubble.y, GRID_SIZE * 7, GRID_SIZE * 2.5f, 255); //red speech bubble
	CP_Image_Draw(keyspeechbubble, (float)keybubble.x, (float)keybubble.y, GRID_SIZE * 6, GRID_SIZE * 2, 255); //key speech bubble
	CP_Image_Draw(movespeechbubble, (float)movebubble.x, (float)movebubble.y, GRID_SIZE * 3.5f, GRID_SIZE * 3.75f, 255); //move speech bubble
	CP_Image_Draw(pausespeechbubble, (float)pausebubble.x, (float)pausebubble.y, GRID_SIZE * 3.5f, GRID_SIZE * 2, 255); //pause speech bubble

	if (gravity == FALSE)
	{
		directionY = 1;
	}
	else if (gravity == TRUE)
	{
		directionY = -1;
	}
	//keycard
	if (keytaken == 0)
	{
		anim(keycard, keycard, key, 1, 1, 1, 1);
	}
	if (velX > 0)		//right
	{
		directionX = -1;
		if (pushchk == 1)
		{
			anim(pushLeft1, pushLeft2, dr, 0.2f, 0.4f, directionX, 2 * directionY);
			pushchk = 0;
		}
		else
		{
			anim(docWalk1, docWalk2, dr, 0.2f, 0.4f, directionX, 2 * directionY);
		}
	}
	else if (velX < 0)	//left
	{
		directionX = 1;
		if (pushchk == 1)
		{
			anim(pushLeft1, pushLeft2, dr, 0.2f, 0.4f, directionX, 2 * directionY);
			pushchk = 0;
		}
		else
		{
			anim(docWalk1, docWalk2, dr, 0.2f, 0.4f, directionX, 2 * directionY);
		}
	}
	else				//idle
	{
		anim(docIdle, sprite, dr, 0.5f, 1, directionX, 2 * directionY);
	}
}

void paused(void)
{


	CP_Image_Draw(pausescreen, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

	CP_Image_Draw(restart1, centerXbuttonRESTART, centerYbuttonRESTART, halfX * 2, halfY * 2, 255);		//restart button
	CP_Image_Draw(levels1, centerXbuttonLEVELS, centerYbuttonLEVELS, halfX * 2, halfY * 2, 255);	//controls button
	CP_Image_Draw(resume1, centerXbuttonCONT, centerYbuttonCONT, halfX * 2, halfY * 2, 255);		//credits button
	CP_Image_Draw(quit1, centerXbuttonQUIT, centerYbuttonQUIT, halfX * 2, halfY * 2, 255);				//quit button

	//detect mouse_
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	//cursor in range of restart button
	if (((mouseX >= RESTARTbuttonLeft) && (mouseX <= RESTARTbuttonRight)) && ((mouseY >= RESTARTbuttonTop) && (mouseY <= RESTARTbuttonBottom)))
	{
		//hover effect
		CP_Image_Draw(restart2, centerXbuttonRESTART, centerYbuttonRESTART, halfX * 2, halfY * 2, 255);

		//restart level
		if (CP_Input_MouseClicked())
		{
			//CP_Engine_SetNextGameState(game_init, game_update, game_exit);
			CP_Engine_SetNextGameState(transition_Init, transition_Update, transition_Exit);
			pause++;
		}
	}

	//cursor in range of levels button
	if (((mouseX >= LEVELSbuttonLeft) && (mouseX <= LEVELSbuttonRight)) && ((mouseY >= LEVELSbuttonTop) && (mouseY <= LEVELSbuttonBottom)))
	{
		//hover effect
		CP_Image_Draw(levels2, centerXbuttonLEVELS, centerYbuttonLEVELS, halfX * 2, halfY * 2, 255);

		//launch level select screen
		if (CP_Input_MouseClicked())
		{
			CP_Engine_SetNextGameState(selection_init, selection_update, selection_exit);
		}
	}

	//cursor in range of resume button
	else if (((mouseX >= CONTbuttonLeft) && (mouseX <= CONTbuttonRight)) && ((mouseY >= CONTbuttonTop) && (mouseY <= CONTbuttonBottom)))
	{
		//hover effect
		CP_Image_Draw(resume2, centerXbuttonCONT, centerYbuttonCONT, halfX * 2, halfY * 2, 255);

		//unpause game
		if (CP_Input_MouseClicked())
		{
			//change pause state
			pause = 2;
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

void game_update(void)
{
	CP_Settings_Background(CP_Color_Create(0, 0, 0, 255));
	CP_Image_Draw(bg, (float)window_width / 2, (float)window_height / 2, (float)window_width, (float)window_height, 255);

	//activate pause
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		++pause;
	}
	//if pause
	if (pause == 3)		//paused
	{
		CP_Sound_PauseAll();
		paused();
	}
	else
	{
		CP_Sound_ResumeAll();
		move();
		draw();
		border();
		pause = 2;
	}

}

/*same as other game1 and game2 comments*/
void game_exit(void)
{
	CP_Image_Free(&sprite);
	CP_Image_Free(&docIdle);
	CP_Image_Free(&docWalk1);
	CP_Image_Free(&docWalk2);
	CP_Image_Free(&pushLeft1);
	CP_Image_Free(&pushLeft2);
	CP_Image_Free(&keycard);
	CP_Image_Free(&floor);
	CP_Image_Free(&roof);
	CP_Image_Free(&bg);
	CP_Image_Free(&doors);
	CP_Image_Free(&walls);
	CP_Image_Free(&bluespeechbubble);
	CP_Image_Free(&redspeechbubble);
	CP_Image_Free(&keyspeechbubble);
	CP_Image_Free(&movespeechbubble);
	CP_Image_Free(&pausespeechbubble);

	CP_Sound_Free(bgm);
	CP_Sound_Free(timerbeep);

	CP_Image_Free(&pausescreen);
	CP_Image_Free(&restart1);
	CP_Image_Free(&restart2);
	CP_Image_Free(&levels1);
	CP_Image_Free(&levels2);
	CP_Image_Free(&resume1);
	CP_Image_Free(&resume2);
	CP_Image_Free(&quit1);
	CP_Image_Free(&quit2);

	jump = FALSE;
	gravity = FALSE;
	stand = TRUE;
	state = BOTTOM;
	coll = FALSE;

	xspeed = 1;
	checkinit = 0;
	keytaken = 0;
	bpill = 0;
	rpill = 0;
	bluetime = 0;
	ttime = 0;
	gravitytimer = 7.5f;
	rp = 0;
}