#include "StdAfx.h"
#include <HtmlHelp.h>
#include <ShlObj.h>
#include <ShellAPI.h>
#include <CdErr.h> // Nov. 3, 2005 genta	//CDERR_FINDRESFAILURE等
#include "util/shell.h"
#include "util/string_ex2.h"
#include "util/file.h"
#include "util/os.h"
#include "util/module.h"
#include "_os/COsVersionInfo.h"
#include "env/CShareData.h"
#include "env/DLLSHAREDATA.h"
#include "extmodule/CHtmlHelp.h"

int CALLBACK MYBrowseCallbackProc(
	HWND hwnd,
	UINT uMsg,
	LPARAM lParam,
	LPARAM lpData
)
{
	switch( uMsg ){
	case BFFM_INITIALIZED:
		::SendMessage( hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData );
		break;
	case BFFM_SELCHANGED:
		break;
	}
	return 0;

}


/* フォルダ選択ダイアログ */
BOOL SelectDir( HWND hWnd, const TCHAR* pszTitle, const TCHAR* pszInitFolder, TCHAR* strFolderName )
{
	BOOL	bRes;
	TCHAR	szInitFolder[MAX_PATH];

	_tcscpy( szInitFolder, pszInitFolder );
	/* フォルダの最後が半角かつ'\\'の場合は、取り除く "c:\\"等のルートは取り除かない*/
	CutLastYenFromDirectoryPath( szInitFolder );

	// 2010.08.28 フォルダを開くとフックも含めて色々DLLが読み込まれるので移動
	CCurrentDirectoryBackupPoint dirBack;
	ChangeCurrentDirectoryToExeDir();

	// SHBrowseForFolder()関数に渡す構造体
	BROWSEINFO bi;
	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = strFolderName;
	bi.lpszTitle = pszTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS/* | BIF_EDITBOX*//* | BIF_STATUSTEXT*/;
	bi.lpfn = MYBrowseCallbackProc;
	bi.lParam = (LPARAM)szInitFolder;
	bi.iImage = 0;
	// アイテムＩＤリストを返す
	// ITEMIDLISTはアイテムの一意を表す構造体
	LPITEMIDLIST pList = ::SHBrowseForFolder(&bi);
	if( NULL != pList ){
		// SHGetPathFromIDList()関数はアイテムＩＤリストの物理パスを探してくれる
		bRes = ::SHGetPathFromIDList( pList, strFolderName );
		// :SHBrowseForFolder()で取得したアイテムＩＤリストを削除
		::CoTaskMemFree( pList );
		if( bRes ){
			return TRUE;
		}else{
			return FALSE;
		}
	}
	return FALSE;
}



/*!	特殊フォルダのパスを取得する
	SHGetSpecialFolderPath API（shell32.dll version 4.71以上が必要）と同等の処理をする

	@author ryoji
	@date 2007.05.19 新規
*/
BOOL GetSpecialFolderPath( int nFolder, LPTSTR pszPath )
{
	BOOL bRet = FALSE;
	HRESULT hres;
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;

	hres = ::SHGetMalloc( &pMalloc );
	if( FAILED( hres ) )
		return FALSE;

	hres = ::SHGetSpecialFolderLocation( NULL, nFolder, &pidl );
	if( SUCCEEDED( hres ) ){
		bRet = ::SHGetPathFromIDList( pidl, pszPath );
		pMalloc->Free( (void*)pidl );
	}

	pMalloc->Release();

	return bRet;
}



///////////////////////////////////////////////////////////////////////
// From Here 2007.05.25 ryoji 独自拡張のプロパティシート関数群

static WNDPROC s_pOldPropSheetWndProc;	// プロパティシートの元のウィンドウプロシージャ

