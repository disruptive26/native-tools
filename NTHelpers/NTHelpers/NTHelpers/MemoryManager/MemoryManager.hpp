#pragma once

#include <Windows.h>
#include <memory>

class MemoryManager
{
public:
	template <typename T>
	static T read( HANDLE processHandle, const void* ptr )
	{
		T buffer;
		if ( ReadProcessMemory( processHandle, ptr, &buffer, sizeof( T ), nullptr ) )
			return buffer;
		return 0;
	}

	template <typename T>
	static bool read( HANDLE processHandle, const void* ptr, T& buffer )
	{
		return !!ReadProcessMemory( processHandle, ptr, &buffer, sizeof( T ), nullptr );
	}

	static std::shared_ptr<byte> read( HANDLE processHandle, const void* ptr, size_t length = sizeof( uintptr_t ) );
	static bool read( HANDLE processHandle, const void* ptr, void* buffer, size_t length = sizeof( uintptr_t ) );

public:
	template <typename T>
	static bool write( HANDLE processHandle, void* ptr, const T& buffer )
	{
		return !!WriteProcessMemory( processHandle, ptr, &buffer, sizeof( T ), nullptr );
	}

	static bool writeArray( HANDLE processHandle, void* ptr, const byte* buffer, int length = -1 );
};