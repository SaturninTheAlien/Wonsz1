#include "apple.h"
#include "snake.h"
#include <math.h>

#define TEXTURES_NUMBER 3

#ifndef M_PI
#define M_PI 3.14159265359
#endif

struct
{
    ANZ_Image* images[TEXTURES_NUMBER];
    ANZ_SpriteMap sprites[TEXTURES_NUMBER];
}AppleData;

int Apple_entitiesNumber=0;


int Apple_loadAssets(void)
{
    AppleData.images[0] = ANZ_LoadImageAndSpriteMap("assets/apple/apple1.png",&AppleData.sprites[0]);
    AppleData.images[1] = ANZ_LoadImageAndSpriteMap("assets/apple/apple2.png",&AppleData.sprites[1]);
    AppleData.images[2] = ANZ_LoadImageAndSpriteMap("assets/apple/apple3.png",&AppleData.sprites[2]);

    int i,result = 0;
    for(i=0;i<TEXTURES_NUMBER;i++)
    {
        result += AppleData.images[i]!=0;
    }
    return result;
}
void Apple_unloadAssets(void)
{
    int i;
    for(i=0;i<TEXTURES_NUMBER;i++)
    {
        ANZ_FreeImage(AppleData.images[i]);
        ANZ_FreeSpriteMap(&AppleData.sprites[i]);
    }
}

typedef struct
{
    int posX,posY,py;
    int type;
    double da1;
}Apple;

void Apple_delete(AbstractEntity  * ae);
void Apple_render(AbstractEntity  * ae,int layer);
int Apple_onUpdate(AbstractEntity  * ae,ANZ_SpriteMap*level);

AbstractEntity*Apple_create(int posX,int posY,AbstractEntity*parent)
{
    AbstractEntity * result = AbstractEntity_create(parent);

    result->onUpdate=Apple_onUpdate;
    result->deleteEntity=Apple_delete;
    result->render=Apple_render;

    Apple* apple = (Apple* )malloc(sizeof(Apple));
    apple->posX = posX;
    apple->py = posY;
    apple->posY = posY;
    apple->type = 0;
    apple->da1 = 0;

    result->entity=apple;
    result->posX=&apple->posX;
    result->posY=&apple->posY;

    Apple_entitiesNumber++;

    return result;
}

int Apple_tryFindEmptyArea(ANZ_SpriteMap*level,Snake*snake,SDL_Rect* rect,int max_iteration)
{
    int i,dx,dy,r;
    for(i=0;i<max_iteration;i++)
    {
        rect->x = rand()%(level->width - rect->w);
        rect->y = rand()%(level->height - rect->h);

        dx=snake->posX - rect->x;
        dy=snake->posY - rect->y;
        r= dx*dx + dy*dy;
        /*if(r<1600)
        {
            continue;
        }*/


        if(r>3600 && !ANZ_CollisionRS1(rect,0,0,level,SDL_FLIP_NONE,50))
        {
            return 1;
        }
    }
    return 0;
}

AbstractEntity*Apple_createInRandomPosition(ANZ_SpriteMap*level,int appleType,AbstractEntity*parent)
{

    Snake *snake=(Snake*)(AbstractEntity_firstElement(parent)->entity); /*Snake is always first element of the entities list*/


    SDL_Rect rect;
    rect.x=0;
    rect.y=0;
    rect.w=AppleData.sprites[appleType].width+10;
    rect.h=AppleData.sprites[appleType].height+10;

    /*printf("Snake->posX=%i\n",snake->posX);
    printf("Snake->posX=%i\n",snake->posY);*/

    if(!Apple_tryFindEmptyArea(level,snake,&rect,10))
    {
        return 0;
    }

    AbstractEntity*ae= Apple_create(rect.x+5,rect.y+5,parent);

    Apple*apple=(Apple*)(ae->entity);
    apple->type= appleType;


    return ae;
}


void Apple_delete(AbstractEntity  * ae)
{
    /*printf("Removing apple\n");*/
    Apple*apple = (Apple*)(ae->entity);
    free(apple);
    ae->entity=0;

    Apple_entitiesNumber--;
    /*printf("END\n");*/
}

void Apple_render(AbstractEntity*ae,int layer)
{
    if(layer==0)
    {
        Apple * apple = (Apple*)(ae->entity);
        ANZ_RenderImage(AppleData.images[apple->type],apple->posX,apple->posY);
    }
}

int Apple_onUpdate(AbstractEntity*ae,ANZ_SpriteMap*level)
{
    Apple * apple= (Apple*)(ae->entity);
    apple->da1 += 0.025;
    if(apple->da1 > M_PI)
    {
        apple->da1 -= M_PI;
    }
    apple->posY = apple->py + 10*sin(apple->da1);

    Snake* snake = (Snake*)(AbstractEntity_firstElement(ae)->entity);
    if(snake==0)return 0;

    if(Snake_checkCollisionWithHeadS(snake,apple->posX,apple->posY,&AppleData.sprites[apple->type],SDL_FLIP_NONE))
    {
        Snake_addNewSegment(snake);
        printf("Eaten apple :-)\n");
        return 1;
    }
    return 0;
}


