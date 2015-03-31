
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iomanip>
#include <iostream>
#include <math.h>

#define abs(x) ((x<0?(-x):(x)))

class RawSound
{
private:
    short* buffer;
    unsigned int _length;
    unsigned int _channels;
    unsigned int _rate_per_second;

public:
    RawSound(){
        buffer           = NULL;
        _length          = 0;
        _channels        = 0;
        _rate_per_second = 0;
    }

    RawSound(RawSound* _ptr){
        buffer           = new short[_ptr->length()];
        _length          = _ptr->length();
        _channels        = _ptr->channels();
        _rate_per_second = _ptr->sample_rate();
    }

    RawSound(sf::SoundBuffer* _sb){
        _length          = _sb->getSampleCount();
        _channels        = _sb->getChannelCount();
        _rate_per_second = _sb->getSampleRate();
        buffer           = new short[_length];

        short* dataS  = (short*)_sb->getSamples();
        for(int i=0; i<_length; i++){
            buffer[i] = dataS[i];
        }
    }

    ~RawSound(){
        if( buffer != NULL ){
            delete buffer;
        }
    }

    short* data(){
        return buffer;
    }

    unsigned int channels(){
        return _channels;
    }

    unsigned int length(){
        return _length;
    }

    unsigned int sample_rate(){
        return _rate_per_second;
    }

    void fill(RawSound* _ptr){
        if( buffer != NULL ){
            delete buffer;
        }
        _length          = _ptr->length();
        _channels        = _ptr->channels();
        _rate_per_second = _ptr->sample_rate();
        buffer           = new short[_length];

        short* dataS  = _ptr->data();
        for(int i=0; i<_length; i++){
            buffer[i] = dataS[i];
        }
    }
};


class SoundSystem : public sf::Drawable, public sf::Transformable
{
public:

    SoundSystem(unsigned int count) :
        m_particles(count),
        m_vertices_ref(sf::Points, count),
        m_vertices_mod(sf::Points, count),
        m_zoom ( 1 ),
        m_start( 0 ),
        ref_signal( ),
        mod_signal( ),
        m_emitter(0, 0)
    {

    }


    void setEmitter(sf::Vector2f position)
    {
        m_emitter = position;
    }


    void update_ref(unsigned int count, short values[])
    {
        ref_dataSet.resize( count );
        for (std::size_t i = 0; i < m_particles.size(); ++i)
        {
            ref_dataSet[i] = values[i];
        }
        update_ref_plot();
    }

    void update_ref_plot()
    {
        unsigned int count  = ref_dataSet.size();
        int pStart = m_zoom * m_start;          // Premier point dans la waveform
        int pStop  = m_zoom * (m_start + 1024); // Dernier point dans la waveform

        if( pStop > count ){
            pStop = count;
        }

        unsigned int limit  = pStop - pStart;   // Nombre de point a tracer
        unsigned int xScale = m_zoom;
        int yMax = 0;

        double yScale = 32667.0 /*((double)yMax)*/ / 128;
        for (std::size_t i = 0; i < limit; ++i)
        {
            int posX = i/xScale;
            int posY = ((double)ref_dataSet[i+pStart])/yScale + 128;
            sf::Vector2f pos(posX, posY);
            m_vertices_ref[i].position = pos;
            if( i%2 == 0 ){
                m_vertices_ref[i].color = sf::Color::Red;
            }else{
                m_vertices_ref[i].color = sf::Color::Blue;
            }

        }
        printf("(II) Updating ref plot [start=%d, stop=%d] and zoom = [%d]\n", pStart, pStop, m_zoom);
    }

    void update_mod(unsigned int count, short values[])
    {
        mod_dataSet.resize( count );
        for (std::size_t i = 0; i < count; ++i)
        {
            mod_dataSet[i] = values[i];
        }
        update_mod_plot();
    }

    void zoom_in()
    {
        if( m_zoom != 1 ){
           m_zoom -= 1;
        }
        update_ref_plot();
        update_mod_plot();
    }

    void zoom_out()
    {
        m_zoom += 1;
        update_ref_plot();
        update_mod_plot();
    }


    void slide_left()
    {
        m_start -= 1024;
        if( m_start < 0 ){
            m_start = 0;
        }
        update_ref_plot();
        update_mod_plot();
    }


    void slide_right()
    {
        m_start += 1024;
        unsigned int count  = ref_dataSet.size();
        if( m_start > (ref_dataSet.size()-1024) ){
            m_start = ref_dataSet.size() - 1024;
        }
        update_ref_plot();
        update_mod_plot();
    }


    void update_mod_plot()
    {
        unsigned int count  = mod_dataSet.size();
        int pStart = m_zoom * m_start;          // Premier point dans la waveform
        int pStop  = m_zoom * (m_start + 1024); // Dernier point dans la waveform

        if( pStop > count ){
            pStop = count;
        }

        unsigned int limit  = pStop - pStart;   // Nombre de point a tracer
        unsigned int xScale = m_zoom;
        int yMax = 0;

        double yScale = 32667.0 /*((double)yMax)*/ / 128;
        for (std::size_t i = 0; i < limit; ++i)
        {
            int posX = i/xScale;
            int posY = ((double)mod_dataSet[i+pStart])/yScale + 128;
            
            m_vertices_mod[i].position = sf::Vector2f(posX, posY + 256);
            if( i%2 == 0 ){
                m_vertices_mod[i].color = sf::Color::Red;
            }else{
                m_vertices_mod[i].color = sf::Color::Blue;
            }
        }
    }

