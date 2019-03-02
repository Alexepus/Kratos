#pragma once
class CBackupFileManager
{
public:
	CBackupFileManager(CString originalFilePath);
	CBackupFileManager() = delete;
	~CBackupFileManager() = default;

	void DeleteBackup();

private:
	CString _backupPath;
};

