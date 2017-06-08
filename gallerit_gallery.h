#pragma once

#include <Windows.h>
#include <vector>

#include "cximage\ximage.h"

class GalleritGallery
{
    public:
        GalleritGallery( void );
        //  ~GalleritGallery( void );
        GERROR load( LPCWSTR lpszDirOfGallery );
        GERROR reload( void );
        CxImage* getImage( void );
        void addImage( wchar_t* lpszFileName );
        void prevImage( void );
        void nextImage( void );
        void lastImage( void );
        void clear( void );

    protected:
        void setCurrentImage( int nIndex );

        std::vector<CxImage *> m_vGallery;
        int m_nCurrentIndex;
        wchar_t m_lsPath[ 128 ];
};
