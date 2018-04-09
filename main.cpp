#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

using namespace sf;
using namespace std;

int WIDTH = 600;
int HEIGHT = 600;
int scale = 3;
int maxScale = 10;
int lineWidth = 4;
bool inMenu = true;

string int_to_string(int i){
    stringstream ss;
    ss<<i;
    return ss.str();
}

class Point{
public:
    int x, y;
    string s;
    Text text;
    Point(int _x, int _y, string _s, Font* font){
        x = _x;
        y = _y;
        s = _s;
        text.setFont(*font);
        text.setCharacterSize(75);
        text.setFillColor(Color::Black);
        text.setString(s);
        FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
        text.setPosition(Vector2f(x * (WIDTH / scale) + (WIDTH / scale) / 2 , y * (HEIGHT / scale) + (HEIGHT / scale) / 2));
    }

    void draw(RenderWindow* window){
        window->draw(text);
    }
};

class Button{
public:
    int x, y, id, width, height;
    Text buttonText;
    RectangleShape body;
    Button(int _x, int _y, int _width, int _height, string s, Font* font, int fontSize, Color textColor, Color bodyColor, int _id){
        x = _x;
        y = _y;
        id = _id;
        width = _width;
        height = _height;
        body = RectangleShape(Vector2f(width, height));
        body.setPosition(Vector2f(x, y));
        body.setFillColor(bodyColor);
        buttonText.setFont(*font);
        buttonText.setCharacterSize(fontSize);
        buttonText.setFillColor(textColor);
        buttonText.setString(s);
        FloatRect textRect = buttonText.getLocalBounds();
        buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
        buttonText.setPosition(Vector2f(x + width / 2.0f, y + height / 2.0f));

    }

    void draw(RenderWindow* window){
        window->draw(body);
        window->draw(buttonText);
    }

    void click(){
        if(id == 0 && scale < maxScale){
            scale++;
        }else if(id == 1 && scale > 3){
            scale--;
        }else if(id == 2){
            inMenu = false;
        }
    }
};

void drawField(RenderWindow* window){
    for(int i = 0; i < scale; ++i){
        for(int j = 0; j < scale; ++j){
            RectangleShape piece(Vector2f(WIDTH / scale - lineWidth, HEIGHT / scale - lineWidth));
            piece.setFillColor(Color::White);
            piece.setPosition(Vector2f(WIDTH / scale * i + lineWidth / 2, HEIGHT / scale * j + lineWidth / 2));
            window->draw(piece);
        }
    }
}

string checkWinCondition(vector<Point>* points){
    int** field = new int*[scale];
    for(int i = 0; i < scale; ++i){
        field[i] = new int[scale];
    }
    for(int i = 0; i < scale; ++i){
        for(int j = 0; j < scale; ++j){
            field[i][j] = 0;
        }
    }
    for(int i = 0; i < points->size(); ++i){
        field[points->at(i).x][points->at(i).y] = points->at(i).s == "O" ? 1 : 2;
    }
    string stck1 = "";
    string stck2 = "";
    for(int i = 0; i < scale; ++i){
        stck1 = "";
        stck2 = "";
        for(int j = 0; j < scale; ++j){
            stck1 += field[i][j] == 0 ? "-" : (field[i][j] == 1 ? "O" : "X");
            stck2 += field[j][i] == 0 ? "-" : (field[j][i] == 1 ? "O" : "X");
        }
        if(stck1.find("OOO") != stck1.npos) return "O";
        if(stck1.find("XXX") != stck1.npos) return "X";
        if(stck2.find("OOO") != stck2.npos) return "O";
        if(stck2.find("XXX") != stck2.npos) return "X";
    }
    stck1 = "";
    stck2 = "";
    for( int k = 0 ; k < scale * 2 ; k++ ) {
        stck1 = "";
        stck2 = "";
        for( int j = 0 ; j <= k ; j++ ) {
            int i = k - j;
            if( i < scale && j < scale) {
                stck1 += field[scale - i - 1][j] == 0 ? "-" : (field[scale - i - 1][j] == 1 ? "O" : "X");
                stck2 += field[i][j] == 0 ? "-" : (field[i][j] == 1 ? "O" : "X");
            }
        }
        if(stck1.find("OOO") != stck1.npos) return "O";
        if(stck1.find("XXX") != stck1.npos) return "X";
        if(stck2.find("OOO") != stck2.npos) return "O";
        if(stck2.find("XXX") != stck2.npos) return "X";
    }
    return "-";
}

