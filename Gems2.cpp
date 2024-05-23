#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <ctime>
#define Size 10
#define radius 3
using namespace std;
constexpr float WINDOW_HEIGHT = 720.0;
constexpr float WINDOW_WIDTH = 800.0;
sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gems"); 
sf::Vector2i place_size(66,60);

class Place {
public:  
    int color, x_pos, y_pos; 
    bool clear,bonus;
    void Fill(int _x, int _y) {
        color = rand() % 6 ;
        clear = false;
        bonus = false;
        x_pos=_x* place_size.x;
        y_pos=_y* place_size.y;
    }
};

void SetBonus(int &_x, int& _y,vector<vector<Place>> field) {
    int rad_type_x, rad_type_y,rad_x,rad_y;
    do {
        do
        {
            rad_type_x = pow(-1, rand() % 2 + 1);
            rad_type_y = pow(-1, rand() % 2 + 1);
            rad_x = rad_type_x * rand() % (radius + 1);
            rad_y = rad_type_y * (radius - abs(rad_x));
            _x += rad_x;
            _y += rad_y;
        } while ((_x > Size) || (_y > Size) || (_x<0) || (_y<0));
    }while(field[_x][_y].bonus==true);
}

void ActBomb(vector <vector <Place>>& field) {
    int temp_x, temp_y;
    for (int i = 0; i < 4; i++) {
        do {
            temp_x = rand() % Size;
            temp_y = rand() % Size;
        } while ((field[temp_x][temp_y].clear) || (field[temp_x][temp_y].bonus));
        field[temp_x][temp_y].clear = true;
    }
}

void ActRecolor(vector <vector< Place>>& field, int bonus_color,int _x, int _y) {
    int temp_x, temp_y;
    field[_x][_y].color = bonus_color;
    for (int i = 0; i < 2; i++) {
        do
        {
            temp_x = rand() % Size;
            temp_y = rand() % Size;
        } while ((abs(temp_x-_x)+abs(temp_y-_y)<2)||(field[temp_x][temp_y].color==bonus_color));
        field[temp_x][temp_y].color = bonus_color;
    }
}

class IsSwitchPos : public Place {
public:
    bool SetPos(int _x, int _y, int _x1, int _y1,vector< vector<Place>> & field) {
        if (abs(_x - _x1) + abs(_y - _y1) == 1) {
            return(true);
        }
        else
            return(false);
    }
};

class Bonus :public Place {
public:
    int bonuses_type,bonus_color;
    void Fiil(int _x, int _y, vector<vector<Place>> & field) {
        bonuses_type = rand() % 2;
        if (bonuses_type == 0) {
            bonus_color = field[_x][_y].color;
        }
        SetBonus(_x, _y,field);
        field[_x][_y].bonus = true;
        x_pos = field[_x][_y].x_pos;
        y_pos = field[_x][_y].y_pos;
    }
    void ActBonuses(vector <vector <Place>>& field) {
        if (bonuses_type == 1) {
            field[x_pos/ place_size.x][y_pos/ place_size.y].clear = true;
            ActBomb(field);
        }
        else {
            ActRecolor(field, bonus_color,x_pos/place_size.x,y_pos/place_size.y);
        }
        field[y_pos / place_size.y][x_pos / place_size.x].bonus = false;
    }
};

void FillField(vector <vector<Place>>& field) {
    field.resize(Size);
    for (int i = 0; i < Size; i++) {
        field[i].resize(Size);
    }
    for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) {
            field[i][j].Fill(j, i);
        }
    }
}

void Find(vector <vector <Place>>& field, bool& isFind, vector<Bonus> &bonuses) {
    int count = 0;
    for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size - 2; j++) {
            if (field[i][j].color == field[i][j + 1].color)
                if (field[i][j].color == field[i][j + 2].color) {
                    isFind = true;
                    count = 3;
                    for (int k = j + 3; k < Size; k++) {
                        if (field[i][j].color == field[i][k].color)
                            count++;
                        else
                            break;
                    }
                    if (count > 3) {
                        bonuses.resize(bonuses.size() + 1);
                        bonuses[bonuses.size() - 1].Fiil(i, j, field);
                    }
                    for (int k = 0; k < count; k++) {
                        field[i][j + k].clear = true;
                    }
                }
            count = 0;
        }
    }
    for (int i = 0; i < Size - 2; i++) {
        for (int j = 0; j < Size; j++) {
            if (field[i][j].color == field[i + 1][j].color)
                if (field[i][j].color == field[i + 2][j].color) {
                    isFind = true;
                    count = 3;
                    for (int k = i + 3; k < Size; k++) {
                        if (field[i][j].color == field[k][j].color)
                            count++;
                        else
                            break;
                    }
                    if (count > 3) {
                        bonuses.resize(bonuses.size() + 1);
                        bonuses[bonuses.size()-1].Fiil(i,j,field);
                    }
                    for (int k = 0; k < count; k++) {
                        field[i + k][j].clear = true;
                    }
                }
            count = 0;
        }
    }
}