/*!	独自拡張プロパティシートのウィンドウプロシージャ
	@author ryoji
	@date 2007.05.25 新規
*/
static LRESULT CALLBACK PropSheetWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg ){
	case WM_SHOWWINDOW:
		// 追加ボタンの位置を調整する
		if( wParam ){
			HWND hwndBtn;
			RECT rcOk;
			RECT rcTab;
			POINT pt;

			hwndBtn = ::GetDlgItem( hwnd, 0x02000 );
			::GetWindowRect( ::GetDlgItem( hwnd, IDOK ), &rcOk );
			::GetWindowRect( PropSheet_GetTabControl( hwnd ), &rcTab );
			pt.x = rcTab.left;
			pt.y = rcOk.top;
			::ScreenToClient( hwnd, &pt );
			::MoveWindow( hwndBtn, pt.x, pt.y, 140, rcOk.bottom - rcOk.top, FALSE );
		}
		break;

	case WM_COMMAND:
		// 追加ボタンが押された時はその処理を行う
		if( HIWORD( wParam ) == BN_CLICKED && LOWORD( wParam ) == 0x02000 ){
			HWND hwndBtn = ::GetDlgItem( hwnd, 0x2000 );
			RECT rc;
			POINT pt;

			// メニューを表示する
			::GetWindowRect( hwndBtn, &rc );
			pt.x = rc.left;
			pt.y = rc.bottom;
			GetMonitorWorkRect( pt, &rc );	// モニタのワークエリア

			HMENU hMenu = ::CreatePopupMenu();
			::InsertMenu( hMenu, 0, MF_BYPOSITION | MF_STRING, 100, _T("開く(&O)...") );
			::InsertMenu( hMenu, 1, MF_BYPOSITION | MF_STRING, 101, _T("インポート／エクスポートの起点リセット(&R)") );

			int nId = ::TrackPopupMenu( hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
										( pt.x > rc.left )? pt.x: rc.left,
										( pt.y < rc.bottom )? pt.y: rc.bottom,
										0, hwnd, NULL );
			::DestroyMenu( hMenu );

			// 選択されたメニューの処理
			switch( nId ){
			case 100:	// 設定フォルダを開く
				TCHAR szPath[_MAX_PATH];
				GetInidir( szPath );

				// フォルダの ITEMIDLIST を取得して ShellExecuteEx() で開く
				// Note. MSDN の ShellExecute() の解説にある方法でフォルダを開こうとした場合、
				//       フォルダと同じ場所に <フォルダ名>.exe があるとうまく動かない。
				//       verbが"open"やNULLではexeのほうが実行され"explore"では失敗する
				//       （フォルダ名の末尾に'\\'を付加してもWindows 2000では付加しないのと同じ動作になってしまう）
				LPSHELLFOLDER pDesktopFolder;
				if( SUCCEEDED(::SHGetDesktopFolder(&pDesktopFolder)) ){
					LPMALLOC pMalloc;
					if( SUCCEEDED(::SHGetMalloc(&pMalloc)) ){
						LPITEMIDLIST pIDL;
						WCHAR pwszDisplayName[_MAX_PATH];
						_tcstowcs(pwszDisplayName, szPath, _countof(pwszDisplayName));
//#ifdef _UNICODE
//						pwszDisplayName = szPath;
//#else
//						WCHAR wszPath[_MAX_PATH];
//						::MultiByteToWideChar( CP_ACP, 0, szPath, -1, wszPath, _MAX_PATH );
//						pwszDisplayName = wszPath;
//#endif
						if( SUCCEEDED(pDesktopFolder->ParseDisplayName(NULL, NULL, pwszDisplayName, NULL, &pIDL, NULL)) ){
							SHELLEXECUTEINFO si;
							::ZeroMemory( &si, sizeof(si) );
							si.cbSize   = sizeof(si);
							si.fMask    = SEE_MASK_IDLIST;
							si.lpVerb   = _T("open");
							si.lpIDList = pIDL;
							si.nShow    = SW_SHOWNORMAL;
							::ShellExecuteEx( &si );	// フォルダを開く
							pMalloc->Free( (void*)pIDL );
						}
						pMalloc->Release();
					}
					pDesktopFolder->Release();
				}
				break;
			case 101:	// インポート／エクスポートの起点リセット（起点を設定フォルダにする）
				int nMsgResult = MYMESSAGEBOX(
					hwnd,
					MB_OKCANCEL | MB_ICONINFORMATION,
					GSTR_APPNAME,
					_T("各種設定のインポート／エクスポート用ファイル選択画面の\n")
					_T("初期表示フォルダを設定フォルダに戻します。")
				);
				if( IDOK == nMsgResult )
				{
					DLLSHAREDATA *pShareData = CShareData::getInstance()->GetShareData();
					GetInidir( pShareData->m_sHistory.m_szIMPORTFOLDER );
					AddLastChar( pShareData->m_sHistory.m_szIMPORTFOLDER, _countof2(pShareData->m_sHistory.m_szIMPORTFOLDER), _T('\\') );
				}
				break;
			}
		}
		break;

	case WM_DESTROY:
		::SetWindowLongPtr( hwnd, GWLP_WNDPROC, (LONG_PTR)s_pOldPropSheetWndProc );
		break;
	}

	return ::CallWindowProc( s_pOldPropSheetWndProc, hwnd, uMsg, wParam, lParam );
}

