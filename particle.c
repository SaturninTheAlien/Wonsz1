#include "particle.h"
#include "snake.h"
void SnakeParticle_delete(AbstractEntity * ae);
void SnakeParticle_render(AbstractEntity * ae,int layer);
int SnakeParticle_onUpdate(AbstractEntity * ae,ANZ_SpriteMap*levelMap);

typedef struct
{
    Snake*segment;
    int lifetime;
    int isHead;

}SnakeParticle;

AbstractEntity* SnakeParticle_create(void*snakeSegment,int isHead, AbstractEntity*parent)
{
    AbstractEntity*result=AbstractEntity_create(parent);

    result->deleteEntity=SnakeParticle_delete;
    result->onUpdate=SnakeParticle_onUpdate;
    result->render=SnakeParticle_render;

    SnakeParticle*particle=(SnakeParticle*)malloc(sizeof(SnakeParticle));
    particle->segment=(Snake*)snakeSegment;
    particle->lifetime=rand()%11+10;
    particle->isHead=isHead;

    result->entity=particle;
    result->posX=&particle->segment->posX;
    result->posY=&particle->segment->posY;

    return result;
}

void SnakeParticle_delete(AbstractEntity * ae)
{
    SnakeParticle*particle=(SnakeParticle*)(ae->entity);
    if(particle->segment!=0)
    {
        free(particle->segment);
    }
    free(particle);
}

void SnakeParticle_render(AbstractEntity * ae,int layer)
{
    if(layer==0)
    {
        SnakeParticle*particle=(SnakeParticle*)(ae->entity);
        if(particle->isHead)
        {
            Snake_renderHead(particle->segment,1);
        }
        else
        {
            Snake_renderSegment(particle->segment,1);
        }
    }

}
int SnakeParticle_onUpdate(AbstractEntity * ae,ANZ_SpriteMap*levelMap)
{
    SnakeParticle*particle=(SnakeParticle*)(ae->entity);
    if(particle->lifetime>0)
    {
        particle->lifetime--;
    }
    else
    {
        return 1;
    }


    return 0;
}
