#include "Game.h"
using namespace std;

void showChooseMode()
{
	cout << "==========================" << endl;
	cout << "==\tSIMPLE  (1)\t==" << endl;
	cout << "==\tNORMAL  (2)\t==" << endl;
	cout << "==\t!HARD!  (3)\t==" << endl;
	cout << "==\tQUIT  [Esc]\t==" << endl;
	cout << "==========================" << endl;
}

int main()
{
	while (true)
	{
		int k;
		showChooseMode();
		k = Choice("Please choose:", "123\x1b");
		GAME_MODE m = SIMPLE;
		switch (k-48)
		{
		case 1: m = SIMPLE; break;
		case 2: m = NORMAL; break;
		case 3: m = HARD; break;
		case 0:
		case -21: cout << "\nTHANKS FOR USING!" << endl; return 0;
		}
		Game g(m);
		g.toPlay();
	}
}