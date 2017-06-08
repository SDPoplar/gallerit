#include "gallerit.h"
#include "resource.h"

bool ResizeByMouseWheel( short pro )
{
    if( pro > 0 )
    {
        Gallerit::getInstance()->getWindow()->sizePlus();
        return true;
    }
    if( pro < 0 )
    {
        Gallerit::getInstance()->getWindow()->sizeMinus();
        return true;
    }

    return false;
}

bool loadDropFiles( HDROP h )
{
    wchar_t fileName[ 128 ];
    UINT fileNum = DragQueryFile( h, 0xFFFFFFFF, fileName, 128 );
    for( UINT i = 0; i < fileNum; i++ )
    {
        DragQueryFile( h, i, fileName, 128 );
        Gallerit::getInstance()->getGallery()->addImage( fileName );
    }
    Gallerit::getInstance()->getGallery()->lastImage();
    return true;
}

void openSelf( void )
{
    wchar_t lpszFileName[ 256 ];
    char sFileName[ 256 ];
    size_t t;
    GetModuleFileName( NULL, lpszFileName, 256 );
    wcstombs_s( &t, sFileName, 256, lpszFileName, 256 );
    WinExec( sFileName, SW_NORMAL );
}

void KeyDown( WPARAM nVkValue )
{
    switch( nVkValue )
    {
        case 49:
            //  button '1'
            Gallerit::getInstance()->getWindow()->setSize( GALLERIT_WND_SIZE::GWS_COMMON );
            break;
        case 50:
            //  button '2'
            Gallerit::getInstance()->getWindow()->setSize( GALLERIT_WND_SIZE::GWS_TINY );
            break;
        case 51:
            //  button '3'
            Gallerit::getInstance()->getWindow()->setSize( GALLERIT_WND_SIZE::GWS_SMALL );
            break;
        case 52:
            //  button '4'
            Gallerit::getInstance()->getWindow()->setSize( GALLERIT_WND_SIZE::GWS_LARGE );
            break;
        case 53:
            //  button '5'
            Gallerit::getInstance()->getWindow()->setSize( GALLERIT_WND_SIZE::GWS_VERY_LARGE );
            break;
        case 83:
            //  button 'S'
        case 54:
            //  button '6'
            Gallerit::getInstance()->getWindow()->setSize( GALLERIT_WND_SIZE::GWS_SCREEN );
            break;
        case 70:
            //  button 'F'
            Gallerit::getInstance()->getWindow()->toggleFixToImage();
            break;
        case 76:
            //  button 'L'
            Gallerit::getInstance()->getGallery()->reload();
            break;
        case 79:
            //  button 'O'
            openSelf();
            break;
        case 80:
            //  button 'P'
            Gallerit::getInstance()->getWindow()->toggleSlide();
            break;
        case 84:
            //  button 'T'
            Gallerit::getInstance()->getWindow()->toggleTopMost();
            break;
        case VK_TAB:
            Gallerit::getInstance()->getGallery()->prevImage();
            break;
        case VK_SPACE:
            Gallerit::getInstance()->getGallery()->nextImage();
            break;
    }
}

LRESULT CALLBACK GalleritMainWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_RBUTTONDBLCLK:
        case WM_QUIT:
        case WM_DESTROY:
            Gallerit::getInstance()->quit();
            return 0;
        case WM_MOUSEWHEEL:
            ( ( short )LOWORD( wParam ) & MK_SHIFT ) &&
                ResizeByMouseWheel( HIWORD( wParam ) );
            return 0;
        case WM_MOUSEMOVE:
            if( wParam & MK_LBUTTON )
            {
                SetCursor( LoadCursor( NULL, IDC_SIZEALL ) );
                Gallerit::getInstance()->getWindow()->doMove( LOWORD( lParam ), HIWORD( lParam ) );
            }
            return 0;
        case WM_LBUTTONDOWN:
            Gallerit::getInstance()->getWindow()->startMove( LOWORD( lParam ), HIWORD( lParam) );
            return 0;
        case WM_LBUTTONUP:
            SetCursor( LoadCursor( NULL, IDC_ARROW ) );
            return 0;
        case WM_KEYDOWN:
            KeyDown( wParam );
            return 0;
        case WM_TIMER:
            if( ! Gallerit::getInstance()->getWindow()->sliding() )
            {
                return 0;
            }
            Gallerit::getInstance()->getWindow()->slideCycle();
        case WM_SIZE:
        case WM_PAINT:
            Gallerit::getInstance()->getWindow()->draw();
            return 0;
        case WM_DROPFILES:
            loadDropFiles( ( HDROP )wParam );
            return 0;
        default:
            return DefWindowProc( hWnd, msg, wParam, lParam );
    }
}

