#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "../../class/UIutils.h"
#include <string>
#include <sstream>
#include <istream>
#include <fstream>

using namespace std;
using namespace sf;

typedef Vector2f vec2;

RenderWindow window(VideoMode(612, 512), "Matrice generator");
Event e;
int width = 0;
int height = 0;
bool isDefined = false;
bool isDrawing = false;
bool showGrid = true;
bool isClearing = false;
Font font;
vector<vector<int>> mat;
UIutils ui;

void generateMat();
string toStr(int val);
int toInt(string str);
float clamp(float v, float minv, float maxv);
void outputMat();
void outputMatToFile();

int main()
{
    if(!font.loadFromFile("res/font.ttf"))
        cout << "error, couldn't load font.ttf";
    ui.font = font;
    vector<string> choices;
    for(int i = 0; i < 100; i++)
        choices.push_back(toStr(i));

    ui.addButton(vec2(512, 0), vec2(100, 50), generateMat, "update");
    ui.addButton(vec2(512, 50), vec2(100, 80), outputMat, "print \nMat");
    ui.addButton(vec2(512, 130), vec2(100, 80), outputMatToFile, "save \nMat");
    ui.addDropDown(vec2(512, 210), vec2(100, 50), choices, 5, "width");
    ui.addDropDown(vec2(512, 260), vec2(100, 50), choices, 5, "height");

    while(window.isOpen())
    {
        while(window.pollEvent(e))
        {
            ui.updateElements(e, window);
            if(e.type == Event::Closed)
                window.close();
            if(e.type == Event::MouseButtonPressed)
            {
                if(e.mouseButton.button == Mouse::Left)
                    isDrawing = true;
                else if(e.mouseButton.button == Mouse::Right)
                    isClearing = true;
            }
            else if(e.type == Event::MouseButtonReleased)
            {
                if(e.mouseButton.button == Mouse::Left)
                    isDrawing = false;
                else if(e.mouseButton.button == Mouse::Right)
                    isClearing = false;
            }
        }

        window.clear(Color::Black);
        if(mat.size() > 0)
        {
            float ratioSize = min({512 / mat.size(), 512 / mat[0].size()});
            vec2 matPos = vec2(floor(clamp(Mouse::getPosition(window).x / ratioSize, 0, mat[0]w.size())),
                               floor(clamp(Mouse::getPosition(window).y / ratioSize, 0, mat.size())));
            mat[matPos.y][matPos.x] = isDrawing ? 1 : isClearing ? 0 : mat[matPos.y][matPos.x];
            for(int i = 0; i < mat.size(); i++)
            {
                for(int j = 0; j < mat[i].size(); j++)
                {

                    VertexArray line(LineStrip, 2);
                    line[0].color = Color::White;
                    line[1].color = Color::White;
                    line[0].position = vec2(0, i * ratioSize);
                    line[1].position = vec2(ratioSize * (512 / ratioSize), i * ratioSize);
                    window.draw(line);
                    line[0].position = vec2(j * ratioSize, 0);
                    line[1].position = vec2(j * ratioSize, ratioSize * (512 / ratioSize));
                    window.draw(line);
                    if(mat[i][j] == 1)
                    {
                        RectangleShape rect(vec2(ratioSize, ratioSize));
                        rect.setFillColor(Color::White);
                        rect.setPosition(j * ratioSize, i * ratioSize);
                        window.draw(rect);
                    }
                }
            }
        }
        ui.displayElements(window);
        window.display();
    }
    return 0;
}

void generateMat()
{
    width = toInt(ui.dropDowns[0].value);
    height = toInt(ui.dropDowns[1].value);
    mat.clear();
    vector<int> row;
    for(int i = 0; i < width; i++)
        row.push_back(0);
    for(int j = 0; j < height; j++)
        mat.push_back(row);
}

float clamp(float v, float minv, float maxv)
{
    if(v > maxv)
        return maxv;
    else if(v < minv)
        return minv;
    return v;
}

void outputMat()
{
    cout << "\nvector<vector<int>> mat = {" << endl;
    for(int i = 0; i < mat.size(); i++)
    {
        cout << "{";
        for(int j = 0; j < mat[i].size(); j++)
        {
            cout << toStr(mat[i][j]);
            if(j < mat[i].size() - 1)
                cout << ",";
        }
        cout << "}";
        if(i < mat.size() - 1)
            cout << ",";
        cout << endl;
    }
    cout << "};";
}

void outputMatToFile()
{
    ifstream f("out.txt");
    fstream file;
    file.open("out.txt", ios::out);
    file << "vector<vector<int>> mat = {" << endl;
    for(int i = 0; i < mat.size(); i++)
    {
        file << "{";
        for(int j = 0; j < mat[i].size(); j++)
        {
            file << toStr(mat[i][j]);
            if(j < mat[i].size() - 1)
                file << ",";
        }
        file << "}";
        if(i < mat.size() - 1)
            file << ",";
        file << endl;
    }
    file << "};";
    file.close();
    cout << "Saved matrix to file successfully\n";
    return;
}

string toStr(int val)
{
    string str;
    stringstream ss;
    ss << val;
    ss >> str;
    return str;
}

int toInt(string str)
{
    int val;
    istringstream iss(str);
    iss >> val;
    return val;
}
