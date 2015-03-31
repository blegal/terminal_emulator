
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <sstream>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "SoundSystem.h"

#include <iomanip>
#include <iostream>
#include <math.h>

#define abs(x) ((x<0?(-x):(x)))

#import <fcntl.h>
#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <stdlib.h>
#include <stdio.h>

/** Returns true on success, or false if there was an error */
bool SetSocketBlockingEnabled(int fd, bool blocking)
{
   if (fd < 0) return false;

#ifdef WIN32
   unsigned long mode = blocking ? 0 : 1;
   return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
   int flags = fcntl(fd, F_GETFL, 0);
   if (flags < 0) return false;
   flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
   return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}


int main(int argc, char* argv[])
{

#define ECHO_PORT  (2002)
#define MAX_LINE   (1000)
#define LISTENQ    (1024)   /*  Backlog for listen()   */


    // on crée la fenêtre
    sf::RenderWindow window(sf::VideoMode(1146, 512), "Particles");

    // on crée un système de 1000 particules
//    ParticleSystem particles(1000);
    SoundSystem particles;
//    particles.update_ref( &buff_1 );
//    particles.update_mod( &buff_2 );
//    ph_filter(dataS, dataS, nbSamples);

    int i = 0;

    // on fait tourner la boucle principale
    vector<char> l_touches;
    vector<int>  l_buttons;


    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *endptr;                /*  for strtol()              */


    /*  Get port number from the command line, and
        set to default port if no arguments were supplied  */

    if ( argc == 2 ) {
    port = strtol(argv[1], &endptr, 0);
    if ( *endptr ) {
        fprintf(stderr, "ECHOSERV: Invalid port number.\n");
        exit(EXIT_FAILURE);
    }
    }
    else if ( argc < 2 ) {
    port = ECHO_PORT;
    }
    else {
    fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
    exit(EXIT_FAILURE);
    }

    
    /*  Create the listening socket  */

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
    exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    /*  Bind our socket addresss to the 
    listening socket, and call listen()  */

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
    fprintf(stderr, "ECHOSERV: Error calling bind()\n");
    exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling listen()\n");
        exit(EXIT_FAILURE);
    }

while( true ){

    /*  Wait for a connection, then accept() it  */
    cout << "(II) Waiting for client connection" << endl;

    if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling accept()\n");
        exit(EXIT_FAILURE);
    }
    SetSocketBlockingEnabled(conn_s, false);
    cout << "(II) Client connection accepted" << endl;

    /* Fin de Wait */

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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            {

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            {

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
//                    particles.zoom_out();

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
//                particles.slide_left();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
//                particles.slide_right();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
//                buff_1.fill( &buff_2 );
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                return EXIT_SUCCESS;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                int button = 0;
                if( localPosition.x < 128 ){
                    button = (localPosition.y / 128) + 1;
                }else if( localPosition.x > 1018 ){
                    button = (localPosition.y / 128) + 5;
                }
                if( button != 0 ){
                    //
                    // On va essayer de détecter les double appuis !
                    //
                     cout << "- Button pressed x = " << button << endl;
                     l_buttons.push_back( button );
                }
            }

        }


        // on le dessine
        if( i%40 == 0 ){
            window.clear();
            window.draw(particles);
            window.display();
        } i+= 1;

        sf::Time t2 = sf::milliseconds(50);
        sf::sleep( t2 );


        /*  Retrieve an input line from the connected socket
            then simply write it back to the same socket.     */
        char buffer[1024];
        int rSize = read(conn_s, buffer, 1024);

        if( rSize > 0 ){
            cout << "Socket read [" << rSize << "] = " << buffer <<  endl;
            if( buffer[0] == '#' ){
                
                if( buffer[2] == 'L' ){ // text line
                    char *ligne = NULL;
                    char *texte = NULL;
                    if( buffer[5] == ' ' ){
                        buffer[5] = 0;
                        ligne = &buffer[4];
                        texte = &buffer[6];
                    }else{
                        buffer[6] = 0;
                        ligne = &buffer[4];
                        texte = &buffer[7];
                    }
                    int nLigne = atoi( ligne );
                    printf("> Demande de modification, ligne %d [%s]\n", nLigne, texte);
                    
                    string text = texte;
                    particles.setText( nLigne, text );

                    char valid[] = "# OK";
                    int wSize    = write(conn_s, valid, 5);
                
                }else if( buffer[2] == 'B' ){ // READ BUTTON STATUS
                    printf("> Demande de lecture de l'état des bouttons\n");
                    char valid[] = "# OK";
                    int wSize    = write(conn_s, valid, 5);

                }else if( buffer[2] == 'K' ){ // READ KEYBOARD
                    printf("> Demande de lecture de l'état du keyboard\n");
                    char valid[] = "# OK";
                    int wSize    = write(conn_s, valid, 5);
                
                }else if( buffer[2] == 'C' ){ // CHANGE BUTTON COLOR
                    char *ligne = NULL;
                    char *texte = NULL;
                    if( buffer[5] == ' ' ){
                        buffer[5] = 0;
                        ligne = &buffer[4];
                        texte = &buffer[6];
                    }else{
                        buffer[6] = 0;
                        ligne = &buffer[4];
                        texte = &buffer[7];
                    }
                    int nLigne = atoi( ligne );
                    int nColor = atoi( texte );
                    printf("Demande de modification, boutton %d [%d]\n", nLigne, nColor);

                    particles.setButtonState( nLigne, nColor );

                    cout << "Socket read [" << rSize << "] = " << buffer <<  endl;
                    char valid[] = "# OK";
                    int wSize    = write(conn_s, valid, 5);
                }

            }else{
                printf("> Demande inconnue :-(\n");
                char valid[] = "# NOK";
                int wSize    = write(conn_s, valid, sizeof(valid));
            }
        }


        /*  Close the connected socket  */

        if( rSize == -2 ){
            if ( close(conn_s) < 0 ) {
                fprintf(stderr, "ECHOSERV: Error calling close()\n");
                //(EXIT_FAILURE);
                break;
            }
        }

        /* Fin connexion de socket */

    }
    }

    return EXIT_SUCCESS;
}
