/*!	@file
	@brief MRUリストと呼ばれるリストを管理する

	@author YAZAKI
	@date 2001/12/23  新規作成
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2001, YAZAKI
	Copyright (C) 2002, YAZAKI, Moca, genta
	Copyright (C) 2003, MIK

	This source code is designed for sakura editor.
	Please contact the copyright holder to use this code for other purpose.
*/

#include "StdAfx.h"
#include "CMRUFolder.h"
#include "env/CShareData.h"
#include "env/DLLSHAREDATA.h"
#include "uiparts/CMenuDrawer.h"	//	これでいいのか？
#include "util/string_ex.h"
#include "util/string_ex2.h"

/*!	コンストラクタ

	@date 2002.2.17 YAZAKI CShareDataのインスタンスは、CProcessにひとつあるのみ。
*/
CMRUFolder::CMRUFolder()
{
	//	初期化。
	m_pShareData = CShareData::getInstance()->GetShareData();
}

/*	デストラクタ	*/
CMRUFolder::~CMRUFolder()
{
	m_cRecentFolder.Terminate();
}

/*!
	フォルダ履歴メニューの作成
	
	@param pCMenuDrawer [in] (out?) メニュー作成で用いるMenuDrawer
	
	@return 生成したメニューのハンドル

	2010/5/21 Uchi 組み直し
*/
HMENU CMRUFolder::CreateMenu( CMenuDrawer* pCMenuDrawer ) const
{
	HMENU	hMenuPopUp;

	hMenuPopUp = ::CreatePopupMenu();	// Jan. 29, 2002 genta
	return CreateMenu( hMenuPopUp, pCMenuDrawer );
}

/*!
	フォルダ履歴メニューの作成
	
	@param 追加するメニューのハンドル
	@param pCMenuDrawer [in] (out?) メニュー作成で用いるMenuDrawer
	
	@author Norio Nakantani
	@return メニューのハンドル
*/
HMENU CMRUFolder::CreateMenu( HMENU	hMenuPopUp, CMenuDrawer* pCMenuDrawer ) const
{
	TCHAR	szFolder2[_MAX_PATH * 2];	//	全部&でも問題ないように :-)
	TCHAR	szMemu[_MAX_PATH * 2 + 10];				//	メニューキャプション
	int		i;
	bool	bFavorite;

	CFileNameManager::getInstance()->TransformFileName_MakeCache();
	for( i = 0; i < m_cRecentFolder.GetItemCount(); ++i )
	{
		//	「共通設定」→「全般」→「ファイルの履歴MAX」を反映
		if ( i >= m_cRecentFolder.GetViewCount() ) break;

		CFileNameManager::getInstance()->GetTransformFileNameFast( m_cRecentFolder.GetItemText( i ), szMemu, _MAX_PATH );
		wcsReplace( szMemu, L'\\', L'/' );
		//	&を&&に置換。
		//	Jan. 19, 2002 genta
		dupamp( szMemu, szFolder2 );

		bFavorite = m_cRecentFolder.IsFavorite( i );
		const int nAccKey = i % 36;
		auto_sprintf( szMemu, _T("%ts%ts (&%tc)"), 
			(!m_pShareData->m_Common.m_sWindow.m_bMenuIcon && bFavorite) ? _T("★ ") : _T(""),
			szFolder2,
			(nAccKey < 10) ? (_T('0') + nAccKey) : (_T('A') + nAccKey - 10) );

		//	メニューに追加
		pCMenuDrawer->MyAppendMenu( hMenuPopUp, MF_BYPOSITION | MF_STRING, IDM_SELOPENFOLDER + i, szMemu, _T(""), TRUE,
			bFavorite ? F_FAVORITE : -1 );
	}
	return hMenuPopUp;
}

std::vector<LPCTSTR> CMRUFolder::GetPathList() const
{
	std::vector<LPCTSTR> ret;
	for( int i = 0; i < m_cRecentFolder.GetItemCount(); ++i ){
		//	「共通設定」→「全般」→「フォルダの履歴MAX」を反映
		if ( i >= m_cRecentFolder.GetViewCount() ) break;
		ret.push_back(m_cRecentFolder.GetItemText(i));
	}
	return ret;
}

int CMRUFolder::Length() const
{
	return m_cRecentFolder.GetItemCount();
}

void CMRUFolder::ClearAll()
{
	m_cRecentFolder.DeleteAllItem();
}

/*	@brief 開いたフォルダ リストへの登録

	@date 2001.12.26  CShareData::AddOPENFOLDERListから移動した。（YAZAKI）
*/
void CMRUFolder::Add( const TCHAR* pszFolder )
{
	if( NULL == pszFolder
	 || pszFolder[0] == _T('\0') )
	{	//	長さが0なら排除。
		return;
	}

	// すでに登録されている場合は、除外指定を無視する
	if( -1 == m_cRecentFolder.FindItemByText( pszFolder ) ){
		int nSize = m_pShareData->m_sHistory.m_aExceptMRU.size();
		for( int i = 0 ; i < nSize; i++ ){
			TCHAR szExceptMRU[_MAX_PATH];
			CFileNameManager::ExpandMetaToFolder( m_pShareData->m_sHistory.m_aExceptMRU[i], szExceptMRU, _countof(szExceptMRU) );
			if( NULL != _tcsistr( pszFolder, szExceptMRU ) ){
				return;
			}
		}
	}

	m_cRecentFolder.AppendItem( pszFolder );
}

const TCHAR* CMRUFolder::GetPath(int num) const
{
	return m_cRecentFolder.GetItemText( num );
}

