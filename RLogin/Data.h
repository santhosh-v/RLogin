// Data.h: CData クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__6A23DC3E_3DDC_47BD_A6FC_E0127564AE6E__INCLUDED_)
#define AFX_DATA_H__6A23DC3E_3DDC_47BD_A6FC_E0127564AE6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include <afxmt.h>
#include "Regex.h"
#include "ChatStatDlg.h"

#include "openssl/bn.h"
#include "openssl/ec.h"

int	BinaryFind(void *ptr, void *tab, int size, int max, int (* func)(const void *, const void *), int *base);
BOOL IsZeroMemory(void *ptr, int len);

#define	NIMALLOC		256
#define	CBUFGETMAX		(512 * 1024)

#define	KANJI_UNKNOWN	0
#define	KANJI_EUC		1
#define	KANJI_SJIS		2
#define	KANJI_UTF8		3
#define	KANJI_UTF16LE	4
#define	KANJI_UTF16BE	5

#define	MAPING_FILL		0
#define	MAPING_FIT		1
#define	MAPING_STRETCH	2
#define	MAPING_TILE		3
#define	MAPING_CENTER	4
#define	MAPING_PAN		5
#define	MAPING_DESKTOP	6
#define	MAPING_LEFTOP	7
#define	MAPING_LEFCEN	8
#define	MAPING_LEFBTM	9
#define	MAPING_CENTOP	10
#define	MAPING_CENCEN	11
#define	MAPING_CENBTM	12
#define	MAPING_RIGTOP	13
#define	MAPING_RIGCEN	14
#define	MAPING_RIGBTM	15

class CBuffer : public CObject
{
public:
	int m_Max;
	int m_Ofs;
	int m_Len;
	LPBYTE m_Data;
	BOOL m_bZero;

	void ReAlloc(int len);
	inline int GetSize() { return (m_Len - m_Ofs); }
	void Consume(int len);
	inline void ConsumeEnd(int len) { m_Len -= len; }
	void Apend(LPBYTE buff, int len);
	void RemoveAll();
	void Move(CBuffer &data);
	void Swap(CBuffer &data);
	inline void Clear() { m_Len = m_Ofs = 0; }
	inline LPBYTE GetPtr() { return (m_Data + m_Ofs); }
	inline LPBYTE GetPos(int pos) { return (m_Data + m_Ofs + pos); }
	inline BYTE GetAt(int pos) { return m_Data[m_Ofs + pos]; }
	LPBYTE PutSpc(int len);
	void RoundUp(int len);

	void Put8Bit(int val);
	void Put16Bit(int val);
	void Put32Bit(LONG val);
	void Put64Bit(LONGLONG val);
	void PutBuf(LPBYTE buf, int len);
	void PutStr(LPCSTR str);
	void PutStrT(LPCTSTR str);
	void PutBIGNUM(const BIGNUM *val);
	void PutBIGNUM2(const BIGNUM *val);
	void PutEcPoint(const EC_GROUP *curve, const EC_POINT *point);
	inline void PutByte(int val) { Put8Bit(val); }
	void PutWord(int val);
	void PutDword(int val);
	void PutText(LPCWSTR str);

	int Get8Bit();
	int Get16Bit();
	LONG Get32Bit();
	LONGLONG Get64Bit();
	int GetStr(CStringA &str);
	int GetStrT(CString &str);
	int GetBuf(CBuffer *buf);
	BIGNUM *GetBIGNUM(BIGNUM *val);
	BIGNUM *GetBIGNUM2(BIGNUM *val);
	BIGNUM *GetBIGNUM_SecSh(BIGNUM *val);
	int GetEcPoint(const EC_GROUP *curve, EC_POINT *point);
	int GetDword();
	int GetWord();
	inline int GetByte() { return GetChar(); }
	int GetChar();
	void GetText(CString &str);

	inline void SET8BIT(LPBYTE pos, int val) { *pos = (BYTE)(val); }
	void SET16BIT(LPBYTE pos, int val);
	void SET32BIT(LPBYTE pos, int val);
	void SET64BIT(LPBYTE pos, LONGLONG val);

	inline int PTR8BIT(LPBYTE pos) { return (int)(*pos); }
	int PTR16BIT(LPBYTE pos);
	LONG PTR32BIT(LPBYTE pos);
	LONGLONG PTR64BIT(LPBYTE pos);

	LPCSTR Base64Param(LPCSTR str);
	LPCTSTR Base64Decode(LPCTSTR str);
	void Base64Encode(LPBYTE buf, int len);
	LPCTSTR Base16Decode(LPCTSTR str);
	void Base16Encode(LPBYTE buf, int len);
	void PutHexBuf(LPBYTE buf, int len);
	LPCTSTR QuotedDecode(LPCTSTR str);
	void QuotedEncode(LPBYTE buf, int len);
	void BubbleBabble(LPBYTE buf, int len);
	LPCTSTR BubBabDecode(LPCTSTR str);
	void md5(LPCTSTR str);
	BOOL LoadFile(LPCTSTR filename);
	BOOL SaveFile(LPCTSTR filename);
	int KanjiCheck(int type = KANJI_UNKNOWN);
	void KanjiConvert(int type);
	BOOL ReadString(CString &str);

