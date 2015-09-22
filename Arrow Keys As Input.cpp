#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

#define UKEY 72
#define DKEY 80
#define LKEY 75
#define RKEY 77
#define ENTER 13

void displayList(boolean[][4]);
void setBooleanArray(int, int&, int&, int, int, boolean[][4]);
void initArray(boolean[][4]);

int main()
{
	int c = 0, currentSelection = 1;
	boolean pos[4][4];
	initArray(pos);
	pos[1][1] = true; //cursor starts on the option in the upper left corner
	int x = 1, y = 1;
	displayList(pos);
	while (1)
	{
		c = 0;
		
		switch (c = _getch())
		{
			
		case UKEY:
			system("CLS");
			setBooleanArray(c, x, y, 3, 2, pos);
			displayList(pos);
			break;

		case DKEY:
			system("CLS");
			setBooleanArray(c, x, y, 3, 2, pos);
			displayList(pos);
			break;

		case LKEY:
			system("CLS");
			setBooleanArray(c, x, y, 3, 2, pos);
			displayList(pos);
			break;

		case RKEY:
			system("CLS");
			setBooleanArray(c, x, y, 3, 2, pos);
			displayList(pos);
			break;

		case ENTER:
			break;

		default:
			cout << c << endl;
		}
	}
}



void setBooleanArray(int c, int& x, int& y, int xmax, int ymax, boolean pos[][4]){
	pos[x][y] = false;
	switch (c)
	{
	case UKEY: //on a press of the up key
		y--;
		break;

	case DKEY: //on a press of the up key
		y++;
		break;

	case LKEY:
		x--;
		break;

	case RKEY:
		x++;
		break;
	}


		if (y < 1){ y = (ymax); } //loop around to the other side.
		if (x < 1){ x = (xmax); }
		if (y > ymax){ y = 1; }
		if (x > xmax){ x = 1; }

		pos[x][y] = true;
		
	
}


	void displayList(boolean pos[][4]){

		if (pos[1][1]){ cout << "->"; }
		else{ cout << "  "; }
		cout << "1. Do this"; cout <<"\t\t";

		if (pos[2][1]){ cout << "->"; }
		else{ cout << "  "; }
		cout << "2. Do that"; cout << "\t\t";

		if (pos[3][1]){ cout << "->"; }
		else{ cout << "  "; }
		cout << "3. Do this other thing" <<  endl;

		if (pos[1][2]){ cout << "->"; }
		else{ cout << "  "; }
		cout << "4. Do one of these"; cout << "\t";


		if (pos[2][2]){ cout << "->"; }
		else{ cout << "  "; }
		cout << "5. Do one of those"; cout << "\t";

		if (pos[3][2]){ cout << "->"; }
		else{ cout << "  "; }
		cout << "6. Do this other thing" << endl << endl;
	}

	void initArray(boolean pos[][4]){
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				pos[i][j] = false;
			}
		}

		pos[1][1] = true;
	}