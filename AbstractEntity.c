#include "AbstractEntity.h"


AbstractEntity* AbstractEntity_create(AbstractEntity* parent)
{
    parent= AbstractEntity_lastElement(parent);

    AbstractEntity* result = (AbstractEntity*)malloc(sizeof(AbstractEntity));

    result->deleteEntity=0;
    result->onUpdate=0;
    result->render=0;

    result->entity=0;
    result->posX=0;
    result->posY=0;

    result->next=0;

    result->spawner=0;

    result->previous=parent;

    if(parent!=0)parent->next=result;

    return result;
}




void AbstractEntity_delete(AbstractEntity * ae)
{
    AbstractEntity*aep =ae->previous;
    AbstractEntity*aen =ae->next;

    if(aen!=0)
    {
        aen->previous=aep;
    }
    if(aep!=0)
    {
        aep->next = aen;
    }

    if(ae->entity!=0 && ae->deleteEntity!=0)
    {
        ae->deleteEntity(ae);
    }
    if(ae->spawner!=0)
    {
        ae->spawner->active=0;
    }

    free(ae);
}
AbstractEntity* AbstractEntity_firstElement(AbstractEntity* ae)
{
    if(ae==0)return 0;
    while(ae->previous !=0 )
    {
        ae=ae->previous;
    }
    return ae;
}
AbstractEntity* AbstractEntity_lastElement(AbstractEntity* ae)
{
    if(ae==0)return 0;
    while(ae->next != 0)
    {
        ae=ae->next;
    }
    return ae;
}


void AbstractEntity_deleteALL(AbstractEntity * ae)
{
    ae=AbstractEntity_firstElement(ae);

    AbstractEntity*ae1;
    while(ae!=0)
    {
        ae1=ae;
        ae=ae->next;
        if(ae1->entity!=0 && ae1->deleteEntity!=0)
        {
            ae1->deleteEntity(ae1);
        }
        if(ae1->spawner!=0)
        {
            ae1->spawner->active=0;
        }
        free(ae1);
    }
}

/**Returns 1 on game over otherwise 0*/
int  AbstractEntity_updateALL(AbstractEntity * ae_in,ANZ_SpriteMap*levelMap)
{
    AbstractEntity*ae=AbstractEntity_firstElement(ae_in);

    if(ae==0)return 1;
    int i1;
    AbstractEntity* ae1;
    while(ae!=0)
    {
        ae1=ae;
        ae=ae->next;
        if(ae1->entity == 0)continue;
        i1=ae1->onUpdate(ae1,levelMap);

        if(i1>0)
        {
            if(i1==2)
            {
                return 1;
            }
            AbstractEntity_delete(ae1);
        }
    }
    return 0;
}
void AbstractEntity_renderALL(AbstractEntity * ae,int layer)
{
    ae=AbstractEntity_firstElement(ae);
    while(ae!=0)
    {
        if(ae->entity!=0)ae->render(ae,layer);
        ae=ae->next;
    }
}