	void SetMbsStr(LPCTSTR str);
	LPCSTR operator += (LPCSTR str);
	LPCWSTR operator += (LPCWSTR str);
	void AddFormat(LPCSTR pszFormat, ...);
	void AddFormat(LPCWSTR pszFormat, ...);
	void Format(LPCSTR pszFormat, ...);
	void Format(LPCWSTR pszFormat, ...);

	inline LPCSTR operator += (CHAR ch) { PutByte(ch); return *this; }
	inline LPCWSTR operator += (WCHAR ch) { PutWord(ch); return *this; }
	inline LPCSTR operator = (LPCSTR str) { Clear(); *this += str; return *this; }
	inline LPCWSTR operator = (LPCWSTR str) { Clear(); *this += str; return *this; }
	inline const CBuffer & operator = (CBuffer &data) { Clear(); Apend(data.GetPtr(), data.GetSize()); return *this; }
	inline operator LPCSTR() { if ( m_Max <= m_Len) ReAlloc(1); m_Data[m_Len] = 0; return (LPCSTR)GetPtr(); }
	inline operator LPCWSTR() { if ( m_Max <= (m_Len + 1) ) ReAlloc(2); m_Data[m_Len] = 0; m_Data[m_Len + 1] = 0; return (LPCWSTR)GetPtr(); }
	inline operator const DWORD *() { if ( m_Max <= (m_Len + 3) ) ReAlloc(4); m_Data[m_Len] = 0; m_Data[m_Len + 1] = 0; m_Data[m_Len + 2] = 0; m_Data[m_Len + 3] = 0; return (const DWORD *)GetPtr(); }

#ifdef	DEBUG
	void Dump();
#endif

	CBuffer(LPCSTR str);
	CBuffer(LPCWSTR str);
	CBuffer(LPBYTE pData, int len);
	CBuffer(int size);
	CBuffer();
	~CBuffer();
};

class CSpace : public CObject
{
public:
	int m_Len;
	LPBYTE m_Data;

	inline int GetSize() { return m_Len; }
	inline LPBYTE GetPtr() { return m_Data; }
	LPBYTE PutBuf(LPBYTE buf, int len)  { if ( m_Data != NULL ) delete [] m_Data; if ( len <= 0 ) m_Data = NULL; else { m_Data = new BYTE[len]; memcpy(m_Data, buf, len); } m_Len = len; return m_Data; }
	LPBYTE SetSize(int len) { if ( m_Data != NULL ) delete [] m_Data; if ( len <= 0 ) m_Data = NULL; else m_Data = new BYTE[len]; m_Len = len; return m_Data;  }

	const CSpace & operator = (CSpace &data) { PutBuf(data.GetPtr(), data.GetSize()); return *this; }

	CSpace() { m_Len = 0; m_Data = NULL; }
	~CSpace() { if ( m_Data != NULL ) delete [] m_Data; }
};

class CMenuLoad : public CMenu
{
public:
	inline BOOL LoadMenu(UINT nIDResource) { return LoadMenu(MAKEINTRESOURCE(nIDResource)); }
	BOOL LoadMenu(LPCTSTR lpszResourceName);
	
	static BOOL UpdateMenuShortCut(CMenu *pMenu, CMenu *pUpdateMenu, LPCTSTR pShortCut);
	static CMenu *GetItemSubMenu(UINT nId, CMenu *pMenu);
	static BOOL GetPopUpMenu(UINT nId, CMenu &PopUpMenu);
};

class CStringLoad : public CString
{
public:
	CStringLoad();
	CStringLoad(int nID) { LoadString(nID); }

	inline void operator = (LPCWSTR str) { *((CString *)this) = str; }
	inline void operator = (LPCSTR  str) { *((CString *)this) = str; }

	BOOL LoadString(UINT nID);

	BOOL IsDigit(LPCTSTR str);
	int CompareDigit(LPCTSTR dis);

#ifdef	DEBUG
	int Compare(LPCTSTR dis);
#endif
};

class CStrNode : public CObject
{
public:
	int m_Type;
	CString m_Str;
	class CStrNode *m_Next;
	class CStrNode *m_List;
	class CStrNode *m_Stack;

	void AddNext(CStrNode *np);
	void AddList(CStrNode *np);

	CStrNode();
	CStrNode(int type, LPCTSTR str);
	~CStrNode();
};

