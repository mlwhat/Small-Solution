#include "OSSimple.h"

#ifdef _WIN32

#include <sys/types.h>
#include <Shlwapi.h>
#include <shlobj.h>
#include <string>
#include "Framework.h"


bool isDebuggerPresent()
{
#ifdef _DEBUG
	return ::IsDebuggerPresent() == TRUE;
#else
	return false;
#endif
}

const std::string& getExecutableName()
{
	static std::string filename;
	if (filename.size() == 0)
	{
		CHAR exeName[MAX_PATH];
		GetModuleFileNameA(nullptr, exeName, ARRAYSIZE(exeName));
		const std::string tmp(exeName);

		auto last = tmp.find_last_of("/\\");
		filename = tmp.substr(last + 1, std::string::npos);
	}
	return filename;
}
const std::string& getExecutableDirectory()
{
	static std::string folder;
	if (folder.size() == 0)
	{
		CHAR exeName[MAX_PATH];
		GetModuleFileNameA(nullptr, exeName, ARRAYSIZE(exeName));
		const std::string tmp(exeName);

		auto last = tmp.find_last_of("/\\");
		folder = tmp.substr(0, last);
	}
	return folder;
}

bool doesFileExist(const std::string& filename)
{
	DWORD attr = GetFileAttributesA(filename.c_str());
	return (attr != INVALID_FILE_ATTRIBUTES);
}

bool isDirectoryExists(const std::string& filename)
{
	DWORD attr = GetFileAttributesA(filename.c_str());
	return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));
}

bool createDirectory(const std::string& path)
{
	DWORD res = CreateDirectoryA(path.c_str(), NULL);

	return res == TRUE;
}


bool findAvailableFilename(const std::string& prefix, const std::string& directory, const std::string& extension, std::string& filename)
{
	for (UINT32 i = 0; i < UINT32_MAX; i++)
	{
		std::string newPrefix = prefix + '.' + std::to_string(i);
		filename = directory + '\\' + newPrefix + "." + extension;

		if (doesFileExist(filename) == false)
		{
			return true;
		}
	}
	should_not_get_here();
	filename = "";
	return false;
}



template<bool bOpen>
static bool fileDialogCommon(const char* pFilters, std::string& filename)
{
	OPENFILENAMEA ofn;
	CHAR chars[512] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetForegroundWindow();
	ofn.lpstrFilter = pFilters;
	ofn.lpstrFile = chars;
	ofn.nMaxFile = arraysize(chars);
	ofn.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
	if (bOpen == true)
	{
		ofn.Flags |= OFN_FILEMUSTEXIST;
	}
	ofn.lpstrDefExt = "";

	BOOL b = bOpen ? GetOpenFileNameA(&ofn) : GetSaveFileNameA(&ofn);
	if (b)
	{
		filename = std::string(chars);
		return true;
	}

	return false;
}


bool openFileDialog(const char* pFilters, std::string& filename)
{
	return fileDialogCommon<true>(pFilters, filename);
}

bool saveFileDialog(const char* pFilters, std::string& filename)
{
	return fileDialogCommon<false>(pFilters, filename);
}

void debugBreak()
{
	__debugbreak();
}


MsgBoxButton msgBox(const std::string& msg, MsgBoxType mbType)
{
	UINT Type = MB_OK;
	switch (mbType)
	{
	case MsgBoxType::Ok:
		Type = MB_OK;
		break;
	case MsgBoxType::OkCancel:
		Type = MB_OKCANCEL;
		break;
	case  MsgBoxType::RetryCancel:
		Type = MB_RETRYCANCEL;
		break;
	default:
		should_not_get_here();
		break;
	}

	int value = MessageBoxA(nullptr, msg.c_str(), "Falcor", Type | MB_TOPMOST);
	switch (value)
	{
	case IDOK:
		return MsgBoxButton::Ok;
	case IDCANCEL:
		return MsgBoxButton::Cancel;
	case IDRETRY:
		return MsgBoxButton::Retry;
	default:
		should_not_get_here();
		return MsgBoxButton::Cancel;
	}
}

void printToDebugWindow(const std::string& s)
{
	OutputDebugStringA(s.c_str());
}
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