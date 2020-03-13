#include "ANZ_Sprite.h"

int ANZ_RectangleCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	return x2 <= x1 + w1 && x1 <= x2 + w2
         && y2 <= y1 + h1 && y1 <= y2 + h2;
}

int ANZ_RectangleCollisionRR(SDL_Rect *r1,SDL_Rect * r2)
{
    return r2->x <= r1->x + r1->w && r1->x <= r2->x + r2->w &&
            r2->y <= r1->y + r1->h && r1->y <= r2->y + r2->h;
}

int ANZ_CreateSpriteMap(ANZ_SpriteMap* sprite,SDL_Surface * surface)
{
    sprite->width=surface->w;
    sprite->height=surface->h;

    Uint32 * pixels = (Uint32*)surface->pixels;
    Uint8 red, green, blue, alpha;
	Uint32 pixelColor;

	int size,i;

	size = sprite->width* sprite->height;
	if(size<=0)return 0;

	sprite->pixels_alpha = (unsigned char*)malloc(size);

	if(sprite->pixels_alpha == 0)
	{
        fprintf(stderr,"[Error] Not enough memory\n");
        sprite->width=-1;
        sprite->height=-1;
        return 0;
    }
	for (i = 0; i < size; i++)
    {
		pixelColor = pixels[i];
        SDL_GetRGBA(pixelColor, surface->format, &red, &green, &blue, &alpha);
		sprite->pixels_alpha[i] = alpha;
    }

   /* printf("sprite->width = %i\n,sprite->height = %i\n",sprite->width,sprite->height);*/

    return 1;
}

void ANZ_FreeSpriteMap(ANZ_SpriteMap* sprite)
{
    sprite->width=-1;
    sprite->height=-1;
    free(sprite->pixels_alpha);
}

unsigned char ANZ_GetSpritePixel(ANZ_SpriteMap* sprite,int x, int y,int flip_horizontal,int flip_vertical)
{
    if (flip_horizontal)x = sprite->width - x - 1;
	if (flip_vertical)y = sprite->height - y - 1;

	if (x >= 0 && x < sprite->width && y >= 0 && y < sprite->height)
	{
		return sprite->pixels_alpha[y*sprite->width + x];
	}
	else
	{
		fprintf(stderr, "Kurwa mać\n");
		return 0;
	}
}

unsigned char ANZ_GetSpritePixel1(ANZ_SpriteMap* sprite,int x, int y,SDL_RendererFlip sdlFlip)
{
    int flip_horizontal = sdlFlip == SDL_FLIP_HORIZONTAL;
    int flip_vertical = sdlFlip == SDL_FLIP_VERTICAL;
    return ANZ_GetSpritePixel(sprite,x,y,flip_horizontal,flip_vertical);
}

int ANZ_CollisionSS(int px1,int py1,SDL_RendererFlip flip1, ANZ_SpriteMap* sprite1,
                    int px2,int py2,SDL_RendererFlip flip2, ANZ_SpriteMap* sprite2,
                    unsigned char max_alpha)
{
    if (ANZ_RectangleCollision(px1, py1, sprite1->width, sprite1->height, px2, py2, sprite2->width, sprite2->height))
	{

		int x_min1;
		int x_min2;

		if (px1 > px2)
		{
			x_min1 = 0;
			x_min2 = px1 - px2;
		}
		else
		{
			x_min1 = px2 - px1;
			x_min2 = 0;
		}

		int y_min1;
		int y_min2;

		if (py1 > py2)
		{
			y_min1 = 0;
			y_min2 = py1 - py2;
		}
		else
		{
			y_min1 = py2 - py1;
			y_min2 = 0;
		}

		int w_max = sprite1->width - x_min1;
		int w_max1 = sprite2->width - x_min2;
		if (w_max1 < w_max)w_max = w_max1;


		int h_max = sprite1->height - y_min1;
		int h_max1 = sprite2->height - y_min2;
		if (h_max1 < h_max) h_max = h_max1;


		int i, j;
		for (i = 0; i < w_max; i++)
		{
			for (j = 0; j < h_max; j++)
			{
				if (ANZ_GetSpritePixel(sprite1,x_min1 + i, y_min1 + j,flip1 == SDL_FLIP_HORIZONTAL,flip1==SDL_FLIP_VERTICAL) > max_alpha
					&& ANZ_GetSpritePixel(sprite2,x_min2 + i, y_min2 + j, flip2 == SDL_FLIP_HORIZONTAL, flip2 == SDL_FLIP_VERTICAL) > max_alpha)
					return 1;
			}
		}
	}
	return 0;
}
int  ANZ_CollisionRS(int x1, int y1, int w1, int h1,
	int px2, int py2, ANZ_SpriteMap* sprite2, SDL_RendererFlip flip2,
	unsigned char max_alpha)
{


	if (ANZ_RectangleCollision(x1, y1, w1, h1, px2, py2, sprite2->width,sprite2->height))
	{

		int x_min1;
		int x_min2;

		if (x1 > px2)
		{
			x_min1 = 0;
			x_min2 = x1 - px2;
		}
		else
		{
			x_min1 = px2 - x1;
			x_min2 = 0;
		}

		int y_min1;
		int y_min2;

		if (y1 > py2)
		{
			y_min1 = 0;
			y_min2 = y1 - py2;
		}
		else
		{
			y_min1 = py2 - y1;
			y_min2 = 0;
		}

		int w_max = w1 - x_min1;
		int w_max1 = sprite2->width - x_min2;
		if (w_max1 < w_max)w_max = w_max1;


		int h_max = h1 - y_min1;
		int h_max1 = sprite2->height - y_min2;
		if (h_max1 < h_max) h_max = h_max1;


		int i, j;
		for (i = 0; i < w_max; i++)
		{
			for (j = 0; j < h_max; j++)
			{
				if (ANZ_GetSpritePixel(sprite2,x_min2 + i, y_min2 + j, flip2 == SDL_FLIP_HORIZONTAL, flip2 == SDL_FLIP_VERTICAL) > max_alpha)
					return 1;
			}
		}
	}
	return 0;
}

int  ANZ_CollisionRS1(SDL_Rect*rect1,
	int px2, int py2, ANZ_SpriteMap* sprite2, SDL_RendererFlip flip2,
	unsigned char max_alpha)
{
    return ANZ_CollisionRS(rect1->x,rect1->y,rect1->w,rect1->h,px2,py2,sprite2,flip2,max_alpha);
}

