#pragma once
#include<iostream>
using namespace std;
enum SUIT
{
	HEA, CLU, DIA, SPA
// ºìÌÒ,Ã·»¨,·½¿é£¬ºÚÌÒ
};

enum CARDNUM
{
	A,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	NUM_10,
	JACK,
	QUEEN,
	KING
};

struct Poker
{
	SUIT suit;
	CARDNUM cnum;
	bool on;

	Poker(SUIT s = HEA, CARDNUM c = A, bool On = 0) :suit(s), cnum(c), on(On) {}

	ostream& output(ostream& out) const;
};

ostream& Poker::output(ostream& out) const
{
	if (on == 0)
	{
		out << "##";
	}
	else
	{
		switch (suit)
		{
		case HEA:	out << "H";	break;
		case CLU:	out << "C";	break;
		case DIA:	out << "D";	break;
		case SPA:	out << "S";	break;
		}
		switch (cnum)
		{
		case NUM_2:
			out << "2";
			break;
		case NUM_3:
			out << "3";
			break;
		case NUM_4:
			out << "4";
			break;
		case NUM_5:
			out << "5";
			break;
		case NUM_6:
			out << "6";
			break;
		case NUM_7:
			out << "7";
			break;
		case NUM_8:
			out << "8";
			break;
		case NUM_9:
			out << "9";
			break;
		case NUM_10:
			out << "T";
			break;
		case JACK:
			out << "J";
			break;
		case QUEEN:
			out << "Q";
			break;
		case KING:
			out << "K";
			break;
		case A:
			out << "A";
			break;
		}
	}
	return out;
}

ostream& operator<<(ostream& out, const Poker& p)
{
	p.output(out);
	return out;
}