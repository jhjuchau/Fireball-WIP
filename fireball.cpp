//Written by James Juchau
//Between Sunday, August 30th and
//Fireball 

//Fireball is a game my friends and I came up with in elementary school. It was a way to incorporate a little more skill and psychology into Rock, Paper, Scissors. 
//My brothers and I still use this game to determine who has to help Mom when she's bringing in groceries.

//The rules are pretty straightforward. The legal actions are 'block', 'charge' and 'fireball'. You can't throw a 'fireball' unless you have at least one 'charge'. You can have more than one charge at a time. 
//You hit your opponent by throwing a fireball when your opponent choses to charge. If both players throw fireballs, both players are safe, but both players still lose a charge. Choosing to block protects you from a fireball.
//You can only block twice in a row. For the purposes of this program, three blocks in a row won't be allowed. In the real world, the third block means groceries. This is in place to avoid stalemates and stalling.
//Originally, the game ended on the first 'hit'; the player who throws a fireball while the other is charging is the winner.
//However, to avoid anticlimactic games, I've increased the amount of hits each player can take to 3, 5 or 7, depending on player input
//The games may be longer, but I feel this will make them more enjoyable.

//While this game doesn't eliminate guessing and luck entirely, 'Fireball' games between thoughtful players become intense, fun, and give a lot of insight into a person's risk-taking inclinations.

#include <iostream>		//mainly for cout
#include <conio.h>		//for _getch()
#include <Windows.h>	//mainly for system("CLS"), among other functions
#include <iomanip>		//for setprecision() on the results screen
#include <string>		//for display() to work with coutPastMoves() more easily
#include "MenuInterface.h"



using namespace std;

//These are the numerical values returned when the labelled key is pressed while _getch() is active.
//For example, if c=_getch(), and the next keystroke entered is the down arrow key, c is assigned the value of 80.
//All these keys are used within normal program function.

#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKEY 77
#define ENTER 13
#define SPACE 32
#define ESC 27

#define AKEY 97
#define SKEY 115
#define DKEY 100
#define JKEY 106
#define KKEY 107
#define LKEY 108

void gameLoop(double&, double&, int, string[]);		//this function calls all the necessary game functions in the proper order
void display(int, int, int, int, int[], int[], int, string[]);	//reads out impoortant game data, such as current life, current charges, and last move used
void collectInput(int&, int&, int, int, int[], int[]);	//collects, validates, and ships player input back to gameLoop()
boolean analyzePlay(int, int, int&, int&, int&, int&);	//determines what happened with the player input, changes player values, and determines whether or not the game is over
void resultsAndRetry(double, double, int, string[]);	//displays both players' win rates and offers to restart the game
void push(int, int[2]);	//standard array push function to help store each player's previous two moves
string coutPastMove(int);	//calls out a player's previous move based on the integer value of the move. used to make display() less verbose

int main()
{
	string playerNames[2] = { "Player 1", "Player 2" };
	int lives = mMenu(playerNames);	//[0] is player 1's name, [1] is player 2's name, mMenu returns # of lives
	if (lives == 0){ return 0; }
	double p1wins = 0, p2wins = 0;		//for keeping score on the results screen
	cout << "Loading..." << endl;
	system("CLS");
	gameLoop(p1wins, p2wins, lives, playerNames);

	return 0;
}

void gameLoop(double& p1wins, double& p2wins, int lives, string playerNames[2])
{
	boolean ongoing = true;	//boolean sentinel to detect when the game is over
	int p1move = 0, p2move = 0;			  //1 - charge; 2 - block; 3 - fireball
	int p1charge = 0, p2charge = 0; //counts the number of 'charges' each player has stored
	int p1life = lives, p2life = lives;     //keeps track of the life the players have
	int p1PastMoves[2] = { 0, 0 }, p2PastMoves[2] = { 0, 0 };	//two small arrays that track each players' previous two moves
																//used to prevent players from blocking three times in a row
																//also used in display() to show each players previous move
																//initialized to {0,0} to simplify how display() reads these arrays
	
	while (ongoing) //the following function calls loop while the game hasn't ended
	{
		display(p1charge, p2charge, p1life, p2life, p1PastMoves, p2PastMoves, lives, playerNames);	//displays controls and current stats
		collectInput(p1move, p2move, p1charge, p2charge, p1PastMoves, p2PastMoves); //loops until both players have entered valid input
		ongoing = analyzePlay(p1move, p2move, p1charge, p2charge, p1life, p2life);	//calculates the results of each play and sets 'ongoing' to false if the game is over, ending the loop
		
	}//close while(ongoing) loop
	if (p1life == 0){ p2wins++; }	//only one point of damage is possible per turn in fireball. therefore, it is impossible that both players reach 0 life simultaneously. therefore, the order of placement of these
									//win conditions is irrelevant, as the proper player will win in normal play.
	else {p1wins++;}
	resultsAndRetry(p1wins, p2wins, lives, playerNames);
}

