#ifndef SPIDER_H_INCLUDED
#define SPIDER_H_INCLUDED

#include "AbstractEntity.h"

int Spider_loadAssets(void);
void Spider_unloadAssets(void);
AbstractEntity* Spider_create(int posX,int posY,ANZ_SpriteMap*level,AbstractEntity*parent);

#endif