/*!	独自拡張プロパティシートのコールバック関数
	@author ryoji
	@date 2007.05.25 新規
*/
static int CALLBACK PropSheetProc( HWND hwndDlg, UINT uMsg, LPARAM lParam )
{
	// プロパティシートの初期化時にボタン追加、プロパティシートのサブクラス化を行う
	if( uMsg == PSCB_INITIALIZED ){
		s_pOldPropSheetWndProc = (WNDPROC)::SetWindowLongPtr( hwndDlg, GWLP_WNDPROC, (LONG_PTR)PropSheetWndProc );
		HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle( NULL );
		HWND hwndBtn = ::CreateWindowEx( 0, _T("BUTTON"), _T("設定フォルダ(&/) >>"), BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 0, 0, 140, 20, hwndDlg, (HMENU)0x02000, hInstance, NULL );
		::SendMessage( hwndBtn, WM_SETFONT, (WPARAM)::SendMessage( hwndDlg, WM_GETFONT, 0, 0 ), MAKELPARAM( FALSE, 0 ) );
		::SetWindowPos( hwndBtn, ::GetDlgItem( hwndDlg, IDHELP), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
	}
	return 0;
}


/*!	独自拡張プロパティシート（共通設定／タイプ別設定画面用）
	@author ryoji
	@date 2007.05.25 新規
*/
int MyPropertySheet( LPPROPSHEETHEADER lppsph )
{
	// 個人設定フォルダを使用するときは「設定フォルダ」ボタンを追加する
	if( CShareData::getInstance()->IsPrivateSettings() ){
		lppsph->dwFlags |= PSH_USECALLBACK;
		lppsph->pfnCallback = PropSheetProc;
	}
	return ::PropertySheet( lppsph );
}


// To Here 2007.05.25 ryoji 独自拡張のプロパティシート関数群
///////////////////////////////////////////////////////////////////////




// Stonee, 2001/12/21
// NetWork上のリソースに接続するためのダイアログを出現させる
// NO_ERROR:成功 ERROR_CANCELLED:キャンセル それ以外:失敗
// プロジェクトの設定でリンクモジュールにMpr.libを追加のこと
DWORD NetConnect ( const TCHAR strNetWorkPass[] )
{
	//char sPassWord[] = "\0";	//パスワード
	//char sUser[] = "\0";		//ユーザー名
	DWORD dwRet;				//戻り値　エラーコードはWINERROR.Hを参照
	TCHAR sTemp[256];
	TCHAR sDrive[] = _T("");
    int i;

	if (_tcslen(strNetWorkPass) < 3)	return ERROR_BAD_NET_NAME;  //UNCではない。
	if (strNetWorkPass[0] != _T('\\') && strNetWorkPass[1] != _T('\\'))	return ERROR_BAD_NET_NAME;  //UNCではない。

	//3文字目から数えて最初の\の直前までを切り出す
	sTemp[0] = _T('\\');
	sTemp[1] = _T('\\');
	for (i = 2; strNetWorkPass[i] != _T('\0'); i++) {
		if (strNetWorkPass[i] == _T('\\')) break;
		sTemp[i] = strNetWorkPass[i];
	}
	sTemp[i] = _T('\0');	//終端

	//NETRESOURCE作成
	NETRESOURCE nr;
	ZeroMemory( &nr, sizeof( nr ) );
	nr.dwScope       = RESOURCE_GLOBALNET;
	nr.dwType        = RESOURCETYPE_DISK;
	nr.dwDisplayType = RESOURCEDISPLAYTYPE_SHARE;
	nr.dwUsage       = RESOURCEUSAGE_CONNECTABLE;
	nr.lpLocalName   = sDrive;
	nr.lpRemoteName  = sTemp;

	//ユーザー認証ダイアログを表示
	dwRet = WNetAddConnection3(0, &nr, NULL, NULL, CONNECT_UPDATE_PROFILE | CONNECT_INTERACTIVE);

	return dwRet;
}




/*! ショートカット(.lnk)の解決
	@date 2009.01.08 ryoji CoInitialize/CoUninitializeを削除（WinMainにOleInitialize/OleUninitializeを追加）
*/
BOOL ResolveShortcutLink( HWND hwnd, LPCTSTR lpszLinkFile, LPTSTR lpszPath )
{
	BOOL			bRes;
	HRESULT			hRes;
	IShellLink*		pIShellLink;
	IPersistFile*	pIPersistFile;
	WIN32_FIND_DATA	wfd;
	/* 初期化 */
	pIShellLink = NULL;
	pIPersistFile = NULL;
	*lpszPath = 0; // assume failure
	bRes = FALSE;

// 2009.01.08 ryoji CoInitializeを削除（WinMainにOleInitialize追加）

	// Get a pointer to the IShellLink interface.
//	hRes = 0;
	TCHAR szAbsLongPath[_MAX_PATH];
	if( ! ::GetLongFileName( lpszLinkFile, szAbsLongPath ) ){
		return FALSE;
	}

	// 2010.08.28 DLL インジェクション対策としてEXEのフォルダに移動する
	CCurrentDirectoryBackupPoint dirBack;
	ChangeCurrentDirectoryToExeDir();

	if( SUCCEEDED( hRes = ::CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&pIShellLink ) ) ){
		// Get a pointer to the IPersistFile interface.
		if( SUCCEEDED(hRes = pIShellLink->QueryInterface( IID_IPersistFile, (void**)&pIPersistFile ) ) ){
			// Ensure that the string is Unicode.
			WCHAR wsz[MAX_PATH];
			_tcstowcs(wsz, szAbsLongPath, _countof(wsz));
//			MultiByteToWideChar( CP_ACP, 0, lpszLinkFile, -1, wsz, MAX_PATH );
			// Load the shortcut.
			if( SUCCEEDED(hRes = pIPersistFile->Load( wsz, STGM_READ ) ) ){
				// Resolve the link.
				if( SUCCEEDED( hRes = pIShellLink->Resolve(hwnd, SLR_ANY_MATCH ) ) ){
					// Get the path to the link target.
					TCHAR szGotPath[MAX_PATH];
					szGotPath[0] = _T('\0');
					if( SUCCEEDED( hRes = pIShellLink->GetPath(szGotPath, MAX_PATH, &wfd, SLGP_SHORTPATH ) ) ){
						// Get the description of the target.
						TCHAR szDescription[MAX_PATH];
						if( SUCCEEDED(hRes = pIShellLink->GetDescription(szDescription, MAX_PATH ) ) ){
							if( _T('\0') != szGotPath[0] ){
								/* 正常終了 */
								_tcscpy_s( lpszPath, _MAX_PATH, szGotPath );
								bRes = TRUE;
							}
						}
					}
				}
			}
		}
	}
	// Release the pointer to the IPersistFile interface.
	if( NULL != pIPersistFile ){
		pIPersistFile->Release();
		pIPersistFile = NULL;
	}
	// Release the pointer to the IShellLink interface.
	if( NULL != pIShellLink ){
		pIShellLink->Release();
		pIShellLink = NULL;
	}
// 2009.01.08 ryoji CoUninitializeを削除（WinMainにOleUninitialize追加）
	return bRes;
}




