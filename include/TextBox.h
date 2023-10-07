#pragma once
#include <SFML/Graphics.hpp>
#include "Utility.h"

class TextBox{
public:
    sf::Text m_Text;
    sf::Text m_Instruction;
    sf::RectangleShape m_Background;
    Vec2D m_Position;
    std::string m_Value = "";
    TextBox(sf::Font &font){
        m_Instruction.setFont(font);
        m_Instruction.setColor(sf::Color::Black);
        m_Instruction.setCharacterSize(30);
        m_Instruction.setString("Enter Value");
        m_Text.setFont(font);
        m_Text.setColor(sf::Color::Black);
        m_Text.setCharacterSize(24);
        
        m_Background.setSize(sf::Vector2f(150,40));
        m_Background.setFillColor(sf::Color::White);
    }
    void SetPosition(Vec2D pos){
        m_Position = pos;
    }
    void TakeInput(char c, long double **val, bool &takingInput){
        if(c == 13){
            takingInput = false;
            if(m_Value.size() > 0){
                **val = std::stof(m_Value);
            }
            m_Value = "";
            m_Text.setString(m_Value);
            *val = 0;
            return;
        }
        if(c == 8 && m_Value.size() > 0){
            m_Value.pop_back();
            m_Text.setString(m_Value);
            return;
        }
        if(m_Value.size() > 8) return;
        if(c == '.'){
            for(int i=0; i<m_Value.size(); i++){
                if(m_Value[i] == '.') return;
            }
            m_Value += c;
        }
        else{
            if(c-'0' < 0 || c-'0' > 9) return;
            m_Value += c;
        }
        m_Text.setString(m_Value);
    }
    void Draw(sf::RenderWindow &window){
        m_Text.setString(m_Value);
        if(m_Text.getGlobalBounds().width > m_Background.getGlobalBounds().width){
            m_Background.setSize(sf::Vector2f(m_Text.getGlobalBounds().width*1.2,40));
        }
        m_Background.setPosition(sf::Vector2f(m_Position.x,m_Position.y));
        m_Text.setPosition(sf::Vector2f(m_Position.x,m_Position.y));
        m_Instruction.setPosition(sf::Vector2f(m_Position.x,m_Position.y-50));
        window.draw(m_Background);
        window.draw(m_Text);
        window.draw(m_Instruction);
    }
};