GalleritWindow::GalleritWindow( HINSTANCE hInstance ) : m_hMainWnd( NULL ), m_hInstance( hInstance ),
    m_nBaseCursorX( 0 ), m_nBaseCursorY( 0 ), m_nBaseWndX( 0 ), m_nBaseWndY( 0 ), m_bTopMost( false ),
    m_nSize( GALLERIT_WND_SIZE::GWS_COMMON ), m_bFixImage( false ), m_bSlide( false )
{}

GERROR GalleritWindow::loadWindow( int nCmdShow )
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon = LoadIcon( this->m_hInstance, MAKEINTRESOURCE( IDI_GALLERY ) );
    wc.hIconSm = LoadIcon( this->m_hInstance, MAKEINTRESOURCE( IDI_GALLERY ) );
    wc.hInstance = this->m_hInstance;
    wc.lpfnWndProc = GalleritMainWndProc;
    wc.lpszClassName = TEXT( "GALLERIT_MAIN_WND" );
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

    if( !RegisterClassEx( &wc ) )
    {
        return GERROR::GE_WNDCLASS_REGIST_FAILD;
    }

    this->m_hMainWnd = CreateWindowEx( WS_EX_ACCEPTFILES,
        TEXT( "GALLERIT_MAIN_WND" ), TEXT( "Gallerit" ), WS_POPUP,
        ( GetSystemMetrics( SM_CXSCREEN ) - BASE_WIDTH ) / 2,
        ( GetSystemMetrics( SM_CYSCREEN ) - BASE_HEIGHT ) / 2,
        BASE_WIDTH, BASE_HEIGHT, NULL, NULL, this->m_hInstance, NULL
    );
    if( !this->m_hMainWnd )
    {
        return GERROR::GE_WINDOW_CREATE_FAILD;
    }

    ShowWindow( this->m_hMainWnd, nCmdShow );
    UpdateWindow( this->m_hMainWnd );
    this->setSize( GALLERIT_WND_SIZE::GWS_COMMON );
    return GERROR::GE_OK;
}

void GalleritWindow::setSize( GALLERIT_WND_SIZE nSize )
{
    this->m_nSize = nSize;
    this->fixToImage();
}

void GalleritWindow::startMove( int nBaseX, int nBaseY )
{
    RECT rect;
    GetWindowRect( this->m_hMainWnd, &rect );
    this->m_nBaseWndX = rect.left;
    this->m_nBaseWndY = rect.top;
    this->m_nBaseCursorX = rect.left + nBaseX;
    this->m_nBaseCursorY = rect.top + nBaseY;
}

void GalleritWindow::doMove( int nCursorX, int nCursorY )
{
    RECT rect;
    GetWindowRect( this->m_hMainWnd, &rect ) && MoveWindow( this->m_hMainWnd,
        this->m_nBaseWndX + rect.left + nCursorX - this->m_nBaseCursorX,
        this->m_nBaseWndY + rect.top + nCursorY - this->m_nBaseCursorY,
        rect.right - rect.left,
        rect.bottom - rect.top,
        false
    );
}

void GalleritWindow::draw( void )
{
    
    PAINTSTRUCT ps;
    if( ! Gallerit::getInstance()->running() )
    {
        return;
    }
    CxImage *img = Gallerit::getInstance()->getGallery()->getImage(), *realShowImage = new CxImage();
    if( ! img )
    {
        return;
    }
    BeginPaint( this->m_hMainWnd, &ps );
    HDC hDc = GetDC( this->m_hMainWnd );
    RECT rect;
    GetWindowRect( this->m_hMainWnd, &rect );
    img->Resample( rect.right - rect.left, rect.bottom - rect.top, 0, realShowImage );
    realShowImage->Draw( hDc );
    delete realShowImage;
    ReleaseDC( this->m_hMainWnd, hDc );
    EndPaint( this->m_hMainWnd, &ps );
}