/*フォント選択ダイアログ
	@param plf [in/out]
	@param piPointSize [in/out] 1/10ポイント単位
	
	2008.04.27 kobake CEditDoc::SelectFont から分離
	2009.10.01 ryoji ポイントサイズ（1/10ポイント単位）引数追加
*/
BOOL MySelectFont( LOGFONT* plf, INT* piPointSize, HWND hwndDlgOwner, bool FixedFontOnly )
{
	// 2004.02.16 Moca CHOOSEFONTをメンバから外す
	CHOOSEFONT cf;
	/* CHOOSEFONTの初期化 */
	::ZeroMemory( &cf, sizeof( cf ) );
	cf.lStructSize = sizeof( cf );
	cf.hwndOwner = hwndDlgOwner;
	cf.hDC = NULL;
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	if( FixedFontOnly ){
		//FIXEDフォント
		cf.Flags |= CF_FIXEDPITCHONLY;
	}
	cf.lpLogFont = plf;
	if( !ChooseFont( &cf ) ){
#ifdef _DEBUG
		DWORD nErr;
		nErr = CommDlgExtendedError();
		switch( nErr ){
		case CDERR_FINDRESFAILURE:	MYTRACE( _T("CDERR_FINDRESFAILURE \n") );	break;
		case CDERR_INITIALIZATION:	MYTRACE( _T("CDERR_INITIALIZATION \n") );	break;
		case CDERR_LOCKRESFAILURE:	MYTRACE( _T("CDERR_LOCKRESFAILURE \n") );	break;
		case CDERR_LOADRESFAILURE:	MYTRACE( _T("CDERR_LOADRESFAILURE \n") );	break;
		case CDERR_LOADSTRFAILURE:	MYTRACE( _T("CDERR_LOADSTRFAILURE \n") );	break;
		case CDERR_MEMALLOCFAILURE:	MYTRACE( _T("CDERR_MEMALLOCFAILURE\n") );	break;
		case CDERR_MEMLOCKFAILURE:	MYTRACE( _T("CDERR_MEMLOCKFAILURE \n") );	break;
		case CDERR_NOHINSTANCE:		MYTRACE( _T("CDERR_NOHINSTANCE \n") );		break;
		case CDERR_NOHOOK:			MYTRACE( _T("CDERR_NOHOOK \n") );			break;
		case CDERR_NOTEMPLATE:		MYTRACE( _T("CDERR_NOTEMPLATE \n") );		break;
		case CDERR_STRUCTSIZE:		MYTRACE( _T("CDERR_STRUCTSIZE \n") );		break;
		case CFERR_MAXLESSTHANMIN:	MYTRACE( _T("CFERR_MAXLESSTHANMIN \n") );	break;
		case CFERR_NOFONTS:			MYTRACE( _T("CFERR_NOFONTS \n") );			break;
		}
#endif
		return FALSE;
	}
	*piPointSize = cf.iPointSize;

	return TRUE;
}
