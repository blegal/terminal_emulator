#import <fcntl.h>
#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

class Socket{
private:
	int list_s;
    int conn_s;

public:
    Socket();
    Socket(string addr);

	bool SetSocketBlockingEnabled(int fd, bool blocking);

    bool isConnected();
    string Receive();
    bool Send	(string	text);
    bool Send	(char	text);
};
