#ifndef ANZ_IMAGE_H_INCLUDED
#define ANZ_IMAGE_H_INCLUDED

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "ANZ_Sprite.h"

int initSDL();
void quitSDL();

extern SDL_Renderer*sdlrenderer;

typedef struct
{
    SDL_Texture* texture;
    int width;
    int height;
}ANZ_Image;

ANZ_Image* ANZ_LoadImage(const char*file);
ANZ_Image* ANZ_LoadImageAndSpriteMap(const char *file, ANZ_SpriteMap * spriteMap);
ANZ_Image* ANZ_CreateImageFromSurface(SDL_Surface*surface);

ANZ_Image* ANZ_RenderTextToImage(const char*text,TTF_Font *font,SDL_Color color);
ANZ_Image* ANZ_RenderTextToImage1(const char*text,TTF_Font *font,
                unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha);

void ANZ_FreeImage(ANZ_Image*image);
void ANZ_RenderImage(ANZ_Image*image,int posX,int posY);
void ANZ_RenderImageR(ANZ_Image*image,int x, int y, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip);
void ANZ_RenderImageF(ANZ_Image*image,int posX,int posY,SDL_RendererFlip rendererFlip);
void ANZ_RenderImageX(ANZ_Image*image,int posX,int posY,double angle, SDL_Point* center);

void ANZ_DrawFillRect(int x,int y,int w,int h);

typedef struct
{
    SDL_Color strokeColor;
    SDL_Color fillColor;
    int strokeWidth;
    int x,y,w,h;
}ANZ_ImageFrame;

ANZ_ImageFrame ANZ_CreateImageFrame(void);
void ANZ_RenderFramedImage(ANZ_Image*image,int posX,int posY,ANZ_ImageFrame*frame);

#endif
