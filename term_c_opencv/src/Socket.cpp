#include "Socket.hpp"

#define ECHO_PORT  (2002)
#define LISTENQ    (1024)   /*  Backlog for listen()   */

Socket::Socket()
{
    struct    sockaddr_in servaddr;
//    short int port = ECHO_PORT;


    /*  Get port number from the command line, and
        set to default port if no arguments were supplied  */

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
    servaddr.sin_port        = htons(ECHO_PORT);

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

    /*  Wait for a connection, then accept() it  */
    cout << "(II) Waiting for client connection (" << ECHO_PORT << ")" << endl;
    if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling accept()\n");
        exit(EXIT_FAILURE);
    }

    cout << "(II) Client connection accepted" << endl;
    SetSocketBlockingEnabled(conn_s, false);
}


/** Returns true on success, or false if there was an error */
bool Socket::SetSocketBlockingEnabled(int fd, bool blocking)
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


bool Socket::isConnected()
{
	return true;
}


string Socket::Receive( )
{
	char buffer[1024];
	bzero(buffer, 1024);
    int rSize = read(conn_s, buffer, 1024);
    string r = buffer;
    return r;
}


bool Socket::Send(string text){
	int wSize = write(conn_s, text.c_str(), text.length()+1);
	return true;
}


bool Socket::Send(char text){
	int wSize = write(conn_s, &text, 1);
	return true;
}
