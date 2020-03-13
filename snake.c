#include "snake.h"
#include "particle.h"
#include "game_gui.h"

#include <math.h>

#define TEXTURES_NUMBER 8
/*#define snakeVelocity 3*/
#define DENSITY_W 30
#define DENSITY_H 30

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
/*
#define SNAKE_DEBUG_GRAPHICS
*/
struct
{
    ANZ_Image *images[TEXTURES_NUMBER];
    int tanimation;
    int costume;
    int velocity;
    int isDead;
}Snake_data;

void Snake_delete(AbstractEntity  * ae);
void Snake_render(AbstractEntity  * ae,int layer);
int Snake_onUpdate(AbstractEntity  * ae,ANZ_SpriteMap*levelMap);

int Snake_checkInnerCollision(AbstractEntity*ae, int leftSegments);


Snake * Snake_createSegment();



int Snake_loadAssets(void)
{
    Snake_data.tanimation=0;
    Snake_data.costume=0;


    Snake_data.images[0]=ANZ_LoadImage("assets/snake/head1.png");
    Snake_data.images[1]=ANZ_LoadImage("assets/snake/head2.png");
    Snake_data.images[2]=ANZ_LoadImage("assets/snake/head3.png");
    Snake_data.images[3]=ANZ_LoadImage("assets/snake/head4.png");
    Snake_data.images[4]=ANZ_LoadImage("assets/snake/head5.png");
    Snake_data.images[5]=ANZ_LoadImage("assets/snake/head6.png");

    Snake_data.images[6]=ANZ_LoadImage("assets/snake/segment.png");
    Snake_data.images[7]=ANZ_LoadImage("assets/snake/segment1.png");
    int i,result=0;
    for(i=0;i<TEXTURES_NUMBER;i++)
    {
        result += Snake_data.images[i] == 0;
    }
    return !result;
}

void Snake_unloadAssets(void)
{
    int i;
    for(i=0;i<TEXTURES_NUMBER;i++)
    {
        ANZ_FreeImage(Snake_data.images[i]);
    }
}

AbstractEntity * Snake_create(int posX,int posY,int velocity,AbstractEntity*parent)
{
    int i;
    AbstractEntity * result= AbstractEntity_create(parent);
    result->onUpdate = Snake_onUpdate;
    result->render = Snake_render;
    result->deleteEntity = Snake_delete;


    Snake * snake=Snake_createSegment();
    snake->posX=posX;
    snake->posY=posY;

    for(i=0;i<2;i++)
    {
        Snake_addNewSegment(snake);
    }

    result->entity = snake;
    result->posX= &snake->posX;
    result->posY= &snake->posY;

    Snake_data.velocity=velocity;
    Snake_data.isDead=0;

    gameGUI_updateScore(Snake_length(snake));

    return result;
}


Snake * Snake_createSegment()
{
    Snake * result = (Snake *) malloc(sizeof(Snake));

    result->next=0;
    result->previous=0;

    result->movementDirection=RIGHT;
    result->posX=0;
    result->posY=0;
    result->inflection=0;

    result->zez.posX=0;
    result->zez.posY=0;

    return result;
}
void Snake_delete(AbstractEntity  * ae)
{
    Snake* head=(Snake*)(ae->entity);

    Snake *snakeT;
    /*printf("Removing snake\n");*/
    while(head!=0)
    {
        snakeT=head;
        head=head->next;
        free(snakeT);
    }
    /*printf("END\n");*/
}
Snake * Snake_getTail(Snake * snake)
{
    Snake * tail=snake;
    while(tail->next!=0)
    {
        tail=tail->next;
    }
    return tail;
}

int Snake_length(Snake * head)
{
    int result=0;
    while(head!=0)
    {
        result++;
        head=head->next;
    }
    return result;
}/*
int Snake_checkDistanceRadiusToHead(Snake*head,int x,int y,int r2)
{
    int dx,dy;
    dx = head->posX - x;
    dy = head->posY - y;

    return dx*dx + dy*dy < r2;
}*/


