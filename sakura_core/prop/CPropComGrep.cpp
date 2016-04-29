/*!	@file
	@brief ���ʐݒ�_�C�A���O�{�b�N�X�A�u�����v�y�[�W

	@author Norio Nakatani
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2001, MIK, genta
	Copyright (C) 2002, YAZAKI, MIK
	Copyright (C) 2003, KEITA
	Copyright (C) 2006, ryoji

	This source code is designed for sakura editor.
	Please contact the copyright holder to use this code for other purpose.
*/

#include "StdAfx.h"
#include "prop/CPropCommon.h"
#include "extmodule/CBregexp.h"	// 2007.08/12 genta �o�[�W�����擾
#include "util/shell.h"
#include "util/window.h"
#include "sakura_rc.h"


//	From Here Jun. 2, 2001 genta
/*!
	@param hwndDlg �_�C�A���O�{�b�N�X��Window Handle
	@param uMsg ���b�Z�[�W
	@param wParam �p�����[�^1
	@param lParam �p�����[�^2
*/
INT_PTR CALLBACK CPropGrep::DlgProc_page(
	HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return DlgProc( reinterpret_cast<pDispatchPage>(&CPropGrep::DispatchEvent), hwndDlg, uMsg, wParam, lParam );
}
//	To Here Jun. 2, 2001 genta

/* ���b�Z�[�W���� */
INT_PTR CPropGrep::DispatchEvent( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//	WORD		wNotifyCode;
//	WORD		wID;
//	HWND		hwndCtl;
	NMHDR*		pNMHDR;
	NM_UPDOWN*	pMNUD;
	int			idCtrl;
//	int			nVal;
//    LPDRAWITEMSTRUCT pDis;

	switch( uMsg ){

	case WM_INITDIALOG:
		/* �_�C�A���O�f�[�^�̐ݒ� Grep */
		SetData( hwndDlg );
		// Modified by KEITA for WIN64 2003.9.6
		::SetWindowLongPtr( hwndDlg, DWLP_USER, lParam );

		/* ���[�U�[���G�f�B�b�g �R���g���[���ɓ��͂ł���e�L�X�g�̒����𐧌����� */

		return TRUE;
	case WM_NOTIFY:
		idCtrl = (int)wParam;
		pNMHDR = (NMHDR*)lParam;
		pMNUD  = (NM_UPDOWN*)lParam;
//		switch( idCtrl ){
//		default:
			switch( pNMHDR->code ){
			case PSN_KILLACTIVE:
				/* �_�C�A���O�f�[�^�̎擾 Grep */
				GetData( hwndDlg );
				return TRUE;
//@@@ 2002.01.03 YAZAKI �Ō�ɕ\�����Ă����V�[�g�𐳂����o���Ă��Ȃ��o�O�C��
			case PSN_SETACTIVE:
				m_nPageNum = ID_PAGENUM_GREP;
				return TRUE;
			}
//			break;	/* default */
//		}
		break;	/* WM_NOTIFY */
	case WM_COMMAND:
		//	2007.08.12 genta ���K�\��DLL�̕ύX�ɉ�����Version���Ď擾����
		if( wParam == MAKEWPARAM( IDC_EDIT_REGEXPLIB, EN_KILLFOCUS )){
			SetRegexpVersion( hwndDlg );
		}
		break;
	}
	return FALSE;
}


