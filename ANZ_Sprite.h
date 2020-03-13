#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int ANZ_RectangleCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
int ANZ_RectangleCollisionRR(SDL_Rect *r1,SDL_Rect * r2);

typedef struct
{
    int width,height;
    unsigned char * pixels_alpha;
}ANZ_SpriteMap;

int ANZ_CreateSpriteMap(ANZ_SpriteMap* sprite,SDL_Surface * surface);
void ANZ_FreeSpriteMap(ANZ_SpriteMap* sprite);

unsigned char ANZ_GetSpritePixel(ANZ_SpriteMap* sprite,int x, int y,int flip_horizontal,int flip_vertical);
unsigned char ANZ_GetSpritePixel1(ANZ_SpriteMap* sprite,int x, int y,SDL_RendererFlip sdlFlip);

int ANZ_CollisionSS(int px1,int py1,SDL_RendererFlip flip1, ANZ_SpriteMap* sprite1,
                    int px2,int py2,SDL_RendererFlip flip2, ANZ_SpriteMap* sprite2,
                    unsigned char max_alpha);

int  ANZ_CollisionRS(int x1, int y1, int w1, int h1,
	int px2, int py2, ANZ_SpriteMap* sprite2, SDL_RendererFlip flip2,
	unsigned char max_alpha);

int  ANZ_CollisionRS1(SDL_Rect*rect1,
	int px2, int py2, ANZ_SpriteMap* sprite2, SDL_RendererFlip flip2,
	unsigned char max_alpha);

#endif
