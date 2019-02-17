#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
 
int main(int argc, char* argv[])
{
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;

 	std::string path;

	std::cout << "Please enter your path file > ";
	std::cin >> path;

	std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> spriteSheet(IMG_Load(path.data()), &SDL_FreeSurface);

	if (spriteSheet == nullptr)
	{
		std::cerr << "Connot laod your file : " << IMG_GetError() << std::endl;
		SDL_Quit();
		return -1;
	} 

	auto nSpriteW = 0;
	auto nSpriteH = 0;

	std::cout << "Enter the number of the sprite in width : ";
	while (!(std::cin >> nSpriteW) || nSpriteW <= 0)
	{
		std::cout << "Enter the number of the sprite in width : ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	
	std::cout << "Enter the number of the sprite in height : ";
	while (!(std::cin >> nSpriteH) || nSpriteH <= 0)
	{
		std::cout << "Enter the number of the sprite in height : ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	auto w = spriteSheet->w / nSpriteW;
	auto h = spriteSheet->h / nSpriteH;
 

	std::vector<SDL_Rect> position;
 
	for (auto i{ 0 }; i < spriteSheet->h; i += w)
	{
		for (auto j{ 0 }; j < spriteSheet->w; j += w)
		{
			SDL_Rect pos;
			pos.x = j;
			pos.y = i;
			pos.w = w;
			pos.h = h;

			position.push_back(pos);
		}
	}

	auto i = 0;
 
	for (auto& r : position)
	{
		SDL_Rect rect{ r.x, r.y, r.w, r.h};
		std::ostringstream stream;
		i++;
		stream << i << ".png";

		std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> outputSurface{ SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0), &SDL_FreeSurface };

		if (outputSurface == nullptr)
			std::cerr << "Surface creation error : " << IMG_GetError() << std::endl;
		
		SDL_BlitSurface(spriteSheet.get(), &rect, outputSurface.get(), nullptr);

		IMG_SavePNG(outputSurface.get(), stream.str().data());
  	}

	SDL_Quit();
	return 0;
}