class CStringArrayExt : public CStringArray  
{
public:
	void AddVal(int value) { CString str; str.Format(_T("%d"), value); Add(str); }
	int GetVal(int index) { return _tstoi(GetAt(index)); }
	void AddBin(void *buf, int len);
	int GetBin(int index, void *buf, int len);
	void GetBuf(int index, CBuffer &buf);
	void AddArray(CStringArrayExt &stra);
	void GetArray(int index, CStringArrayExt &stra);
	void SetString(CString &str, int sep = '\t');
	void GetString(LPCTSTR str, int sep = '\t');
	void SetBuffer(CBuffer &buf);
	void GetBuffer(CBuffer &buf);
	const CStringArrayExt & operator = (CStringArrayExt &data);
	void Serialize(CArchive& ar);
	int Find(LPCTSTR str);
	int FindVal(int value) { CString str;  str.Format(_T("%d"), value); return Find(str); }
	int FindNoCase(LPCTSTR str);
	int Match(LPCTSTR str);
	void AddSort(LPCTSTR str);
	int FindSort(LPCTSTR str);

	CStrNode *ParseWord(LPCTSTR *ptr);
	CStrNode *ParseLine(CStrNode *top, LPCTSTR *ptr, BOOL bNest);
	CStrNode *ParseList(LPCTSTR *ptr, BOOL bNest);
	void ParseNode(CStrNode *top, CStrNode *stack, CString &str);
	void GetParam(LPCTSTR str);

	void GetCmds(LPCTSTR cmds);

	CStringArrayExt();
	CStringArrayExt(int nID);
};

class CStringMaps : public CObject
{
public:
	CArray<CStringW, CStringW &> m_Data;

	int Add(LPCWSTR wstr);
	int Find(LPCWSTR wstr);
	int Next(LPCWSTR wstr, int pos);
	LPCWSTR GetAt(int n) { return m_Data[n]; }
	void RemoveAll() { m_Data.RemoveAll(); }
	int GetSize() { return (int)m_Data.GetSize(); }
	void AddWStrBuf(LPBYTE lpBuf, int nLen);

	CStringMaps();
	~CStringMaps();
};

#define	JSON_UTF16		0
#define	JSON_UTF8		1
#define	JSON_OEM		2

#ifdef	_UNICODE
#define	JSON_TCODE		JSON_UTF16
#else
#define	JSON_TCODE		JSON_OEM
#endif

class CStringIndex : public CObject
{
public:
	BOOL m_bNoCase;
	BOOL m_bNoSort;
	BOOL m_bString;
	BOOL m_bEmpty;
	int m_Value;
	CString m_nIndex;
	CString m_String;
	CArray<CStringIndex, CStringIndex &> m_Array;
	CStringIndex *m_pOwner;
	CDWordArray m_TabData;

	CStringIndex();
	CStringIndex(BOOL bNoCase, BOOL bNoSort);
	~CStringIndex();

	const CStringIndex & operator = (CStringIndex &data);
	CStringIndex & operator [] (LPCTSTR str);
	CStringIndex & operator [] (int nIndex) { return m_Array[nIndex]; }
	const LPCTSTR operator = (LPCTSTR str) { m_bString = TRUE; m_bEmpty = FALSE; return (m_String = str); }
	operator LPCTSTR () const { return m_String; }
	const int operator = (int val) { m_bString = FALSE; m_bEmpty = FALSE; return (m_Value = val); }
	operator int () const { return (m_bString ? _tstoi(m_String) : m_Value); }

	BOOL IsEmpty() { return m_bEmpty; }
	int GetSize() { return (int)m_Array.GetSize(); }
	void SetSize(int nIndex) { m_Array.SetSize(nIndex); }
	LPCTSTR GetIndex() { return m_nIndex; }
	void RemoveAll() { m_Array.RemoveAll(); m_bEmpty = TRUE; }
	void RemoveAt(int index) { m_Array.RemoveAt(index); }
	int Add(LPCTSTR str) { CStringIndex tmp(m_bNoCase, m_bNoSort); tmp = str; return (int)m_Array.Add(tmp); }
	int Add(int value) { CStringIndex tmp(m_bNoCase, m_bNoSort); tmp = value; return (int)m_Array.Add(tmp); }
	int Add(CStringIndex &data) { int n = (int)m_Array.Add(data); return n; }
	CStringIndex & Add() { CStringIndex tmp(m_bNoCase, m_bNoSort); int n = (int)m_Array.Add(tmp); return m_Array[n]; }
	void SetNoCase(BOOL b) { m_bNoCase = b; }
	void SetNoSort(BOOL b) { m_bNoSort = b; }

	int Find(LPCTSTR str);
	void SetArray(LPCTSTR str);

	CStringIndex &AddPath(LPCTSTR path, BOOL *pNest = NULL);
	CStringIndex *FindPath(LPCTSTR path);
	CStringIndex *RootPath(LPCTSTR path);
	void GetPath(CString &path);
	void OwnerLink(CStringIndex *pOwner);

