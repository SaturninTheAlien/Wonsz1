#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#include "AbstractEntity.h"

int Snake_loadAssets(void);
void Snake_unloadAssets(void);

enum MovementDirection
{
    UP=0,
    DOWN=1,
    RIGHT=2,
    LEFT=3
};


typedef struct _wonsz
{
    struct _wonsz * next;
    struct _wonsz * previous;

    enum MovementDirection movementDirection;
    int posX,posY,inflection;

    struct
    {
        int posX;
        int posY;
    }zez;

}Snake;


AbstractEntity * Snake_create(int posX,int posY,int velocity,AbstractEntity*parent);


Snake * Snake_getTail(Snake * snake);
int Snake_length(Snake * head);
void Snake_addNewSegment(Snake*head);
/*void Snake_removeTail(Snake* head);*/
void Snake_segmentate(AbstractEntity*snake_ae,Snake* dead_segment);
/*SDL_Rect Snake_getHeadRectangle(Snake * head);*/
int Snake_updateEnemyCollision(AbstractEntity*entity,ANZ_SpriteMap*sprite,SDL_RendererFlip flip);
int Snake_checkCollisionWithHeadR(Snake*snake, SDL_Rect*rect);
int Snake_checkCollisionWithHeadS(Snake*snake,int px2,int py2,ANZ_SpriteMap*sprite,SDL_RendererFlip flip);
int Snake_checkInnerCollision(AbstractEntity*ae, int leftSegments);

void Snake_renderSegment(Snake*segment,int isDead);
void Snake_renderHead(Snake*segment,int isDead);

#endif
