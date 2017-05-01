#include "ProcessManager.hpp"

bool ProcessManager::processExists( const std::string& processName )
{
	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if ( snapshot == INVALID_HANDLE_VALUE )
		return false;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof( PROCESSENTRY32 );

	if ( Process32First( snapshot, &pe ) )
	{
		do
		{
			char buf[ 512 ];
			RtlZeroMemory( buf, 512 );
			if ( !Misc::cvt2mb( pe.szExeFile, buf ) )
				continue;

			if ( processName == buf )
			{
				auto pinfo = std::make_shared<procInfo>( );
				pinfo->pid = pe.th32ProcessID;
				pinfo->procName = processName;
				this->cachedPids.push_back( pinfo );
				return true;
			}
		} while ( Process32Next( snapshot, &pe ) );
	}

	return false;
}

uint32_t ProcessManager::queryProcessId( const std::string& processName )
{
	for ( auto x : this->cachedPids )
		if ( x->procName == processName )
			return x->pid;

	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if ( snapshot == INVALID_HANDLE_VALUE )
		return false;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof( PROCESSENTRY32 );

	if ( Process32First( snapshot, &pe ) )
	{
		do
		{
			char buf[ 512 ];
			RtlZeroMemory( buf, 512 );
			if ( !Misc::cvt2mb( pe.szExeFile, buf ) )
				continue;

			if ( processName == buf )
			{
				auto pinfo = std::make_shared<procInfo>( );
				pinfo->pid = pe.th32ProcessID;
				pinfo->procName = processName;
				this->cachedPids.push_back( pinfo );
				return pe.th32ProcessID;
			}
		} while ( Process32Next( snapshot, &pe ) );
	}

	return false;
}

std::shared_ptr<procInfo> ProcessManager::queryProcInfo( const std::string& processName )
{
	for ( auto x : this->cachedPids )
		if ( x->procName == processName )
			return x;

	return nullptr;
}

ProcessManager::ProcessManager( HANDLE handle )
{
	this->attachedProcess = handle;
}

ProcessManager::ProcessManager( const std::string& processName )
{
	this->attachedProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, this->queryProcessId( processName ) );
}

ProcessManager::ProcessManager( uint32_t processId )
{
	this->attachedProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, processId );
}

bool ProcessManager::Attach( HANDLE handle )
{
	this->attachedProcess = handle;
	return handle != INVALID_HANDLE_VALUE && handle != NULL;
}

bool ProcessManager::Attach( const std::string& processName )
{
	this->attachedProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, this->queryProcessId( processName ) );
	return this->attachedProcess != INVALID_HANDLE_VALUE && this->attachedProcess != NULL;
}

bool ProcessManager::Attach( uint32_t processId )
{
	this->attachedProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, processId );
	return this->attachedProcess != INVALID_HANDLE_VALUE && this->attachedProcess != NULL;
}

bool ProcessManager::moduleExists( const std::string& moduleName )
{
	for ( auto x : this->cachedMods )
		if ( x->moduleName == moduleName )
			return true;

	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetProcessId( this->attachedProcess ) );
	if ( snapshot == INVALID_HANDLE_VALUE )
		return false;

	MODULEENTRY32 me;
	me.dwSize = sizeof( MODULEENTRY32 );

	if ( Module32First( snapshot, &me ) )
	{
		do
		{
			char buf[ 512 ];
			RtlZeroMemory( buf, 512 );
			if ( !Misc::cvt2mb( me.szModule, buf ) )
				continue;

			if ( moduleName == buf )
			{
				std::shared_ptr<modInfo> minfo = std::make_shared<modInfo>( );
				minfo->moduleBase = reinterpret_cast< uint32_t >( me.modBaseAddr );
				minfo->moduleSize = me.modBaseSize;
				minfo->moduleName = moduleName;
				this->cachedMods.push_back( minfo );
				return true;
			}
		} while ( Module32Next( snapshot, &me ) );
	}

	return false;
}

std::shared_ptr<modInfo> ProcessManager::queryModInfo( const std::string& moduleName )
{
	for ( auto x : this->cachedMods )
		if ( x->moduleName == moduleName )
			return x;

	return nullptr;
}