	void GetBuffer(CBuffer *bp);
	void SetBuffer(CBuffer *bp);
	void GetString(LPCTSTR str);
	void SetString(CString &str);

	void SetPackStr(CStringA &mbs);
	LPCTSTR GetPackStr(LPCTSTR str);
	BOOL ReadString(CArchive& ar, CString &str);
	void Serialize(CArchive& ar, LPCSTR base = NULL, int version = 3);
	BOOL MsgStr(CString &str,  LPCSTR base = NULL);

	void SubOscValue(LPCTSTR &str);
	void SubOscParam(LPCTSTR &str);
	void GetOscString(LPCTSTR str);

	TCHAR SubJsonChara(LPCTSTR &str);
	TCHAR SubJsonString(LPCTSTR &str);
	TCHAR SubJsonNumber(LPCTSTR &str);
	TCHAR SubJsonValue(LPCTSTR &str);
	TCHAR SubJsonArray(LPCTSTR &str);
	TCHAR SubJsonObject(LPCTSTR &str);
	BOOL GetJsonFormat(LPCTSTR str);
	void AddJasonTstr(CBuffer &mbs, LPCTSTR str, int nCode);
	void AddJasonString(CBuffer &mbs, LPCTSTR str, int nCode);
	void SetJsonFormat(CBuffer &mbs, int nest = 0, int nCode = JSON_OEM);

	void SubQueryValue(LPCTSTR &str);
	void GetQueryString(LPCTSTR str);
	void AddQueryString(CStringA &mbs, LPCTSTR str, BOOL bUtf8);
	void SetQueryString(CStringA &mbs, LPCSTR base = NULL, BOOL bUtf8 = FALSE);

#ifdef	DEBUG
	void Dump(int nest);
#endif
};

class CStringBinary : public CObject
{
public:
	CStringBinary *m_pRoot;
	CStringBinary *m_pLeft;
	CStringBinary *m_pRight;
	CString m_Index;
	CString m_String;
	int m_Value;
	int m_Balance;

	CStringBinary();
	~CStringBinary();

	// Root
	void RemoveAll();
	CStringBinary & operator [] (LPCTSTR str);
	CStringBinary & operator [] (int number);
	CStringBinary * Find(LPCTSTR str);
	CStringBinary * FindValue(int value);

	// Node
	CStringBinary(LPCTSTR str);
	CStringBinary * Add(CStringBinary **ppTop, LPCTSTR str, BOOL &bNew);
	CStringBinary * FindNode(LPCTSTR str);
	CStringBinary * FindNodeValue(int value);

#ifdef	DEBUG
	void Tree(int nest);
	void TreeNode(int nest);
#endif

	const LPCTSTR operator = (LPCTSTR str) { m_Value = 0; return (m_String = str); }
	operator LPCTSTR () const { return m_String; }
	const int operator = (int val) { return (m_Value = val); }
	operator int () const { return m_Value; }
};

#define	PARA_NOT	0xFFFFFFFF
#define	PARA_OPT	0xFFFFFFFE
#define	PARA_MAX	0x7FFFFFFF

class CParaIndex : CObject
{
public:
	DWORD m_Data;
	CString m_Str;
	CArray<CParaIndex, CParaIndex &> m_Array;

	CParaIndex();
	CParaIndex(DWORD data);
	~CParaIndex();

	const CParaIndex & operator = (CParaIndex &data);
	const DWORD operator = (DWORD data) { m_Data = data; return m_Data; }
	CParaIndex & operator [] (int nIndex) { return m_Array[nIndex]; }
	operator DWORD () const { return m_Data; }
	INT_PTR Add(DWORD data) { CParaIndex tmp(data); return m_Array.Add(tmp); }
	INT_PTR Add(CParaIndex &data) { return m_Array.Add(data); }
	INT_PTR GetSize() { return m_Array.GetSize(); }
	void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1) { m_Array.SetSize(nNewSize, nGrowBy); }
	void RemoveAll() { m_Array.RemoveAll(); }
	BOOL IsEmpty() { return (m_Data == PARA_NOT || m_Data == PARA_OPT ? TRUE : FALSE); }
	BOOL IsOpt() { return (m_Data == PARA_OPT ? TRUE : FALSE); }
	BOOL AddOpt(BYTE c, BOOL bAdd);
};

class CBmpFile : public CObject
{
public:
	CBitmap m_Bitmap;
	int m_Width, m_Height;
	CString m_FileName;
	COLORREF m_BkColor;
	int m_Alpha;
	class CTextBitMap *m_pTextBitMap;
	CString m_Title;
	CImage m_Image;
	int m_bkIndex;
	int m_Style;
	CRect m_ScreenPos;