/* �_�C�A���O�f�[�^�̐ݒ� */
void CPropGrep::SetData( HWND hwndDlg )
{
	/* 2006.08.23 ryoji �J�[�\���ʒu�̕�������f�t�H���g�̌���������ɂ��� */
	::CheckDlgButton( hwndDlg, IDC_CHECK_bCaretTextForSearch, m_Common.m_sSearch.m_bCaretTextForSearch );

	CheckDlgButtonBool( hwndDlg, IDC_CHECK_INHERIT_KEY_OTHER_VIEW, m_Common.m_sSearch.m_bInheritKeyOtherView );

	/* Grep���[�h�ŕۑ��m�F���邩 */
	::CheckDlgButton( hwndDlg, IDC_CHECK_bGrepExitConfirm, m_Common.m_sSearch.m_bGrepExitConfirm );

	/* Grep���ʂ̃��A���^�C���\�� */
	::CheckDlgButton( hwndDlg, IDC_CHECK_GREPREALTIME, m_Common.m_sSearch.m_bGrepRealTimeView );	// 2006.08.08 ryoji ID�C��


	/* Grep���[�h: �G���^�[�L�[�Ń^�O�W�����v */
	::CheckDlgButton( hwndDlg, IDC_CHECK_GTJW_RETURN, m_Common.m_sSearch.m_bGTJW_RETURN );

	/* Grep���[�h: �_�u���N���b�N�Ń^�O�W�����v */
	::CheckDlgButton( hwndDlg, IDC_CHECK_GTJW_LDBLCLK, m_Common.m_sSearch.m_bGTJW_LDBLCLK );

	//	2007.08.12 genta ���K�\��DLL
	EditCtl_LimitText( ::GetDlgItem( hwndDlg, IDC_EDIT_REGEXPLIB ), _countof(m_Common.m_sSearch.m_szRegexpLib ) - 1 );
	::DlgItem_SetText( hwndDlg, IDC_EDIT_REGEXPLIB, m_Common.m_sSearch.m_szRegexpLib);
	SetRegexpVersion( hwndDlg );

	return;
}




/* �_�C�A���O�f�[�^�̎擾 */
int CPropGrep::GetData( HWND hwndDlg )
{
//@@@ 2002.01.03 YAZAKI �Ō�ɕ\�����Ă����V�[�g�𐳂����o���Ă��Ȃ��o�O�C��
//	m_nPageNum = ID_PAGENUM_GREP;

	/* 2006.08.23 ryoji �J�[�\���ʒu�̕�������f�t�H���g�̌���������ɂ��� */
	m_Common.m_sSearch.m_bCaretTextForSearch = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_bCaretTextForSearch );

	m_Common.m_sSearch.m_bInheritKeyOtherView = IsDlgButtonCheckedBool( hwndDlg, IDC_CHECK_INHERIT_KEY_OTHER_VIEW );

	/* Grep���[�h�ŕۑ��m�F���邩 */
	m_Common.m_sSearch.m_bGrepExitConfirm = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_bGrepExitConfirm );

	/* Grep���ʂ̃��A���^�C���\�� */
	m_Common.m_sSearch.m_bGrepRealTimeView = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_GREPREALTIME );	// 2006.08.08 ryoji ID�C��

	/* Grep���[�h: �G���^�[�L�[�Ń^�O�W�����v */
	m_Common.m_sSearch.m_bGTJW_RETURN = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_GTJW_RETURN );

	/* Grep���[�h: �_�u���N���b�N�Ń^�O�W�����v */
	m_Common.m_sSearch.m_bGTJW_LDBLCLK = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_GTJW_LDBLCLK );

	//	2007.08.12 genta ���K�\��DLL
	::DlgItem_GetText( hwndDlg, IDC_EDIT_REGEXPLIB, m_Common.m_sSearch.m_szRegexpLib, _countof( m_Common.m_sSearch.m_szRegexpLib ));

	return TRUE;
}

void CPropGrep::SetRegexpVersion( HWND hwndDlg )
{
	TCHAR regexp_dll[_MAX_PATH];
	
	::DlgItem_GetText( hwndDlg, IDC_EDIT_REGEXPLIB, regexp_dll, _countof( regexp_dll ));
	CBregexp breg;
	if( DLL_SUCCESS != breg.InitDll( regexp_dll ) ){
		::DlgItem_SetText( hwndDlg, IDC_LABEL_REGEXP_VER, _T("���K�\���͎g�p�ł��܂���") );
		return;
	}
	::DlgItem_SetText( hwndDlg, IDC_LABEL_REGEXP_VER, breg.GetVersionT() );
}



