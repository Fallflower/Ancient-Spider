#pragma once
#include "LinkList.h"
#include "Poker.h"
#include<random>
using namespace std;
constexpr auto CARD_LIST_SIZE = 10;
constexpr auto TOTAL_NUM = 104;


enum GAME_MODE
{
	SIMPLE, NORMAL, HARD
};



class Game
{
private:
	GAME_MODE Mode;
	int suit_size;
	SUIT* Types;
	LinkList<Poker>* Heads;
	int top;
	Poker* Pile;
	int Finished;

protected:
	bool __shuffle__();
	bool __init_by_mode__();

	bool __to_deal__(bool On);
	int __to_count__(const int& operated) const;
	Status __to_move__(const int& operated, const int& target, const int& start_row = -1);
	void __refresh__();
	bool __start_game__();



	void __clear_pile__();
	void __clear_heads__();

public:
	Game(GAME_MODE mode);
	~Game();

	void showPile() const;
	void show() const;
	void showMenuinGame() const;

	void toDeal();
	void toPlay();
};



inline bool Game::__shuffle__()
{
	random_device seed;//硬件生成随机数种子
	ranlux48 engine(seed());//利用种子生成随机数引擎
	uniform_int_distribution<> distrib(0, TOTAL_NUM - 1);//设置随机数范围，并为均匀分布
	for (int i = 0; i < TOTAL_NUM; i++)
	{
		int r;
		while ((r = distrib(engine)) == i);
		Swap(Pile[i], Pile[r]);
	}
	return 0;
}

inline bool Game::__init_by_mode__()
{
	if (Mode == SIMPLE)
	{
		suit_size = 1;
		Types = new SUIT[suit_size]{ CLU };
	}
	else if (Mode == NORMAL)
	{
		suit_size = 2;
		Types = new SUIT[suit_size]{ HEA, CLU };
	}
	else if (Mode == HARD)
	{
		suit_size = 4;
		Types = new SUIT[suit_size]{ HEA, CLU, DIA, SPA };
	}
	else
	{
		cerr << "Error: Invalid game mode" << endl;
		return 1;
	}
	Pile = new Poker[TOTAL_NUM];
	int	MODED = (TOTAL_NUM / suit_size);
	for (int i = 0; i < TOTAL_NUM; i++)
	{
		Pile[i] = Poker(Types[i / MODED], CARDNUM(i / 8), false);
	}
	top = TOTAL_NUM - 1;
	return 0;
}

inline bool Game::__to_deal__(bool On)
{
	if (top < 9) return true;
	for (int i = 0; i < CARD_LIST_SIZE; i++)
	{
		Pile[top].on = On;
		Heads[i].InsertElem(Pile[top--]);
	}
	return false;
}

inline int Game::__to_count__(const int& operated) const
{
	int count = 1;
	int len = Heads[operated].GetLength();
	Poker fP, lP;
	Heads[operated].GetElem(len, fP);
	for (int i = len - 1; i > 0; i--)
	{
		lP = fP;
		Heads[operated].GetElem(i, fP);
		if (fP.cnum - 1 == lP.cnum && fP.suit == lP.suit && fP.on)
			count++;
		else
			break;
	}
	return count;
}

inline Status Game::__to_move__(const int& operated, const int& target, const int& start_row)
{
	if (operated < 0 || operated >= CARD_LIST_SIZE) return RANGE_ERROR;
	if (target < 0 || target >= CARD_LIST_SIZE) return RANGE_ERROR;
	int count = __to_count__(operated);
	int s = start_row;
	if (start_row == -1)	s = Heads[operated].GetLength() - count + 1;
	if (s <= Heads[operated].GetLength() - count) return OBEY_RULE;
	Poker headPoker, endPoker;
	Status St;
	if (!Heads[target].IsEmpty())
	{
		if ((St = Heads[target].GetElem(Heads[target].GetLength(), endPoker)) != ENTRY_FOUND) return St;
		if ((St = Heads[operated].GetElem(s, headPoker)) != ENTRY_FOUND) return St;
		if (headPoker.cnum != endPoker.cnum - 1) return OBEY_RULE;
	}
	if ((St = Heads[operated].MovePart(Heads[target], s)) != SUCCESS) return St;
	return SUCCESS;
}

inline void Game::__refresh__()
{
	for (int i = 0; i < CARD_LIST_SIZE; i++)
	{
		int count = __to_count__(i);
		if (count == 13)
		{
			Heads[i].DeletePart();
			Finished++;
		}
		int len = Heads[i].GetLength();
		Poker lastPoker; Status St;
		St = Heads[i].GetElem(len, lastPoker);
		if (lastPoker.on == false)
		{
			lastPoker.on = true;
			Heads[i].SetElem(len, lastPoker);
		}
	}
}