void display(int p1charge, int p2charge, int p1life, int p2life, int p1PastMoves[], int p2PastMoves[], int lives, string playerNames[])
{
	cout << playerNames[0]<<":\t\t\t\t\t"<<playerNames[1]<<":" << endl;
	cout << "A=Charge\t\t\t\t\tJ=Charge" << endl;
	cout << "S=Block\t\t\t\t\t\tK=Block" << endl;
	cout << "D=Fireball\t\t\t\t\tL=Fireball" << endl << endl;
	cout << "Life: " << p1life << "/" << lives;
	cout << "\t\t\t\t\tLife: " << p2life << "/" <<lives<< endl;
	cout << "Charges: " << p1charge;
	cout << "\t\t\t\t\tCharges: " << p2charge << endl << endl;

	if (p1PastMoves[0] != 0)	//this if statement is here to prevent the game from reading this message on the first turn
								//on every following turn, p1PastMoves[0] will always contain a nonzero value
	{
		cout << "On the previous turn..." << endl;
		cout << "Player 1 ";
		cout << coutPastMove(p1PastMoves[0]) << endl;	//p1PastMoves[0] is Player 1's previous move
		//coutPastMove() translates the integer value of the move into an in-game message for the players
		cout << "Player 2 ";
		cout << coutPastMove(p2PastMoves[0]) << endl;
	}

	cout << "TAP A KEY TWICE TO LOCK IN YOUR SELECTION!" << endl;
}

string coutPastMove(int in)
{
	switch (in)
	{
	case 1: 
		return "charged up!";
		break;
	case 2: 
		return "chose to block!";
		break;
	case 3:
		return "threw a fireball!";
		break;
	default:
		return "did nothing! (this should never happen)";
		break;
	}

}

