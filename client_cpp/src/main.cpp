/*

  ECHOCLNT.C
  ==========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net
  
  Simple TCP/IP echo client.

*/


#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <unistd.h>             /*  for ssize_t data type  */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <iostream>

using namespace std;

/*  Global constants  */

#define MAX_LINE           (1000)


/*  Function declarations  */


/*  main()  */

#include <vector>


class Device{
private:
    int       _socket;               /*  connection socket         */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    vector<string> screen;

public:
    Device( ) : screen() {

        /*  Set the remote port  */
        short int port   = 2002;
        char szAddress[] = "127.0.0.1";

        /*  Create the listening socket  */

        if ( (_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
            fprintf(stderr, "ECHOCLNT: Error creating listening socket.\n");
            exit(EXIT_FAILURE);
        }


        /*  Set all bytes in socket address structure to
            zero, and fill in the relevant data members   */

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family      = AF_INET;
        servaddr.sin_port        = htons(port);


        /*  Set the remote IP address  */

        if ( inet_aton(szAddress, &servaddr.sin_addr) <= 0 ) {
        printf("ECHOCLNT: Invalid remote IP address.\n");
        exit(EXIT_FAILURE);
        }

        
        /*  connect() to the remote echo server  */

        if ( connect(_socket, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) {
            printf("ECHOCLNT: Error calling connect()\n");
            exit(EXIT_FAILURE);
        }

        for( int ii=0; ii<24; ii++ ){
            screen.push_back( "Initialization line - nothing to draw for now" );
        }
    }


    void SendTextLine(string text, int line)
    {
        char buffer[256];
        sprintf(buffer, "# L %d %s", line, text.c_str());
        int nwritten = write(_socket, buffer, strlen(buffer) + 1 );
        int rSize    = read(_socket, buffer, 1024);
    }


    void SendButtonColor(int button, int color)
    {
        char buffer[256];
        sprintf(buffer, "# C %d %d", button, color);
        int nwritten = write(_socket, buffer, strlen(buffer) + 1 );
        int rSize    = read(_socket, buffer, 1024);
    }


    void SendTextBuffer( )
    {
        for(int ii=0; ii<24; ii++){
            SendTextLine(screen.at(ii), ii);
        }
    }


    void UpdateScreenBuffer(string text)
    {
        screen.erase( screen.begin() );
        screen.push_back( text );
        SendTextBuffer( );
    }


    void UpdateScreenBuffer(vector<string> texts)
    {
        for(int ii=0; ii<24; ii++){
            screen.erase    ( screen.begin() );
            screen.push_back( texts.at(ii)   );
        }
        SendTextBuffer( );
    }


    void ClearScreenBuffer( )
    {
        for(int ii=0; ii<24; ii++){
            screen.erase    ( screen.begin() );
            screen.push_back(           ""   );
        }
        SendTextBuffer( );
    }


    int ReadKeyboard( )
    {
        char buffer[256];
        sprintf(buffer, "# K");
        int nwritten = write(_socket, buffer, strlen(buffer) + 1 );
        int rSize    = read(_socket, buffer, 1024);
        return buffer[0];
    }

    int ReadButton( )
    {
        char buffer[256];
        sprintf(buffer, "# B");
        int nwritten = write(_socket, buffer, strlen(buffer) + 1 );
        int rSize    = read(_socket, buffer, 1024);
        return buffer[0];
    }

};



int main(int argc, char *argv[]) {


    vector<string> lignes;

    lignes.push_back( "La République Galactique est en pleine ébullition." );
    lignes.push_back( "La taxation des routes commerciales reliant les systèmes" );
    lignes.push_back( "éloignés provoque la discorde." );
    lignes.push_back( "" );
    lignes.push_back( "Pour régler la question, la cupide Fédération du Commerce" );
    lignes.push_back( "et ses redoutables vaisseaux de guerre imposent un blocus" );
    lignes.push_back( "à la petite planète Naboo." );
    lignes.push_back( "" );
    lignes.push_back( "Face à ce dangereux engrenage, alors que le Congrès de la" );
    lignes.push_back( "République s’enlise dans des débats sans fin, le Chancelier" );
    lignes.push_back( "Suprême charge en secret deux Chevaliers Jedi, gardiens de" );
    lignes.push_back( "la paix et de la justice dans la galaxie, de résoudre le" );
    lignes.push_back( "conflit..." );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );
    lignes.push_back( "" );


    vector<string> smiley;
    smiley.push_back( "                          oooo$$$$$$$$$$$$oooo" );
    smiley.push_back( "                      oo$$$$$$$$$$$$$$$$$$$$$$$$o" );
    smiley.push_back( "                   oo$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o         o$   $$ o$" );
    smiley.push_back( "   o $ oo        o$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o       $$ $$ $$o$" );
    smiley.push_back( "oo $ $ '$      o$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$o       $$$o$$o$" );
    smiley.push_back( "'$$$$$$o$     o$$$$$$$$$      $$$$$$$$$$$      $$$$$$$$$$o    $$$$$$$$" );
    smiley.push_back( "  $$$$$$$    $$$$$$$$$$$      $$$$$$$$$$$      $$$$$$$$$$$$$$$$$$$$$$$" );
    smiley.push_back( "  $$$$$$$$$$$$$$$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$$$$$$  '''$$$" );
    smiley.push_back( "   '$$$''''$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     '$$$" );
    smiley.push_back( "    $$$   o$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     '$$$o" );
    smiley.push_back( "   o$$'   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$       $$$o" );
    smiley.push_back( "   $$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" "$$$$$$ooooo$$$$o" );
    smiley.push_back( "  o$$$oooo$$$$$  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   o$$$$$$$$$$$$$$$$$" );
    smiley.push_back( "  $$$$$$$$'$$$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     $$$$''''''''" );
    smiley.push_back( " ''''       $$$$    '$$$$$$$$$$$$$$$$$$$$$$$$$$$$'      o$$$" );
    smiley.push_back( "            '$$$o     '''$$$$$$$$$$$$$$$$$$'$$'         $$$" );
    smiley.push_back( "              $$$o          '$$''$$$$$$''''           o$$$" );
    smiley.push_back( "               $$$$o                                o$$$'" );
    smiley.push_back( "                '$$$$o      o$$$$$$o'$$$$o        o$$$$" );
    smiley.push_back( "                  '$$$$$oo     ""$$$$o$$$$$o   o$$$$''" );
    smiley.push_back( "                     ''$$$$$oooo  '$$$o$$$$$$$$$'''" );
    smiley.push_back( "                        ''$$$$$$$oo $$$$$$$$$$" );
    smiley.push_back( "                                ''''$$$$$$$$$$$" );
    smiley.push_back( "                                    $$$$$$$$$$$$" );
    smiley.push_back( "                                     $$$$$$$$$$'" );
    smiley.push_back( "                                      '$$$''  " );

    vector<string> question;
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "   QUELLE EST LA COULEUR NORMALE DES CAROTTES ?                                 " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "    BLEU                                                              ORANGE    " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "    VERT                                                           REPONSE D    " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );
    question.push_back( "                                                                                " );

