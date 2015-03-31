#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include "Socket.hpp"

using namespace std;

class CvButton{
    int x;
    int y;
    int w;
    int h;
    int number;
    string text;
    int state;

    CvScalar t_color = CV_RGB( 255, 255, 255 );
    CvScalar b_color = CV_RGB( 255, 255, 255 );
    CvScalar f_color = CV_RGB(   0,   0,   0 );

};


//void on_mouse( int event, int x, int y, int flags )
vector<int>*  p_buttons; // UGLY !
bool b_states[8]; // PAREIL


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
   if (event == CV_EVENT_LBUTTONDOWN)
   {
        cout << "MOUSE DOWN => posX = " << x << " and posY = " << y << endl;
        int button = 0;
        if( x < 128 ){
            button = (y / 128) + 1;
        }else if( x > 1018 ){
            button = (y / 128) + 5;
        }
        cout << " > button = " << button << endl;
        if( (button != 0) && (b_states[button-1] != 0) ){
            p_buttons->push_back( button );
        }
   }
}

void CleanImage(IplImage* pic){

    int width   = pic->width;
    int height  = pic->height;
    int grid    = 100;
    for (int y = 0; y < height; y += 1) {
        for (int x = 0; x < width; x += 1) {
            CvScalar s = CV_RGB( 0, 0, 0 );
            cvSet2D(pic, y, x, s);
        }
    }

}

void DrawGrid(IplImage* pic){

    int width   = pic->width;
    int height  = pic->height;
    int grid    = 100;
    for (int y = 0; y < height; y += 1) {
        for (int x = 0; x < width; x += 1) {
            CvScalar s = CV_RGB( 0, 0, 0 );
            cvSet2D(pic, y, x, s);
        }
    }
/*
    for (int y = 0; y < height; y += grid) {
        for (int x = 0; x < width; x += 1) {
            CvScalar s = CV_RGB( 255, 255, 255 );
            cvSet2D(pic, y, x, s);
        }
    }
    for (int x = 0; x < width; x += grid) {
        for (int y = 0; y < height; y += 1) {
            CvScalar s = CV_RGB( 255, 255, 255 );
            cvSet2D(pic, y, x, s);
        }
    }
*/
}

// Helper function to put text in the center of a rectangle
void set_label(cv::Mat& im, cv::Rect r, const std::string label, CvScalar color)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.7;
    int thickness = 1;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Point pt(r.x + (r.width-text.width)/2, r.y + (r.height+text.height)/2);

    //cv::rectangle(
    //    im, 
    //    pt + cv::Point(0, baseline), 
    //    pt + cv::Point(text.width, -text.height), 
    //    CV_RGB(255,255,255), CV_FILLED
    //);
  int face[] = {cv::FONT_HERSHEY_SIMPLEX, cv::FONT_HERSHEY_PLAIN, cv::FONT_HERSHEY_DUPLEX, cv::FONT_HERSHEY_COMPLEX, 
                cv::FONT_HERSHEY_TRIPLEX, cv::FONT_HERSHEY_COMPLEX_SMALL, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 
                cv::FONT_HERSHEY_SCRIPT_COMPLEX, cv::FONT_ITALIC};
    cv::putText(im, label, pt, face[0]|face[8]/*fontface*/, scale, color, thickness, CV_AA);
}


void set_label(cv::Mat& im, int x, int y, const std::string label, CvScalar color)
{
    int fontface  = cv::FONT_HERSHEY_SIMPLEX;
    double scale  = 0.55;
    int thickness = 1;
    int baseline  = 0;
    CvPoint pt    = cvPoint(x, y);
//    cv::Point pt(r.x + (r.width-text.width)/2, r.y + (r.height+text.height)/2);
    int face[] = {cv::FONT_HERSHEY_SIMPLEX, cv::FONT_HERSHEY_PLAIN, cv::FONT_HERSHEY_DUPLEX, cv::FONT_HERSHEY_COMPLEX, 
                cv::FONT_HERSHEY_TRIPLEX, cv::FONT_HERSHEY_COMPLEX_SMALL, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 
                cv::FONT_HERSHEY_SCRIPT_COMPLEX, cv::FONT_ITALIC};
    cv::putText(im, label, pt, face[0]/*fontface*/, scale, color, thickness, CV_AA);
}


void set_label(cv::Mat& im, string label, int x, int y, int w, int h)
{
    CvScalar white = CV_RGB( 255, 255, 255 );
    CvRect  rect   = cvRect( x, y, w, h );
    set_label(im, rect, label, white);
}


void set_label(cv::Mat& im, int x, int y, const std::string label)
{
    CvScalar white = CV_RGB( 255, 255, 255 );
    set_label(im, x, y, label, white);
}


