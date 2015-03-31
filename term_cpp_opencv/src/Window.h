/*
 *  CImage.h
 *  P_Bitmap
 *
 *  Created by Le Gal on 24/10/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

//#include <iostream>
//#include <stdio.h>
#include <string.h>
using namespace std;



#ifndef FastImage_H_
#define FastImage_H_

class FastImage{
private:
        int taille;

        int largeur;
        int hauteur;

        int line_width;

public:
        unsigned char *image;
        FastImage(int hauteur, int largeur);
        FastImage(FastImage *f);

        ~FastImage();

        void FastImageFill( unsigned char *p );
        void FastCopyTo( FastImage *p );

        void FastCopyFrom( IplImage* p );
        void FastCopyTo  ( IplImage* p );

        int width();
        int height();
        void resize(int _height, int _width);
        void RGB(int y, int x, int r, int g, int b);
        int  Red(int y, int x);
        void Red(int y, int x, int v);
        void Red(int y, int x, unsigned char v);

        int  Alpha(int y, int x);
        void Alpha(int y, int x, int v);

        int  Green(int y, int x);
        void Green(int y, int x, int v);
        void Green(int y, int x, unsigned char v);

        int  Blue (int y, int x);
        void Blue(int y, int x, int v);
        void Blue(int y, int x, unsigned char v);
};

#endif /*FastImage_H_*/
