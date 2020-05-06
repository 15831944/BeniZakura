/*!	@file
	@brief ファイルプロパティダイアログ

	@author Norio Nakatani
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2000, genta
	Copyright (C) 2001, Stonee
	Copyright (C) 2002, Moca, MIK, YAZAKI
	Copyright (C) 2006, ryoji
	Copyright (C) 2009, ryoji

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

		1. The origin of this software must not be misrepresented;
		   you must not claim that you wrote the original software.
		   If you use this software in a product, an acknowledgment
		   in the product documentation would be appreciated but is
		   not required.

		2. Altered source versions must be plainly marked as such,
		   and must not be misrepresented as being the original software.

		3. This notice may not be removed or altered from any source
		   distribution.
*/
#include "StdAfx.h"
#include "dlg/CDlgProperty.h"
#include "doc/CEditDoc.h"
#include "func/Funccode.h"		// Stonee, 2001/03/12
#include "_main/global.h"		// Moca, 2002/05/26
#include "_main/CAppMode.h"
#include "env/CShareData.h"
#include "env/DLLSHAREDATA.h"
#include "charset/charcode.h"	// rastiv, 2006/06/28
#include "charset/CESI.h"
#include "io/CBinaryStream.h"
#include "util/shell.h"
#include "sakura_rc.h"


/* モーダルダイアログの表示 */
int CDlgProperty::DoModal( HINSTANCE hInstance, HWND hwndParent, LPARAM lParam )
{
	return (int)CDialog::DoModal( hInstance, hwndParent, IDD_PROPERTY_FILE, lParam );
}

BOOL CDlgProperty::OnBnClicked( int wID )
{
	switch( wID ){
	case IDOK:			/* 下検索 */
		/* ダイアログデータの取得 */
		::EndDialog( GetHwnd(), FALSE );
		return TRUE;
//	case IDCANCEL:							// 未使用 del 2008/7/4 Uchi
//		::EndDialog( GetHwnd(), FALSE );
//		return TRUE;
	}
	/* 基底クラスメンバ */
	return CDialog::OnBnClicked( wID );
}


