/*!	@file
	@brief 文字コードセット設定ダイアログボックス

	@author Uchi
	@date 2010/6/14  新規作成
*/
/*
	Copyright (C) 2010, Uchi

	This source code is designed for sakura editor.
	Please contact the copyright holder to use this code for other purpose.
*/
#include "StdAfx.h"
#include "dlg/CDlgSetCharSet.h"
#include "func/Funccode.h"
#include "util/shell.h"
#include "env/DLLSHAREDATA.h"
#include "sakura_rc.h"


CDlgSetCharSet::CDlgSetCharSet()
{
	m_pnCharSet = NULL;			// 文字コードセット
	m_pbBom = NULL;				// 文字コードセット
}



/* モーダルダイアログの表示 */
int CDlgSetCharSet::DoModal( HINSTANCE hInstance, HWND hwndParent, ECodeType* pnCharSet, bool* pbBom)
{
	m_pnCharSet = pnCharSet;	// 文字コードセット
	m_pbBom = pbBom;			// BOM

	return (int)CDialog::DoModal( hInstance, hwndParent, IDD_SETCHARSET, NULL );
}



BOOL CDlgSetCharSet::OnInitDialog( HWND hwndDlg, WPARAM wParam, LPARAM lParam )
{
	_SetHwnd( hwndDlg );
	
	m_hwndCharSet = ::GetDlgItem( GetHwnd(), IDC_COMBO_CHARSET );	// 文字コードセットコンボボックス
	m_hwndCheckBOM = ::GetDlgItem( GetHwnd(), IDC_CHECK_BOM );		// BOMチェックボックス

	// コンボボックスのユーザー インターフェイスを拡張インターフェースにする
	Combo_SetExtendedUI( m_hwndCharSet, TRUE );

	// 文字コードセット選択コンボボックス初期化
	int i;
	CCodeTypesForCombobox cCodeTypes;
	Combo_ResetContent( m_hwndCharSet );
	for (i = 1; i < cCodeTypes.GetCount(); ++i) {
		int idx = Combo_AddString( m_hwndCharSet, cCodeTypes.GetName(i) );
		Combo_SetItemData( m_hwndCharSet, idx, cCodeTypes.GetCode(i) );
	}

	/* 基底クラスメンバ */
	return CDialog::OnInitDialog( hwndDlg, wParam, lParam );
}



BOOL CDlgSetCharSet::OnBnClicked( int wID )
{
	switch( wID ){
	case IDOK:
		/* ダイアログデータの取得 */
		if( GetData() ){
			CloseDialog( TRUE );
		}
		return TRUE;
	case IDCANCEL:
		CloseDialog( FALSE );
		return TRUE;
	}

	/* 基底クラスメンバ */
	return CDialog::OnBnClicked( wID );
}



// BOM の設定
void CDlgSetCharSet::SetBOM( void )
{
	int 		nIdx;
	LRESULT		lRes;
	WPARAM		fCheck;

	nIdx = Combo_GetCurSel( m_hwndCharSet );
	lRes = Combo_GetItemData( m_hwndCharSet, nIdx );
	CCodeTypeName	cCodeTypeName( lRes );
	if (cCodeTypeName.UseBom()) {
		::EnableWindow( m_hwndCheckBOM, TRUE );
		if (lRes == *m_pnCharSet) {
			fCheck = *m_pbBom ? BST_CHECKED : BST_UNCHECKED;
		}
		else{
			fCheck = cCodeTypeName.IsBomDefOn() ? BST_CHECKED : BST_UNCHECKED;
		}
	}
	else {
		::EnableWindow( m_hwndCheckBOM, FALSE );
		fCheck = BST_UNCHECKED;
	}
	BtnCtl_SetCheck( m_hwndCheckBOM, fCheck );
}



// 文字コード選択時の処理
BOOL CDlgSetCharSet::OnCbnSelChange( HWND hwndCtl, int wID )
{
	int 		nIdx;
	LRESULT		lRes;
	WPARAM		fCheck;

	switch (wID) {
	//	文字コードの変更をBOMチェックボックスに反映
	case IDC_COMBO_CHARSET:
		SetBOM();
		nIdx = Combo_GetCurSel( hwndCtl );
		lRes = Combo_GetItemData( hwndCtl, nIdx );
		CCodeTypeName	cCodeTypeName( lRes );
		if (cCodeTypeName.UseBom()) {
			::EnableWindow( m_hwndCheckBOM, TRUE );
			if (lRes == *m_pnCharSet) {
				fCheck = *m_pbBom ? BST_CHECKED : BST_UNCHECKED;
			}
			else{
				fCheck = cCodeTypeName.IsBomDefOn() ? BST_CHECKED : BST_UNCHECKED;
			}
		}
		else {
			::EnableWindow( m_hwndCheckBOM, FALSE );
			fCheck = BST_UNCHECKED;
		}
		BtnCtl_SetCheck( m_hwndCheckBOM, fCheck );
		break;
	}
	return TRUE;
}



/* ダイアログデータの設定 */
void CDlgSetCharSet::SetData( void )
{
	// 文字コードセット
	int		nIdx, nCurIdx;
	ECodeType nCharSet;
	CCodeTypesForCombobox cCodeTypes;

	nCurIdx = Combo_GetCurSel( m_hwndCharSet );
	for (nIdx = 0; nIdx < Combo_GetCount( m_hwndCharSet ); nIdx++) {
		nCharSet = (ECodeType)Combo_GetItemData( m_hwndCharSet, nIdx );
		if (nCharSet == *m_pnCharSet) {
			nCurIdx = nIdx;
		}
	}
	Combo_SetCurSel( m_hwndCharSet, nCurIdx );

	// BOMを設定
	SetBOM();
}



/* ダイアログデータの取得 */
/* TRUE==正常  FALSE==入力エラー  */
int CDlgSetCharSet::GetData( void )
{
	// 文字コードセット
	int		nIdx;
	nIdx = Combo_GetCurSel( m_hwndCharSet );
	*m_pnCharSet = (ECodeType)Combo_GetItemData( m_hwndCharSet, nIdx );

	// BOM
	*m_pbBom = ( BtnCtl_GetCheck( m_hwndCheckBOM ) == BST_CHECKED );

	return TRUE;
}