	BOOL LoadFile(LPCTSTR filename);
	CBitmap *GetBitmap(CDC *pDC, int width, int height, COLORREF bkcolor, int Alpha = 255, int Style = 0, CWnd *pWnd = NULL);
	CBitmap *GetTextBitmap(CDC *pDC, int width, int height, COLORREF bkcolor, class CTextBitMap *pTextBitMap, LPCTSTR title, int Alpha, int Style, CWnd *pWnd);

	static int GifTrnsIndex(LPBYTE lpBuf, int len);

	CBmpFile();
	virtual ~CBmpFile();
};

class CFontChacheNode : public CObject
{
public:
	CFont *m_pFont;
	LOGFONT m_LogFont;
	CFontChacheNode *m_pNext;
	int m_Width;
	int m_Height;
	int m_CharSet;
	int m_Style;
	int m_Quality;
	BOOL m_bFixed;
	TEXTMETRIC m_Metric;

	CFont *Open(LPCTSTR pFontName, int Width, int Height, int CharSet, int Style, int Quality);

	CFontChacheNode();
	~CFontChacheNode();
};

#define	FONTCACHEMAX	64

class CFontChache : public CObject
{
public:
	CFontChacheNode *m_pTop[4];
	CFontChacheNode m_Data[FONTCACHEMAX];

	CFontChacheNode *GetFont(LPCTSTR pFontName, int Width, int Height, int CharSet, int Style, int Quality);
	CFontChache();
};

class CMutexLock : public CObject
{
public:
	CMutex *m_pMutex;
	CSingleLock *m_pLock;
	LPCTSTR m_pName;

	CMutexLock(LPCTSTR lpszName = NULL); 
	~CMutexLock();

	inline BOOL Lock(DWORD dwTimeOut) { return m_pLock->Lock(dwTimeOut); }
	inline void Unlock() { m_pLock->Unlock(); }
	inline BOOL IsLocked() { return m_pLock->IsLocked(); }
};

class COptObject : public CObject
{
public:
	LPCTSTR m_pSection;
	int m_MinSize;

	virtual void Init();
	virtual void SetArray(CStringArrayExt &stra) = 0;
	virtual void GetArray(CStringArrayExt &stra) = 0;

	virtual void Serialize(int mode);					// To Profile
	virtual void Serialize(int mode, CBuffer &buf);		// To CBuffer
	virtual void Serialize(CArchive &ar);				// To Archive

	COptObject();
};

class CStrScriptNode : public CObject
{
public:
	class CStrScriptNode	*m_Left;
	class CStrScriptNode	*m_Right;

	CRegEx		m_Reg;
	CString		m_RecvStr;
	CString		m_SendStr;

	CStrScriptNode();
	~CStrScriptNode();
};

#define	SCPSTAT_NON		0
#define	SCPSTAT_EXEC	1
#define	SCPSTAT_MAKE	2

class CStrScript : public CObject
{
public:
	CStrScriptNode	*m_Node;
	CStrScriptNode	*m_Exec;

	CStringW		m_Str;
	CRegExRes		m_Res;

	CChatStatDlg	m_StatDlg;
	BOOL			m_MakeChat;
	BOOL			m_MakeFlag;
	CStringW		m_Line[3];

	CString			m_LexTmp;

	void Init();
	CStrScriptNode *CopyNode(CStrScriptNode *np);

	void SetNode(CStrScriptNode *np, CBuffer &buf);
	CStrScriptNode *GetNode(CBuffer &buf);
	void SetBuffer(CBuffer &buf);
	int GetBuffer(CBuffer &buf);

	LPCTSTR QuoteStr(CString &tmp, LPCTSTR str);
	void SetNodeStr(CStrScriptNode *np, CString &str, int nst);
	int GetLex(LPCTSTR &str);
	CStrScriptNode *GetNodeStr(int &lex, LPCTSTR &str);
	void SetString(CString &str);
	void GetString(LPCTSTR str);

	void EscapeStr(LPCWSTR str, CString &buf, BOOL reg = FALSE);
	void AddNode(LPCWSTR recv, LPCWSTR send);

	int Status();
	void ExecInit();
	void ExecStop();

	void ExecNode(CStrScriptNode *np);
	LPCWSTR ExecChar(DWORD ch);

	void SendStr(LPCWSTR str, int len, class CServerEntry *ep = NULL);

	void SetTreeNode(CStrScriptNode *np, CTreeCtrl &tree, HTREEITEM own);
	void SetTreeCtrl(CTreeCtrl &tree);

	CStrScriptNode *GetTreeNode(CTreeCtrl &tree, HTREEITEM hti);
	void GetTreeCtrl(CTreeCtrl &tree);

	const CStrScript & operator = (CStrScript &data);

	CStrScript();
	~CStrScript();
};

