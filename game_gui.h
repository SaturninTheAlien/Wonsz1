#ifndef GAME_GUI_H_INCLUDED
#define GAME_GUI_H_INCLUDED
#include "ANZ_Image.h"

int gameGUI_loadAssets(void);
void gameGUI_unloadAssets(void);

void gameGUI_renderPausedGame(int posX,int posY);
void gameGUI_renderScore(int posX,int posY);
void gameGUI_renderGameOver(int posX,int posY);

void gameGUI_updateScore(int score);
void gameGUI_gameOver(void);

#endif
