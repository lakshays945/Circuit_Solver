#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"
#include "Element.h"
#include "Button.h"
#include <iostream>

Element Wire;
Element Resistor;
Element Battery;
sf::RectangleShape CursorLine;

class Grid{
private:
    int m_Offset = 50;
    int m_Size = 10;
    int m_Edge = 50;
public:
    Element_Type **m_HLines;
    Element_Type **m_VLines;
    long double **m_HValues;
    long double **m_VValues;
    long double **m_HCurrents;
    long double **m_VCurrents;
    long double **m_HVoltages;
    long double **m_VVoltages;
    Grid(){
        m_Edge = 700/m_Size;
        m_HLines = new Element_Type*[m_Size+1];
        m_HValues = new long double*[m_Size+1];
        m_HCurrents = new long double*[m_Size+1];
        m_HVoltages = new long double*[m_Size+1];

        m_VLines = new Element_Type*[m_Size];
        m_VValues = new long double*[m_Size];
        m_VCurrents = new long double*[m_Size];
        m_VVoltages = new long double*[m_Size];

        for(int i=0; i<=m_Size; i++){
            m_HLines[i] = new Element_Type[m_Size];
            m_HValues[i] = new long double[m_Size];
            m_HCurrents[i] = new long double[m_Size];
            m_HVoltages[i] = new long double[m_Size];
            if(i<m_Size){
                m_VLines[i] = new Element_Type[m_Size+1];
                m_VValues[i] = new long double[m_Size+1];
                m_VCurrents[i] = new long double[m_Size+1];
                m_VVoltages[i] = new long double[m_Size+1];
            }
        }
        for(int i=0; i<=m_Size; i++){
            for(int j=0; j<=m_Size; j++){
                if(j<m_Size){
                    m_HLines[i][j] = EMPTY;
                    m_HValues[i][j] = 0;
                    m_HCurrents[i][j] = 0;
                    m_HVoltages[i][j] = 0;
                }
                if(i<m_Size){
                    m_VLines[i][j] = EMPTY;
                    m_VValues[i][j] = 0;
                    m_VCurrents[i][j] = 0;
                    m_VVoltages[i][j] = 0;
                }
            }
        }
    }
    Grid(int size){
        m_Size = size;
        m_Edge = 700/m_Size;
    }
    int GetEdge(){return m_Edge;}
    int GetSize(){return m_Size;}
    Element_Type* GetLine(const sf::Vector2i &mousePos){
        int x = mousePos.x-m_Offset;
        int y = mousePos.y-m_Offset;
        if(x/m_Edge < 0 || y/m_Edge < 0 || x/m_Edge >= m_Size || y/m_Edge >= m_Size) return nullptr;
        int distUp = y-m_Edge*(y/m_Edge);
        int distLeft = x-m_Edge*(x/m_Edge);
        int distDown = m_Edge*(y/m_Edge+1)-y;
        int distRight = m_Edge*(x/m_Edge+1)-x;
        if(std::min(distUp,distDown) < std::min(distRight,distLeft)){
            if(distUp < distDown) return &m_HLines[y/m_Edge][x/m_Edge];
            else return &m_HLines[y/m_Edge+1][x/m_Edge];
        }
        else{
            if(distRight < distLeft) return &m_VLines[y/m_Edge][x/m_Edge+1];
            else return &m_VLines[y/m_Edge][x/m_Edge];
        }
        return 0;
    }

    long double *GetVal(const sf::Vector2i &mousePos){
        int x = mousePos.x-m_Offset;
        int y = mousePos.y-m_Offset;
        if(x/m_Edge < 0 || y/m_Edge < 0 || x/m_Edge >= m_Size || y/m_Edge >= m_Size) return nullptr;
        int distUp = y-m_Edge*(y/m_Edge);
        int distLeft = x-m_Edge*(x/m_Edge);
        int distDown = m_Edge*(y/m_Edge+1)-y;
        int distRight = m_Edge*(x/m_Edge+1)-x;
        if(std::min(distUp,distDown) < std::min(distRight,distLeft)){
            if(distUp < distDown) return &m_HValues[y/m_Edge][x/m_Edge];
            else return &m_HValues[y/m_Edge+1][x/m_Edge];
        }
        else{
            if(distRight < distLeft) return &m_VValues[y/m_Edge][x/m_Edge+1];
            else return &m_VValues[y/m_Edge][x/m_Edge];
        }
        return nullptr;
    }

    long double *GetVoltage(const sf::Vector2i &mousePos){
        int x = mousePos.x-m_Offset;
        int y = mousePos.y-m_Offset;
        if(x/m_Edge < 0 || y/m_Edge < 0 || x/m_Edge >= m_Size || y/m_Edge >= m_Size) return nullptr;
        int distUp = y-m_Edge*(y/m_Edge);
        int distLeft = x-m_Edge*(x/m_Edge);
        int distDown = m_Edge*(y/m_Edge+1)-y;
        int distRight = m_Edge*(x/m_Edge+1)-x;
        if(std::min(distUp,distDown) < std::min(distRight,distLeft)){
            if(distUp < distDown) return &m_HVoltages[y/m_Edge][x/m_Edge];
            else return &m_HVoltages[y/m_Edge+1][x/m_Edge];
        }
        else{
            if(distRight < distLeft) return &m_VVoltages[y/m_Edge][x/m_Edge+1];
            else return &m_VVoltages[y/m_Edge][x/m_Edge];
        }
        return nullptr;
    }