class CServerEntry : public CObject
{
public:
	CString m_EntryName;
	CString m_HostName;
	CString m_PortName;
	CString m_UserName;
	CString m_PassName;
	CString m_TermName;
	CString m_IdkeyName;
	int m_KanjiCode;
	int m_ProtoType;
	CBuffer m_ProBuffer;
	BOOL m_CheckFlag;
	int m_Uid;
	CStrScript m_ChatScript;
	int m_ProxyMode;
	BOOL m_ProxySSLKeep;
	CString m_ProxyHost;
	CString m_ProxyPort;
	CString m_ProxyUser;
	CString m_ProxyPass;
	CString m_Memo;
	CString m_Group;
	CString m_ScriptFile;
	CString m_ScriptStr;
	CString m_HostNameProvs;
	CString m_PortNameProvs;
	CString m_UserNameProvs;
	CString m_PassNameProvs;
	CString m_ProxyHostProvs;
	CString m_ProxyPortProvs;
	CString m_ProxyUserProvs;
	CString m_ProxyPassProvs;
	CString m_BeforeEntry;
	BOOL m_ReEntryFlag;
	int m_DocType;
	CString m_IconName;
	BOOL m_bPassOk;
	BOOL m_bSelFlag;
	BOOL m_bOptFixed;

	void Init();
	void SetArray(CStringArrayExt &stra);
	void GetArray(CStringArrayExt &stra);
	void ScriptInit(int cmds, int shift, class CScriptValue &value);
	void ScriptValue(int cmds, class CScriptValue &value, int mode);
	void SetBuffer(CBuffer &buf);
	int GetBuffer(CBuffer &buf);
	void SetProfile(LPCTSTR pSection);
	int GetProfile(LPCTSTR pSection, int Uid);
	void DelProfile(LPCTSTR pSection);
	void Serialize(CArchive &ar);
	void SetIndex(int mode, CStringIndex &index);
	void DiffIndex(CServerEntry &orig, CStringIndex &index);

	LPCTSTR GetKanjiCode();
	void SetKanjiCode(LPCTSTR str);
	LPCTSTR GetProtoName();
	int GetProtoType(LPCTSTR str);

	const CServerEntry & operator = (CServerEntry &data);
	CServerEntry();
};

class CServerEntryTab : public COptObject
{
public:
	CArray<CServerEntry, CServerEntry &> m_Data;

	void Init();
	void SetArray(CStringArrayExt &stra);
	void GetArray(CStringArrayExt &stra);
	void Serialize(int mode);

	int AddEntry(CServerEntry &Entry);
	void UpdateAt(int nIndex);
	void RemoveAt(int nIndex);
	void ReloadAt(int nIndex);

	inline CServerEntry &GetAt(int nIndex) { return m_Data[nIndex]; }
	inline INT_PTR GetSize() { return m_Data.GetSize(); }

	CServerEntryTab();
};

#define	MASK_SHIFT	00001
#define	MASK_CTRL	00002
#define	MASK_ALT	00004
#define	MASK_APPL	00010
#define	MASK_CKM	00020
#define	MASK_VT52	00040
//#define	MASK_NUMLCK	00100
//#define	MASK_SCRLCK	00200
//#define	MASK_CAPLCK	00400

#ifdef	USE_CLIENTKEY
	#define	VK_LMOUSE_LEFT_TOP			512
	#define	VK_LMOUSE_LEFT_CENTER		513
	#define	VK_LMOUSE_LEFT_BOTTOM		514
	#define	VK_LMOUSE_CENTER_TOP		515
	#define	VK_LMOUSE_CENTER_CENTER		516
	#define	VK_LMOUSE_CENTER_BOTTOM		517
	#define	VK_LMOUSE_RIGHT_TOP			518
	#define	VK_LMOUSE_RIGHT_CENTER		519
	#define	VK_LMOUSE_RIGHT_BOTTOM		520
#endif

class CKeyNode : public CObject
{
public:
	int m_Code;
	int m_Mask;
	CBuffer m_Maps;
	CString m_Temp;

	void SetKeyMap(LPCTSTR str, int type, char map[256]);
	LPCTSTR GetMaps();
	void SetMaps(LPCTSTR str);
	LPCTSTR GetCode();
	void SetCode(LPCTSTR name);
	LPCTSTR GetMask();
	void CommandLine(LPCWSTR str, CStringW &cmd);
	void SetComboList(CComboBox *pCombo);

	const CKeyNode & operator = (CKeyNode &data);

	CKeyNode();
};

class CKeyCmds : public CObject
{
public:
	int m_Id;
	BOOL m_Flag;
	CString m_Menu;
	CString m_Text;

	void SetMenu(CMenu *pMenu);
	void ResetMenu(CMenu *pMenu);
	const CKeyCmds & operator = (CKeyCmds &data);
};

class CKeyCmdsTab : public CObject
{
public:
	CArray<CKeyCmds, CKeyCmds &> m_Data;

	int Add(CKeyCmds &cmds);
	int Find(int id);
	void ResetMenuAll(CMenu *pMenu);
	void SortText();

