#include "TuxedoCat.h"
#include <cctype>

using namespace TuxedoCat;

PieceColor TuxedoCat::GetPieceColorFromChar(char piece)
{
	PieceColor color;

	if (std::isupper(piece))
	{
		color = PieceColor::WHITE;
	}
	else
	{
		color = PieceColor::BLACK;
	}

	return color;
}