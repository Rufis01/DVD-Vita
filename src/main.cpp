#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/rng.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

constexpr auto SCREEN_W = 960;
constexpr auto SCREEN_H = 544;

int getRandomInt(int max, int min = 0)
{
	SceUInt random;
	sceKernelGetRandomNumber(&random, 4);
	return ((random % (max - min)) + min);
}

void changeColor(SDL_Texture* tTexture)
{
	//There seems to be only a few fixed colors in the original tho.
	//If you want to keep it like this, you must add some saturation checks (hsv to rgb seems like a good option)
	SceUInt8 r = getRandomInt(256, 50);
	SceUInt8 g = getRandomInt(256, 50);
	SceUInt8 b = getRandomInt(256, 50);

	SDL_SetTextureColorMod(tTexture, r, g, b);
}

int main(int argc, char* argv[])
{
	//TODO: Add Checks
	//TODO: Fix the logo (remove the TradeMark logo), also, it's DVD Video!!!
	SDL_Window* wWindow = SDL_CreateWindow("https://www.youtube.com/watch?v=dQw4w9WgXcQ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, 0);
	SDL_Renderer* rRenderer = SDL_CreateRenderer(wWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Surface* sDvdSur = IMG_Load("app0:assets/img/dvdLogo.png");
	SDL_Texture* tDvdTex = SDL_CreateTextureFromSurface(rRenderer, sDvdSur);
	SDL_FreeSurface(sDvdSur);

	SDL_Rect rDvdSrc = { 0, 0, 960, 425 };
	SDL_Rect rDvdDst = { 0, 0, 200, 89 };
	int iVelX = 3;
	int iVelY = iVelX;


	SDL_SetRenderDrawBlendMode(rRenderer, SDL_BLENDMODE_ADD);


	rDvdDst.x = getRandomInt(SCREEN_W - rDvdDst.w);
	rDvdDst.y = getRandomInt(SCREEN_H - rDvdDst.h);

	while (1)
	{
		//CLEAN THE SCREEN
		SDL_RenderClear(rRenderer);

		rDvdDst.x += iVelX;
		rDvdDst.y += iVelY;

		if (rDvdDst.x <= 0 || rDvdDst.x + rDvdDst.w >= SCREEN_W)
		{
			iVelX = -iVelX;
			changeColor(tDvdTex);
		}

		if (rDvdDst.y <= 0 || rDvdDst.y + rDvdDst.h >= SCREEN_H)
		{
			iVelY = -iVelY;
			changeColor(tDvdTex);
		}

		//Should reposition the rect properly or it might get stuck, apparently.

		//DRAW
		SDL_RenderCopy(rRenderer, tDvdTex, &rDvdSrc, &rDvdDst);
		SDL_RenderPresent(rRenderer);
	}


	sceKernelExitProcess(0);
	return 0;
}