void collectInput(int& p1move, int& p2move, int p1charge, int p2charge, int p1PastMoves[], int p2PastMoves[])
{
	p1move = 0; p2move = 0; //wiping move variables clean for a new turn


	while (p1move<= 0 || p2move<= 0) //this while loop will continue until both players have entered valid input, as explained below
	{
		//The way I programmed this section of the game is a little confusing if you just read it, so I'll break it down.
		//p1move and p2move are integers.	1 corresponds to the move "charge"
		//									2 corresponds to the move "block"
		//									3 corresponds to the move "fireball
		//I wanted to have some kind of confirmation system to prevent players from accidentally selecting the wrong choice. My initial thought was to have both players input a move, have a timer tick down from three,
		//and if either player hit the spacebar during this countdown, the moves would be reset and input would be collected again. Unfortunately, I couldn't find a way to collect input while a timer was ticking down,
		//as the only real-time input collection methods I'm familiar with (cin, getline(),  and _getch()) all stop the program in progress and wait specifically for input.

		//Therefore, to help ensure players get the input they intend, I implemented a double-tapping confirmation method. You have to press the same key twice in a row to lock in your action.
		//To detect if the previous player input was the same key, I made the first tap assign the negative value of the option they're considering.
		//									-1 corresponds to 'considering' charge
		//									-2 corresponds to 'considering' block
		//									-3 corresponds to 'considering' fireball

		//Therefore, if player one hits 'S' for the first time, the p1move value will chang to -2, considering block.
		//If player one hits 'D" after that, the p1move value will change to -3, considering fireball.
		//If player one hits 'D' again after that, the p1move value will change to 3, and the player will be locked into performing fireball.

		//I chose to use negatives to simplify verification of locking in a move. I can simply check if(p1move>0) to see if they've locked in a move, and prevent further change.
		//It also help with the loop that governs this whole process: while (p1move<= 0 || p2move<= 0)
		//		because the default value (0), and the 'considering' values are negative, the loop will continue while both players are either considering or have not made any decision.
		//		The loop will only end if both players have double-tapped an option.

		//Currently, even if a player has double-tapped an option, they have the option of switching. I'm trying to decide whether that functionality is conducive to competition or unnecessary and limiting.
		//Once both players have double-tapped an option, however, the loop ends and both players' moves will be passed into analyzePlay()


		int c; // integer input variable

		switch (c = _getch())		  //a switch block pivoting on user input
		{				
		case AKEY:			//If the A key is pressed, meaning Player 1 selected 'Charge'
			if (p1move == -1) //if p1move was already -1, which could only happen if the immediately prior p1 input was also 'A'...
			{
				cout << "Player 1 move selected!" << endl; //confirm to the players that player 1 has double-tapped their input
				p1move = 1;	//assign p1move the value of positive 1. the input collection loop only breaks when p1move and p2move are both positive, meaning both players have double-tapped to confirm their move.
			}
			else //if p1move was anything other than -1, meaning the user 
				//NOTE: since, in this build, you CAN switch moves after double tapping, this case could still occur if this is the third, fifth, seventh (etc etc etc) input of the same key.
				//if a player double taps a selection, then presses the same key again, their 'confirmation' is undone and they have to press the key one more time to confirm again
				//I could, and probably will, fix this so that subsequent presses of the same key after confirmation don't change the value of p1move, but it's not high on my list of priorities 
			{
				cout << "Player 1 is thinking...!" << endl;
				
				
				p1move = -1; // p1move is assigned -1. if the next player one input (A, S or D) is 'A', p1move will be assigned 1 (charge)
			}
				break;


		case SKEY:	//Player 1 selects 'block'
			if (p1PastMoves[0] == 2 && p1PastMoves[1] == 2)	//if Player 1's previous two moves were also block...
			{
				cout << "You cannot block three times in a row!" << endl;
			}
			else if (p1move == -2)
			{
				cout << "Player 1 move selected!" << endl;
				p1move = 2;
			}
			else
			{
				cout << "Player 1 is thinking...!" << endl;
				p1move = -2;
			}
			break;

		case DKEY:		//player 1 selects 'fireball'
			if (p1move == -3 && p1charge > 0) //if player 2 double-taps 'fireball' and has one or more charges
			{
				cout << "Player 1 move selected!" << endl;
				p1move = 3;
			}
			else if (p1move == -3 && p1charge <= 0)	//if the player chooses 'fireball' but does not have a charge
			{
				cout << "No charges!" << endl;
			}
			else
			{
				cout << "Player 1 is thinking...!" << endl;
				p1move = -3;
			}
			break;


			//player 2 options
		case JKEY:
			if (p2move == -1)
			{
				cout << "\t\t\t\t\t\tPlayer 2 move selected!" << endl;
				p2move = 1;
			}
			else
			{
				cout << "\t\t\t\t\t\tPlayer 2 is thinking...!" << endl;
				p2move = -1;
			}
			
			break;
			
		case KKEY:	//if Player 2 selects 'block'...
			if (p2PastMoves[0] == 2 && p2PastMoves[1] == 2)
			{
				cout << "\t\t\t\t\t\tYou cannot block three times in a row!" << endl;
			}
			else if (p2move == -2)
			{
				cout << "\t\t\t\t\t\tPlayer 2 move selected!" << endl;
				p2move = 2;
			}
			else
			{
				cout << "\t\t\t\t\t\tPlayer 2 is thinking...!" << endl;
				p2move = -2;
			}
			
			break;

		case LKEY:		//player 2 selects 'fireball'
			if (p2move == -3 && p2charge > 0) //if player 2 double-taps 'fireball' and has one or more charges
			{
				cout << "\t\t\t\t\t\tPlayer 2 move selected!" << endl;
				p2move = 3;
			}
			else if (p2move == -3 && p2charge <= 0)  //if the player chooses 'fireball' but does not have a charge
			{
				cout << "\t\t\t\t\t\tNo charges!" << endl;
			}
			else
			{
				cout << "\t\t\t\t\t\tPlayer 2 is thinking...!" << endl;
				p2move = -3;
			}
			break;
			


		}//close switch

		
	}//close while(p1move == 0 && p2move == 0) loop
	push(p1move, p1PastMoves);
	push(p2move, p2PastMoves);
	system("CLS");
}

