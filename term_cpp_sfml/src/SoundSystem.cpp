#include "SoundSystem.h"


SoundSystem::SoundSystem( ) :
    listeString(), listB(),
    _fName( "None" )
{
    for(int i=0; i<32; i++){
        string* str = new string();
        (*str) = "";
        for(int j=0; j<80; j++)
        (*str) = (*str) + (char)('0' + rand()%2);
        listeString.push_back( str );
    }

    listB.push_back( new GL_Button("Button 1",    0.0f,   0.0f) );
    listB.push_back( new GL_Button("Button 2",    0.0f, 128.0f) );
    listB.push_back( new GL_Button("Button 3",    0.0f, 256.0f) );
    listB.push_back( new GL_Button("Button 4",    0.0f, 384.0f) );
    listB.push_back( new GL_Button("Button 5", 1018.0f,   0.0f) );
    listB.push_back( new GL_Button("Button 6", 1018.0f, 128.0f) );
    listB.push_back( new GL_Button("Button 7", 1018.0f, 256.0f) );
    listB.push_back( new GL_Button("Button 8", 1018.0f, 384.0f) );

}


void SoundSystem::zoom_in()
{
//    for (unsigned i=0; i<liste.size(); i++)
//        liste.at(i)->zoom_in();
}


void SoundSystem::zoom_out()
{
//    for (unsigned i=0; i<liste.size(); i++)
//        liste.at(i)->zoom_out();
}


void SoundSystem::slide_left()
{
//    for (unsigned i=0; i<liste.size(); i++)
//        liste.at(i)->slide_left();
}


void SoundSystem::slide_right()
{
//    for (unsigned i=0; i<liste.size(); i++)
//        liste.at(i)->slide_right();
}


void SoundSystem::update_fName(std::string _name)
{
    _fName = _name;
}


void SoundSystem::setText(int line, string text )
{
    (*listeString[line]) = text;
}


void SoundSystem::setButtonState(int button, int value)
{
    listB[button]->setColor( value );
}


void SoundSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // on applique la transformation
    states.transform *= getTransform();

    // nos particules n'utilisent pas de texture
    states.texture = NULL;

    //for (unsigned i=0; i<liste.size(); i++)
    //    liste.at(i)->draw(target, states);

    sf::Vertex line_1[] =
    {
        sf::Vertex(sf::Vector2f(128,   0)),
        sf::Vertex(sf::Vector2f(128, 512))
    }; target.draw(line_1, 2, sf::Lines);

    sf::Vertex line_2[] =
    {
        sf::Vertex(sf::Vector2f(1018,   0)),
        sf::Vertex(sf::Vector2f(1018, 512))
    }; target.draw(line_2, 2, sf::Lines);

//    sf::Vertex line_3[] =
//    {
//        sf::Vertex(sf::Vector2f(0,    256)),
//        sf::Vertex(sf::Vector2f(1280, 256))
//    }; target.draw(line_3, 2, sf::Lines);

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        exit( 1 );

    for(int i=0; i<8; i++)
    {
        listB[i]->draw(target, states);
    }

    for(int i=0; i<23; i++)
    {
        sf::Text t_filter_name;
        t_filter_name.setFont(font);
        t_filter_name.setCharacterSize(18);
        t_filter_name.setPosition(128.0f + 5.f, 22.0f * i);
        t_filter_name.setColor(sf::Color::White);

        t_filter_name.setString( *listeString.at(i) );
        target.draw(t_filter_name);
    }
}