void Snake_normalize(Snake*snake1,Snake*snake2)
{
    snake1->movementDirection=snake2->movementDirection;
    snake1->posX=snake2->posX;
    snake1->posY=snake2->posY;

    switch(snake1->movementDirection)
    {
    case LEFT:
        snake1->posX+=DENSITY_W;
        break;
    case RIGHT:
        snake1->posX-=DENSITY_W;
        break;
    case UP:
        snake1->posY+=DENSITY_H;
        break;
    case DOWN:
        snake1->posY-=DENSITY_H;
        break;
    }
}



void Snake_addNewSegment(Snake*head)
{
    Snake * newSegment = Snake_createSegment();
    Snake * tail = Snake_getTail(head);

    tail->next=newSegment;
    newSegment->previous=tail;

    Snake_normalize(newSegment,tail);

    gameGUI_updateScore(Snake_length(head));
}
/*
void Snake_removeTail(Snake* head)
{
    Snake*tail=Snake_getTail(head);
    if(tail!=head)
    {
        tail->previous->next=0;
        free(tail);
    }
    gameGUI_updateScore(Snake_length(head));
}*/

void Snake_segmentate(AbstractEntity*snake_ae,Snake* dead_segment)
{
    int i;

    Snake*head=(Snake*)(snake_ae->entity);
    if(head==0||dead_segment==0||Snake_data.isDead)return;

    Snake*segment=head;
    for(i=0;i<2;i++)
    {
        if(segment==0)break;
        else if(segment==dead_segment)
        {
            Snake_data.isDead=1;
            dead_segment=head;
            break;
        }
        segment=segment->next;
    }

    if(!Snake_data.isDead)
    {

        dead_segment->previous->next=0;
        dead_segment->previous=0;
        gameGUI_updateScore(Snake_length(head));
    }

    AbstractEntity* ae1 = SnakeParticle_create(dead_segment,dead_segment==head,snake_ae);
    for(segment=dead_segment->next;segment!=0;segment=segment->next)
    {
        ae1=SnakeParticle_create(segment,0,ae1);
    }


    /*segment=head->next;
    while(segment!=0)
    {
        ae1=SnakeParticle_create(segment,0,ae1);
        segment=segment->next;
    }*/

    /*ae->entity=0;*/
}

