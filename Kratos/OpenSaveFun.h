#pragma once
class CMainFrame;
BOOL WindowSaveAsOpen(CMainFrame* pMainFrame);
BOOL OpenProjectWithMessageAndRetry(CMainFrame* pMainFrame, const char* fullpath);