    void update(sf::Time elapsed)
    {
        //for (std::size_t i = 0; i < m_particles.size(); ++i)
        //{
        //    // on met à jour la durée de vie de la particule
        //    Particle& p = m_particles[i];
        //    p.lifetime -= elapsed;

//            // si la particule est arrivée en fin de vie, on la réinitialise
//            if (p.lifetime <= sf::Time::Zero)
//                resetParticle(i);

//            // on met à jour la position du vertex correspondant
//            m_vertices[i].position += p.velocity * elapsed.asSeconds();

//            // on met à jour l'alpha (transparence) de la particule en fonction de sa durée de vie
//            float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
//            m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
//        }
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // on applique la transformation
        states.transform *= getTransform();

        // nos particules n'utilisent pas de texture
        states.texture = NULL;

        // on dessine enfin le vertex array
        target.draw(m_vertices_ref, states);
        target.draw(m_vertices_mod, states);

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(0, 128)),
            sf::Vertex(sf::Vector2f(1024, 128))
        };
//        line.setFillColor(sf::Color(100, 250, 50));

        target.draw(line, 2, sf::Lines);

        // Load the text font
        sf::Font font;
        if (!font.loadFromFile("resources/sansation.ttf"))
            exit( 1 );

        // Initialize the pause message
        sf::Text infos;
        infos.setFont(font);
        infos.setCharacterSize(18);
        //infos.setPosition(170.f, 150.f);
        infos.setColor(sf::Color::White);
        infos.setString("Sound parameters (min=X, max=X, avg=X, etc=");
        target.draw(infos);
    }

private:

    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    void resetParticle(std::size_t index)
    {
        // on donne une vitesse et une durée de vie aléatoires à la particule
//        float angle = (std::rand() % 360) * 3.14f / 180.f;
//        float speed = (std::rand() % 50) + 50.f;
//        m_particles[index].velocity = sf::Vector2f(cos(angle) * speed, sin(angle) * speed);
//        m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

        // on remet à zéro la position du vertex correspondant à la particule
//        m_vertices[index].position = m_emitter;
    }

    RawSound ref_signal;
    RawSound mod_signal;
    std::vector<short> ref_dataSet;
    std::vector<short> mod_dataSet;

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices_ref;
    sf::VertexArray m_vertices_mod;
    int m_zoom;
    int m_start;
    sf::Vector2f m_emitter;
};

////////////////////////////////////////////////////////////
/// Play a sound
///
////////////////////////////////////////////////////////////

void playS(sf::SoundBuffer* _buffer)
{
    sf::SoundBuffer buffer;
    buffer.loadFromSamples(
            _buffer->getSamples(),
            _buffer->getSampleCount (),
            _buffer->getChannelCount(),
            _buffer->getSampleRate()
    );
    std::cout << " " << buffer.getDuration().asSeconds() << " seconds"       << std::endl;
    std::cout << " " << buffer.getSampleRate()           << " samples / sec" << std::endl;
    std::cout << " " << buffer.getChannelCount()         << " channels"      << std::endl;

    sf::Sound sound(buffer);
    sound.play();
    while (sound.getStatus() == sf::Sound::Playing)
    {
        // Leave some CPU time for other processes
        sf::sleep(sf::milliseconds(100));

        // Display the playing position
        std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << sound.getPlayingOffset().asSeconds() << " sec   ";
        std::cout << std::flush;
    }
}


////////////////////////////////////////////////////////////
/// Play a music
///
////////////////////////////////////////////////////////////
void playMusic()
{
    // Load an ogg music file
    sf::Music music;
    if (!music.openFromFile("resources/orchestral.ogg"))
        return;

    // Display music informations
    std::cout << "orchestral.ogg :" << std::endl;
    std::cout << " " << music.getDuration().asSeconds() << " seconds"       << std::endl;
    std::cout << " " << music.getSampleRate()           << " samples / sec" << std::endl;
    std::cout << " " << music.getChannelCount()         << " channels"      << std::endl;

    // Play it
    music.play();

    // Loop while the music is playing
    while (music.getStatus() == sf::Music::Playing)
    {
        // Leave some CPU time for other processes
        sf::sleep(sf::milliseconds(100));

        // Display the playing position
        std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << music.getPlayingOffset().asSeconds() << " sec   ";
        std::cout << std::flush;
    }
    std::cout << std::endl;
}


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////

