#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"
#include <iostream>
#include "Element.h"

sf::Font DefaultFont;

class Button{
public:
    Element_Type m_SelectType = WIRE;
    Vec2D m_Position;
    sf::RectangleShape m_Shape;
    sf::Text m_Text;
    int m_Width = 20, m_Height = 20;
    Button(){
        m_Shape.setPosition(sf::Vector2f(m_Position.x,m_Position.y));
        m_Shape.setSize(sf::Vector2f(m_Width,m_Height));
        m_Shape.setFillColor(sf::Color::White);
        m_Text.setFont(DefaultFont);
        m_Text.setString("Button");
        //AlignAtCenter();
    }

    Button(Vec2D pos, int width, int height, std::string text, Element_Type type){
        m_SelectType = type;
        m_Position = pos;
        m_Width = width;
        m_Height = height;
        m_Shape.setPosition(sf::Vector2f(m_Position.x,m_Position.y));
        m_Shape.setSize(sf::Vector2f(m_Width,m_Height));
        m_Shape.setFillColor(sf::Color(255,80,80));
        m_Shape.setOrigin(sf::Vector2f(m_Width/2.0f, m_Height/2.0f));

        m_Text.setFont(DefaultFont);
        m_Text.setString(text);
        m_Text.setCharacterSize(30);
        float Xshift = (m_Text.getCharacterSize()+m_Text.getLetterSpacing())*std::string(m_Text.getString()).size()/3.5f;
        m_Text.setOrigin(Xshift, m_Text.getGlobalBounds().top/2.0f);
        m_Text.setFillColor(sf::Color(50,50,50));
    }

    void AlignAtCenter(){
        m_Shape.setOrigin(sf::Vector2f(m_Width/2.0f, m_Height/2.0f));
        m_Text.setCharacterSize(30);
        m_Text.setOrigin(std::floor(m_Text.getLocalBounds().width / 2.0 + 0.5), std::floor(m_Text.getLocalBounds().height / 2.0 + 0.5));
        m_Text.setFillColor(sf::Color(50,50,50));
    }

    void Draw(sf::RenderWindow &window){
        m_Text.setOrigin(std::floor(m_Text.getLocalBounds().width / 2.0 + 0.5), std::floor(m_Text.getLocalBounds().height / 2.0 + 0.5));
        sf::CircleShape cir(5);
        cir.setFillColor(sf::Color::Black);
        cir.setPosition(sf::Vector2f(m_Position.x,m_Position.y));
        m_Shape.setPosition(sf::Vector2f(m_Position.x,m_Position.y));
        m_Text.setPosition(sf::Vector2f(m_Position.x,m_Position.y));
        window.draw(m_Shape);
        window.draw(m_Text);
        //window.draw(cir);
    }

    bool Listen(sf::Vector2i mousePos, Element_Type &current){
        if(std::abs(m_Position.x-mousePos.x) <= m_Width/2.0 && std::abs(m_Position.y-mousePos.y) <= m_Height/2.0f){
            current = m_SelectType;
            return true;
        }
        return false;
    }

};