void DrawRactangle(cv::Mat& im, string texte, bool state, int x, int y, int w, int h)
{
    CvScalar white = CV_RGB( 255, 255, 255 );
    CvScalar black = CV_RGB(   0,   0,   0 );
    CvRect  rect   = cvRect( x, y, w, h );
    if( state == true ){
        cv::rectangle(im, rect, white, 1, 8, 0);
        set_label(im, rect, texte, white);
    }else{
        cv::rectangle(im, rect, white, 1, 8, 0);
        set_label(im, rect, texte, black);
    }
}

void CleanImage(cv::Mat& im){
    CvMat   oldmat = im;
    CvScalar black = CV_RGB( 0, 0, 0 );
    printf("%d %d\n", oldmat.cols, oldmat.rows);
    CvRect  rect   = cvRect( 0, 0, oldmat.cols, oldmat.rows );
    cv::rectangle(im, rect, black, CV_FILLED, 8, 0);
}

int main( int argc, char** argv ) {
    cout << "(II) CREATION OF THE WINDOW" <<  endl;
    cvNamedWindow( "Image Window", CV_WINDOW_AUTOSIZE );
    cv::setMouseCallback("Image Window", CallBackFunc, NULL);

    IplImage* frame;

    bool pause   = true;
    int  picture = 1;
    bool leave   = false;

    int filter  = 0;

    IplImage* macrob = NULL;


    vector<string*> listeString;
    for(int i=0; i<24; i++){
        string* str = new string();
        (*str) = "";
        for(int j=0; j<80; j++)
        (*str) = (*str) + (char)('0' + rand()%2);
        listeString.push_back( str );
    }

    frame = cvCreateImage(cvSize(1146, 512), IPL_DEPTH_8U , 3);
    cv::Mat image( frame );


    Socket sock;

    bool needUpdate = true;

    vector<char> l_touches;
    vector<int>  l_buttons;
    p_buttons = &l_buttons; // Floreal... ferme les yeux !

    for(int i=0; i<8; i++){
        b_states[i] = false;
    }

    while(1) {

        //
        // UGLY Draw on request to save CPU
        //
        if( needUpdate == true ){
            CleanImage( image );
            DrawRactangle(image, "Button 1", b_states[0], 0,   0, 128, 128 );
            DrawRactangle(image, "Button 2", b_states[1], 0, 128, 128, 128 );
            DrawRactangle(image, "Button 3", b_states[2], 0, 256, 128, 128 );
            DrawRactangle(image, "Button 4", b_states[3], 0, 384, 128, 128 );
            DrawRactangle(image, "Button 5", b_states[4], 1018,   0, 128, 128 );
            DrawRactangle(image, "Button 6", b_states[5], 1018, 128, 128, 128 );
            DrawRactangle(image, "Button 7", b_states[6], 1018, 256, 128, 128 );
            DrawRactangle(image, "Button 8", b_states[7], 1018, 384, 128, 128 );
            for(int i=0; i<24; i++)
            {
                set_label(image, 133, 22 * i, *listeString.at(i));
            }
            cv::imshow("Image Window", image);
            needUpdate = false;
        }

        //
        //
        //
        for(int qq=0; qq<100; qq++)
        {
            int c = cvWaitKey(1);
            if( c != -1 ){
                l_touches.push_back( c );
            }
            if( c == 'q' ){
                leave = true;
                break;
            }      

            string t_recv = sock.Receive();
            char *buffer  = (char*)t_recv.c_str();
            int rSize     = t_recv.length();

        if( rSize > 0 ){
            needUpdate = true;
            //cout << "Socket read [" << rSize << "] = " << buffer <<  endl;
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
                    (*listeString[nLigne]) = texte;
                    sock.Send( "# OK" );

                }else if( buffer[2] == 'B' ){ // READ BUTTON STATUS
                    if( l_buttons.size() > 0 ){
                        char answ = l_buttons.at( 0 );
                        l_buttons.erase( l_buttons.begin() );
                        sock.Send( answ );
                    }else{
                        sock.Send( 0 );
                    }

                }else if( buffer[2] == 'K' ){ // READ KEYBOARD
                    if( l_touches.size() > 0 ){
                        char answ = l_touches.at( 0 );
                        l_touches.erase( l_touches.begin() );
                        sock.Send( answ );
                    }else{
                        sock.Send( 0 );
                    }
                
                }else if( buffer[2] == 'C' ){ // CHANGE BUTTON COLOR
                    int nLigne = buffer[4] - '0';
                    int nState = buffer[6] - '0';
                    if( ((nLigne-1) >= 0) && ((nLigne-1) <= 8) )
                    {
                        b_states[nLigne-1] = nState & 0x01;
                    }
                    sock.Send( "# OK" );
                }

            }else{
                printf("> Demande inconnue :-(\n");
                sock.Send( "# NOK" );
            }
        }
}

        int c = cvWaitKey(20);
        if( c != -1 ){
            l_touches.push_back( c );
        }
        if( c == 'q' ){
            leave = true;
        }

        //
        //
        //
        if( leave == true ){
            break;
        }      
    }

    cvDestroyWindow( "Video" );
    return 0;
}

