
#include "FightGame.h"

int main(int argc, char* argv[])
{
	FightGame* fightGame = new FightGame();
	int retVal = fightGame->run();
	delete fightGame;
	return retVal;
}
