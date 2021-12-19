#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

const double G = -9.8;
typedef vector<pair<double, double>> Walls;

class Trajectory {
public:

    Trajectory(const string& filename);

    pair<double, double> velocity;
    Walls walls_;
    double h;
    double x = 0;

    int interval = 0;
    int fall_interval;
    int size = 0;
    string filename_;

    int findInterval();

};

Trajectory::Trajectory(const string& filename) {
    filename_ = filename;

    fall_interval = findInterval();
    std::cout << fall_interval << std::endl;
}

int Trajectory::findInterval() {

    std::ifstream infile(filename_);
    infile >> h;
    infile >> velocity.first >> velocity.second;

    while (1) {
        int target = 0;
        
        if (velocity.first < 0) {
            target = interval - 1;
            return target;
        }

        if (velocity.first > 0) {
            target = interval;
        }

        if (target > size - 1) {
            double wall_x, wall_h;
            if (infile >> wall_x >> wall_h) {
                walls_.push_back(std::make_pair(wall_x, wall_h));
                size++;
            }

            else {
                return size;
            }

        }

        double t = fabs((x - walls_[target].first) / velocity.first);

        x = walls_[target].first;
        h = h + velocity.second * t + (G * t * t) / 2.0;
        velocity.second += G * t;

        if (h <= 0) {
            return interval;
        }

        if (h > walls_[target].second) {
            if (velocity.first < 0) {
                interval--;
            }

            if (velocity.first > 0) {
                interval++;
            }
        }

        else {
            velocity.first *= -1;
        }
    }
    
}


int main(int argc, char** argv) {

    if (argc == 2)
        Trajectory test(argv[1]);

    return 0;
}
