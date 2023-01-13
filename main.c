#include <string.h>

#include <libgen.h>
#include <unistd.h>

#include "ANZ_Image.h"
#include "AbstractEntity.h"

#include "snake.h"
#include "game_gui.h"
#include "level.h"
#include "apple.h"
#include "spider.h"

#undef main

int loadAllAssets(/*char* levelName,SnakeLevel*level*/)
{
    int r=0;

    r += !Snake_loadAssets();
    r += !gameGUI_loadAssets();
    /*r += !SnakeLevel_loadFromFile(level,levelName);*/
    r += !Apple_loadAssets();
    r += !Spider_loadAssets();

    return !r;
}
void unloadAllAssets()
{
    Snake_unloadAssets();
    gameGUI_unloadAssets();
    /*SnakeLevel_free(&snakeLevel);*/
    Apple_unloadAssets();
    Spider_unloadAssets();
}

typedef enum
{
    GAME_ACTIVE = 0,
    GAME_OVER = 1,
    GAME_OVER_T=2,
    GAME_QUIT = 3,
    PROGRAM_QUIT = 4

}GameStatus;

GameStatus SnakeGameMain(int snakeVel,SnakeLevel*level)
{
    GameStatus gameStatus = GAME_ACTIVE;

    AbstractEntity* entitiesS = Snake_create(level->startX,level->startY,snakeVel,0);
    SnakeLevel_startSpawn(level,entitiesS);


    SDL_Event event;
    int renderScore=1,paused=0,tGameOver=0;
    srand(SDL_GetTicks());


    while(gameStatus < GAME_QUIT)
    {
        while(SDL_PollEvent(&event)>0)
        {
            if(event.type==SDL_QUIT)
            {
                gameStatus = PROGRAM_QUIT;
            }
            else if(event.type==SDL_KEYDOWN&&event.key.repeat==0)
            {
                if(gameStatus == GAME_OVER)
                {
                    gameStatus = GAME_QUIT;
                }
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    gameStatus = PROGRAM_QUIT;
                    break;
                /*case SDLK_d:
                    gameStatus = GAME_OVER;
                    gameGUI_gameOver();
                    break;*/
                case SDLK_b:
                    printf("Banany\n");
                    break;
                case SDLK_F1:
                    renderScore=!renderScore;
                    break;
                case SDLK_p:
                    paused=!paused;
                    break;
                }
            }
        }
        SDL_SetRenderDrawColor(sdlrenderer,0,68,0,255);

        if((!paused) && (gameStatus==GAME_ACTIVE))
        {
            SnakeLevel_onUpdate(level,entitiesS);
            if(AbstractEntity_updateALL(entitiesS,&level->spriteMap))
            {
                printf("GAME OVER\n");
                gameGUI_gameOver();
                gameStatus = GAME_OVER_T;
            }
        }


        SDL_RenderClear(sdlrenderer);
        if(level->background!=0)
        {
            ANZ_RenderImage(level->background,0,0);
        }

        AbstractEntity_renderALL(entitiesS,0);
        AbstractEntity_renderALL(entitiesS,1);
        ANZ_RenderImage(level->image,0,0);
        AbstractEntity_renderALL(entitiesS,2);

        switch(gameStatus)
        {
        case GAME_OVER_T:
            if(tGameOver<30)
            {
                tGameOver++;
            }
            else
            {
                gameStatus=GAME_OVER;
            }
        case GAME_ACTIVE:
            if(renderScore)
            {
                gameGUI_renderScore(21,21);
            }
            if(paused)
            {
                gameGUI_renderPausedGame(300,300);
            }
            break;
        case GAME_OVER:
            gameGUI_renderGameOver(300,300);
            break;
        default:
            break;
        }
        SDL_RenderPresent(sdlrenderer);
    }
    AbstractEntity_deleteALL(entitiesS);
    printf("The end of the main game loop\n");
    return gameStatus;
}


int main(int argc,char**argv)
{
    char*levelName = "level0.sm";
    int i,snakeVelocity=4;
    SnakeLevel level;

    i = chdir(dirname(argv[0]));
    if(i != 0)
    {
        fprintf(stderr,"chdir error code %i\n",i);
        return -2;
    }

    for(i=1;i<argc;i++)
    {
        if(!strcmp(argv[i],"-level"))
        {
            i++;
            if(i<argc)
            {
                levelName =argv[i];
            }
        }
        else if(!strcmp(argv[i],"-vel"))
        {
            i++;
            if(i<argc)
            {
                if(sscanf(argv[i],"%i",&snakeVelocity) != 1 || snakeVelocity<1 || snakeVelocity>10)
                {
                    fprintf(stderr,"Snake velocity has to be an integer from interval [1,10]\n");
                    return -1;
                }
            }
        }
        else if(!strcmp(argv[i],"-v") || !strcmp(argv[i],"-version"))
        {
            printf("Wonsz1 version R1.3\n");
            return 0;
        }
        else if(!strcmp(argv[i],"-h") || !strcmp(argv[i],"-help") ||
                !strcmp(argv[i],"--h") || !strcmp(argv[i],"--help"))
        {
            FILE * file = fopen("assets/help.txt","r");
            char buffer[1024];
            if(file==0)
            {
                fprintf(stderr,"[Error] Missing help file\n");
            }
            else
            {
                while(!feof(file))
                {
                    if(fgets(buffer,1024,file)!=0)printf("%s",buffer);
                }
                fclose(file);
            }

            return 0;
        }
        else
        {
            fprintf(stderr,"[Warning] Skipping incorrect argument:\"%s\"\n",argv[i]);
        }
    }

    if(!initSDL())
    {
        return 1;
    }

    SDL_Window*window=SDL_CreateWindow("I am not a casual snake but I am a \"wonsz\". :-)",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1280,800,SDL_WINDOW_SHOWN);
    if(window==0)
    {
        fprintf(stderr, "Unable to create window:\nSDL_Error: %s\n",SDL_GetError());
        return 2;
    }

    sdlrenderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(sdlrenderer==0)
    {
        fprintf(stderr, "Unable to create renderer:\nSDL_Error: %s\n",SDL_GetError());
        return 3;
    }

    SDL_SetRenderDrawBlendMode(sdlrenderer, SDL_BLENDMODE_BLEND);

    int quit=0;

    ANZ_Image*startScreen=ANZ_LoadImage("assets/start_screen.png");
    if(!loadAllAssets() || startScreen==0)
    {
        fprintf(stderr,"Unable to load all assets\n");
        return 4;
    }

    if(!SnakeLevel_loadFromFile(&level,levelName))
    {
        fprintf(stderr,"Unable to load level: %s\n",levelName);
        return 5;
    }



    SDL_Event event;
    while(!quit)
    {
        SDL_RenderClear(sdlrenderer);
        ANZ_RenderImage(startScreen,0,0);
        SDL_RenderPresent(sdlrenderer);
        while(SDL_PollEvent(&event)>0)
        {
            if(event.type==SDL_QUIT)
            {
                quit=1;
            }
            else if(event.type==SDL_KEYDOWN&&event.key.repeat==0)
            {
                if(event.key.keysym.sym==SDLK_ESCAPE)
                {
                    quit=1;
                }
                else if(SnakeGameMain(snakeVelocity,&level)==PROGRAM_QUIT)
                {
                    quit=1;
                }
            }
        }
    }
    ANZ_FreeImage(startScreen);
    unloadAllAssets();
    SnakeLevel_free(&level);

    SDL_DestroyRenderer(sdlrenderer);
    SDL_DestroyWindow(window);
    quitSDL();


    return 0;
}