inline bool Game::__start_game__()
{
	for (int i = 0; i < 4; i++)
		if(__to_deal__(0)) return true;
	for (int i = 0; i < 4; i++)
	{
		Pile[top].on = 0;
		Heads[i].InsertElem(Pile[top--]);
	}
	if (__to_deal__(1)) return true;
	Finished = 0;
	return false;
}

inline void Game::__clear_pile__()
{
	delete[] Pile;
	top = 0;
	delete[] Types;
	suit_size = 0;
}

inline void Game::__clear_heads__()
{
	for (int i = 0; i < CARD_LIST_SIZE; i++)
	{
		Heads[i].Clear();
	}
}

inline Game::Game(GAME_MODE mode)
{
	Mode = mode;
	Heads = new LinkList<Poker>[CARD_LIST_SIZE];
	if (__init_by_mode__()) return;
	if (__shuffle__()) return;
	if (__start_game__()) return;
	cout << "\n==========================================" << endl;
	cout << "== Game init SUCCESS! Game mode: ";
	switch (Mode)
	{
	case SIMPLE:
		cout << "SIMPLE";
		break;
	case NORMAL:
		cout << "NORMAL";
		break;
	case HARD:
		cout << "!HARD!";
		break;
	}
	cout << " ==" << endl;
	cout << "==========================================\n" << endl;
	// system("pause");
	toPlay();
}

inline Game::~Game()
{
	//__clear_heads__();
	delete[] Heads;
	__clear_pile__();
}

inline void Game::showPile() const
{
	for (int i = 0; i < TOTAL_NUM; i++)
	{
		cout << Pile[i] << " ";
	}
	cout << endl;
}

inline void Game::show() const
{
	// LinkList<Poker>* ps = new LinkList<Poker>[CARD_LIST_SIZE];
	cout << "---------------------------------------------------------------------------------------\n\t" << flush;
	for (int i = 0; i < CARD_LIST_SIZE; i++)
		cout << "Col " << (i + 1) % 10 << "\t";
	cout << "\n\t" << flush;
	for (int i = 0; i < CARD_LIST_SIZE;i++)
		cout << "  ↓\t";
	cout << endl;
	for (int i = 1; i <= TOTAL_NUM; i++)
	{
		int status = 0;
		cout << "Row " << i << ":\t";
		for (int j = 0; j < CARD_LIST_SIZE; j++)
		{
			if (i >= Heads[j].GetLength())
				status++;
			if (i > Heads[j].GetLength())
				cout << "\t";
			else
			{
				Poker t;
				Heads[j].GetElem(i, t);
				cout << "  " << t << "\t";
			}
		}
		cout << endl;
		if (status >= 10) break;
	}
	cout << "------------                                                                -----------" << endl;
	cout << "Left Deal: " << (top + 1) / CARD_LIST_SIZE;
	cout << "                                                                Finished: " << Finished << endl;
	cout << "---------------------------------------------------------------------------------------" << endl;
}

inline void Game::showMenuinGame() const
{
	cout << "--- 1. move ---" << endl;
	cout << "--- 2. deal ---" << endl;
	cout << "---------------" << endl;
}


inline void Game::toDeal()
{
	if (__to_deal__(1)) cerr << "Fail to deal" << endl;
}

inline void Game::toPlay()
{
	while (true)
	{
		this->show();
		showMenuinGame();
		int k = Choice("Please choose: ", "12");
		switch (k - 48)
		{
		case 1:
		{
			int o, t, s = -1;
			char c; int i = 0;
			while (1)
			{
				c = GetChar(cin);
				if ((i == 2 && c == '\n') || i >= 3) break;// 输入两个或三个数后停止
				if (c == '.') c = '\x3A';
				if (i == 0) o = (int)c - 48;
				if (i == 1) t = (int)c - 48;
				if (i == 2) s = (int)c - 48;
				i++;
			}
			//cout  << "ots" << o << t << s << endl;
			switch (__to_move__(o - 1, t - 1, s))
			{
			case SUCCESS:
				break;
			case OBEY_RULE:
				cout << "Operation Against the Rule!" << endl;
				break;
			default:
				cerr << "ERROR!" << endl;
				break;
			}
			break;
		}
		case 2: toDeal(); break;
		}
		__refresh__();
		if (Finished == 8)
		{
			cout << "================================" << endl;
			cout << "=        Congratulations!      =" << endl;
			cout << "= U WON THE GAME, MODE: ";
			switch (Mode)
			{
			case SIMPLE:
				cout << "SIMPLE";
				break;
			case NORMAL:
				cout << "NORMAL";
				break;
			case HARD:
				cout << "!HARD!";
				break;
			}
			cout << " =" << endl;
			cout << "================================" << endl;
			break;
		}
	}
}
