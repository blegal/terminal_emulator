#include "GL_Button.h"

GL_Button::GL_Button( std::string _text, int _x, int _y ) :
    text(), textRec(), font()
{
    texte  = _text;
    enable = false;
    posX   = _x;
    posY   = _y;

    if (!font.loadFromFile("resources/sansation.ttf"))
        exit( 1 );

    text.setFont(font);
    text.setCharacterSize(14);
    text.setString( _text );

    sf::FloatRect textBounds = text.getLocalBounds() ;
    int tPosX = posX + 1.0f + (126.0f - textBounds.width ) / 2.0f;
    int tPosY = posY + 1.0f + (126.0f - textBounds.height) / 2.0f;
    text.setPosition(tPosX, tPosY);

    textRec.setSize(sf::Vector2f(126, 126)) ;
    textRec.setPosition(posX+1, posY+1) ;

    setColor( 0 );
    //textRec.setFillColor   (sf::Color::White) ;
    //textRec.setOutlineColor(sf::Color::Black);

    textRec.setOutlineThickness(1);
//    setText( texte );
}


void GL_Button::setColor(int _color)
{
    color = _color;

    if( color == 0 ){
        text.setColor          (sf::Color::White);
        textRec.setFillColor   (sf::Color::White) ;
        textRec.setOutlineColor(sf::Color::White);
    }else if( color == 2 ){
        text.setColor          (sf::Color::Green);
        textRec.setFillColor   (sf::Color::White) ;
        textRec.setOutlineColor(sf::Color::Green);
    }else if( color == 3 ){
        text.setColor          (sf::Color::Red);
        textRec.setFillColor   (sf::Color::White) ;
        textRec.setOutlineColor(sf::Color::Black);
    }else{
        text.setColor          (sf::Color::Black);
        textRec.setFillColor   (sf::Color::White) ;
        textRec.setOutlineColor(sf::Color::Black);
    }
}

void GL_Button::setText( std::string _text )
{

}

void GL_Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(textRec);
    target.draw(text);
}
