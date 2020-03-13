#ifndef APPLE_H_INCLUDED
#define APPLE_H_INCLUDED

#include "AbstractEntity.h"

int Apple_loadAssets(void);
void Apple_unloadAssets(void);

AbstractEntity*Apple_create(int posX,int posY,AbstractEntity*parent);

AbstractEntity*Apple_createInRandomPosition(ANZ_SpriteMap*level,int appleType,AbstractEntity*parent);

extern int Apple_entitiesNumber;

#endif
