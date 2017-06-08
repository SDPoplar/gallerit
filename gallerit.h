#pragma once

#include <Windows.h>

#include "gallerit_error.h"
#include "gallerit_window.h"
#include "gallerit_gallery.h"
#include "use_cximage.h"

class Gallerit
{
    public:
        static Gallerit* getInstance( void );
        int init( HINSTANCE hInstance, int nCmdShow );
        int run( void );
        void quit( void );
        GalleritWindow* getWindow( void );
        GalleritGallery* getGallery( void );
        bool running( void );
    
    protected:
        Gallerit( void );

        static Gallerit* g_m_pGallerit;
        GalleritWindow* m_pMainWnd;
        GalleritGallery* m_pGallery;
        bool m_bRun;
};
