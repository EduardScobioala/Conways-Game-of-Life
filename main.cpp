#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

int neighbours_count(int i_id, int j_id, vector<vector<int>> &cells);
void alive_or_dead_update(vector<vector<int>> &cells);
void lines_drawing(int hr_lines, int vr_lines, Vertex horizontal_lines[][2], Vertex vertical_lines[][2], RenderWindow &window);
void cells_drawing(int hr_lines, int vr_lines, int width, int height, vector<vector<int>> &cells, RenderWindow &window);
void life_creator(int hr_lines, int vr_lines, int width, int height, vector<vector<int>> &cells, RenderWindow &window);

int main()
{
    // GAME'S CONSTANTS
    int VR_LINES = 80;
    int HR_LINES = 40;
    unsigned int width = 1600;
    unsigned int height = 800;
    bool GAME_ON = false;

    sf::RenderWindow window(sf::VideoMode(width, height), "Game of Life");
    window.setFramerateLimit(5);

    // VERTICAL LINES
    Vertex vertical_lines[VR_LINES-1][2];
    for (int i = 0; i < VR_LINES-1; i++)
    {
        vertical_lines[i][0] = Vertex{Vector2f(width / VR_LINES * (i+1), 0)};
        vertical_lines[i][1] = Vertex{Vector2f(width / VR_LINES * (i+1), height)};
    }

    // HORIZONTAL LINES
    Vertex horizontal_lines[HR_LINES-1][2];
    for (int i = 0; i < HR_LINES-1; i++)
    {
        horizontal_lines[i][0] = Vertex{Vector2f(0, height / HR_LINES * (i+1))};
        horizontal_lines[i][1] = Vertex{Vector2f(width, height / HR_LINES * (i+1))};
    }

    // CELLS VECTOR
    vector<vector<int>> cells;
    for (int i = 0; i < HR_LINES; i++)
    {
        vector<int> vect;
        for (int j = 0; j < VR_LINES; j++)
            vect.push_back(0);

        cells.push_back(vect);
    }

    // INITIAL CONFIGURATION
    //cells[6][6] = 1;

    // // TEXT CARACTERISTICS
    // // Declare and load a font
    // sf::Font font;
    // font.loadFromFile("fonts/audio_font.ttf");
    // // Create a text
    // sf::Text text;
    // text.setString("HELLO");
    // text.setFont(font);
    // text.setCharacterSize(30);
    // text.setStyle(sf::Text::Bold);
    // text.setFillColor(sf::Color::Red);

    // GAME RENDERING
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        if (Keyboard::isKeyPressed(Keyboard::Space))
            GAME_ON = !GAME_ON;

        // UPDATE THE GAME STATE
        if (!GAME_ON) life_creator(HR_LINES, VR_LINES, width, height, cells, window);
        else alive_or_dead_update(cells);

        // CLEAR THE LAST FRAME
        window.clear(Color(213, 204, 255, 100));

        // DRAW THE NEXT FRAME
        lines_drawing(HR_LINES, VR_LINES, horizontal_lines, vertical_lines, window);
        cells_drawing(HR_LINES, VR_LINES, width, height, cells, window);

        window.display();
    }

    return 0;
}

int neighbours_count(int i_id, int j_id, vector<vector<int>> &cells)
{
    int hr_lines = cells.size();
    int vr_lines = cells[0].size();

    int neighbours = 0;
    if (i_id >= 1 && cells[i_id-1][j_id]) neighbours++;
    if (j_id >= 1 && cells[i_id][j_id-1]) neighbours++;
    if (i_id < hr_lines-1 && cells[i_id+1][j_id]) neighbours++;
    if (j_id < vr_lines-1 && cells[i_id][j_id+1]) neighbours++;
    if (i_id >= 1 && j_id >= 1 && cells[i_id-1][j_id-1]) neighbours++;
    if (i_id >= 1 && j_id < vr_lines-1 && cells[i_id-1][j_id+1]) neighbours++;
    if (i_id < hr_lines-1 && j_id >= 1 && cells[i_id+1][j_id-1]) neighbours++;
    if (i_id < hr_lines-1 && j_id < vr_lines-1 && cells[i_id+1][j_id+1]) neighbours++;

    return neighbours;
}

void alive_or_dead_update(vector<vector<int>> &cells)
{
    int hr_lines = cells.size();
    int vr_lines = cells[0].size();

    // NEIGHBOUR COUT
    vector<int> neighbours_vector;

    for (int i = 0; i < hr_lines; i++)
        for (int j = 0; j < vr_lines; j++)
            neighbours_vector.push_back(neighbours_count(i, j, cells));

    // LIVE OR DIE
    for (int i = 0; i < hr_lines; i++)
        for (int j = 0; j < vr_lines; j++)
        {
            int n = neighbours_vector[i * vr_lines + j];
            if (n < 2) cells[i][j] = 0;
            else if (n > 3) cells[i][j] = 0;
            else if (n == 3) cells[i][j] = 1;
        }
}

void lines_drawing(int hr_lines, int vr_lines, Vertex horizontal_lines[][2], Vertex vertical_lines[][2], RenderWindow &window)
{
    // horizontal lines
    for (int i = 0; i < hr_lines-1; i++)
        window.draw(horizontal_lines[i], 2, Lines);
    // vertical lines
    for (int i = 0; i < vr_lines-1; i++)
        window.draw(vertical_lines[i], 2, Lines);
}

void cells_drawing(int hr_lines, int vr_lines, int width, int height, vector<vector<int>> &cells, RenderWindow &window)
{
    for (int i = 0; i < hr_lines; i++)
        for (int j = 0; j < vr_lines; j++)
        {
            if (cells[i][j])
            {
                RectangleShape square(sf::Vector2f(width / vr_lines, height / hr_lines));
                square.setFillColor(Color(128, 0, 21, 255));
                square.setPosition(width / vr_lines * j, height / hr_lines * i);
                window.draw(square);
            }
        }
}

void life_creator(int hr_lines, int vr_lines, int width, int height, vector<vector<int>> &cells, RenderWindow &window)
{
    bool button_pressed = false;

    if (Mouse::isButtonPressed(Mouse::Left)) button_pressed = true;

    if (button_pressed)
    {
        float x_coordonate = Mouse::getPosition(window).x;
        float y_coordonate = Mouse::getPosition(window).y;

        int x_cell_id = x_coordonate / (width / vr_lines);
        int y_cell_id = y_coordonate / (height / hr_lines);

        cells[y_cell_id][x_cell_id] = !cells[y_cell_id][x_cell_id];
        button_pressed = false;
    }
}
