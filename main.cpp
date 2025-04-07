#include <iostream>
#include <fstream>
#include <cctype>
#include <SFML/Graphics.hpp>

void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {
    sf::Clock clock;
    bool ingame = false;
    bool leaderBoard = false;
    std::string name = "";
    bool cursor = false;
    int cursorpos = 0;
    std::ifstream config("files/config.cfg");
    if (!config.is_open()) {
        std::cout << "Error opening config" << std::endl;
        return -1;
    }
    int colCount, rowCount, mineCount;
    config >> colCount >> rowCount >> mineCount;
    int width = colCount*32;
    int height = rowCount*32 + 100;
    int tileCount = colCount * rowCount;
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        std::cout << "Error loading font" << std::endl;
        return -1;
    }
    std::ifstream leaderfile("files/leaderboard.txt");
    if (!leaderfile.is_open()) {
        std::cout << "Error opening leaderboard" << std::endl;
        return -1;
    }
    std::string leadersls;
    std::string line;
    int linenum = 1;
    while (getline(leaderfile, line) && linenum < 6) {
        int commapos = line.find(',');
        if (commapos != std::string::npos) {
            line[commapos] = '\t';
        }
        leadersls += std::to_string(linenum) + ".\t" + line + "\n\n";
        linenum++;
    }

    sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Welcome!", sf::Style::Close);

    sf::Text welcome("WELCOME TO MINESWEEPER!", font, 24);
    welcome.setFillColor(sf::Color::White);
    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcome, width/2, height/2 - 150);

    sf::Text enterName("Enter your name:", font, 20);
    enterName.setFillColor(sf::Color::White);
    enterName.setStyle(sf::Text::Bold);
    setText(enterName, width/2, height/2 - 75);

    sf::Text nameText(name + "|", font, 18);
    nameText.setFillColor(sf::Color::White);
    nameText.setStyle(sf::Text::Bold);
    setText(nameText, width/2, height/2 - 45);

    sf::Text leaderboard("LEADERBOARD", font, 20);
    leaderboard.setFillColor(sf::Color::White);
    leaderboard.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leaderboard, (colCount*16)/2, (rowCount*16 + 50)/2 - 120);

    sf::Text leaders(leadersls, font, 18);
    leaders.setFillColor(sf::Color::White);
    leaders.setStyle(sf::Text::Bold);
    setText(leaders, (colCount*16)/2, (rowCount*16 + 50)/2 + 20);

    sf::Texture happytex;
    if (!happytex.loadFromFile("files/images/face_happy.png")) {
        std::cout << "Error loading face_happy" << std::endl;
    }
    sf::Sprite happy;
    happy.setTexture(happytex);
    happy.setPosition(sf::Vector2f((colCount/2 *32)-32, 32*(rowCount+0.5)));

    sf::Texture debugtex;
    if (!debugtex.loadFromFile("files/images/debug.png")) {
        std::cout << "Error loading debug" << std::endl;
    }
    sf::Sprite debug;
    debug.setTexture(debugtex);
    debug.setPosition(sf::Vector2f((colCount *32)-304, 32*(rowCount+0.5)));

    sf::Texture playtex;
    if (!playtex.loadFromFile("files/images/play.png")) {
        std::cout << "Error loading play" << std::endl;
    }
    sf::Sprite play;
    play.setTexture(playtex);
    play.setPosition(sf::Vector2f((colCount *32)-240, 32*(rowCount+0.5)));

    sf::Texture pausetex;
    if (!pausetex.loadFromFile("files/images/pause.png")) {
        std::cout << "Error loading pause" << std::endl;
    }
    sf::Sprite pause;
    pause.setTexture(pausetex);
    pause.setPosition(sf::Vector2f((colCount *32)-240, 32*(rowCount+0.5)));

    sf::Texture leadertex;
    if (!leadertex.loadFromFile("files/images/leaderboard.png")) {
        std::cout << "Error loading leaderboard" << std::endl;
    }
    sf::Sprite leadersprite;
    leadersprite.setTexture(leadertex);
    leadersprite.setPosition(sf::Vector2f((colCount *32)-176, 32*(rowCount+0.5)));

    while (welcomeWindow.isOpen()) {
        sf::Event event;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                ingame = false;
                welcomeWindow.close();
            }

            if(event.type == sf::Event::TextEntered) {
                char typedChar = static_cast<char>(event.text.unicode);

                if (((typedChar >= 'a' && typedChar <= 'z') || (typedChar >= 'A' && typedChar <= 'Z')) && name.size() < 10) {
                    name += typedChar;
                    cursorpos++;
                    name[0] = std::toupper(name[0]);
                    if (name.size() > 1) {
                        for (int i = 1; i < name.size(); i++) {
                            name[i] = std::tolower(name[i]);
                        }
                    }
                }
            }
            if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::BackSpace && !name.empty()) {
                    name.pop_back();
                    cursorpos--;
                } else if (event.key.code == sf::Keyboard::Delete && name.size() > cursorpos) {
                    name.erase(name.begin() + cursorpos);
                } else if (event.key.code == sf::Keyboard::Right && cursorpos < name.size()) {
                    cursorpos++;
                } else if (event.key.code == sf::Keyboard::Left && cursorpos > 0) {
                    cursorpos--;
                } else if (event.key.code == sf::Keyboard::Enter && name.size() > 0) {
                    std::cout << "Change screen to in game" << std::endl;
                    ingame = true;
                    welcomeWindow.close();
                } else if (event.key.code == sf::Keyboard::Escape) {
                    ingame = false;
                    welcomeWindow.close();
                }
            }
        }

        welcomeWindow.clear(sf::Color::Blue);

        if (clock.getElapsedTime() >= sf::milliseconds(500)) {
            clock.restart();
            cursor = !cursor;
            if (cursor) {
                std:: string cursorname = name;
                cursorname.insert(cursorpos, "|");
                nameText.setString(cursorname);
            } else {
                nameText.setString(name);
            }
        }

        welcomeWindow.draw(welcome);
        welcomeWindow.draw(enterName);
        setText(nameText, width/2, height/2 - 45);
        welcomeWindow.draw(nameText);

        welcomeWindow.display();
    }
    while (ingame) {
        sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
        sf::Cursor hand;
        if (hand.loadFromSystem(sf::Cursor::Hand)) {
            welcomeWindow.setMouseCursor(hand);
        }

        while (gameWindow.isOpen()) {
            sf::Event event;
            while(gameWindow.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    ingame = false;
                    gameWindow.close();
                }
                if(event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        std::cout << "Open leader board screen" << std::endl;
                        leaderBoard = true;
                    }
                }
            }

            gameWindow.clear(sf::Color::White);
            gameWindow.draw(happy);
            gameWindow.draw(debug);
            gameWindow.draw(play);
            gameWindow.draw(leadersprite);
            gameWindow.display();

            if (leaderBoard) {
                sf::RenderWindow leaderBoardWindow(sf::VideoMode(colCount*16, rowCount*16 + 50), "Leader Board", sf::Style::Close);

                while (leaderBoardWindow.isOpen()) {
                    sf::Event levent;
                    while(leaderBoardWindow.pollEvent(levent)) {
                        if(levent.type == sf::Event::Closed) {
                            leaderBoard = false;
                            leaderBoardWindow.close();
                        }
                        if(levent.type == sf::Event::KeyPressed) {
                            if (levent.key.code == sf::Keyboard::Escape) {
                                std::cout << "Closing leader board screen" << std::endl;
                                leaderBoard = false;
                                leaderBoardWindow.close();
                            }
                        }
                    }

                    leaderBoardWindow.clear(sf::Color::Blue);

                    leaderBoardWindow.draw(leaderboard);
                    leaderBoardWindow.draw(leaders);

                    leaderBoardWindow.display();
                }
            }
        }
    }
    return 0;
}