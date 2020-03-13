#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include "ANZ_Image.h"
#include "AbstractEntity.h"

typedef struct
{
    ANZ_Image*background;
    ANZ_Image*image;
    ANZ_SpriteMap spriteMap;

    void* firstEntitySpawner;

    int startX,startY,applesNumber;

}SnakeLevel;

/*extern SnakeLevel snakeLevel;*/

int SnakeLevel_loadFromFile(SnakeLevel*level,char*fileName);
void SnakeLevel_free(SnakeLevel*level);

void SnakeLevel_onUpdate(SnakeLevel*level,AbstractEntity*ae);
void SnakeLevel_startSpawn(SnakeLevel*level,AbstractEntity*ae);

#endif
