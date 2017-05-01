#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include "../Misc/Misc.hpp"

typedef struct
{
	std::string procName;
	uint32_t pid;
} procInfo;

typedef struct
{
	std::string moduleName;
	uint32_t moduleBase;
	uint32_t moduleSize;
} modInfo;

class ProcessManager
{
public:
	bool processExists( const std::string& processName );
	uint32_t queryProcessId( const std::string& processName );
	std::shared_ptr<procInfo> queryProcInfo( const std::string& processName );

public:
	ProcessManager( ) = default;
	ProcessManager( HANDLE handle );
	ProcessManager( const std::string& processName );
	ProcessManager( uint32_t processId );

	bool Attach( HANDLE handle );
	bool Attach( const std::string& processName );
	bool Attach( uint32_t processId );

public:
	bool moduleExists( const std::string& moduleName );
	std::shared_ptr<modInfo> queryModInfo( const std::string& moduleName );

private:
	HANDLE attachedProcess;
	std::vector<std::shared_ptr<procInfo>> cachedPids;
	std::vector<std::shared_ptr<modInfo>> cachedMods;
};