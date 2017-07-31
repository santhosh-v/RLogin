// RLogin.h : RLogin アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル
#include "Data.h"

#define	WM_SOCKSEL		(WM_USER + 0)
#define WM_GETHOSTADDR	(WM_USER + 1)
#define	WM_ICONMSG		(WM_USER + 2)
#define WM_THREADCMD	(WM_USER + 3)
#define WM_AFTEROPEN	(WM_USER + 4)

class CCommandLineInfoEx : public CCommandLineInfo
{
public:
	int m_PasStat;
	int m_Proto;
	CString m_Addr;
	CString m_Port;
	CString m_User;
	CString m_Pass;
	CString m_Term;
	CString m_Name;
	BOOL m_InUse;
	BOOL m_InPane;
	int m_AfterId;

	CCommandLineInfoEx();
	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
	BOOL ParseUrl(const TCHAR* pszParam);
	void GetString(CString &str);
	void SetString(LPCTSTR str);
};

// CRLoginApp:
// このクラスの実装については、RLogin.cpp を参照してください。
//

class CRLoginApp : public CWinApp
{
public:
	int m_NextSock;
	CPtrArray m_SocketIdle;
	class CFontChache m_FontData;
	WSADATA wsaData;
	CString m_BaseDir;
	CCommandLineInfoEx *m_pCmdInfo;
	CServerEntry *m_pServerEntry;

#ifdef	USE_KEYMACGLOBAL
	CKeyMacTab m_KeyMacGlobal;
#endif

#ifdef	USE_DIRECTWRITE
	ID2D1Factory *m_pD2DFactory;
	IDWriteFactory *m_pDWriteFactory;

	inline ID2D1Factory *GetD2D1Factory() { return m_pD2DFactory; }
	inline IDWriteFactory *GetDWriteFactory() { return m_pDWriteFactory; }
#endif

#ifdef	USE_JUMPLIST
	void AddShellLink(LPCTSTR pEntryName, IObjectCollection *pObjCol);
	void CreateJumpList(CServerEntryTab *pEntry);
#endif

#ifdef	USE_SAPI
	ISpVoice *m_pVoice;
	void Speek(LPCTSTR str);
#endif

	void SetSocketIdle(class CExtSocket *pSock);
	void DelSocketIdle(class CExtSocket *pSock);
	virtual CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL);
	void GetProfileData(LPCTSTR lpszSection, LPCTSTR lpszEntry, void *lpBuf, int nBufLen, void *lpDef = NULL);
	void GetProfileBuffer(LPCTSTR lpszSection, LPCTSTR lpszEntry, CBuffer &Buf);
	void GetProfileStringArray(LPCTSTR lpszSection, LPCTSTR lpszEntry, CStringArrayExt &stra);
	void WriteProfileStringArray(LPCTSTR lpszSection, LPCTSTR lpszEntry, CStringArrayExt &stra);
	void GetProfileArray(LPCTSTR lpszSection, CStringArrayExt &stra);
	void WriteProfileArray(LPCTSTR lpszSection, CStringArrayExt &stra);
	int GetProfileSeqNum(LPCTSTR lpszSection, LPCTSTR lpszEntry);
	void GetProfileKeys(LPCTSTR lpszSection, CStringArrayExt &stra);
	void DelProfileEntry(LPCTSTR lpszSection, LPCTSTR lpszEntry);
	void RegisterShellProtocol(LPCTSTR pSection, LPCTSTR pOption);
	void RegisterDelete(HKEY hKey, LPCTSTR pSection, LPCTSTR pKey);
	void RegisterSave(HKEY hKey, LPCTSTR pSection, CBuffer &buf);
	void RegisterLoad(HKEY hKey, LPCTSTR pSection, CBuffer &buf);
	void GetVersion(CString &str);
	void SSL_Init();

	void OpenProcsCmd(CCommandLineInfoEx *pCmdInfo);
	void OpenCommandEntry(LPCTSTR entry);
	void OpenCommandLine(LPCTSTR str);
	BOOL CheckDocument(class CRLoginDoc *pDoc);

	BOOL OnInUseCheck(COPYDATASTRUCT *pCopyData);
	BOOL InUseCheck();
	BOOL OnlineEntry(COPYDATASTRUCT *pCopyData);
	BOOL OnlineCheck(LPCTSTR entry);
#ifdef	USE_KEYMACGLOBAL
	void OnUpdateKeyMac(COPYDATASTRUCT *pCopyData);
	void UpdateKeyMacGlobal();
#endif
	void OnSendBroadCast(COPYDATASTRUCT *pCopyData);
	void SendBroadCast(CBuffer &buf);
	void OnSendBroadCastMouseWheel(COPYDATASTRUCT *pCopyData);
	void SendBroadCastMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	void SetDefaultPrinter();

	CRLoginApp();

// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

// 実装
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnFilePrintSetup();
	afx_msg void OnDispwinidx();
	afx_msg void OnDialogfont();
};

extern CRLoginApp theApp;
extern BOOL ExDwmEnable;
extern void ExDwmEnableWindow(HWND hWnd, BOOL bEnable);