#include <SFML/Graphics.hpp>
#include <time.h>
#include <string.h>
#include <chrono>
#include <thread>

// Program constants
int const XSide = 8; int const XS = XSide+2;
int const YSide = 8; int const YS = YSide+4;
int const difficulty = 7;

// Function provide random bomb positions
void getRandom(int grid[][YS], int sgrid[][YS])
{
    srand(time(NULL));
    for (int i=1;i<(XS-1);i++)
        for (int j=3;j<(YS-1);j++)
            {
                sgrid[i][j]=10; // Set default block for showing
                if (rand()%difficulty == 0)  grid[i][j] = 9;
                else grid[i][j]=0;
            }
}

// Function calculate quantity of nearest bombs
void getMines(int grid[][YS])
{
    int n=0;
    for (int i=1;i<(XS-1);i++)
        for (int j=3;j<(YS-1);j++)
            {
                n=0;                        //0
                if (grid[i][j]==9) continue;
                if (grid[i+1][j]==9) n++;   //1
                if (grid[i][j+1]==9) n++;   //2
                if (grid[i-1][j]==9) n++;   //3
                if (grid[i][j-1]==9) n++;   //4
                if (grid[i+1][j+1]==9) n++; //5
                if (grid[i-1][j-1]==9) n++; //6
                if (grid[i-1][j+1]==9) n++; //7
                if (grid[i+1][j-1]==9) n++; //8
                grid[i][j]=n;
            }
}

