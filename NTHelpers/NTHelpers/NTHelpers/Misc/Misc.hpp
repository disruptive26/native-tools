#pragma once

#include <Windows.h>

class Misc
{
public:
	static bool cvt2mb( wchar_t* str, char* out )
	{
		if ( wcslen( str ) == 0 )
			return false;

		size_t size = WideCharToMultiByte( CP_UTF8, 0, &str[ 0 ], wcslen( str ), nullptr, 0, nullptr, nullptr );
		if ( !size )
			return false;

		return WideCharToMultiByte( CP_UTF8, 0, &str[ 0 ], wcslen( str ), &out[ 0 ], size, nullptr, nullptr );
	}

	static bool cvt2wc( char* str, wchar_t* out )
	{
		if ( strlen( str ) == 0 )
			return false;

		size_t size = MultiByteToWideChar( CP_UTF8, 0, &str[ 0 ], strlen( str ), nullptr, 0 );
		if ( !size )
			return false;

		return MultiByteToWideChar( CP_UTF8, 0, &str[ 0 ], strlen( str ), &out[ 0 ], size );
	}
};