	inline CKeyCmds & operator [] (int nIndex) { return m_Data[nIndex]; }
	inline int GetSize() { return (int)m_Data.GetSize(); }
	inline void SetSize(int sz) { 	m_Data.SetSize(sz); }
	inline void RemoveAt(int pos) { m_Data.RemoveAt(pos); }
	inline void RemoveAll() { m_Data.RemoveAll(); }

	const CKeyCmdsTab & operator = (CKeyCmdsTab &data);
};

class CKeyNodeTab : public COptObject
{
public:
	BOOL m_CmdsInit;
	CKeyCmdsTab m_Cmds;
	CArray<CKeyNode, CKeyNode &> m_Node;

	void Init();
	void SetArray(CStringArrayExt &stra);
	void GetArray(CStringArrayExt &stra);
	void SetIndex(int mode, CStringIndex &index);
	void DiffIndex(CKeyNodeTab &orig, CStringIndex &index);
	void ScriptInit(int cmds, int shift, class CScriptValue &value);
	void ScriptValue(int cmds, class CScriptValue &value, int mode);

	BOOL Find(int code, int mask, int *base);
	int Add(CKeyNode &node);
	int Add(int code, int mask, LPCTSTR str);
	int Add(LPCTSTR code, int mask, LPCTSTR maps);
	BOOL FindKeys(int code, int mask, CBuffer *pBuf, int base, int bits);
	BOOL FindMaps(int code, int mask, CBuffer *pBuf);
	BOOL FindCapInfo(LPCTSTR name, CBuffer &buf);

	inline CKeyNode &GetAt(int pos) { return m_Node[pos]; }
	inline int GetSize() { return (int)m_Node.GetSize(); }
	inline void SetSize(int sz) { 	m_Node.SetSize(sz, 16); }
	inline void RemoveAt(int pos) { m_Node.RemoveAt(pos); m_CmdsInit = FALSE; }
	inline CKeyNode & operator[](int nIndex) { return m_Node[nIndex]; }

	const CKeyNodeTab & operator = (CKeyNodeTab &data);
	CKeyNodeTab();

	void BugFix(int fix);
	void CmdsInit();
	static int GetCmdsKey(LPCWSTR str);
	static void SetComboList(CComboBox *pCombo);
	static int GetDecKeyToCode(int code);
	static LPCTSTR GetCmdsStr(int code);
};

class CKeyMac : public CObject
{
public:
	int m_Len;
	LPBYTE m_Data;

	LPBYTE GetPtr() { return m_Data; }
	int GetSize() { return m_Len; }

	void GetMenuStr(CString &str);
	void GetStr(CString &str);
	void SetStr(LPCTSTR str);
	void SetBuf(LPBYTE buf, int len);

	BOOL operator == (CKeyMac &data);
	const CKeyMac & operator = (CKeyMac &data);
	CKeyMac();
	~CKeyMac();
};

#define	KEYMACBUFMAX	(32 * 1024)

class CKeyMacTab : public COptObject
{
public:
#ifdef	USE_KEYMACGLOBAL
	BOOL m_bUpdate;
	BOOL m_bGlobal;
#endif
	CArray<CKeyMac, CKeyMac &> m_Data;

	void Init();
	void GetArray(CStringArrayExt &stra);
	void SetArray(CStringArrayExt &stra);
	void SetIndex(int mode, CStringIndex &index);
	void ScriptInit(int cmds, int shift, class CScriptValue &value);
	void ScriptValue(int cmds, class CScriptValue &value, int mode);

	void Top(int nIndex);
	void Add(CKeyMac &tmp);
	void GetAt(int nIndex, CBuffer &buf);
	void SetHisMenu(CMenu *pMainMenu);

	inline CKeyMac & operator[](int nIndex) { return m_Data[nIndex]; }
	inline int GetSize() { return (int)m_Data.GetSize(); }
	const CKeyMacTab & operator = (CKeyMacTab &data);
	CKeyMacTab();
};

#define	PFD_LOCAL		0
#define	PFD_SOCKS		1
#define	PFD_REMOTE		2
#define	PFD_RSOCKS		3

#define	PFD_IS_SOCKS(n)	((n & 1) != 0)

#define	SEL_IPV6V4		0
#define	SEL_IPV6		1
#define	SEL_IPV4		2

typedef struct _ttymode_node {
	BYTE	opcode;
	DWORD	param;
} ttymode_node;

class CParamTab : public COptObject
{
public:
	CString m_IdKeyStr[9];
	CStringArrayExt m_AlgoTab[12];
	CStringArrayExt m_PortFwd;
	CStringArrayExt m_IdKeyList;
	CString m_XDisplay;
	CString m_ExtEnvStr;
	DWORD m_OptTab[8];
	CString m_Reserve;
	int m_SelIPver;
	CArray<ttymode_node, ttymode_node> m_TtyMode;
	int m_RsaExt;
	CString m_VerIdent;
	BOOL m_x11AuthFlag;
	CString m_x11AuthName;
	CString m_x11AuthData;
	BOOL m_bInitPageant;
	int m_StdIoBufSize;
	int m_TransmitLimit;

