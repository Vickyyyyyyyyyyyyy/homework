#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cmath>
#include <io.h>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

const double EarthGravity = 9.8;

//Èñïîëüçóåì êîíòåéíåð ñ êëþ÷îì õ è çíà÷åíèåì ó
typedef std::map<double, double> Walls;
typedef std::map<double, int> Interval;

//Ñòðóêòóðà - êîìïîíåíòû ñêîðîñòè ÌÒ
typedef struct Velocity {
	double v_x;
	double v_y;
};

class Ball {
public:
	Ball() {}
	Ball(const std::string& filename);

	std::ifstream file;
	~Ball() { file.close(); }

	Walls walls;
	Interval interval;
	Velocity velocity;

	bool isInInterval();
	void setVelocity();
	void BuildWalls();
	void KudaUpal();

private:
	double h0;
	double x0;

	double ymove, wall_x, wall_y, number_of_interval; //êîñòûëè

	inline double time(double x, double x0) { return (x - x0) / velocity.v_x; }
	inline double xMove(double t) { return x0 + velocity.v_x * t; }
	inline double yMove(double t) { return h0 + velocity.v_y * t - (EarthGravity * t * t) / 2; }
};

Ball::Ball(const std::string& filename) {
	x0 = 0;
	number_of_interval = 0;
	file.open(filename);
	setVelocity();
	BuildWalls();
}

bool Ball::isInInterval()
{
	return true;
}

void Ball::setVelocity() {
	file >> h0;
	file >> velocity.v_x >> velocity.v_y;
	std::cout << "v_x v_y set: " << velocity.v_x << " " << velocity.v_y << std::endl;
}

void Ball::BuildWalls() {
	double tmpx, tmpy;
	walls.insert(std::make_pair(-1, -1)); /*åùå îäèí êîñòûëü*/
	int i = 0;
	do {
		file >> tmpx >> tmpy;
		walls.insert(std::make_pair(tmpx, tmpy));
		interval.insert(std::make_pair(tmpx, i));
		i++;
	} while (!file.eof());

	std::cout << "walls succesfully built... " << std::endl;
} 

void Ball::KudaUpal() {

	//Ïîëåò âïðàâî -> -> ->
	std::map<double, double>::iterator it1;
	for (it1 = std::next(walls.begin()); it1 != walls.end(); ++it1) {
		number_of_interval++;
		if (yMove(time(it1->first, x0)) < it1->second && yMove(time(it1->first, x0)) > 0) { //åñëè y_øàðèêà < h0
			wall_x = it1->first; wall_y = it1->second;
			ymove = yMove(time(it1->first, x0)); //çàäàåì âûñîòó ñòîëêíîâåíèÿ
			std::cout << "Ball hitted wall with x = " << it1->first << std::endl;
			std::cout << "visota udara " << yMove(time(it1->first, x0)) << std::endl;

			h0 = yMove(time(it1->first, x0)); //çàäàåì íîâóþ h0 êàê âûñîòó ñòîëêíîâåíèÿ
			x0 = it1->first; //çàäàåì íîâûé x0 êàê x_ñòåíêè
			break; //äàëüøå ïåðåáèðàòü ñòåíêè íàì íå íóæíî - âûõîäèì èç öèêëà
		}

		else if (yMove(time(it1->first, x0)) < 0)
		{
			std::cout << it1->first << std::endl;
			std::cout << yMove(time(it1->first, x0)) << std::endl;
			std::cout << " n_of_i -> " << interval.find(it1->first)->second << std::endl;
			return;
		}
	}
	
	//Ñ ïîìîùüþ èòåðàòîðà ÷èñòèì íàøå äåðåâî îò ñòåíîê, ñòîÿùèõ ïðàâåå òîé, ñ êîòîðîé áûëî ñòîëêíîâåíèå
	//erase äåëàåì òàê, ÷òîáû èòåðàòîð íå òåðÿëñÿ è íå óêàçûâàë íà ïóñòîé áëîê ïàìÿòè
	//Ïîñëå ÷èñòêè ëèøíèå ñòåíêè óäàëèëèñü ñïðàâà è ìû ìîæåì èäòè ñ êîíöà äåðåâà
	std::map<double, double>::iterator it2 = std::next(it1);
	while (it2 != walls.end()) {
		if (it2->first != NULL)
			it2 = walls.erase(it2);
		else
			it2++;
	}

	//Ïîëåò âëåâî <- <- <-
 	if (ymove < wall_y && ymove > 0) {
		std::map<double, double>::reverse_iterator it3;
		for (it3 = std::next(walls.rbegin()); it3 != walls.rend(); ++it3) {
			number_of_interval--;
			velocity.v_y = velocity.v_y - EarthGravity * time(it3->first, x0);
			if (yMove(time(it3->first, x0)) < it3->second && yMove(time(it3->first, x0)) > 0) {
				wall_x = it3->first; wall_y = it3->second;
				ymove = yMove(time(it3->first, x0));
				std::cout << "Ball hitted another wall with x = " << it3->first << std::endl;
				std::cout << "visota udara " << yMove(time(it3->first, x0)) << std::endl;

				h0 = yMove(time(it3->first, x0));
				x0 = it3->first;
				velocity.v_x = -velocity.v_x;
				velocity.v_y = velocity.v_y - EarthGravity * time(it3->first, x0);

				number_of_interval--;
				break; //Íàøëè ñòîëêíîâåíèå - âûõîäèì èç öèêëà
			}

			else if (yMove(time(it3->first, x0)) < 0)
			{
				std::cout << " n_of_i <- " << interval.find(it1->first)->second << std::endl;
				return;
			}
		}

		//Ïî òåì òåì æå ïðàâèëàì ÷èñòèì ñòåíêè ëåâåå, èñïîëüçóÿ ðåâåðñèâíûé èòåðàòîð êîíòåéíåðà
		std::map<double, double>::reverse_iterator it4 = std::next(it3);
		while (it4 != walls.rend()) {
			if (it4->first != NULL)
			{
				walls.erase(std::next(it4).base());
			}
			else
				it4++;
		} 
	} 

	if (ymove < wall_y && ymove > 0) {
		std::cout << " list of walls after 2nd hit: " << std::endl;
		for (auto ii = walls.begin(); ii != walls.end(); ++ii)
			std::cout << ii->first << " " << ii->second << std::endl;
		KudaUpal();
	}

}

int main(void) {
	Ball ball("in.txt");
	ball.KudaUpal();

	return 0;
}
