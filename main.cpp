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

void drawDigit(sf::RenderWindow &window, sf::Sprite &digits, int digit, float x, float y) {
    digits.setTextureRect(sf::IntRect(digit*21, 0, 21, 32));
    digits.setPosition(x, y);
    window.draw(digits);
}

int main() {
    sf::Clock clock;
    sf::Time pausedDuration = sf::Time::Zero;
    sf::Time pauseStartTime;
    bool ingame = false;
    bool leaderBoard = false;
    bool win = false;
    bool lose = false;
    bool pause = false;
    bool timestart = false;
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
    int flags = mineCount;
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

    sf::Texture wintex;
    if (!wintex.loadFromFile("files/images/face_win.png")) {
        std::cout << "Error loading face_win" << std::endl;
    }
    sf::Sprite winface;
    winface.setTexture(wintex);
    winface.setPosition(sf::Vector2f((colCount/2 *32)-32, 32*(rowCount+0.5)));

    sf::Texture losetex;
    if (!losetex.loadFromFile("files/images/face_lose.png")) {
        std::cout << "Error loading face_lose" << std::endl;
    }
    sf::Sprite loseface;
    loseface.setTexture(losetex);
    loseface.setPosition(sf::Vector2f((colCount/2 *32)-32, 32*(rowCount+0.5)));

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
    sf::Sprite pausesprite;
    pausesprite.setTexture(pausetex);
    pausesprite.setPosition(sf::Vector2f((colCount *32)-240, 32*(rowCount+0.5)));

    sf::Texture leadertex;
    if (!leadertex.loadFromFile("files/images/leaderboard.png")) {
        std::cout << "Error loading leaderboard" << std::endl;
    }
    sf::Sprite leadersprite;
    leadersprite.setTexture(leadertex);
    leadersprite.setPosition(sf::Vector2f((colCount *32)-176, 32*(rowCount+0.5)));

    sf::Texture tilehtex;
    if (!tilehtex.loadFromFile("files/images/tile_hidden.png")) {
        std::cout << "Error loading tile_hidden" << std::endl;
    }
    sf::Sprite tileh;
    tileh.setTexture(tilehtex);

    sf::Texture tilertex;
    if (!tilertex.loadFromFile("files/images/tile_revealed.png")) {
        std::cout << "Error loading tile_revealed" << std::endl;
    }
    sf::Sprite tiler;
    tiler.setTexture(tilertex);

    sf::Texture digittex;
    if (!digittex.loadFromFile("files/images/digits.png")) {
        std::cout << "Error loading digits" << std::endl;
    }
    sf::Sprite digits;
    digits.setTexture(digittex);

    sf::Texture minetex;
    if (!minetex.loadFromFile("files/images/mine.png")) {
        std::cout << "Error loading mine" << std::endl;
    }
    sf::Sprite mine;
    mine.setTexture(minetex);

    sf::Texture flagtex;
    if (!flagtex.loadFromFile("files/images/flag.png")) {
        std::cout << "Error loading flag" << std::endl;
    }
    sf::Sprite flag;
    flag.setTexture(flagtex);

    sf::Texture onetex;
    if (!onetex.loadFromFile("files/images/number_1.png")) {
        std::cout << "Error loading number 1" << std::endl;
    }
    sf::Sprite one;
    one.setTexture(onetex);

    sf::Texture twotex;
    if (!twotex.loadFromFile("files/images/number_2.png")) {
        std::cout << "Error loading number 2" << std::endl;
    }
    sf::Sprite two;
    two.setTexture(twotex);

    sf::Texture threetex;
    if (!threetex.loadFromFile("files/images/number_3.png")) {
        std::cout << "Error loading number 3" << std::endl;
    }
    sf::Sprite three;
    three.setTexture(threetex);

    sf::Texture fourtex;
    if (!fourtex.loadFromFile("files/images/number_4.png")) {
        std::cout << "Error loading number 4" << std::endl;
    }
    sf::Sprite four;
    four.setTexture(fourtex);

    sf::Texture fivetex;
    if (!fivetex.loadFromFile("files/images/number_5.png")) {
        std::cout << "Error loading number 5" << std::endl;
    }
    sf::Sprite five;
    five.setTexture(fivetex);

    sf::Texture sixtex;
    if (!sixtex.loadFromFile("files/images/number_6.png")) {
        std::cout << "Error loading number 6" << std::endl;
    }
    sf::Sprite six;
    six.setTexture(sixtex);

    sf::Texture seventex;
    if (!seventex.loadFromFile("files/images/number_7.png")) {
        std::cout << "Error loading number 7" << std::endl;
    }
    sf::Sprite seven;
    seven.setTexture(seventex);

    sf::Texture eightex;
    if (!eightex.loadFromFile("files/images/number_8.png")) {
        std::cout << "Error loading number 8" << std::endl;
    }
    sf::Sprite eight;
    eight.setTexture(eightex);

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
                std::string cursorname = name;
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
            while (gameWindow.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    ingame = false;
                    gameWindow.close();
                }
                if(event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        pause = !pause;
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if ((event.mouseButton.x < width && event.mouseButton.x > 0) &&
                                (event.mouseButton.y < height && event.mouseButton.y > 0)) {
                            if (!timestart) {
                                timestart = true;
                                clock.restart();
                            }
                        } else if ((event.mouseButton.x < colCount*32 - 240 && event.mouseButton.x > colCount*32 - 304) &&
                            (event.mouseButton.y < 32*(rowCount+0.5) + 64 && event.mouseButton.y > 32*(rowCount+0.5))) {
                            win = !win;
                            flags--;
                        } else if ((event.mouseButton.x < colCount*32 - 176 && event.mouseButton.x > colCount*32 - 240) &&
                                (event.mouseButton.y < 32*(rowCount+0.5) + 64 && event.mouseButton.y > 32*(rowCount+0.5))) {
                            pause = !pause;
                        } else if ((event.mouseButton.x < colCount*32 - 112 && event.mouseButton.x > colCount*32 - 176) &&
                                        (event.mouseButton.y < 32*(rowCount+0.5) + 64 && event.mouseButton.y > 32*(rowCount+0.5))) {
                            std::cout << "Open leader board screen" << std::endl;
                            leaderBoard = true;
                            pause = true;
                        }
                    }
                }
            }

            gameWindow.clear(sf::Color::White);

            if (!timestart) {
                for (int row = 0; row < rowCount; row++) {
                    for (int col = 0; col < colCount; col++) {
                        tileh.setPosition(col*32, row*32);
                        gameWindow.draw(tileh);
                    }
                }
            }

            if (!pause) {
                pauseStartTime = clock.getElapsedTime();
                gameWindow.draw(pausesprite);
            } else if (pause) {
                pausedDuration = clock.getElapsedTime() - pauseStartTime;
                gameWindow.draw(play);
                for (int row = 0; row < rowCount; row++) {
                    for (int col = 0; col < colCount; col++) {
                        tiler.setPosition(col*32, row*32);
                        gameWindow.draw(tiler);
                    }
                }
            }

            sf::Time gameTime = timestart ? clock.getElapsedTime() - pausedDuration : sf::Time::Zero;
            int time = static_cast<int>(gameTime.asSeconds());
            int mins = time / 60;
            int seconds = time % 60;
            drawDigit(gameWindow, digits, mins/10, (colCount*32)-97, 32*(rowCount+0.5) + 16); //tens minutes
            drawDigit(gameWindow, digits, mins%10, (colCount*32)-76, 32*(rowCount+0.5) + 16); //ones minutes
            drawDigit(gameWindow, digits, seconds/10, (colCount*32)-54, 32*(rowCount+0.5) + 16); //tens seconds
            drawDigit(gameWindow, digits, seconds%10, (colCount*32)-33, 32*(rowCount+0.5) + 16); //ones seconds

            //flag counter
            drawDigit(gameWindow, digits, abs(flags)/100 %10, 33, 32*(rowCount+0.5) + 16);
            drawDigit(gameWindow, digits, abs(flags)/10 %10, 54, 32*(rowCount+0.5) + 16);
            drawDigit(gameWindow, digits, abs(flags)%10, 75, 32*(rowCount+0.5) + 16);
            if (flags < 0) {
                drawDigit(gameWindow, digits, 10, 12, 32*(rowCount+0.5) + 16);
            }

            if (!lose && !win) {
                gameWindow.draw(happy);
            } else if (win) {
                gameWindow.draw(winface);
            } else if (lose) {
                gameWindow.draw(loseface);
            }
            gameWindow.draw(debug);
            gameWindow.draw(leadersprite);
            gameWindow.display();

            if (leaderBoard) {
                sf::RenderWindow leaderBoardWindow(sf::VideoMode(colCount*16, rowCount*16 + 50), "Leader Board", sf::Style::Close);

                while (leaderBoardWindow.isOpen()) {
                    sf::Event levent;
                    while(leaderBoardWindow.pollEvent(levent)) {
                        if(levent.type == sf::Event::Closed) {
                            leaderBoard = false;
                            pause = false;
                            leaderBoardWindow.close();
                        }
                        if(levent.type == sf::Event::KeyPressed) {
                            if (levent.key.code == sf::Keyboard::Escape) {
                                std::cout << "Closing leader board screen" << std::endl;
                                leaderBoard = false;
                                pause = false;
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