    long double *GetCurrent(const sf::Vector2i &mousePos){
        int x = mousePos.x-m_Offset;
        int y = mousePos.y-m_Offset;
        if(x/m_Edge < 0 || y/m_Edge < 0 || x/m_Edge >= m_Size || y/m_Edge >= m_Size) return nullptr;
        int distUp = y-m_Edge*(y/m_Edge);
        int distLeft = x-m_Edge*(x/m_Edge);
        int distDown = m_Edge*(y/m_Edge+1)-y;
        int distRight = m_Edge*(x/m_Edge+1)-x;
        if(std::min(distUp,distDown) < std::min(distRight,distLeft)){
            if(distUp < distDown) return &m_HCurrents[y/m_Edge][x/m_Edge];
            else return &m_HCurrents[y/m_Edge+1][x/m_Edge];
        }
        else{
            if(distRight < distLeft) return &m_VCurrents[y/m_Edge][x/m_Edge+1];
            else return &m_VCurrents[y/m_Edge][x/m_Edge];
        }
        return nullptr;
    }

    ~Grid(){
        for(int i=0; i<=m_Size; i++){
            if(i<m_Size) delete[] m_HLines[i];
            delete[] m_VLines[i];
        }
    }

    void DrawElement(Element_Type type, sf::RenderWindow &window, Vec2D Pos, long double rot, bool currentSelected){
        sf::Vector2f pos(Pos.x,Pos.y);
        sf::Color temp;
        switch(type){
            case EMPTY:
                CursorLine.setFillColor(sf::Color(150,150,150,150));
                CursorLine.setRotation(rot);
                CursorLine.setPosition(pos);
                window.draw(CursorLine);
                break;
            case CURSOR:
                CursorLine.setFillColor(sf::Color::Red);
                CursorLine.setRotation(rot);
                CursorLine.setPosition(pos);
                window.draw(CursorLine);
                break;
            case WIRE:
                Wire.m_Sprite.setRotation(rot);
                Wire.m_Sprite.setPosition(pos);
                window.draw(Wire.m_Sprite);
                break;
            case RESISTOR:
                Resistor.m_Sprite.setRotation(rot);
                Resistor.m_Sprite.setPosition(pos);
                window.draw(Resistor.m_Sprite);
                break;
            case BATTERY:
                if(abs(rot) < 1) {
                    Battery.m_Sprite.setScale(-0.9,1);
                    Battery.m_Sprite.setPosition({pos.x+m_Edge+3,pos.y});
                }
                else{
                    Battery.m_Sprite.setScale(0.9,1);
                    Battery.m_Sprite.setPosition({pos.x,pos.y+3});
                }
                Battery.m_Sprite.setRotation(rot);
                window.draw(Battery.m_Sprite);
                break;
        }
        if(currentSelected && (type == RESISTOR || type == BATTERY)){
            CursorLine.setSize(sf::Vector2f(m_Edge,8));
            CursorLine.setFillColor(sf::Color(0,255,0,100));
            CursorLine.setRotation(rot);
            CursorLine.setPosition(pos);
            window.draw(CursorLine);
            CursorLine.setSize(sf::Vector2f(m_Edge,4));
        }
    }

    void Reset(){
        for(int i=0; i<=m_Size; i++){
            for(int j=0; j<=m_Size; j++){
                if(i<m_Size){
                    m_VLines[i][j] = EMPTY;
                    m_VValues[i][j] = 0;
                    m_VCurrents[i][j] = 0;
                    m_VVoltages[i][j] = 0;
                }
                if(j<m_Size){
                    m_HLines[i][j] = EMPTY;
                    m_HValues[i][j] = 0;
                    m_HCurrents[i][j] = 0;
                    m_HVoltages[i][j] = 0;
                }
            }
        }
    }

    void DrawGrid(sf::RenderWindow &window, long double *currentSelected){
        Element_Type *element = GetLine(sf::Mouse::getPosition(window));
        Element_Type temp;
        if(element){
            temp = *element;
            *element = CURSOR;
        } 
        for(int i=0; i<=m_Size; i++){
            for(int j=0; j<m_Size; j++){
                DrawElement(m_HLines[i][j],window,{j*m_Edge+m_Offset,i*m_Edge+m_Offset},0, &m_HValues[i][j] == currentSelected);
            }
        }
        for(int i=0; i<m_Size; i++){
            for(int j=0; j<=m_Size; j++){
                DrawElement(m_VLines[i][j],window,{j*m_Edge+m_Offset,(i+1)*m_Edge+m_Offset},-90, &m_VValues[i][j] == currentSelected);
            }
        }
        if(element) *element = temp;
    }
};

void Initialise(int size){
    Elements.loadFromFile("res/Elements.png");
    DefaultFont.loadFromFile("res/Roboto.ttf");
    CursorLine.setSize(sf::Vector2f(size,4));
    CursorLine.setFillColor(sf::Color::Red);
    Wire = Element(size,WIRE,Elements);
    Resistor = Element(size,RESISTOR,Elements);
    Battery = Element(size,BATTERY,Elements);
}