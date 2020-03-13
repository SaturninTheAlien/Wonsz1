#include "ANZ_Image.h"


int initSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)
    {
        fprintf(stderr,"SDL2_Error: %s\n",SDL_GetError());
        fprintf(stderr, "\"Wonsz1\" cannot be run in text-only mode.\n");
        return 0;
    }

    int flags=IMG_INIT_JPG|IMG_INIT_PNG;
    int initted=IMG_Init(flags);

    if((initted&flags) != flags)
    {
        fprintf(stderr,"SDL2_image_Error: Failed to init required jpg and png support!\n");
        fprintf(stderr,"SDL2_image_Error: %s\n", IMG_GetError());
        return 0;
    }

    if(TTF_Init()==-1)
    {
        fprintf(stderr,"SDL2_TTF_Error: %s\n",TTF_GetError());
        return 0;
    }


    return 1;
}
void quitSDL()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Renderer*sdlrenderer=0;
ANZ_Image* ANZ_LoadImage(const char*file)
{
    return ANZ_LoadImageAndSpriteMap(file,0);
}
ANZ_Image* ANZ_LoadImageAndSpriteMap(const char *file, ANZ_SpriteMap * spriteMap)
{
    SDL_Surface*surface=IMG_Load(file);
    if(surface==0)
    {
        fprintf(stderr,"SDL_Error: %s\n",IMG_GetError());
        return 0;
    }
    if(spriteMap!=0)
    {
        if(!ANZ_CreateSpriteMap(spriteMap,surface))
        {
            return 0;
        }
    }
    ANZ_Image * result= ANZ_CreateImageFromSurface(surface);
    SDL_FreeSurface(surface);
    return result;
}
ANZ_Image* ANZ_CreateImageFromSurface(SDL_Surface*surface)
{
    ANZ_Image* image=(ANZ_Image*)malloc(sizeof(ANZ_Image));
    image->width=surface->w;
    image->height=surface->h;
    image->texture=SDL_CreateTextureFromSurface(sdlrenderer,surface);
    if(image->texture==0)
    {
        fprintf(stderr, "SDL_Error:%s\n",SDL_GetError());
        free(image);
        return 0;
    }
    return image;
}
ANZ_Image* ANZ_RenderTextToImage(const char*text,TTF_Font *font,SDL_Color color)
{
    SDL_Surface* surface=TTF_RenderText_Solid(font,text,color);
    if(surface==0)
    {
        fprintf(stderr, "SDL2_TTF_Error: %s\n",TTF_GetError());
        return 0;
    }
    ANZ_Image * result = ANZ_CreateImageFromSurface(surface);
    SDL_FreeSurface(surface);
    return result;
}

ANZ_Image* ANZ_RenderTextToImage1(const char*text,TTF_Font *font,
                unsigned char red,unsigned char green,unsigned char blue,unsigned char alpha)
{
    SDL_Color color;
    color.a=alpha;
    color.r=red;
    color.g=green;
    color.b=blue;
    return ANZ_RenderTextToImage(text,font,color);
}

void ANZ_FreeImage(ANZ_Image*image)
{
    SDL_DestroyTexture(image->texture);
    free(image);
}


void ANZ_RenderImage(ANZ_Image*image,int posX,int posY)
{
    SDL_Rect renderQuad;
    renderQuad.x = posX;
    renderQuad.y = posY;
    renderQuad.w = image->width;
	renderQuad.h = image->height;
	SDL_RenderCopy(sdlrenderer,image->texture,0,&renderQuad);
}
void ANZ_RenderImageX(ANZ_Image*image,int posX,int posY,double angle, SDL_Point* center)
{
    ANZ_RenderImageR(image,posX,posY,SDL_FLIP_NONE,angle,center,0);
}
void ANZ_RenderImageF(ANZ_Image*image,int posX,int posY,SDL_RendererFlip rendererFlip)
{
    ANZ_RenderImageR(image,posX,posY,rendererFlip,0.0d,0,0);
}
void ANZ_RenderImageR(ANZ_Image*image,int x, int y, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip)
{
    SDL_Rect renderQuad;
	renderQuad.x = x;
	renderQuad.y = y;

	renderQuad.w = image->width;
	renderQuad.h = image->height;
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(sdlrenderer, image->texture, clip, &renderQuad, angle, center, flip);

}

void ANZ_DrawFillRect(int x,int y,int w,int h)
{
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    rect.w=w;
    rect.h=h;
    SDL_RenderFillRect(sdlrenderer,&rect);
}

ANZ_ImageFrame ANZ_CreateImageFrame(void)
{
    ANZ_ImageFrame result;
    result.strokeWidth=5;
    result.x=20;
    result.y=20;

    result.w=20;
    result.h=20;

    result.strokeColor.a=255;
    result.strokeColor.r=0;
    result.strokeColor.g=0;
    result.strokeColor.b=0;

    result.fillColor.a=255;
    result.fillColor.r=255;
    result.fillColor.g=255;
    result.fillColor.b=255;

    return result;
}

void ANZ_RenderFramedImage(ANZ_Image*image,int x,int y,ANZ_ImageFrame*frame)
{
	int var1 = image->width  + frame->x + frame->w;
	int var2 = image->height + frame->y + frame->h;
	if (frame->strokeWidth> 0 && frame->strokeColor.a>0)
	{
        SDL_SetRenderDrawColor(sdlrenderer,frame->strokeColor.r,frame->strokeColor.g,frame->strokeColor.b,frame->strokeColor.a);
        ANZ_DrawFillRect(x,y,var1, frame->strokeWidth);
        ANZ_DrawFillRect(x, y + frame->strokeWidth, frame->strokeWidth, var2 - frame->strokeWidth);
        ANZ_DrawFillRect(x+var1-frame->strokeWidth, y + frame->strokeWidth, frame->strokeWidth, var2 - frame->strokeWidth);
        ANZ_DrawFillRect(x+frame->strokeWidth, y+var2-frame->strokeWidth,var1- 2*frame->strokeWidth, frame->strokeWidth);

		/*anzEngine.setDrawColor(borderColor);
		anzEngine.drawFillRect(x,y,var1, borderSize);
		anzEngine.drawFillRect(x, y + borderSize, borderSize, var2 - borderSize);
		anzEngine.drawFillRect(x+var1-borderSize, y + borderSize, borderSize, var2 - borderSize);
		anzEngine.drawFillRect(x+borderSize, y+var2-borderSize,var1- 2*borderSize, borderSize);*/
	}

	if (frame->fillColor.a > 0)
	{
        SDL_SetRenderDrawColor(sdlrenderer,frame->fillColor.r,frame->fillColor.g,frame->fillColor.b,frame->fillColor.a);
        ANZ_DrawFillRect(x + frame->strokeWidth, y + frame->strokeWidth, var1 - 2 * frame->strokeWidth, var2 - 2 * frame->strokeWidth);

		/*anzEngine.setDrawColor(backgroundColor);
		anzEngine.drawFillRect(x + borderSize, y + borderSize, var1 - 2 * borderSize, var2 - 2 * borderSize);*/
	}
    ANZ_RenderImage(image,x + frame->x, y + frame->y);
}