//
// Fc = Fs / 10
//
#define N   45
float h[N] = 
 {
  -1.839E-3,-2.657E-3,-1.437E-7, 3.154E-3, 2.595E-3,-4.159E-3,-1.540E-2, 
  -2.507E-2,-2.547E-2,-1.179E-2, 1.392E-2, 4.206E-2, 5.888E-2, 5.307E-2, 
   2.225E-2,-2.410E-2,-6.754E-2,-8.831E-2,-7.475E-2,-2.956E-2, 3.030E-2, 
   8.050E-2, 1.000E-1, 8.050E-2, 3.030E-2,-2.956E-2,-7.475E-2,-8.831E-2, 
  -6.754E-2,-2.410E-2, 2.225E-2, 5.307E-2, 5.888E-2, 4.206E-2, 1.392E-2, 
  -1.179E-2,-2.547E-2,-2.507E-2,-1.540E-2,-4.159E-3, 2.595E-3, 3.154E-3, 
  -1.437E-7,-2.657E-3,-1.839E-3
}; 
void fir_filter(short *in, short *out, int n)
{
    for (int i = 0; i < n; i++)
    {
        int nb_taps = (i < N) ? (i+1) : N;
        float yn = 0.0;
        for(int z=0; z<nb_taps; z++){
            yn += (float)in[i-z] * h[z];
        }
        out[i] = yn;
    }
}



void ph_filter(short *in, short *out, int n)
{
    for (int i = 1; i < n; i++)
    {
        out[i-1] = in[i] - in[i-1];
    }out[n-1] = 0;
}


void lp_filter(short *in, short *out, int n)
{
    for (int i = 1; i < n; i++)
    {
        out[i-1] = ((int)in[i] + (int)in[i-1])/2;
    }out[n-1] = 0;
}

void noise_filter(short *in, short *out, int n)
{
    for (int i = 0; i < n; i++)
    {
        if( rand()%2 ){
            out[i-1] = ((int)in[i] + rand()%511);
        }else{
            out[i-1] = ((int)in[i] - rand()%511);
        }
    }
}

void enhance_filter(short *in, short *out, int n)
{
    int max = 0;
    for (int i = 0; i < n; i++){
        max = max > abs(in[i]) ? max : abs(in[i]);
    }
    double vScale = 32767.0 / (double)max;
    for (int i = 0; i < n; i++)
    {
        out[i] = ((double)in[i]) * vScale;
    }
}

void sin_filter(short *in, short *out, int n)
{
    for (int i = 0; i < n; i++)
    {
        out[i] = round(256.0 * sin( (i*50.0/22400.0)));
    }
}

/*
int counter;
short echo_buffer[48600];
void echo_fx(short *in, short *out, int n)
{
    echo_buffer[48600]
}

void echo_filter(short *in, short *out, int n)
{
    for (int i = 0; i < n; i++)
    {
        out[i] = round(256.0 * sin( (i*50.0/22400.0)));
    }
}
*/


int main(int argc, char* argv[])
{

    std::string filename = "./resources/canary.wav";
    if (argc == 2)
    {
        filename = argv[1];
    }

    sf::SoundBuffer buffer1;
    if (!buffer1.loadFromFile( filename ))
        return 1;

    sf::SoundBuffer buffer2;
    if (!buffer2.loadFromFile( filename ))
        return 1;

    short* dataS  = (short*)buffer1.getSamples();
    short* data2  = (short*)buffer2.getSamples();
    int nbSamples = buffer1.getSampleCount ();


    // on crée la fenêtre
    sf::RenderWindow window(sf::VideoMode(1024, 512), "Particles");

    // on crée un système de 1000 particules
//    ParticleSystem particles(1000);
    SoundSystem particles(nbSamples);
    particles.update_ref(nbSamples, dataS);
//    ph_filter(dataS, dataS, nbSamples);

    // on crée un chrono pour mesurer le temps écoulé
    sf::Clock clock;

    // on fait tourner la boucle principale
    while (window.isOpen())
    {
        // on gère les évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            {
                    particles.update_mod(nbSamples, dataS);

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            {
                    ph_filter(dataS, data2, nbSamples);
                    particles.update_mod(nbSamples, data2);

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            {
                    fir_filter(dataS, data2, nbSamples);
                    particles.update_mod(nbSamples, data2);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            {
                    sin_filter(dataS, data2, nbSamples);
                    particles.update_mod(nbSamples, data2);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            {
                    noise_filter(dataS, data2, nbSamples);
                    particles.update_mod(nbSamples, data2);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                    enhance_filter(dataS, data2, nbSamples);
                    particles.update_mod(nbSamples, data2);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                    playS( &buffer2 );
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                    particles.zoom_out();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                    particles.zoom_in();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                    particles.slide_left();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                    particles.slide_right();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                    return EXIT_SUCCESS;
            }
        }

        // on fait en sorte que l'émetteur des particules suive la souris
//        sf::Vector2i mouse = sf::Mouse::getPosition(window);
//        particles.setEmitter(window.mapPixelToCoords(mouse));

        // on met à jour le système de particules
//        sf::Time elapsed = clock.restart();
//        particles.update(elapsed);

        // on le dessine
        window.clear();
        window.draw(particles);
        window.display();

        sf::Time t2 = sf::milliseconds(30);
        sf::sleep( t2 );
    }

    // Play a music
    //playMusic();

    // Wait until the user presses 'enter' key
//    std::cout << "Press enter to exit..." << std::endl;
//    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
