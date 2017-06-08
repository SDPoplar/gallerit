#include "gallerit.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow )
{
    Gallerit* pEngine = Gallerit::getInstance();
    int nRunResult = pEngine->init( hInstance, nCmdShow );
    if( nRunResult != GERROR::GE_OK )
    {
        return nRunResult;
    }

    if( strcmp( lpszCmdLine, "" ) != 0 )
    {
        wchar_t cmdlineBuff[ 128 ];
        size_t t;
        mbstowcs_s( &t, cmdlineBuff, 128, lpszCmdLine, _TRUNCATE );
        pEngine->getGallery()->load( cmdlineBuff );
    }
    else
    {
        pEngine->getGallery()->load( TEXT( "imgres" ) );
    }

    return pEngine->run();
}
