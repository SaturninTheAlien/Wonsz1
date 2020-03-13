#ifndef ENTITY_PAWNER_H_INCLUDED
#define ENTITY_PAWNER_H_INCLUDED


typedef struct _EntitySpawner
{
    int posX,posY,active;
    char* entityName;

    struct _EntitySpawner*next;
}EntitySpawner;


EntitySpawner * EntitySpawner_create(int entityX,int entityY,char* entityName);
void EntitySpawner_createNext(EntitySpawner ** spawner,int entityX,int entityY,char* entityName);
void EntitySpawner_free(EntitySpawner*es);

EntitySpawner * EntitySpawner_lastElement(EntitySpawner*es);
EntitySpawner* EntitySpawner_selectRandom(EntitySpawner*first,const char* name,int lockSpawner);
int EntitySpawner_countSpawners(EntitySpawner*fs,const char*name);


#endif
