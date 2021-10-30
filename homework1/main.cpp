#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <cmath>

using namespace std;
typedef pair <int, int> Coords;
typedef vector <Coords> Dots;

Dots get_dots(const string& filename) {
    Dots temp;             
    ifstream input_file;                  
    input_file.open(filename);   

    if (!input_file.is_open()) {
        cout << "error unable to open file((" << endl;
        return temp;
    }

    int x, y;
    do {
        input_file >> x >> y;
        Coords xy_i = make_pair(x, y);
        temp.push_back(xy_i);
    } while (!input_file.eof());

    input_file.close();
    return temp;
}

double getCos(const Coords& src, const Coords& cmp) {
    auto x = src.first;
    auto y = src.second;

    auto x_i = cmp.first;
    auto y_i = cmp.second;

    double first_part = (x * x_i) + (y * y_i);
    double second_part = sqrt(x * x + y * y) * sqrt(x_i * x_i + y_i * y_i);

    double result = first_part / second_part;

    return result;
}


Dots get_result(Dots& mat) {
    Dots result;
    Coords left_result;
    Coords right_result;
    auto vect = mat[0];
    double cos_left = 1.0;
    double cos_right = 1.0;

    while (!mat.empty()) {

        Coords current_coords = mat.back();
        mat.pop_back();

        double cos = getCos(vect, current_coords);
        if (cos == -1) {
            right_result = current_coords;
            cos_right = cos;
            continue;
        }
        bool isOnTheRight;

        if (vect.first == 0) { 
            if (current_coords.first > 0) {   
                isOnTheRight = true;
            }
            else
                isOnTheRight = false;
        }
        else {
            auto x = vect.first;
            auto y = vect.second;

            auto x_i = current_coords.first;
            auto y_i = current_coords.second;

            double k = (double)y / x;

            bool isXPositive = x > 0;
            auto Y_x_i = k * x_i;
            bool is_Y_ot_Xi_Greater_than_Yi = Y_x_i > y_i;

            isOnTheRight = isXPositive ^ is_Y_ot_Xi_Greater_than_Yi;
        }

        if (isOnTheRight) {
            if (cos < cos_right) {
                cos_right = cos;
                right_result = current_coords;
            }
        }
        else {
            if (cos < cos_left) {
                cos_left = cos;
                left_result = current_coords;
            }
        }

    }
    result.push_back(left_result);
    result.push_back(right_result);

    return result;
}



int main() {
    Dots mat = get_dots("in.txt");

    Dots result = get_result(mat);

    cout << "Leftmost:\t" << result[0].first << " " << result[0].second << endl;
    cout << "Rightmost:\t" << result[1].first << " " << result[1].second << endl;

    return 0;
}

