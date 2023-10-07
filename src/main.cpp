#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Circuit.h"
#include "TextBox.h"

const int ResX = 1200;
const int ResY = 800;


int main(){
    sf::RenderWindow window(sf::VideoMode(ResX, ResY), "SFML works!");
    int SelectedButton = 0;
    Element_Type CurrentElement = WIRE;
    bool TakingInput = false;
    bool Displaying = false;
    long double *CurrentVal = nullptr;
    std::vector<Button> AllButtons;
    Grid MainGrid;
    Initialise(MainGrid.GetEdge());
    TextBox InputTextBox(DefaultFont);
    TextBox VDisplayBox(DefaultFont);
    TextBox CDisplayBox(DefaultFont);
    VDisplayBox.m_Instruction.setString("");
    CDisplayBox.m_Instruction.setString("");
    VDisplayBox.SetPosition(Vec2D(800,300));
    CDisplayBox.SetPosition(Vec2D(800,500));
    InputTextBox.SetPosition(Vec2D(800,300));
    AllButtons.push_back(Button(Vec2D(1100,100),120,80,"Wire",WIRE));
    AllButtons.push_back(Button(Vec2D(1100,300),120,80,"Resistor",RESISTOR));
    AllButtons.push_back(Button(Vec2D(1100,500),120,80,"Battery",BATTERY));
    AllButtons.push_back(Button(Vec2D(1100,700),120,80,"Remove",EMPTY));
    AllButtons.push_back(Button(Vec2D(900,100),120,80,"Solve",EMPTY));
    AllButtons.push_back(Button(Vec2D(900,300),120,80,"Reset",EMPTY));
    AllButtons.push_back(Button(Vec2D(900,500),120,80,"Analyse",EMPTY));
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !TakingInput){
                if(Displaying){
                    Displaying = false;
                    break;
                }
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                for(int i=0; i<AllButtons.size(); i++){
                    if(AllButtons[i].Listen(pos,CurrentElement)){
                        if(i == 4){
                            StartSolving(MainGrid.m_HLines,MainGrid.m_VLines,MainGrid.m_VValues,MainGrid.m_HValues,MainGrid.m_HCurrents,MainGrid.m_VCurrents,MainGrid.m_HVoltages,MainGrid.m_VVoltages, MainGrid.GetSize());
                        }
                        else if(i == 5){
                            MainGrid.Reset();
                        }
                        else if( i == 6){
                            Displaying = true;
                        }
                        SelectedButton = i;
                    }
                }
                for(int i=0; i<AllButtons.size(); i++){
                    if(i == SelectedButton){
                        AllButtons[i].m_Shape.setFillColor(sf::Color::Green);
                    }
                    else{
                        AllButtons[i].m_Shape.setFillColor(sf::Color(255,80,80));
                    }
                }
                Element_Type *ele = MainGrid.GetLine(pos);
                long double *val = MainGrid.GetVal(pos);
                if(ele){
                    if(CurrentElement == RESISTOR) *val = 100;
                    else if(CurrentElement == BATTERY) *val = 10;
                    else *val = 0;
                    *ele = CurrentElement;
                } 
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && !TakingInput){
                if(Displaying){
                    Displaying = false;
                    break;
                }
                CurrentVal = MainGrid.GetVal(sf::Mouse::getPosition(window));
                Element_Type *type = MainGrid.GetLine(sf::Mouse::getPosition(window));
                if(CurrentVal && (*type == RESISTOR || *type == BATTERY)){
                    if(fabs(*CurrentVal) < 1e-6) InputTextBox.m_Value = "0";
                    else InputTextBox.m_Value = std::to_string(*CurrentVal);
                    InputTextBox.m_Text.setString(InputTextBox.m_Value);
                    TakingInput = true;
                }
            }
            else if(event.type == sf::Event::TextEntered && TakingInput){
                InputTextBox.TakeInput(event.text.unicode,&CurrentVal,TakingInput);
            }
            else if(!TakingInput && Displaying){
                long double *Cval = (MainGrid.GetCurrent(sf::Mouse::getPosition(window)));
                long double *Vval = (MainGrid.GetVoltage(sf::Mouse::getPosition(window)));
                if(Cval){
                    Displaying = true;
                    CDisplayBox.m_Value = "Current = " + std::to_string(*Cval)+"A";
                    VDisplayBox.m_Value = "Voltage = " + std::to_string(*Vval)+"V";
                } 
            }
        }
        window.clear(sf::Color(175,175,175));
        MainGrid.DrawGrid(window,CurrentVal);
        if(!Displaying && !TakingInput){
            for(int i=0; i<AllButtons.size(); i++){
                AllButtons[i].Draw(window);
            }
        }
        if(TakingInput) InputTextBox.Draw(window);
        if(Displaying){
            VDisplayBox.Draw(window);
            CDisplayBox.Draw(window);
        }
        window.display();
    }
    return 0;
}