    /*  Get command line arguments  */
    Device device;

    int loop = 5;
    while( loop-- )
    {
        device.UpdateScreenBuffer( smiley );    sleep( 1 );
        device.ClearScreenBuffer (        );    sleep( 1 );
    }

    device.UpdateScreenBuffer( question );
    device.SendButtonColor( 1, 0 );
    device.SendButtonColor( 2, 0 );
    device.SendButtonColor( 3, 1 );
    device.SendButtonColor( 4, 1 );
    device.SendButtonColor( 5, 0 );
    device.SendButtonColor( 6, 0 );
    device.SendButtonColor( 7, 1 );
    device.SendButtonColor( 8, 1 );

    while( true )
    {
        int b_action = device.ReadButton( );
        if( b_action == 7 )
        {
            printf("(II) Button action detected (%d)\n", b_action);
            break;
        }
        usleep( 10000 );
    }
    printf("(II) Reponse correcte, partie terminee !\n");

    /*  Get string to echo from user  */
    int i = 0;
    while( 1 ){
        string ligne = lignes.at( i % lignes.size() );
        
        device.UpdateScreenBuffer( ligne );

        for(int qq=0; qq<8; qq++){
            device.SendButtonColor( qq, (qq+i)%4 );
        }

        for(int ii=0; ii<50; ii++)
        {

            int k_action = device.ReadKeyboard( );
            if( k_action != 0 )
            {
                printf("(II) Keyboard action detected (%d)\n", k_action);
            }

            int b_action = device.ReadButton( );
            if( b_action != 0 )
            {
                printf("(II) Button action detected (%d)\n", b_action);
            }

            usleep( 10000 );
        }

        i += 1;

        if ( i == lignes.size() ) break;

    }

    /*  Output echoed string  */

    return EXIT_SUCCESS;
}