int main() {

    int const w = 32;
    float thk = w/16;

    int grid[XS][YS] = {0};  // set of bombs and numbers
    int sgrid[XS][YS] = {0}; // for showing

    float XFrame = w*XSide+2*w*0.25;
    float YFrame = w*(YSide+1)+3*w*0.25;

    // Input message
    std::string const Meeting = "Minesweeper, by Dmytro Kovryzhenko!";

    // Winning message
    std::string const winMessage = "You won!";

    // Losing message
    std::string const losMessage = "You lose((";

    // Restart button message
    std::string const ResMe = "Restart";

    sf::RenderWindow app(sf::VideoMode(XFrame, YFrame), Meeting, sf::Style::Titlebar | sf::Style::Close);
    app.clear(sf::Color::Black);

    // Load textures for cells
    sf::Texture t;
    t.loadFromFile("images/tiles.jpg");
    sf::Sprite s(t);

    // Load textures for Restart Button
    sf::Texture resTexture;

    // Initilize score texture
    sf::RectangleShape scrField;
    scrField.setSize( sf::Vector2f( (4*w-2*thk),(w-2*thk) ) );
    scrField.setFillColor(sf::Color::Black);
    scrField.setOutlineColor( sf::Color(192,192,192,100) );
    scrField.setOutlineThickness(thk);
    scrField.setPosition( (XFrame-0.25*w-4*w+thk) , (0.25*w+thk) );

    // Load font
    sf::Font scrFont;
    scrFont.loadFromFile("fonts/ariali.ttf");

    // Initilize lostScore score text
    sf::Text lostScore;
    lostScore.setFont(scrFont);
    lostScore.setCharacterSize(0.65*w);
    lostScore.setStyle(sf::Text::Bold);
    lostScore.setPosition( (XFrame-0.25*w-3.6*w+thk) , (0.25*w+thk) );
    lostScore.setFillColor(sf::Color::Red);
    lostScore.setString(losMessage);

    // Initilize winsScore score text
    sf::Text winsScore;
    winsScore.setFont(scrFont);
    winsScore.setCharacterSize(0.65*w);
    winsScore.setStyle(sf::Text::Bold);
    winsScore.setPosition( (XFrame-0.25*w-3.5*w+thk) , (0.25*w+thk) );
    winsScore.setFillColor(sf::Color::Green);
    winsScore.setString(winMessage);

    bool lost;
    bool wins;
    bool stob;

    bool restart;

    int ask;
    int rsk;
    int flg;

    int x = 0;
    int y = 0;

    while ( app.isOpen() ) {

    std::this_thread::sleep_for(std::chrono::microseconds(90000));

    resTexture.loadFromFile("images/restartbutton.jpg");
    sf::Sprite restartButton(resTexture);
    restartButton.setTextureRect( sf::IntRect(0,0,3*w,w) );
    restartButton.setPosition( (0.25*w) , (0.25*w) );

    restart = false;
    lost = false;
    wins = false;
    stob = false;

    // Setting bombs to random places
    getRandom(grid,sgrid);

    // Setting quantity of nearest bombs to the cells
    getMines(grid);

        // Game session before restart
        while ( app.isOpen() && !restart ) {

            /* Get mouse cursor position */ {
            sf::Vector2i pos = sf::Mouse::getPosition(app);
            // Calculate non-liner cursor grid position by "X"
            x = (pos.x<=(0.25*w))? ( (pos.x-0.25*w)/w ) : ( (pos.x-0.25*w)/w+1 ) ;
            // Calculate non-liner cursor grid position by "Y"
            y = (pos.y<=(0.25*w))? ( (pos.y-0.25*w)/w ) :
            (pos.y<=(0.25*w+w))? ( (pos.y-0.25*w)/w+1 ) :
            (pos.y<=(2*0.25*w+w))? ( (pos.y-0.25*w-w)/w+2 ) : ( (pos.y-2*0.25*w-w)/w+3 ) ;
            }

            /* Game window event */ {
            sf::Event gaming;
                while (app.pollEvent(gaming)) {
                        // Close application if user demand
                        if ( gaming.type == sf::Event::Closed ) {app.close();}
                        // React by mouse buttons pressed
                        if ( gaming.type == sf::Event::MouseButtonPressed )
                        {
                            // Show consistent when left button pressed and no flag, yet
                            if ( (sf::Mouse::isButtonPressed(sf::Mouse::Left) ) && (sgrid[x][y] == 10) ) sgrid[x][y] = grid[x][y];
                            // Put flag when right button pressed and no flag, yet
                            if ( (sf::Mouse::isButtonPressed(sf::Mouse::Right) ) && (sgrid[x][y] == 10) ) sgrid[x][y] = 11;
                            // Remove flag when left button pressed and flag here already
                            if ( (sf::Mouse::isButtonPressed(sf::Mouse::Left) ) && (sgrid[x][y] == 11) ) sgrid[x][y] = 10;
                            // Restart game when user hit "Restart" button
                            if ( (sf::Mouse::isButtonPressed(sf::Mouse::Left) ) && ( (x >= 1) && (x <= 3) ) && (y == 1) )
                                {
                                    // Load another texture for pressed button
                                    resTexture.loadFromFile("images/respre.jpg");
                                    sf::Sprite restartButton(resTexture);
                                    restartButton.setTextureRect( sf::IntRect(0,0,3*w,w) );
                                    restartButton.setPosition( (0.25*w) , (0.25*w) );
                                    app.draw(restartButton);
                                    // Set binary signal of "Restart" to true
                                    restart = true;
                                }
                        }
                }
            }

            /* Draw game window */ {
                // 1) Draw restart button
                app.draw(restartButton);

                // 2) Draw score field
                app.draw(scrField);

                    // 2.1) Write lost message
                    if ( lost ) app.draw(lostScore);

                    // 2.2) Write win message
                    if ( wins ) app.draw(winsScore);

                // 3) Create game window cells
                for (int i=1;i<(XS-1);i++)
                    for (int j=3;j<(YS-1);j++) {

                        // Every single cell drawing one-by-one
                        s.setTextureRect( sf::IntRect(sgrid[i][j]*w,0,w,w) );
                        s.setPosition( ( (i-1)*w+0.25*w ) , ( (j-2)*w+2*0.25*w ));
                        app.draw(s);

                        // If bomb appeared - show every hidden cell
                        if ( (sf::Mouse::isButtonPressed(sf::Mouse::Left)) && (sgrid[x][y] == 9) ) sgrid[i][j] = grid[i][j];
                    }
            }

            /* Refresh window */ {
            app.display();
            }

            /* Count stopped bombs */ {
                ask = 0;
                rsk = 0;
                flg = 0;

                stob = false;

                for (int i=1;i<(XS-1);i++)
                    for (int j=3;j<(YS-1);j++) {
                        if ( grid[i][j] == 9 ) {
                            ask++;                          // Count every bomb in the field
                            if (sgrid[i][j] == 11) rsk++;   // Count successfully stoped bomb
                        }
                        if ( sgrid[i][j] == 11 ) flg++;     // Count all flags in the field
                    }

                // Generate binary TRUE signal if all bombs are stoped
                // and there is now foolish flag
                if ( (rsk == ask) && (rsk == flg) ) stob = true;

                // Otherwise, continue to play
                else stob = false;
            }

            /* Request for LOOSE */ {
                if ( (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        && (sgrid[x][y] == 9)
                        && !wins )
                    /* Forbid to open WIN window */ {
                    lost = true;
                    }
            }

            /* Request for WIN */ {
                if  (
                        (
                            (
                             (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                             &&
                             (sgrid[x][y] == 11)
                            )
                            ||
                            (
                             (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                             &&
                             (sgrid[x][y] != 9)
                            )
                        )
                        && stob
                        && !lost
                      )
                        /* Forbid to open LOOSE window */ {
                        wins = true;
                        // Draw result
                        for (int i=1;i<(XS-1);i++)
                            for (int j=3;j<(YS-1);j++)
                                if (sgrid[i][j] != 11) sgrid[i][j] = grid[i][j];
                        }
            }
        }
    }

return 0;
}
