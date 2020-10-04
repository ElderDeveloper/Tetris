#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <time.h>

#define TileLoop for(int i=0; i<4; i++)

using namespace sf;


const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point
{
	int x, y;
} a[4], b[4];

int figures[7][4] =
{
	1,3,5,7, //I
	2,4,5,7, //Z
	3,5,4,7, //S
	3,5,4,7, //T
	2,3,5,7, //L
	3,5,7,6, //J
	2,3,4,5, //O
};



bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)return 0;

		else if (field[a[i].y][a[i].x]) return 0;

	return 1;
};





int main()
{
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(320, 480), "This is my title bar");
	window.setVerticalSyncEnabled(true);

	Texture tile, background, frame;
	tile.loadFromFile("images/tiles.png");
	background.loadFromFile("images/background.png");
	frame.loadFromFile("images/frame.png");

	Sprite sprite(tile), bgsprite(background), frsprite(frame);


	int dx = 0; bool rotate = 0; int colorNum = 1;
	float timer = 0; float delay = 0.3f;

	Clock clock;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;


		// <<<<<<<<<<<<<<<<<<  Events >>>>>>>>>>>>>>>
		sf::Event windowevent;
		while (window.pollEvent(windowevent))
		{
			switch (windowevent.type)
			{
			case Event::Closed:
				window.close();
				break;


				//// <<<<<< Inputs >>>>> ////
			case Event::KeyPressed:
				switch (windowevent.key.code)
				{
				case Keyboard::Up:
					rotate = true;
					break;
				case Keyboard::Left:
					dx = -1;
					break;
				case Keyboard::Right:
					dx = 1;
					break;
				case Keyboard::Down:
					delay = 0.05f;
					break;
				default:
					break;
				}
				break;

			default:
				break;

			}
		}

		// <<<<<<<<< Move >>>>>>>>>>
		TileLoop{ b[i] = a[i];	a[i].x += dx; }
		if (!check())	TileLoop{ a[i] = b[i]; }



			// <<<<<<<<< Rotate >>>>>>>>
			if (rotate)
			{
				Point p = a[1]; // center of rotation

				for (int i = 0; i < 4; i++)
				{
					int x = a[i].y - p.y;
					int y = a[i].x - p.x;
					a[i].x = p.x - x;
					a[i].y = p.y + y;
				}
				if (!check())	TileLoop{ a[i] = b[i]; }

			}

		// <<<<<<<<<< Tick >>>>>>>>>>
		if (timer > delay)
		{
			TileLoop{ b[i] = a[i];	a[i].y += 1; }


				if (!check())
				{
					TileLoop{ field[b[i].y][b[i].x] = colorNum; }

					colorNum = 1 + rand() % 7;
					int n = rand() % 7;
					for (int i = 0; i < 4; i++)
					{
						a[i].x = figures[n][i] % 2;
						a[i].y = figures[n][i] / 2;
					}
				}
			timer = 0;
		}



		// <<<<<<<<<<<<<<<<<<< Check Lines >>>>>>>>>>>>>>>>>
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j])	count++;
				field[k][j] = field[i][j];

			}
			if (count < N) k--;
		}


		dx = 0;	rotate = 0;


		// <<<<<<<<<<<<<< DRAW >>>>>>>>>>>>>>>>>>
		window.clear(Color::White);
		window.draw(bgsprite);

		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0) continue;
				sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				sprite.setPosition(j * 18.0f, i * 18.0f);
				sprite.move(28, 31); //offset
				window.draw(sprite);
			}

		for (int i = 0; i < 4; i++)
		{
			sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			sprite.setPosition(a[i].x * 18.0f, a[i].y * 18.0f);
			sprite.move(28, 31); //offset
			window.draw(sprite);
		}
		delay = 0.3;
		window.draw(frsprite);
		window.display();
	}

	return 0;
}