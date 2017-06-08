#include "gallerit.h"

Gallerit* Gallerit::g_m_pGallerit = NULL;

Gallerit* Gallerit::getInstance( void )
{
    if( !Gallerit::g_m_pGallerit )
    {
        new Gallerit();
    }
    return Gallerit::g_m_pGallerit;
}

Gallerit::Gallerit( void ) : m_bRun( false )
{
    Gallerit::g_m_pGallerit = this;
}

int Gallerit::init( HINSTANCE hInstance, int nCmdShow )
{
    this->m_pMainWnd = new GalleritWindow( hInstance );
    if( GERROR e = this->m_pMainWnd->loadWindow( nCmdShow ) )
    {
        return e;
    }
    this->m_pGallery = new GalleritGallery();
    this->m_bRun = true;
    return GERROR::GE_OK;
}

int Gallerit::run()
{
    MSG msg;
    while( this->m_bRun && GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    return GERROR::GE_OK;
}

void Gallerit::quit( void )
{
    this->m_bRun = false;
}

GalleritWindow* Gallerit::getWindow( void )
{
    return this->m_pMainWnd;
}

GalleritGallery* Gallerit::getGallery( void )
{
    return this->m_pGallery;
}

bool Gallerit::running( void )
{
    return this->m_bRun;
}