int main()
{
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "SFML works!");
    Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
       cout << "Failed to load font!" << endl;
    }
    Button add(350, 275, 50, 50, "+", &font, 35, Color::White, Color::Black, 0);
    Button sub(200, 275, 50, 50, "-", &font, 35, Color::White, Color::Black, 1);
    Button start(200, 350, 200, 50, "Start", &font, 35, Color::White, Color::Black, 2);

    int turn = 0;
    string won = "-";

    Button buttons[3] = {add, sub, start};

    Text scaleText;
    scaleText.setFont(font);
    scaleText.setCharacterSize(35);
    scaleText.setFillColor(Color::Black);
    scaleText.setString(int_to_string(scale));
    FloatRect textRect = scaleText.getLocalBounds();
    scaleText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    scaleText.setPosition(Vector2f(WIDTH / 2.0f,HEIGHT / 2.0f));

    RectangleShape winPanel(Vector2f(400, 150));
    winPanel.setOutlineColor(Color::Black);
    winPanel.setOutlineThickness(4);
    textRect = winPanel.getLocalBounds();
    winPanel.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    winPanel.setPosition(Vector2f(WIDTH / 2.0f, HEIGHT / 2.0f));

    Text winText;
    winText.setFont(font);
    winText.setColor(Color::Black);
    winText.setString(won);
    winText.setCharacterSize(100);
    textRect = winText.getLocalBounds();
    winText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    winText.setPosition(Vector2f(WIDTH / 2.0f, HEIGHT / 2.0f - 40));

    Text restartText;
    restartText.setFont(font);
    restartText.setColor(Color::Black);
    restartText.setString("Press 'R' to restart!");
    restartText.setCharacterSize(40);
    textRect = restartText.getLocalBounds();
    restartText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    restartText.setPosition(Vector2f(WIDTH / 2.0f, HEIGHT / 2.0f + 40));

    vector<Point> points;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Left){
                    if(inMenu){
                        for(int i = 0; i < 3; i++){
                            if(event.mouseButton.x < buttons[i].x + buttons[i].width && event.mouseButton.x > buttons[i].x
                               && event.mouseButton.y < buttons[i].y + buttons[i].height && event.mouseButton.y > buttons[i].y){
                                buttons[i].click();
                                scaleText.setString(int_to_string(scale));
                                textRect = scaleText.getLocalBounds();
                                scaleText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
                                scaleText.setPosition(Vector2f(WIDTH / 2.0f,HEIGHT / 2.0f));
                                break;
                            }
                        }
                    }else if(won == "-"){
                        int x = event.mouseButton.x / (WIDTH / scale);
                        int y = event.mouseButton.y / (HEIGHT / scale);
                        bool pass = false;
                        for(int i = 0; i < points.size(); ++i){
                            if(points.at(i).x == x && points.at(i).y == y){
                                pass = true;
                            }
                        }
                        if(pass) continue;
                        Point p = Point(x, y, turn % 2 == 0 ? "O" : "X", &font);
                        points.push_back(p);
                        won = checkWinCondition(&points);
                        if(won == "+"){
                            winText.setString("Draw!");
                            textRect = winText.getLocalBounds();
                            winText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
                            winText.setPosition(Vector2f(WIDTH / 2.0f, HEIGHT / 2.0f - 30));
                        }else if(won != "-"){
                            winText.setString(won + " wins!");
                            textRect = winText.getLocalBounds();
                            winText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
                            winText.setPosition(Vector2f(WIDTH / 2.0f, HEIGHT / 2.0f - 30));
                        }
                        turn++;
                    }
                }
            }

            if(event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::R) && !inMenu){
                inMenu = true;
                turn = 1;
                points.clear();
                won = "-";
            }
        }

        if(inMenu){
            window.clear(Color::White);
            add.draw(&window);
            sub.draw(&window);
            start.draw(&window);
            window.draw(scaleText);
        }else{
            window.clear(Color::Black);
            drawField(&window);
            for(int i = 0; i < points.size(); ++i){
                points.at(i).draw(&window);
            }
            if(won != "-"){
                window.draw(winPanel);
                window.draw(winText);
                window.draw(restartText);
            }
        }
        window.display();
    }

    return 0;
}