int Snake_onUpdate(AbstractEntity  * ae,ANZ_SpriteMap*levelMap)
{
    if(Snake_data.isDead)
    {
        ae->entity=0;
        ae->posX=0;
        ae->posY=0;
        return 2;
    }

    Snake* head=(Snake*)(ae->entity);
    Snake* tail;
    Snake* segment;
    int zez_reached;

    tail=Snake_getTail(head);
    tail->inflection=0;

    if(!head->inflection)
    {
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if(head->movementDirection<RIGHT)
        {
            if (currentKeyStates[SDL_SCANCODE_RIGHT])
            {
                head->movementDirection=RIGHT;
                head->inflection=1;
            }
            else if(currentKeyStates[SDL_SCANCODE_LEFT])
            {
                head->movementDirection=LEFT;
                head->inflection=1;
            }
        }
        else
        {
            if (currentKeyStates[SDL_SCANCODE_DOWN])
            {
                head->movementDirection=DOWN;
                head->inflection=1;
            }
            else if(currentKeyStates[SDL_SCANCODE_UP])
            {
                head->movementDirection=UP;
                head->inflection=1;
            }
        }
        if(head->inflection)
        {
            head->zez.posX=head->posX;
            head->zez.posY=head->posY;
        }
    }


    for(segment = tail;segment->previous!=0;segment=segment->previous)
    {
        if(segment->previous->inflection)
        {
            zez_reached=0;
            switch(segment->movementDirection)
            {
            case LEFT:
                if(segment->posX <= segment->previous->zez.posX)
                {
                    zez_reached=1;
                }
                break;
            case RIGHT:
                if(segment->posX >= segment->previous->zez.posX)
                {
                    zez_reached=1;
                }
                break;
            case UP:
                if(segment->posY <= segment->previous->zez.posY)
                {
                    zez_reached=1;
                }
                break;
            case DOWN:
                if(segment->posY >= segment->previous->zez.posY)
                {
                    zez_reached=1;
                }
                break;
            }

            if(zez_reached)
            {
                segment->inflection=1;
                segment->previous->inflection=0;
                segment->zez=segment->previous->zez;

                Snake_normalize(segment,segment->previous);
            }
        }
    }
    for(segment = head;segment!=0;segment=segment->next)
    {
        switch(segment->movementDirection)
        {
        case LEFT:
            segment->posX-=Snake_data.velocity;

            if(segment->posX<-100)
            {
                segment->posX+=SCREEN_WIDTH+100;
            }
            break;
        case RIGHT:
            segment->posX+=Snake_data.velocity;
            if(segment->posX>SCREEN_WIDTH+50)
            {
                segment->posX-=SCREEN_WIDTH+100;
            }
            break;
        case UP:
            segment->posY-=Snake_data.velocity;
            if(segment->posY<-100)
            {
                segment->posY+=SCREEN_HEIGHT+100;
            }
            break;
        case DOWN:
            segment->posY+=Snake_data.velocity;

            if(segment->posY>SCREEN_HEIGHT+50)
            {
                segment->posY-=SCREEN_HEIGHT+100;
            }

            break;
        }
    }

    if(Snake_checkInnerCollision(ae,3))
    {
        printf("Bitten tail :-(\n");
    }

    if(Snake_checkCollisionWithHeadS(head,0,0,levelMap,SDL_FLIP_NONE))
    {
        Snake_segmentate(ae,head);
    }


    if(Snake_data.tanimation>10)
    {
        Snake_data.tanimation=0;
        Snake_data.costume++;
        if(Snake_data.costume>1)
        {
            Snake_data.costume=0;
        }
    }
    else
    {
        Snake_data.tanimation++;
    }

    return 0;
}
SDL_Rect Snake_getHeadRectangle(Snake * head)
{
    SDL_Rect result;
    result.x=head->posX+5;
    result.y=head->posY+5;

    switch(head->movementDirection)
    {
    case LEFT:
        result.x -= 14;
    case RIGHT:
        result.w = 40;
        result.h = 20;
        break;
    case UP:
        result.y -=7;
    case DOWN:
        result.w = 20;
        result.h = 40;
        break;
    }

    return result;
}

SDL_Rect Snake_getSegmentRect(Snake* segment)
{
    SDL_Rect result;
    result.x = segment->posX+5;
    result.y = segment->posY+5;

    if(segment->movementDirection==LEFT || segment->movementDirection==RIGHT)
    {
        result.w = 25;
        result.h = 17;
    }
    else
    {
        result.x-= 3;
        result.w = 17;
        result.h = 25;
    }
    return result;

}

int Snake_updateEnemyCollision(AbstractEntity*entity,ANZ_SpriteMap*sprite,SDL_RendererFlip flip)
{
    AbstractEntity * snake_ae = AbstractEntity_firstElement(entity);
    if(snake_ae->entity==0)return 0;

    Snake*head=(Snake*)snake_ae->entity;
    if(Snake_checkCollisionWithHeadS(head,*entity->posX,*entity->posY,sprite,flip))
    {
        Snake_segmentate(snake_ae,head);
        return 1;
    }
    else
    {
        Snake*segment;
        SDL_Rect rect;
        for(segment=head->next;segment!=0;segment=segment->next)
        {
            rect = Snake_getSegmentRect(segment);
            if(ANZ_CollisionRS1(&rect,*entity->posX,*entity->posY,sprite,flip,50))
            {
                Snake_segmentate(snake_ae,segment);
                return 1;
            }
        }
    }

    return 0;
}

