#include "Header.h"

void Place::Fill(int _x, int _y) {
    color = rand() % 6;
    clear = false;
    bonus = false;
    x_pos = _x * place_size_x;
    y_pos = _y * place_size_y;
}

void SetBonus(int& _x, int& _y, vector<vector<Place>> field) {
    int rad_type_x, rad_type_y, rad_x, rad_y;
    do {
        do
        {
            rad_type_x = pow(-1, rand() % 2 + 1);
            rad_type_y = pow(-1, rand() % 2 + 1);
            rad_x = rad_type_x * rand() % (radius + 1);
            rad_y = rad_type_y * (radius - abs(rad_x));
            _x += rad_x;
            _y += rad_y;
        } while ((_x > Size) || (_y > Size) || (_x < 0) || (_y < 0));
    } while (field[_x][_y].bonus == true);
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

void ActRecolor(vector <vector< Place>>& field, int bonus_color, int _x, int _y) {
    int temp_x, temp_y;
    field[_x][_y].color = bonus_color;
    for (int i = 0; i < 2; i++) {
        do
        {
            temp_x = rand() % Size;
            temp_y = rand() % Size;
        } while ((abs(temp_x - _x) + abs(temp_y - _y) < 2) || (field[temp_x][temp_y].color == bonus_color));
        field[temp_x][temp_y].color = bonus_color;
    }
}

void Bonus::Fiil(int _x, int _y, vector<vector<Place>>& field) {
    bonuses_type = rand() % 2;
    if (bonuses_type == 0) {
        bonus_color = field[_x][_y].color;
    }
    SetBonus(_x, _y, field);
    field[_x][_y].bonus = true;
    x_pos = field[_x][_y].x_pos;
    y_pos = field[_x][_y].y_pos;
}

void Bonus::ActBonuses(vector <vector <Place>>& field) {
    if (bonuses_type == 1) {
        field[x_pos / place_size_x][y_pos / place_size_y].clear = true;
        ActBomb(field);
    }
    else {
        ActRecolor(field, bonus_color, x_pos / place_size_x, y_pos / place_size_y);
    }
    field[y_pos / place_size_y][x_pos / place_size_x].bonus = false;
}

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

void Find(vector <vector <Place>>& field, bool& isFind, vector<Bonus>& bonuses) {
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
                        bonuses[bonuses.size() - 1].Fiil(i, j, field);
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

