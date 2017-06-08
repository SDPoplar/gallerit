#pragma once

#include "cximage\ximage.h"

#ifdef _DEBUG

#pragma comment( lib, "cximage/lib_debug/cximage.lib" )
#pragma comment( lib, "cximage/lib_debug/libdcr.lib" )
#pragma comment( lib, "cximage/lib_debug/jasper.lib" )
#pragma comment( lib, "cximage/lib_debug/png.lib" )
#pragma comment( lib, "cximage/lib_debug/jpeg.lib" )
#pragma comment( lib, "cximage/lib_debug/mng.lib" )
#pragma comment( lib, "cximage/lib_debug/tiff.lib" )
#pragma comment( lib, "cximage/lib_debug/libpsd.lib" )
#pragma comment( lib, "cximage/lib_debug/zlib.lib" )

#else

#pragma comment( lib, "cximage/lib_release/cximage.lib" )
#pragma comment( lib, "cximage/lib_release/libdcr.lib" )
#pragma comment( lib, "cximage/lib_release/jasper.lib" )
#pragma comment( lib, "cximage/lib_release/png.lib" )
#pragma comment( lib, "cximage/lib_release/jpeg.lib" )
#pragma comment( lib, "cximage/lib_release/mng.lib" )
#pragma comment( lib, "cximage/lib_release/tiff.lib" )
#pragma comment( lib, "cximage/lib_release/libpsd.lib" )
#pragma comment( lib, "cximage/lib_release/zlib.lib" )

#endif

uint32_t parseTypeByFileName( wchar_t lpszFileName );
