#include "MemoryManager.hpp"

std::shared_ptr<byte> MemoryManager::read( HANDLE processHandle, const void* ptr, size_t length )
{
	auto buffer = std::shared_ptr<byte>( new byte[ length ] );
	return !!ReadProcessMemory( processHandle, ptr, buffer.get( ), length, nullptr ) ? buffer : nullptr;
}

bool MemoryManager::read( HANDLE processHandle, const void* ptr, void* buffer, size_t length )
{
	return !!ReadProcessMemory( processHandle, ptr, buffer, length, nullptr );
}

bool MemoryManager::writeArray( HANDLE processHandle, void* ptr, const byte* buffer, int length )
{
	if ( length == -1 )
	{
		byte* i = const_cast< byte* >( buffer );
		while ( *i )
			i++;

		return !!WriteProcessMemory( processHandle, ptr, buffer, i - buffer, nullptr );
	}
	else
		return !!WriteProcessMemory( processHandle, ptr, buffer, length, nullptr );
}
