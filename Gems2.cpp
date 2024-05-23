#include "Header.h"

sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gems");

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
                gems.move(70-place_size_x,-50+place_size_y);
                window.draw(gems);
            }
        }
    }
    for (int i = 0; i < bon.size(); i++) {
        tbon.setTextureRect(sf::IntRect(bon[i].bonuses_type * 66, 0, 66, 60));
        tbon.setPosition(bon[i].x_pos, bon[i].y_pos);
        tbon.move(70-place_size_x, -50 + place_size_y);
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
;                        if (Temp.SetPos(pos_1.x / place_size_x, pos_1.y / place_size_y, pos_2.x / place_size_x, pos_2.y / place_size_y, field)) {
                            int temp;
                            temp = field[pos_2.y / place_size_y][pos_2.x / place_size_x].color;
                            field[pos_2.y / place_size_y][pos_2.x / place_size_x].color = field[pos_1.y / place_size_y][pos_1.x / place_size_x].color;
                            field[pos_1.y / place_size_y][pos_1.x / place_size_x].color = temp;
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