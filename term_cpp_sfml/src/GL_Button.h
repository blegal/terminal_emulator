#ifndef __C_GL_Button__
#define __C_GL_Button__

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;

class GL_Button// : public sf::Drawable, public sf::Transformable
{
public:
    GL_Button( std::string _text, int _x, int _y );
    void setText( std::string _text );
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void setColor(int _color);

private:
    bool enable;
    sf::Font font;
    std::string texte;
    sf::Text text;
    sf::RectangleShape textRec;
    int posX;
    int posY;
    int color;
};

#endif
