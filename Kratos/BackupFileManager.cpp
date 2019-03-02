#include "stdafx.h"
#include "BackupFileManager.h"
#include "Time.h"
#include <filesystem>
namespace fs = std::experimental::filesystem;

CBackupFileManager::CBackupFileManager(CString originalFilePath)
{
	char fname[MAX_PATH];
	char dir[MAX_PATH];
	char drive[_MAX_DRIVE];
	char ext[_MAX_EXT];
	_splitpath_s(originalFilePath.GetString(), drive, dir, fname, ext);
	_backupPath = CString(drive) + dir + fname + FormatTime(time(nullptr), "_backup%Y%m%d_%H%M%S") + ext;
	fs::copy_file(originalFilePath.GetString(), _backupPath.GetString());
}

void CBackupFileManager::DeleteBackup()
{
	fs::remove(_backupPath.GetString());
}