boolean analyzePlay(int p1move, int p2move, int& p1charge, int& p2charge, int& p1life, int& p2life)
{
	//									1 corresponds to the move "charge"
	//									2 corresponds to the move "block"
	//									3 corresponds to the move "fireball



	boolean p1safe = false, p2safe = false;	//this will simplify damage calculation later down the line

	if (p1move == 2 && p2move == 2)
	{
		cout << "Both players choose to block!" << endl;
	}
	//the following two if blocks call out if a player charged this turn, and increment the corresponding player's charge count
	if (p1move == 1)
	{
		cout << "Player 1 charges up!" << endl;
		p1charge++; 
		Sleep(1000);		//for dramatic tension
		
		//every situation but one player charging and the other blocking is covered by the if(move==3) blocks below, so I wrote a special case
		if (p2move == 2)
		{
			cout << "Player 2 chooses to block nothing!" << endl;
		}
	}

	if (p2move == 1)
	{
		cout << "Player 2 charges up!" << endl;
		p2charge++; 
		Sleep(1000);		//for dramatic tension
		if (p1move == 1){ cout << "Both players choose to charge!!" << endl; }	//I chose to put the 'Both players' prompt down here so that the order would be preserved, as follows:
																				//Player 1 charges up!
																				//Player 2 charges up!
																				//Both players choose to charge!!

		if (p1move == 2){ cout << "Player 1 gets ready to block, but the attack never comes!" << endl; }
	}



	if (p1move == 3)		//if player 1 chooses to throw a fireball
	{
		cout << "Player 1 throws a fireball!" << endl; //callout
		p1charge--;			//take off a charge before doing anything
		Sleep(1000);		//for dramatic tension
			if (p2move == 1)	//if player 2 was charging
			{
				cout << "Player 2 gets hit!" << endl; 
				p2life--;		//take a point off of player 2's life
			}

			if (p2move == 2)	//if player 2 was blocking
			{
				cout << "Player 2 skillfully blocks it!" << endl;
				//no damage dealt to player 2
			}

				//I specifically didn't put in the interaction between two fireballs here, to preserve the order of things.
	}

	if (p2move == 3)		//if player 2 chooses to throw a fireball
	{
		cout << "Player 2 throws a fireball!" << endl;
		p2charge--;
			if (p1move == 1) //if player 1 was charging
			{
				cout << "Player 1 is hit!" << endl;
				p1life--;		//take a point off of player 1's life
			}

			if (p1move == 2)	//if player 1 was blocking
			{
				cout << "Player 1 successfully blocks it!" << endl;
				//no damage dealt to player 2
			}

			if (p1move == 3)	//interaction between two fireballs coded here 
			{	
				cout << "The two fireballs collide! No damage!" << endl;
			}
	}

	system("pause");
	cout << endl;

	if (p1life <= 0){ cout << "Player 2 wins!" << endl; return false; }
	if (p2life <= 0){ cout << "Player 1 wins!" << endl; return false; }
	return true;
}

void resultsAndRetry(double p1wins, double p2wins, int lives, string playerNames[2])
{
	int total = p1wins + p2wins;	//there are no tie games in fireball, so adding how many each player has won gives you the total number of games played
	cout << "Player 1: " <<  p1wins << "/" << total;
	cout << "  ("  << ((p1wins / total) * 100.00)<< "%)" << endl;
	cout << "Player 2: " << p2wins << "/" << total;
	cout << "  ("  << ((p2wins / total) * 100.00) << "%)" << endl;

	cout << "Press ESC to quit, SPACEBAR to return to the main menu, or another key to retry." << endl;
	int c = _getch();
	if (c == ESC){ return; }
	else if (c == SPACE){ main(); }
	else { gameLoop(p1wins, p2wins, lives, playerNames); }
}

void push(int currentMove, int pastMoves[2])	
{
	//This function maintains the pastMoves queue. This queue is used to prevent players from blocking three times in a row
	//pastMoves[] also is used in the display() function to inform both players of their last two moves
	if (pastMoves[0] == 0){ pastMoves[0] = currentMove; return; }
	else 
	{ 
		pastMoves[1] = pastMoves[0];
		pastMoves[0] = currentMove;
	}
}