void GalleritWindow::toggleTopMost( void )
{
    if( this->m_bTopMost )
    {
        SetWindowPos( this->m_hMainWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE )
            && (this->m_bTopMost = false);
    }
    else
    {
        SetWindowPos( this->m_hMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE )
            && (this->m_bTopMost = true);
    }
}

void GalleritWindow::sizeMinus( void )
{
    if( m_nSize > GALLERIT_WND_SIZE::GWS_TINY )
    {
        this->setSize( ( GALLERIT_WND_SIZE )(this->m_nSize - 1) );
    }
}

void GalleritWindow::sizePlus( void )
{
    if( m_nSize < GALLERIT_WND_SIZE::GWS_SCREEN )
    {
        this->setSize( ( GALLERIT_WND_SIZE )(this->m_nSize + 1) );
    }
}

void GalleritWindow::toggleFixToImage( void )
{
    this->m_bFixImage = !this->m_bFixImage;
    this->fixToImage();
}

void GalleritWindow::toggleSlide( void )
{
    this->m_bSlide = !this->m_bSlide;
    if( this->m_bSlide )
    {
        this->m_nLastSlide = GetTickCount();
        this->m_nTimer = SetTimer( this->m_hMainWnd, 1, 500, NULL );
    }
    else
    {
        (this->m_nTimer > 0) && KillTimer( this->m_hMainWnd, this->m_nTimer );
    }
}

void GalleritWindow::fixToImage( void )
{
    if( !Gallerit::getInstance()->running() )
    {
        return;
    }
    LONG finalTop, finalLeft, finalWidth, finalHeight;

    if( this->m_nSize == GALLERIT_WND_SIZE::GWS_SCREEN )
    {
        finalLeft = 0;
        finalTop = 0;
        finalWidth = GetSystemMetrics( SM_CXSCREEN );
        finalHeight = GetSystemMetrics( SM_CYSCREEN );
    }
    else
    {

        CxImage *img = Gallerit::getInstance()->getGallery()->getImage();
        RECT rect;
        GetWindowRect( this->m_hMainWnd, &rect );

        if( this->m_bFixImage )
        {
            finalWidth = img->GetWidth();
            finalHeight = img->GetHeight();
        }
        else
        {
            finalWidth = BASE_WIDTH;
            finalHeight = BASE_HEIGHT;
        }
        switch( this->m_nSize )
        {
        case GALLERIT_WND_SIZE::GWS_TINY:
            finalWidth /= 2;
            finalHeight /= 2;
            break;
        case GALLERIT_WND_SIZE::GWS_SMALL:
            finalWidth = finalWidth / 3 * 4;
            finalHeight = finalHeight / 3 * 4;
            break;
        case GALLERIT_WND_SIZE::GWS_LARGE:
            finalWidth *= 2;
            finalHeight *= 2;
            break;
        case GALLERIT_WND_SIZE::GWS_VERY_LARGE:
            finalWidth *= 3;
            finalHeight *= 3;
            break;
        }
        finalLeft = (rect.right + rect.left - finalWidth) / 2;
        finalTop = (rect.bottom + rect.top - finalHeight) / 2;
    }
    MoveWindow( this->m_hMainWnd, finalLeft, finalTop, finalWidth, finalHeight, true );
    PostMessage( this->m_hMainWnd, WM_PAINT, 0, 0 );
}

bool GalleritWindow::sliding( void )
{
    return this->m_bSlide;
}

void GalleritWindow::slideCycle( void )
{
    if( !this->m_bSlide )
    {
        return;
    }
    DWORD now = GetTickCount();
    if( now - this->m_nLastSlide > SLIDE_DELAY )
    {
        Gallerit::getInstance()->getGallery()->nextImage();
    }
}

void GalleritWindow::updateSlideTime()
{
    this->m_nLastSlide = GetTickCount();
}