	CParamTab();
	void Init();
	void GetArray(CStringArrayExt &stra);
	void SetArray(CStringArrayExt &stra);
	void SetIndex(int mode, CStringIndex &index);
	void DiffIndex(CParamTab &orig, CStringIndex &index);
	void ScriptInit(int cmds, int shift, class CScriptValue &value);
	void ScriptValue(int cmds, class CScriptValue &value, int mode);

	void GetProp(int num, CString &str, int shuffle = FALSE);
	int GetPropNode(int num, int node, CString &str);

	const CParamTab & operator = (CParamTab &data);
};

#define	FEXT_BUF_MAX	(16 * 1024)
#define	FEXT_FLUSH_MAX	(512 * 1024)
#define	FEXT_MSG_MAX	64				// 512K / 16K = 32 * 2 = 64

class CFileThread : public CWinThread
{
	DECLARE_DYNCREATE(CFileThread)

public:
	CFileThread();
	virtual ~CFileThread();

public:
	class CFileExt *m_pOwner;
	CEvent *m_pSyncEvent;
	int m_MsgCount;
	BOOL m_bRuning;
	BOOL m_bWriteError;
	CString m_ErrorMsg;

public:
	inline DWORD WaitEvent(DWORD dwMsec) { return WaitForSingleObject(m_pSyncEvent->m_hObject, dwMsec); }

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileWrite(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFileFlush(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFileSync(WPARAM wParam, LPARAM lParam);
};

class CFileExt : public CFile
{
public:
	UINT m_WriteLen;
	BYTE *m_pWriteBuffer;
	UINT m_ReadLen;
	UINT m_ReadPos;
	BYTE *m_pReadBuffer;
	CFileThread m_FileThread;
	LONGLONG m_TotalWrite;
	BOOL m_bWriteFlush;
	BOOL m_bInitBuffer;
	CBuffer m_LoadBuffer;
	int m_KanjiType;

	void Init();
	void WriteFlush();
	inline BOOL IsWriteError() { return m_FileThread.m_bWriteError; }
	BOOL ReadString(CString &str);

	CFileExt();
#if	_MSC_VER >= _MSC_VER_VS10
	CFileExt(CAtlTransactionManager* pTM);
#endif
	CFileExt(HANDLE hFile);
	CFileExt(LPCTSTR lpszFileName, UINT nOpenFlags);

	virtual ~CFileExt();
	
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL);
#if	_MSC_VER >= _MSC_VER_VS10
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CAtlTransactionManager* pTM, CFileException* pError);
#endif

	virtual ULONGLONG GetPosition( ) const;
	virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom);

	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);

	virtual void Flush();
	virtual void Close();
};

class CTranslateString : public CObject
{
public:
	BOOL m_bSplitFlag;
	TCHAR m_SplitChar;
	CString *m_pBaseString;
	CString m_SourceString;
	CString m_FrontString;
	CString m_AddString;

	void SetSourceString(LPCTSTR str);
	void SetTargetString(LPCTSTR str);
		
	const CTranslateString & operator = (CTranslateString &data);

	CTranslateString();
};
class CTranslateStringTab : public CObject
{
public:
	CArray<CTranslateString, CTranslateString &> m_Data;

	inline INT_PTR GetSize() { return m_Data.GetSize(); }
	inline CTranslateString & operator [] (int nIndex) { return m_Data[nIndex]; }

	void Add(CString *pStr);
};

class CHttpSession : public CObject
{
public:
	CString m_Protocl;
	CString m_UserName;
	CString m_Password;
	CString m_HostName;
	CString m_PortNumber;
	CString m_Param;

	void ParseUrl(LPCTSTR url);
	BOOL GetRequest(LPCTSTR url, CBuffer &buf, LPCTSTR head = NULL, LPCSTR body = NULL, CString *pMsg = NULL);

	static void QueryString(LPCSTR mbs, CString &str);

	CHttpSession();
	~CHttpSession();
};

class CHttpThreadSession : public CHttpSession
{
public:
	HWND m_TheadhWnd;
	CString m_ThreadUrl;
	CString m_ThreadHead;
	CStringA m_ThreadBody;

	static void Request(LPCTSTR url, LPCTSTR head, LPCSTR body, CWnd *pWnd);
};

class CEmojiImage : public CObject
{
public:
	class CEmojiImage *m_pNext;
	CString m_String;
	CImage m_Image;
	BOOL m_bFileImage;

	CEmojiImage();
	~CEmojiImage();
};

#endif // !defined(AFX_DATA_H__6A23DC3E_3DDC_47BD_A6FC_E0127564AE6E__INCLUDED_)
