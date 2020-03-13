#include "spider.h"
#include "snake.h"
#define TEXTURES_NUMBER 4

struct
{
    ANZ_Image* images[TEXTURES_NUMBER];
    ANZ_SpriteMap sprites[TEXTURES_NUMBER];
}Spider_data;

int Spider_loadAssets(void)
{
    int i,r=0;
    Spider_data.images[0]=ANZ_LoadImageAndSpriteMap("assets/spider/pajur0.png",&Spider_data.sprites[0]);
    Spider_data.images[1]=ANZ_LoadImageAndSpriteMap("assets/spider/pajur1.png",&Spider_data.sprites[1]);
    Spider_data.images[2]=ANZ_LoadImageAndSpriteMap("assets/spider/pajur2.png",&Spider_data.sprites[2]);
    Spider_data.images[3]=ANZ_LoadImageAndSpriteMap("assets/spider/pajur3.png",&Spider_data.sprites[3]);
    for(i=0;i<TEXTURES_NUMBER;i++)
    {
        r+=Spider_data.images[i]!=0;
    }
    return r;
}
void Spider_unloadAssets(void)
{
    int i;
    for(i=0;i<TEXTURES_NUMBER;i++)
    {
        ANZ_FreeImage(Spider_data.images[i]);
        ANZ_FreeSpriteMap(&Spider_data.sprites[i]);
    }
}

typedef struct
{
    int posX,posY,
    costume,t_costume;

    SDL_Rect filament;
    int maxFilamentLength;
    int yVel,attached;
    /*bPosX,bPosY,filamentLength;*/
}Spider;

void Spider_delete(AbstractEntity  * ae);
void Spider_render(AbstractEntity  * ae,int layer);
int Spider_onUpdate(AbstractEntity  * ae,ANZ_SpriteMap*level);

AbstractEntity* Spider_create(int posX,int posY,ANZ_SpriteMap*level,AbstractEntity*parent)
{
    AbstractEntity * result= AbstractEntity_create(parent);
    result->deleteEntity = Spider_delete;
    result->onUpdate = Spider_onUpdate;
    result->render = Spider_render;

    Spider * spider = (Spider*)malloc(sizeof(Spider));
    spider->posX = posX;
    spider->posY = posY;
    spider->costume = 0;
    spider->t_costume = 0;

    spider->filament.x = posX+53;
    spider->filament.w = 2;

    for(spider->filament.y = posY ; spider->filament.y > 0 ; spider->filament.y--)
    {
        if(ANZ_GetSpritePixel(level,spider->filament.x,  spider->filament.y,0,0)>50
         &&ANZ_GetSpritePixel(level,spider->filament.x+1,spider->filament.y,0,0)>50)break;
    }

    spider->filament.h = posY - spider->filament.y + 52;

    spider->maxFilamentLength=350;
    spider->yVel=3;
    spider->attached=1;

    /*spider->fl=0;
    spider->fl_max=150;*/

    result->entity = spider;
    result->posX=&spider->posX;
    result->posY=&spider->posY;
    return result;
}
void Spider_delete(AbstractEntity  * ae)
{
    Spider * spider = (Spider*)(ae->entity);
    free(spider);
    ae->entity = 0;
}
int Spider_onUpdate(AbstractEntity  * ae,ANZ_SpriteMap*level)
{
    Spider * spider = (Spider*)(ae->entity);
    if(spider->t_costume>8)
    {
        spider->t_costume=0;
        spider->costume++;
        if(spider->costume>3)
        {
            spider->costume=0;
        }
    }
    else
    {
        spider->t_costume++;
    }

    if(spider->attached)
    {
        spider->posY += spider->yVel;
        spider->filament.h = spider->posY - spider->filament.y + 52;
        if(spider->yVel>0)
        {
            if(spider->filament.h>spider->maxFilamentLength||ANZ_CollisionRS(spider->posX,spider->posY,110,91,0,0,level,SDL_FLIP_NONE,50))
            {
                spider->yVel = -spider->yVel;
                spider->posY += spider->yVel;
                spider->filament.h = spider->posY - spider->filament.y + 52;
            }
        }
        else
        {
            if(spider->filament.h<10||ANZ_CollisionRS(spider->posX,spider->posY,110,91,0,0,level,SDL_FLIP_NONE,50))
            {
                spider->yVel = -spider->yVel;
                spider->posY += spider->yVel;
                spider->filament.h = spider->posY - spider->filament.y + 52;
            }
        }
    }
    else
    {
        spider->posY+=6;
        if(spider->posY>level->height)return 1;
    }

    Snake* snake = (Snake*)(AbstractEntity_firstElement(ae)->entity);
    if(snake==0)return 0;

    if(Snake_updateEnemyCollision(ae,&Spider_data.sprites[spider->costume],SDL_FLIP_NONE))
    {
        printf("BAD SPIDER!!!!\n");
    }
    else if(spider->attached&&Snake_checkCollisionWithHeadR(snake,&spider->filament))
    {
        spider->attached = 0;
    }


    return 0;
}
void Spider_render(AbstractEntity  * ae,int layer)
{
    Spider * spider = (Spider*)(ae->entity);
    if(layer==2)
    {
        ANZ_RenderImage(Spider_data.images[spider->costume],spider->posX,spider->posY);
    }
    else if(layer==0 && spider->attached)
    {
        SDL_SetRenderDrawColor(sdlrenderer,200,200,200,255);
        SDL_RenderFillRect(sdlrenderer,&spider->filament);
    }
}