int Snake_checkCollisionWithHeadR(Snake*snake, SDL_Rect*rect)
{
    SDL_Rect headRect = Snake_getHeadRectangle(snake);
    return ANZ_RectangleCollisionRR(&headRect,rect);
}
int Snake_checkCollisionWithHeadS(Snake*snake,int px2,int py2,ANZ_SpriteMap*sprite,SDL_RendererFlip flip)
{
    SDL_Rect headRect = Snake_getHeadRectangle(snake);
    return ANZ_CollisionRS1(&headRect,px2,py2,sprite,flip,50);
}


int Snake_checkInnerCollision(AbstractEntity*ae, int leftSegments)
{
    int i;
    Snake*head=(Snake*)(ae->entity);
    Snake * snake=head;
    for(i=0;i<leftSegments;i++)
    {
        snake=snake->next;
        if(snake==0)return 0;
    }

    SDL_Rect headRect = Snake_getHeadRectangle(head);
    SDL_Rect segmentRect;


    while(snake!=0)
    {
        segmentRect=Snake_getSegmentRect(snake);
        if(ANZ_RectangleCollisionRR(&headRect,&segmentRect))
        {
            Snake_segmentate(ae,snake);
            return 1;
        }
        snake=snake->next;
    }

    return 0;
}

void Snake_renderSegment(Snake*segment,int isDead)
{
#ifdef SNAKE_DEBUG_GRAPHICS
    SDL_Rect rect;
    rect.w=20;
    rect.h=20;
    if(segment->inflection)
    {
        SDL_SetRenderDrawColor(sdlrenderer,255,0,0,255);
    }
    else
    {
        SDL_SetRenderDrawColor(sdlrenderer,255,255,0,255);
    }
    rect.x=segment->posX-10;
    rect.y=segment->posY-10;
    SDL_RenderFillRect(sdlrenderer,&rect);

#else

    ANZ_Image * image;
    image=Snake_data.images[isDead?7:6];
    switch(segment->movementDirection)
    {
        case LEFT:
            ANZ_RenderImage(image,segment->posX,segment->posY+3);
            break;
        case RIGHT:
            ANZ_RenderImage(image,segment->posX,segment->posY+3);
            break;
        case UP:
            ANZ_RenderImageX(image,segment->posX-3,segment->posY,90,0);
            break;
        case DOWN:
            ANZ_RenderImageX(image,segment->posX-3,segment->posY,-90,0);
            break;
    }
#endif
}

void Snake_renderHead(Snake*head,int isDead)
{
    if(isDead)
    {
        switch(head->movementDirection)
        {
        case LEFT:
            ANZ_RenderImageF(Snake_data.images[4],head->posX-14,head->posY,SDL_FLIP_HORIZONTAL);
            break;
        case RIGHT:
            ANZ_RenderImage(Snake_data.images[4],head->posX,head->posY);
            break;
        case UP:
            ANZ_RenderImage(Snake_data.images[5],head->posX, head->posY-7);
            break;
        case DOWN:
            ANZ_RenderImageF(Snake_data.images[5],head->posX,head->posY-5,SDL_FLIP_VERTICAL);
            break;
        }
    }
    else
    {
        switch(head->movementDirection)
        {
        case LEFT:
            ANZ_RenderImageF(Snake_data.images[Snake_data.costume],head->posX-14,head->posY,SDL_FLIP_HORIZONTAL);
            break;
        case RIGHT:
            ANZ_RenderImage(Snake_data.images[Snake_data.costume],head->posX,head->posY);
            break;
        case UP:
            ANZ_RenderImage(Snake_data.images[Snake_data.costume+2],head->posX, Snake_data.costume ? head->posY-5 : head->posY-7);
            break;
        case DOWN:
            ANZ_RenderImageF(Snake_data.images[Snake_data.costume+2],head->posX,head->posY-5,SDL_FLIP_VERTICAL);
            break;
        }
    }
}

void Snake_render(AbstractEntity  * ae,int layer)
{
    if(layer!=1)return;
    Snake* head=(Snake*)(ae->entity);
    Snake* tail=Snake_getTail(head);
    Snake * segment;

    for(segment=tail;segment->previous!=0;segment=segment->previous)
    {
        Snake_renderSegment(segment,0);
    }
    Snake_renderHead(head,0);

}


