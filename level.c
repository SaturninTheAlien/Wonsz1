#include "level.h"
#include "entitySpawner.h"
#include <string.h>
#include <memory.h>
#include "apple.h"
#include "spider.h"
#include "snake.h"


/*SnakeLevel snakeLevel;*/

typedef enum
{
    NONE,
    ERROR,
    LEVEL_NAME,
    BACKGROUND_NAME,
    POSITION,
    APPLES_NUMBER,
    SPAWNER

}DataType;

char* SnakeLevel_mergeStrings(char*dest,char*src,int n)
{
    int i=strlen(dest);

    while(*src != '\0')
    {
        if(i>=n)
        {
            return 0;
        }

        dest[i]=*src;
        src++;
        i++;
    }
    if(i>=n)
    {
        return 0;
    }
    dest[i] = '\0';
    return dest;
}
char * SnakeLevel_getPath(char*buffer,int n,char*src)
{
    char*s="assets/maps/";
    int size=strlen(s)+1;
    if(size>=n)return 0;

    memcpy(buffer,s,size);
    return SnakeLevel_mergeStrings(buffer,src,n);
}


DataType SnakeLevel_parseLine(char * src, char** s1)
{
    int i = 0;
    for(i=0; src[i]!= ':'; i++)
    {

        if(src[i] =='\0')return NONE;
    }

    if(i>=29)
    {
        return ERROR;
    }
    DataType result=NONE;
    if(i>=30)return result;


    char buffer[30];
    memcpy(buffer,src,i);
    buffer[i]='\0';

    printf("%s\n",buffer);

    if(!strcmp(buffer,"Position"))
    {
        printf("Position1\n");
        result=POSITION;
    }
    else if(!strcmp(buffer,"LevelName"))
    {
        printf("LevelName:\n");
        result=LEVEL_NAME;
    }
    else if(!strcmp(buffer,"BackgroundName"))
    {
        printf("BackgroundName:\n");
        result=BACKGROUND_NAME;
    }
    else if(!strcmp(buffer,"Spawner"))
    {
        printf("Spawner:\n");
        result=SPAWNER;
    }
    else if(!strcmp(buffer,"ApplesNumber"))
    {
        result=APPLES_NUMBER;
    }
    *s1 = &src[i+1];

    return result;
}

int SnakeLevel_loadFromFile(SnakeLevel*level,char*fileName1)
{
    char buffer[120];
    char levelName[60] = "level0.png";
    char backgroundName[60] = "NO_BACKGROUND";

    char entityName[60];
    int entityX,entityY;
    EntitySpawner** esp1;

    char *s1;
    int hasBackground=0;
    char*fileName=SnakeLevel_getPath(buffer,120,fileName1);
    DataType datatype;

    level->firstEntitySpawner=0;
    level->startX=0;
    level->startY=0;
    level->applesNumber=1;

    printf("Parsing level file: = %s\n\n",fileName);
    FILE * file = fopen(fileName,"r");
    if(file==0)
    {
        fprintf(stderr,"Cannot open file: %s\n",fileName);
        return 0;
    }


    while(!feof(file))
    {
        if(fgets(buffer,120,file)==0)continue;
        datatype = SnakeLevel_parseLine(buffer,&s1);
        switch(datatype)
        {
        case ERROR:
            fprintf(stderr,"Error while parsing level file %s\n",fileName);
            return 0;
        case POSITION:
            if(sscanf(s1,"%i %i",&level->startX,&level->startY)!=2)return 0;
            printf("StartX = %i StartY = %i\n",level->startX,level->startY);
            break;
        case LEVEL_NAME:
            if(sscanf(s1,"%59s",levelName)!=1)return 0;
            printf("%s\n",levelName);
            break;
        case BACKGROUND_NAME:
            if(sscanf(s1,"%59s",backgroundName)!=1)return 0;
            hasBackground=1;
            printf("%s\n",levelName);
            break;
        case SPAWNER:
            if(sscanf(s1,"%59s %i %i",entityName,&entityX,&entityY)==3)
            {
                esp1=(EntitySpawner**)&level->firstEntitySpawner;
                EntitySpawner_createNext(esp1,entityX,entityY,entityName);
            }
            else
            {
                fprintf(stderr,"Error parsing entity spawner\n");
                return 0;
            }
            break;
        case APPLES_NUMBER:
            if(sscanf(s1,"%i",&level->applesNumber)==1)
            {
                printf("ApplesNumber:%i\n",level->applesNumber);
            }
            else
            {
                return 0;
            }

            break;
        default:
            break;
        }
    }
    fclose(file);
    printf("\nParsing level file ended successfully\n\n");

    level->image=ANZ_LoadImageAndSpriteMap(SnakeLevel_getPath(buffer,120,levelName),&level->spriteMap);
    if(hasBackground)
    {
        level->background=ANZ_LoadImage(SnakeLevel_getPath(buffer,120,backgroundName));
        if(level->background==0)return 0;
    }
    else
    {
        level->background=0;
    }

    return level->image!=0;
}
void SnakeLevel_free(SnakeLevel*level)
{
    EntitySpawner_free((EntitySpawner*)level->firstEntitySpawner);

    ANZ_FreeImage(level->image);
    ANZ_FreeSpriteMap(&level->spriteMap);
}

void SnakeLevel_onUpdate(SnakeLevel* level,AbstractEntity*ae)
{
    int i;
    if(Apple_entitiesNumber<level->applesNumber)
    {
        i=rand() % 30;
        if(i==0)
        {
            Apple_createInRandomPosition(&level->spriteMap,rand()%3,ae);

        }
    }
}

void SnakeLevel_startSpawn(SnakeLevel*level,AbstractEntity*ae)
{
    printf("Start spawn\n");
    EntitySpawner* spawner;
    for(spawner = level->firstEntitySpawner;spawner!=0;spawner=spawner->next)
    {
        if(!strcmp(spawner->entityName,"Spider"))
        {
            ae=Spider_create(spawner->posX,spawner->posY,&level->spriteMap,ae);
        }
    }
}

