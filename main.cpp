#include <iostream>
#include "Game.h"


int main()
{
    //Init rand
    std::srand(static_cast<unsigned>(time(NULL)));

    //Init Game

    Game game;
    

    while (game.running() && !game.getEndGame()) 
    {
        


        game.update();

        game.render();

    }

    return 0;
}