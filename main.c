#include "MyLib.h"
#include "tardis.h"



int main() {

	REG_DISPCNT = MODE_3 | BG2_EN;

	enum GBAState state = START;


	//initialize variables
	int prevButtonPressed = 0;
	int canMove = 0;
	int gameoverFlag = 0;
	//setup player + enemies
	ENEMY objs[ENEMYCOUNT];
	ENEMY oldobjs[ENEMYCOUNT];
	ENEMY *cur = objs;
	char str[30] = "Col reached: 0/240";
	

	while(1) {
		prevButtonPressed = getButtonPressed(); //get button that user is pressing if any
		waitForVblank();
		switch (state) {
		case START:
			setupStart(objs, oldobjs, gameoverFlag); //initialize values for player + enemies
			state = START_NODRAW;
			gameoverFlag = 0;
			break;
		case START_NODRAW:
			if (prevButtonPressed == 1 && canMove == 0) {
				state = INSTRUCTIONS;
				canMove = 1;
			} //move to next state when start is pressed

			//reset game if select is pressed
			if (prevButtonPressed == 7) {
				state = START;
			}
			break;
	
		case INSTRUCTIONS:
			setupInstructions(); //display instructions to user
			state = INSTRUCTIONS_NODRAW;
			break;
		case INSTRUCTIONS_NODRAW:
			if (prevButtonPressed == 1 && canMove == 0) {
				state = STATE_GAME;
				canMove = 1;
			} //move to game if start is pressed

			//reset game if select is pressed
			if (prevButtonPressed == 7) {
				state = START;
			}

			break;


		case STATE_GAME:
			fillScreen(WHITE); //make screen white
			state = GAME_NODRAW;
			break;

		case GAME_NODRAW:
			//reset if select is pressed
			if (prevButtonPressed == 7) {
				state = START;
			}

			setupGame(objs, cur, str); //set up initial positons for all entities
			waitForVblank();
			drawObjects(objs, cur, oldobjs); //draw all entities
			moveObjects(prevButtonPressed, cur, objs, oldobjs); //move player

			//check if collision between player and enemy or player and endzone
			if (checkCollision(objs) == 1) {
				state = GAMEOVER; //if collision with enemy, go to gameover
			} else if(checkCollision(objs) == 2) {
				state = GAMEOVER;
				gameoverFlag++; //if collision with TARDIS, go to gameover with flag incremented to indicate win
			}

		break;

		case GAMEOVER:
			setupEnd();
			if (gameoverFlag == 0) { //check if player lost or won and go to appropriate state
				state = GAMEOVER_LOSE;
			} else {
				state = GAMEOVER_WIN;
			}
			break;
		case GAMEOVER_LOSE:
			setupLose(str); //draw game over image and draw "Good Try" string with how far player got
			//if select is pressed, restart game
			if (prevButtonPressed == 7) {
				state = START;
			}
			break;
			
		case GAMEOVER_WIN:
			setupWin(str); //draw game over image and draw "You Win" string with how far player got
			//if select is pressed, restart game
			if (prevButtonPressed == 7) {
				state = START;
			}
			break;
		}

		//reset canMove flag
		if (!KEY_DOWN_NOW(BUTTON_START)) {
			canMove = 0;
		}
		
	}
	return 0;
}


