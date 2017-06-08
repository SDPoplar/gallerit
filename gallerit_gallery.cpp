#include "gallerit.h"

uint32_t parseTypeByFileName( wchar_t* lpszFileName )
{
    wchar_t *pCursor = lpszFileName, *pPoint = lpszFileName;
    while( *pCursor != L'\0' )
    {
        if( *pCursor == L'.' )
        {
            pPoint = pCursor;
        }
        pCursor++;
    }
    wchar_t ext[ 16 ] = L"";
    wcscpy_s( ext, 16, pPoint );

    if( _wcsicmp( ext, L".bmp" ) )
    {
        return CXIMAGE_FORMAT_BMP;
    }

    if( _wcsicmp( ext, L".jpg" ) )
    {
        return CXIMAGE_FORMAT_JPG;
    }

    if( _wcsicmp( ext, L".gif" ) )
    {
        return CXIMAGE_FORMAT_GIF;
    }

    if( _wcsicmp( ext, L".png" ) )
    {
        return CXIMAGE_FORMAT_PNG;
    }

    return CXIMAGE_FORMAT_UNKNOWN;
}

wchar_t* AssembleFileName( wchar_t* _buff, size_t maxCount, const LPCWSTR lpszPath, const LPCWSTR lpszName )
{
    wcscat_s( _buff, maxCount, lpszPath );
    wcscat_s( _buff, maxCount, L"\\" );
    wcscat_s( _buff, maxCount, lpszName );
    return _buff;
}

GalleritGallery::GalleritGallery( void ) : m_nCurrentIndex( 0 ), m_vGallery( {} ), m_lsPath( L"" )
{}

GERROR GalleritGallery::load( LPCWSTR lpszDirOfGallery )
{
    wcscpy_s( this->m_lsPath, 128, lpszDirOfGallery );
    return this->reload();
}

GERROR GalleritGallery::reload()
{
    this->clear();

    wchar_t findPatten[ 128 ] = L"";
    wcscat_s( findPatten, 128, this->m_lsPath );
    wcscat_s( findPatten, 128, L"\\*.*" );

    WIN32_FIND_DATA fileInfo;
    HANDLE finder = FindFirstFile( findPatten, &fileInfo );
    if( finder == INVALID_HANDLE_VALUE )
    {
        return GERROR::GE_INVALID_RESOURCE_PATH;
    }
    do
    {
        wchar_t fileName[ 256 ] = L"";
        this->addImage( AssembleFileName( fileName, 256, this->m_lsPath, fileInfo.cFileName ) );
    } while( FindNextFile( finder, &fileInfo ) );
    FindClose( finder );
    this->setCurrentImage( 0 );
    return GERROR::GE_OK;
}

void GalleritGallery::prevImage( void )
{
    if( this->m_vGallery.empty() )
    {
        return;
    }
    this->setCurrentImage( (this->m_nCurrentIndex + this->m_vGallery.size() - 1) % this->m_vGallery.size() );
}

void GalleritGallery::nextImage( void )
{
    if( this->m_vGallery.empty() )
    {
        return;
    }
    this->setCurrentImage( (this->m_nCurrentIndex + 1) % this->m_vGallery.size() );
}

void GalleritGallery::addImage( wchar_t* lpszFileName )
{
    CxImage *img = new CxImage();
    img->Load( lpszFileName, parseTypeByFileName( lpszFileName ) );
    if( img->IsValid() )
    {
        this->m_vGallery.push_back( img );
    }
}

CxImage* GalleritGallery::getImage( void )
{
    if( this->m_vGallery.size() > ( size_t )this->m_nCurrentIndex )
    {
        return this->m_vGallery[ this->m_nCurrentIndex ];
    }
    else
    {
        return NULL;
    }
}

void GalleritGallery::clear( void )
{
    while( !this->m_vGallery.empty() )
    {
        CxImage* h = this->m_vGallery[ 0 ];
        this->m_vGallery.pop_back();
        //  DeleteObject( h );
        delete h;
    }
    //  this->setCurrentImage( 0 );
}

void GalleritGallery::lastImage( void )
{
    if( this->m_vGallery.empty() )
    {
        return;
    }
    this->setCurrentImage( this->m_vGallery.size() - 1 );
}

void GalleritGallery::setCurrentImage( int nIndex )
{
    this->m_nCurrentIndex = nIndex;
    Gallerit::getInstance()->getWindow()->fixToImage();
    Gallerit::getInstance()->getWindow()->updateSlideTime();
}
