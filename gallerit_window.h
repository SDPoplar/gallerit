#pragma once

#include <Windows.h>

#include "gallerit_error.h"

typedef enum
{
    GWS_COMMON = 0,
    GWS_TINY = -2,
    GWS_SMALL = -1,
    GWS_LARGE = 1,
    GWS_VERY_LARGE = 2,
    GWS_SCREEN = 3,
} GALLERIT_WND_SIZE;

#define BASE_WIDTH 480
#define BASE_HEIGHT 270

#define SLIDE_DELAY 5000

class GalleritWindow
{
    public:
        GalleritWindow( HINSTANCE hInstance );
        GERROR loadWindow( int nCmdShow );
        void setSize( GALLERIT_WND_SIZE nSize );
        void sizePlus( void );
        void sizeMinus( void );
        void startMove( int nBaseX, int nBaseY );
        void doMove( int nCursorX, int nCursorY );
        void draw( void );
        void toggleTopMost( void );
        void toggleFixToImage( void );
        void toggleSlide( void );
        void fixToImage( void );

        bool sliding( void );
        void slideCycle( void );
        void updateSlideTime( void );


    protected:
        HWND m_hMainWnd;
        HINSTANCE m_hInstance;

    private:
        int m_nBaseCursorX;
        int m_nBaseCursorY;
        int m_nBaseWndX;
        int m_nBaseWndY;
        bool m_bTopMost;
        bool m_bFixImage;
        bool m_bSlide;

        DWORD m_nLastSlide;
        UINT m_nTimer;

        GALLERIT_WND_SIZE m_nSize;
};
