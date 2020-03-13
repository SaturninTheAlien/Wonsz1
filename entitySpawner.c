#include "entitySpawner.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


EntitySpawner * EntitySpawner_create(int entityX,int entityY,char* entityName)
{
    int size1;
    EntitySpawner *result = (EntitySpawner*)malloc(sizeof(EntitySpawner));
    result->next=0;
    result->posX = entityX;
    result->posY = entityY;
    result->active = 0;

    size1=strlen(entityName)+1;

    result->entityName = (char*)malloc(size1);
    memcpy(result->entityName,entityName,size1-1);
    result->entityName[size1-1]='\0';
    printf("EntityName: %s PosX: %i PosY: %i\n",result->entityName,result->posX,result->posY);
    return result;
}

void EntitySpawner_createNext(EntitySpawner ** firstSpawner,int entityX,int entityY,char* entityName)
{
    EntitySpawner* ns=EntitySpawner_create(entityX,entityY,entityName);

    if(*firstSpawner == 0 )
    {
        *firstSpawner = ns;
    }
    else
    {
        EntitySpawner*es = EntitySpawner_lastElement(*firstSpawner);
        es->next=ns;
    }
}



void EntitySpawner_free(EntitySpawner*es)
{
    EntitySpawner* es1;
    while(es!=0)
    {
        es1=es;
        es=es->next;
        free(es1->entityName);
        free(es1);
    }
}

EntitySpawner* EntitySpawner_lastElement(EntitySpawner*es)
{
    if(es==0)return es;
    while(es->next!=0)
    {
        es=es->next;
    }
    return es;
}

EntitySpawner* EntitySpawner_selectRandom(EntitySpawner*first,const char* name,int lockSpawner)
{
    EntitySpawner*es=first;

    int sc=0;
    while(es!=0)
    {
        if(es->active >= 0 && !strcmp(es->entityName,name))
        {
            es->active=1;
            sc++;
        }
        es=es->next;
    }
    if(sc==0)return 0;

    EntitySpawner * result=0;
    int sc1= rand() % (sc + 1);

    es = first;
    while(es!=0)
    {
        if(es->active>0)
        {
            if(sc1==0)
            {
                result=es;
                break;
            }
            else
            {
                sc1--;
            }
        }
        es=es->next;
    }

    es = first;
    while(es!=0)
    {
        if(es->active>0)es->active=0;
        es=es->next;
    }
    if(lockSpawner)
    {
        es->active=-1;
    }

    return result;
}
int EntitySpawner_countSpawners(EntitySpawner*fs,const char*name)
{
    int result=0;
    while(fs!=0)
    {
        if(!strcmp(fs->entityName,name))result++;
        fs=fs->next;
    }
    return result;
}