/*! ダイアログデータの設定

	@date 2002.2.17 YAZAKI CShareDataのインスタンスは、CProcessにひとつあるのみ。
*/
void CDlgProperty::SetData( void )
{
	CEditDoc*		pCEditDoc = (CEditDoc*)m_lParam;
	CNativeT		cmemProp;
	TCHAR			szWork[500];

	HANDLE			nFind;
	WIN32_FIND_DATA	wfd;

	//	Aug. 16, 2000 genta	全角化
	cmemProp.AppendString( _T("ファイル名  ") );
	cmemProp.AppendString( pCEditDoc->m_cDocFile.GetFilePath() );
	cmemProp.AppendString( _T("\r\n") );

	cmemProp.AppendString( _T("設定のタイプ  ") );
	cmemProp.AppendString( pCEditDoc->m_cDocType.GetDocumentAttribute().m_szTypeName );
	cmemProp.AppendString( _T("\r\n") );

	cmemProp.AppendString( _T("文字コード  ") );
	cmemProp.AppendString( CCodeTypeName(pCEditDoc->GetDocumentEncoding()).Normal() );
	//	From Here  2008/4/27 Uchi
	if (pCEditDoc->GetDocumentBomExist()) {
		cmemProp.AppendString( _T(" BOM付") );
	}
	//	To Here  2008/4/27 Uchi
	cmemProp.AppendString( _T("\r\n") );

	auto_sprintf( szWork, _T("行数  %d行\r\n"), pCEditDoc->m_cDocLineMgr.GetLineCount() );
	cmemProp.AppendString( szWork );

	auto_sprintf( szWork, _T("レイアウト行数  %d行\r\n"), pCEditDoc->m_cLayoutMgr.GetLineCount() );
	cmemProp.AppendString( szWork );

	if( CAppMode::getInstance()->IsViewMode() ){
		cmemProp.AppendString( _T("ビューモードで開いています。\r\n") );	// 2009.04.11 ryoji 「上書き禁止モード」→「ビューモード」
	}
	if( pCEditDoc->m_cDocEditor.IsModified() ){
		cmemProp.AppendString( _T("変更されています。\r\n") );
	}else{
		cmemProp.AppendString( _T("変更されていません。\r\n") );
	}

	auto_sprintf( szWork, _T("\r\nコマンド実行回数    %d回\r\n"), pCEditDoc->m_nCommandExecNum );
	cmemProp.AppendString( szWork );

	auto_sprintf( szWork, _T("--ファイル情報-----------------\r\n"), pCEditDoc->m_cDocLineMgr.GetLineCount() );
	cmemProp.AppendString( szWork );

	if( INVALID_HANDLE_VALUE != ( nFind = ::FindFirstFile( pCEditDoc->m_cDocFile.GetFilePath(), &wfd ) ) ){
		if( pCEditDoc->m_cDocFile.IsFileLocking() ){
			if( CAppMode::getInstance()->GetFileShareMode() == SHAREMODE_DENY_WRITE ){
				auto_sprintf( szWork, _T("あなたはこのファイルを、他プロセスからの上書き禁止モードでロックしています。\r\n") );
			}
			else if( CAppMode::getInstance()->GetFileShareMode() == SHAREMODE_DENY_READWRITE ){
				auto_sprintf( szWork, _T("あなたはこのファイルを、他プロセスからの読み書き禁止モードでロックしています。\r\n") );
			}
			else{
				auto_sprintf( szWork, _T("あなたはこのファイルをロックしています。\r\n") );
			}
			cmemProp.AppendString( szWork );
		}
		else{
			auto_sprintf( szWork, _T("あなたはこのファイルをロックしていません。\r\n") );
			cmemProp.AppendString( szWork );
		}

		auto_sprintf( szWork, _T("ファイル属性  "), pCEditDoc->m_cDocLineMgr.GetLineCount() );
		cmemProp.AppendString( szWork );
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ){
			cmemProp.AppendString( _T("/アーカイブ") );
		}
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED ){
			cmemProp.AppendString( _T("/圧縮") );
		}
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ){
			cmemProp.AppendString( _T("/フォルダ") );
		}
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ){
			cmemProp.AppendString( _T("/隠し") );
		}
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL ){
			cmemProp.AppendString( _T("/ノーマル") );
		}
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE ){
			cmemProp.AppendString( _T("/オフライン") );
		}
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY ){
			cmemProp.AppendString( _T("/読み取り専用") );
		}
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ){
			cmemProp.AppendString( _T("/システム") );
		}
		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY ){
			cmemProp.AppendString( _T("/テンポラリ") );
		}
		cmemProp.AppendString( _T("\r\n") );


		cmemProp.AppendString( _T("作成日時  ") );
		CFileTime ctimeCreation = wfd.ftCreationTime;
		auto_sprintf( szWork, _T("%d年%d月%d日 %02d:%02d:%02d"),
			ctimeCreation->wYear,
			ctimeCreation->wMonth,
			ctimeCreation->wDay,
			ctimeCreation->wHour,
			ctimeCreation->wMinute,
			ctimeCreation->wSecond
		);
		cmemProp.AppendString( szWork );
		cmemProp.AppendString( _T("\r\n") );

		cmemProp.AppendString( _T("更新日時  ") );
		CFileTime ctimeLastWrite = wfd.ftLastWriteTime;
		auto_sprintf( szWork, _T("%d年%d月%d日 %02d:%02d:%02d"),
			ctimeLastWrite->wYear,
			ctimeLastWrite->wMonth,
			ctimeLastWrite->wDay,
			ctimeLastWrite->wHour,
			ctimeLastWrite->wMinute,
			ctimeLastWrite->wSecond
		);
		cmemProp.AppendString( szWork );
		cmemProp.AppendString( _T("\r\n") );


		cmemProp.AppendString( _T("アクセス日  ") );
		CFileTime ctimeLastAccess = wfd.ftLastAccessTime;
		auto_sprintf( szWork, _T("%d年%d月%d日"),
			ctimeLastAccess->wYear,
			ctimeLastAccess->wMonth,
			ctimeLastAccess->wDay
		);
		cmemProp.AppendString( szWork );
		cmemProp.AppendString( _T("\r\n") );

		auto_sprintf( szWork, _T("MS-DOSファイル名  %ts\r\n"), wfd.cAlternateFileName );
		cmemProp.AppendString( szWork );

		auto_sprintf( szWork, _T("ファイルサイズ  %d バイト\r\n"), wfd.nFileSizeLow );
		cmemProp.AppendString( szWork );

		::FindClose( nFind );
	}




#ifdef _DEBUG/////////////////////////////////////////////////////
	HGLOBAL					hgData;
	char*					pBuf;
	int						nBufLen;
	CNativeT				ctext;
	/* メモリ確保 & ファイル読み込み */
	hgData = NULL;
	CBinaryInputStream in(pCEditDoc->m_cDocFile.GetFilePath());
	if(!in){
		goto end_of_CodeTest;
	}
	nBufLen = in.GetLength();
	if( nBufLen > CheckKanjiCode_MAXREADLENGTH ){
		nBufLen = CheckKanjiCode_MAXREADLENGTH;
	}
	hgData = ::GlobalAlloc( GHND, nBufLen + 1 );
	if( NULL == hgData ){
		in.Close();
		goto end_of_CodeTest;
	}
	pBuf = GlobalLockChar( hgData );
	in.Read( pBuf, nBufLen );
	in.Close();

	//CESIのデバッグ情報
	CESI::GetDebugInfo(pBuf,nBufLen,&ctext);
	cmemProp.AppendNativeData(ctext);

	if( NULL != hgData ){
		::GlobalUnlock( hgData );
		::GlobalFree( hgData );
		hgData = NULL;
	}
end_of_CodeTest:;
#endif //ifdef _DEBUG/////////////////////////////////////////////////////
	::DlgItem_SetText( GetHwnd(), IDC_EDIT_PROPERTY, cmemProp.GetStringPtr() );

	return;
}

