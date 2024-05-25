#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <ctime>
#define Size 10
#define radius 3
#define place_size_x 66
#define place_size_y 60
using namespace std;
constexpr float WINDOW_HEIGHT = 720.0;
constexpr float WINDOW_WIDTH = 800.0;


class Place {
public:
    int color, x_pos, y_pos;
    bool clear, bonus;
    void Fill(int _x, int _y);

};

class IsSwitchPos : public Place {
public:
    bool SetPos(int _x, int _y, int _x1, int _y1, vector< vector<Place>>& field) {
        if (abs(_x - _x1) + abs(_y - _y1) == 1) {
            return(true);
        }
        else
            return(false);
    }
};

void SetBonus(int& _x, int& _y, vector<vector<Place>> field);

void ActBomb(vector <vector <Place>>& field);

void ActRecolor(vector <vector< Place>>& field, int bonus_color, int _x, int _y);

class Bonus :public Place {
public:
    void Fiil(int _x, int _y, vector<vector<Place>>& field);
    int bonuses_type, bonus_color;
    void ActBonuses(vector <vector <Place>>& field);
};

void FillField(vector <vector<Place>>& field);

void Find(vector <vector <Place>>& field, bool& isFind, vector<Bonus>& bonuses);

void Clear(vector <vector<Place>>& field);

void Fall(vector <vector<Place>>& field);

