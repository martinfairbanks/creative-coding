	/*	Circular collision check
		by Martin Fairbanks
	*/

	#define SDL2
	#include "..\framework\creativeframework.cpp"
	//#include <vector>
	//using namespace std;
	const int32 numCircles = 20;

	struct Circle
	{
		int32 x;
		int32 y;
		int32 radius = 48;
		int32 speed = 2;
		Color col = Color(0,0,255);

		void clicked()
		{
			//check distance -> sqrt((mouseX - x) * (mouseX - x) + (mouseY - y) * (mouseY - y))
			int32 distance = dist(mouseX, mouseY, x, y);
			//if the distance is less than the radius then the mouse cursor is inside the circle
			if (distance < 48) {
				col = Color(0, 255, 0);
			}
		}
		void changeColor() 
		{
			col = Color(random(255), random(255), random(255));
		}

		bool32 intersects(Circle secondCircle)
		{
			int32 distance = dist(x, y, secondCircle.x, secondCircle.y);
			//if the distance between the circles is less than the sum of their radius
			if (distance < (radius + secondCircle.radius))
			{
				return true;
			}
			return false;
		}

		void updateAndDraw()
		{
			x += random(-speed, speed);
			fill(col);
			circle(x, y, radius);
		}

	};
	//vector<Entity*> entities;
	Circle circles[numCircles];
	void setup()
	{
		screen(960, 540, false, "click on circles to change color");
		strokeWeight(3);
		noStroke();
		for (int32 i = 0; i < numCircles; i++)
		{
			//entities.push_back(new Entity());
			circles[i].x = random(50, windowWidth-50);
			circles[i].y = random(50, windowHeight-50);
		}
	}

void updateAndDraw(uint32 t)
{
	clear(cornflowerblue);
	if (mouseIsPressed)
	{
		for (int32 i = 0; i < numCircles; i++)
		{
			circles[i].clicked();
		}
	}

	//check collision
	for (int32 i = 0; i < numCircles; i++)
	{
		for (int32 j = i+1; j < numCircles; j++)
		{
			if (circles[i].intersects(circles[j])) {
				circles[i].changeColor();
				circles[j].changeColor();
			}
		}

		circles[i].updateAndDraw();
	}
	uploadPixels();
}

void shutdown()
{
	//deallocate all reserved memory
	/*for (int i = 0; i < numCircles; i++)
		delete circles[i];*/
}