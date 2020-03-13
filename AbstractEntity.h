#ifndef ABSTRACTENTITY_H_INCLUDED
#define ABSTRACTENTITY_H_INCLUDED

#include"ANZ_Image.h"
#include"entitySpawner.h"
/*
Abstract Entity
polymorphism implementation in ansi c*/

typedef struct _AbstractEntity
{
    void (*deleteEntity)(struct _AbstractEntity *entity);
    int (*onUpdate)(struct _AbstractEntity * entity, ANZ_SpriteMap*levelMap);
    void (*render)(struct _AbstractEntity *entity,int layer);

    void *entity;
    int * posX,*posY;
    EntitySpawner*spawner;

    struct _AbstractEntity* next;
    struct _AbstractEntity* previous;

}AbstractEntity;

AbstractEntity* AbstractEntity_create(AbstractEntity* parent);
void AbstractEntity_delete(AbstractEntity * ae);

AbstractEntity* AbstractEntity_firstElement(AbstractEntity* ae);
AbstractEntity* AbstractEntity_lastElement(AbstractEntity* ae);
/*AbstractEntity* AbstractEntity_getSnakeAE(AbstractEntity* ae);
*/
void AbstractEntity_deleteALL(AbstractEntity * ae);
int  AbstractEntity_updateALL(AbstractEntity * ae_in,ANZ_SpriteMap*levelMap);
void AbstractEntity_renderALL(AbstractEntity * ae,int layer);



#endif
