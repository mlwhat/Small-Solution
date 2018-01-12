#pragma once
#ifndef OS_SIMPLE_H
#define OS_SIMPLE_H



#ifdef _WIN32


#include <string>

enum class MsgBoxButton
{
	Ok,     ///< 'OK' Button
	Retry,  ///< Retry button
	Cancel  ///< Cancel Button
};

enum class MsgBoxType
{
	Ok,          ///< Single 'OK' button
	RetryCancel, ///< Retry/Cancel buttons
	OkCancel,    ///< OK/Cancel buttons
};

bool isDebuggerPresent();
const std::string& getExecutableName();
const std::string& getExecutableDirectory();

bool doesFileExist(const std::string& filename);
bool isDirectoryExists(const std::string& filename);
bool createDirectory(const std::string& path);
bool findAvailableFilename(const std::string& prefix, const std::string& directory, const std::string& extension, std::string& filename);

template<bool bOpen>
static bool fileDialogCommon(const char* pFilters, std::string& filename);

bool openFileDialog(const char* pFilters, std::string& filename);
bool saveFileDialog(const char* pFilters, std::string& filename);

void debugBreak();
MsgBoxButton msgBox(const std::string& msg, MsgBoxType mbType = MsgBoxType::Ok);

void printToDebugWindow(const std::string& s);
#else
/*Not implement yet. */
const std::string& getExecutableName()
{
	return std::string("");
}

const std::string& getExecutableDirectory()
{
	return std::string("");
}


bool isDebuggerPresent()
{
	return false;
}

#endif // WIN32

#endif // WIN32