void Clear(vector <vector<Place>>& field) {
    for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) {
            if ((field[i][j].clear) && (field[i][j].bonus == false)) {
                field[i][j].color = -1;
                field[i][j].clear = false;
            }
        }
    }
}

void Fall(vector <vector<Place>>& field) {
    for (int i = Size - 1; i >= 0; i--) {
        for (int j = 0; j < Size; j++) {
            if (field[i][j].color == -1)
                for (int k = i - 1; k >= 0; k--) {
                    if (field[k][j].color != -1) {
                        swap(field[i][j].color, field[k][j].color);
                        break;
                    }
                }
        }
    }
    int k = 0;
    for (int i = 0; i < Size; i++) {
        k = 0;
        while (k < Size)
        {
            if (field[k][i].color != -1)
                break;
            field[k][i].Fill(i, k);
            k++;
        }
    }
}

void Display(vector <vector <Place>> field,vector <Bonus> bon) {
    sf::Texture textureGems;
    textureGems.loadFromFile("texture.png");
    sf::Sprite gems(textureGems);
    sf::Texture textureBon;
    textureBon.loadFromFile("TexBonuses1.png");
    sf::Sprite tbon(textureBon);
    for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) {
            if ((!field[i][j].clear)&&(!field[i][j].bonus)) {
                gems.setTextureRect(sf::IntRect(field[i][j].color * 66, 0, 66, 60));
                gems.setPosition(field[i][j].x_pos, field[i][j].y_pos);
                gems.move(70-place_size.x,-50+place_size.y);
                window.draw(gems);
            }
        }
    }
    for (int i = 0; i < bon.size(); i++) {
        tbon.setTextureRect(sf::IntRect(bon[i].bonuses_type * 66, 0, 66, 60));
        tbon.setPosition(bon[i].x_pos, bon[i].y_pos);
        tbon.move(70-place_size.x, -50 + place_size.y);
        window.draw(tbon);
    }
    window.display();
    window.clear();
}

int main()
{
    srand(time(0));
    vector<Bonus> bonuses;
    vector <vector <Place>> field;
    FillField(field);
    bool OutPut=true,isFind=true;
    sf::Vector2i pos_1, pos_2;
    int Click = 0;
    while (window.isOpen())
    {                   
        sf::Event close,SwapElements;

        while (window.pollEvent(SwapElements))
        {
            if (SwapElements.key.code == sf::Mouse::Left) {
                Click++;
                if (Click == 1) {
                    pos_1 = sf::Mouse::getPosition(window);
                }
                else
                    {
                        pos_2 = sf::Mouse::getPosition(window);
                        IsSwitchPos Temp;
;                        if (Temp.SetPos(pos_1.x / place_size.x, pos_1.y / place_size.y, pos_2.x / place_size.x, pos_2.y / place_size.y, field)) {
                            int temp;
                            temp = field[pos_2.y / place_size.y][pos_2.x / place_size.x].color;
                            field[pos_2.y / place_size.y][pos_2.x / place_size.x].color = field[pos_1.y / place_size.y][pos_1.x / place_size.x].color;
                            field[pos_1.y / place_size.y][pos_1.x / place_size.x].color = temp;
                            isFind = true;
                        }       
                        Display(field,bonuses);
                        Click = 0;
                        pos_1 = { 0,0 };
                        pos_2 = { 0,0 };
                    }
            }
        } 
        while (window.pollEvent(close))
        {
            if (close.type == sf::Event::Closed)
                window.close();
        }
        while (isFind) {   
            isFind = false;
            for (int i = 0; i < bonuses.size(); i++) {
                bonuses[i].ActBonuses(field);
                sf::sleep(sf::seconds(1));
                isFind = true;
            }
            bonuses.clear();

            Display(field, bonuses);
            Find(field, isFind,bonuses);
            if (isFind) {
                Clear(field);
                sf::sleep(sf::seconds(0.7));
                Display(field, bonuses);
                Fall(field);
                sf::sleep(sf::seconds(0.2));
            }
        }
    }
    return 0;
}   