#include "game_gui.h"

struct
{
    TTF_Font *font1;
    ANZ_ImageFrame frame1,frame2,frame3;
    ANZ_Image* pausedMessage,*scoreImage,*gom1,*gom2;
    int previousScore,maxScore;

}game_gui;


int gameGUI_loadAssets(void)
{

    /*paused game frame*/
    game_gui.frame1=ANZ_CreateImageFrame();

    game_gui.frame1.fillColor.r=255;
    game_gui.frame1.fillColor.g=255;
    game_gui.frame1.fillColor.b=0;
    game_gui.frame1.fillColor.a=128;

    game_gui.frame1.strokeColor.a=255;
    game_gui.frame1.strokeColor.r=255;
    game_gui.frame1.strokeColor.g=0;
    game_gui.frame1.strokeColor.b=0;

    /*game over frame*/
    game_gui.frame2=ANZ_CreateImageFrame();
    game_gui.frame2.fillColor.r=255;
    game_gui.frame2.fillColor.g=255;
    game_gui.frame2.fillColor.b=0;
    game_gui.frame2.fillColor.a=128;

    game_gui.frame2.strokeColor.a=255;
    game_gui.frame2.strokeColor.r=255;
    game_gui.frame2.strokeColor.g=0;
    game_gui.frame2.strokeColor.b=0;
    game_gui.frame2.h=75;

    /*score frame*/
    game_gui.frame3=ANZ_CreateImageFrame();
    game_gui.frame3.fillColor.r=0;
    game_gui.frame3.fillColor.g=0;
    game_gui.frame3.fillColor.b=255;
    game_gui.frame3.fillColor.a=128;

    game_gui.frame3.strokeColor.a=255;
    game_gui.frame3.strokeColor.r=255;
    game_gui.frame3.strokeColor.g=0;
    game_gui.frame3.strokeColor.b=0;


    game_gui.font1=TTF_OpenFont("assets/fonts/consolab.ttf",30);
    if(game_gui.font1==0)
    {
        fprintf(stderr,"SDL2_TTF_ERROR: %s\n",TTF_GetError());
        return 0;
    }

    game_gui.pausedMessage=ANZ_RenderTextToImage1("GAME PAUSED: press \"p\" to continue.",game_gui.font1,0,0,255,255);
    if(game_gui.pausedMessage==0)
    {
        return 0;
    }
    game_gui.scoreImage=0;
    game_gui.previousScore=-1;
    game_gui.maxScore=0;

    game_gui.gom1 = ANZ_RenderTextToImage1("GAME OVER: press any key to continue.",game_gui.font1,0,0,255,255);
    game_gui.gom2 = 0;


    return 1;
}

void gameGUI_unloadAssets(void)
{
    if(game_gui.scoreImage!=0)
    {
        ANZ_FreeImage(game_gui.scoreImage);
        game_gui.scoreImage=0;
    }

    if(game_gui.font1!=0)
    {
        TTF_CloseFont(game_gui.font1);
        game_gui.font1=0;
    }

    if(game_gui.gom1!=0)
    {
        ANZ_FreeImage(game_gui.gom1);
        game_gui.gom1=0;
    }

    if(game_gui.gom2!=0)
    {
        ANZ_FreeImage(game_gui.gom2);
        game_gui.gom2=0;
    }

    ANZ_FreeImage(game_gui.pausedMessage);
}

void gameGUI_updateScore(int score)
{
    if(game_gui.previousScore==score)return;

    if(score>game_gui.maxScore)
    {
        game_gui.maxScore=score;
    }

    game_gui.previousScore=score;

    char buffer[256];
    sprintf(buffer,"Length: %i",score);
    if(game_gui.scoreImage!=0)
    {
        ANZ_FreeImage(game_gui.scoreImage);
        game_gui.scoreImage=0;
    }
    game_gui.scoreImage=ANZ_RenderTextToImage1(buffer,game_gui.font1,255,255,0,255);
}
void gameGUI_gameOver(void)
{
    char buffer[256];
    sprintf(buffer,"Your recent length was %i / greatest %i.",game_gui.previousScore,game_gui.maxScore);

    if(game_gui.gom2!=0)
    {
        ANZ_FreeImage(game_gui.gom2);
        game_gui.gom2=0;
    }
    game_gui.gom2=ANZ_RenderTextToImage1(buffer,game_gui.font1,255,0,0,255);
    int i1=game_gui.gom2->width - game_gui.gom1->width;

    if(i1>0)
    {
        game_gui.frame2.w += i1;

    }
}



void gameGUI_renderScore(int posX,int posY)
{
    ANZ_RenderFramedImage(game_gui.scoreImage,posX,posY,&game_gui.frame3);
}
void gameGUI_renderPausedGame(int posX,int posY)
{
    ANZ_RenderFramedImage(game_gui.pausedMessage,posX,posY,&game_gui.frame1);
}
void gameGUI_renderGameOver(int posX,int posY)
{
    ANZ_RenderFramedImage(game_gui.gom1,posX,posY,&game_gui.frame2);
    ANZ_RenderImage(game_gui.gom2,posX+20,posY+65);
}
