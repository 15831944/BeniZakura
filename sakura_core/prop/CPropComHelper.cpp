/*!	@file
	@brief ���ʐݒ�_�C�A���O�{�b�N�X�A�u�x���v�y�[�W

	@author Norio Nakatani
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2000, jepro
	Copyright (C) 2001, genta, MIK, jepro, asa-o
	Copyright (C) 2002, YAZAKI, MIK, genta
	Copyright (C) 2003, Moca, KEITA
	Copyright (C) 2006, ryoji
	Copyright (C) 2007, ryoji
	Copyright (C) 2009, ryoji
	Copyright (C) 2012, Moca
	Copyright (C) 2013, Uchi

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
#include "prop/CPropCommon.h"
#include "dlg/CDlgOpenFile.h"
#include "util/shell.h"
#include "util/module.h"
#include "sakura_rc.h"


//	From Here Jun. 2, 2001 genta
/*!
	@param hwndDlg �_�C�A���O�{�b�N�X��Window Handle
	@param uMsg ���b�Z�[�W
	@param wParam �p�����[�^1
	@param lParam �p�����[�^2
*/
INT_PTR CALLBACK CPropHelper::DlgProc_page(
	HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return DlgProc( reinterpret_cast<pDispatchPage>(&CPropHelper::DispatchEvent), hwndDlg, uMsg, wParam, lParam );
}
//	To Here Jun. 2, 2001 genta

/* Helper ���b�Z�[�W���� */
INT_PTR CPropHelper::DispatchEvent(
	HWND	hwndDlg,	// handle to dialog box
	UINT	uMsg,		// message
	WPARAM	wParam,		// first message parameter
	LPARAM	lParam 		// second message parameter
)
{
	WORD		wNotifyCode;
	WORD		wID;
	HWND		hwndCtl;
	NMHDR*		pNMHDR;
	NM_UPDOWN*	pMNUD;
	int			idCtrl;

	switch( uMsg ){
	case WM_INITDIALOG:
		/* �_�C�A���O�f�[�^�̐ݒ� Helper */
		SetData( hwndDlg );
		// Modified by KEITA for WIN64 2003.9.6
		::SetWindowLongPtr( hwndDlg, DWLP_USER, lParam );

		return TRUE;
	case WM_COMMAND:
		wNotifyCode = HIWORD(wParam);	/* �ʒm�R�[�h */
		wID			= LOWORD(wParam);	/* ����ID� �R���g���[��ID� �܂��̓A�N�Z�����[�^ID */
		hwndCtl		= (HWND) lParam;	/* �R���g���[���̃n���h�� */
		switch( wNotifyCode ){
		/* �{�^���^�`�F�b�N�{�b�N�X���N���b�N���ꂽ */
		case BN_CLICKED:
			/* �_�C�A���O�f�[�^�̎擾 Helper */
			GetData( hwndDlg );
			switch( wID ){
			// ai 02/05/21 Add S
			case IDC_BUTTON_KEYWORDHELPFONT:	/* �L�[���[�h�w���v�́u�t�H���g�v�{�^�� */
				{
					LOGFONT   lf = m_Common.m_sHelper.m_lf;
					INT nPointSize = m_Common.m_sHelper.m_nPointSize;

					if( MySelectFont( &lf, &nPointSize, hwndDlg, false) ){
						m_Common.m_sHelper.m_lf = lf;
						m_Common.m_sHelper.m_nPointSize = nPointSize;	// 2009.10.01 ryoji
						// �L�[���[�h�w���v �t�H���g�\��	// 2013/4/24 Uchi
						HFONT hFont = SetFontLabel( hwndDlg, IDC_STATIC_KEYWORDHELPFONT, m_Common.m_sHelper.m_lf, m_Common.m_sHelper.m_nPointSize);
						if(m_hKeywordHelpFont != NULL){
							::DeleteObject( m_hKeywordHelpFont );
						}
						m_hKeywordHelpFont = hFont;
					}
				}
				return TRUE;
			// ai 02/05/21 Add E
			case IDC_BUTTON_OPENMDLL:	/* MIGEMODLL�ꏊ�w��u�Q��...�v�{�^�� */
				{
					CDlgOpenFile	cDlgOpenFile;
					TCHAR			szPath[_MAX_PATH];
					// 2003.06.23 Moca ���΃p�X�͎��s�t�@�C������̃p�X
					// 2007.05.21 ryoji ���΃p�X�͐ݒ�t�@�C������̃p�X��D��
					if( _IS_REL_PATH( m_Common.m_sHelper.m_szMigemoDll ) ){
						GetInidirOrExedir( szPath, m_Common.m_sHelper.m_szMigemoDll, TRUE );
					}else{
						_tcscpy( szPath, m_Common.m_sHelper.m_szMigemoDll );
					}
					/* �t�@�C���I�[�v���_�C�A���O�̏����� */
					cDlgOpenFile.Create(
						G_AppInstance(),
						hwndDlg,
						_T("*.dll"),
						szPath
					);
					if( cDlgOpenFile.DoModal_GetOpenFileName( szPath ) ){
						_tcscpy( m_Common.m_sHelper.m_szMigemoDll, szPath );
						::DlgItem_SetText( hwndDlg, IDC_EDIT_MIGEMO_DLL, m_Common.m_sHelper.m_szMigemoDll );
					}
				}
				return TRUE;
			case IDC_BUTTON_OPENMDICT:	/* MigemoDict�ꏊ�w��u�Q��...�v�{�^�� */
				{
					TCHAR	szPath[_MAX_PATH];
					/* �����t�H���_ */
					// 2007.05.27 ryoji ���΃p�X�͐ݒ�t�@�C������̃p�X��D��
					if( _IS_REL_PATH( m_Common.m_sHelper.m_szMigemoDict ) ){
						GetInidirOrExedir( szPath, m_Common.m_sHelper.m_szMigemoDict, TRUE );
					}else{
						_tcscpy( szPath, m_Common.m_sHelper.m_szMigemoDict );
					}
					if( SelectDir( hwndDlg, _T("��������t�H���_��I��ł�������"), szPath, szPath ) ){
						_tcscpy( m_Common.m_sHelper.m_szMigemoDict, szPath );
						::DlgItem_SetText( hwndDlg, IDC_EDIT_MIGEMO_DICT, m_Common.m_sHelper.m_szMigemoDict );
					}
				}
				return TRUE;
			}
			break;	/* BN_CLICKED */
		}
		break;	/* WM_COMMAND */
	case WM_NOTIFY:
		idCtrl = (int)wParam;
		pNMHDR = (NMHDR*)lParam;
		pMNUD  = (NM_UPDOWN*)lParam;
//		switch( idCtrl ){
//		case ???????:
//			return 0L;
//		default:
			switch( pNMHDR->code ){
			case PSN_KILLACTIVE:
//				MYTRACE( _T("Helper PSN_KILLACTIVE\n") );
				/* �_�C�A���O�f�[�^�̎擾 Helper */
				GetData( hwndDlg );
				return TRUE;
//@@@ 2002.01.03 YAZAKI �Ō�ɕ\�����Ă����V�[�g�𐳂����o���Ă��Ȃ��o�O�C��
			case PSN_SETACTIVE:
				m_nPageNum = ID_PAGENUM_HELPER;
				return TRUE;
			}
//			break;	/* default */
//		}

//		MYTRACE( _T("pNMHDR->hwndFrom=%xh\n"), pNMHDR->hwndFrom );
//		MYTRACE( _T("pNMHDR->idFrom  =%xh\n"), pNMHDR->idFrom );
//		MYTRACE( _T("pNMHDR->code    =%xh\n"), pNMHDR->code );
//		MYTRACE( _T("pMNUD->iPos    =%d\n"), pMNUD->iPos );
//		MYTRACE( _T("pMNUD->iDelta  =%d\n"), pMNUD->iDelta );
		break;	/* WM_NOTIFY */

	case WM_DESTROY:
		// �L�[���[�h�w���v �t�H���g�j��	// 2013/4/24 Uchi
		if (m_hKeywordHelpFont != NULL) {
			::DeleteObject( m_hKeywordHelpFont );
			m_hKeywordHelpFont = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

/* �_�C�A���O�f�[�^�̐ݒ� Helper */
void CPropHelper::SetData( HWND hwndDlg )
{
	/* �⊮��⌈��L�[ */
	::CheckDlgButton( hwndDlg, IDC_CHECK_m_bHokanKey_RETURN, m_Common.m_sHelper.m_bHokanKey_RETURN );	//VK_RETURN �⊮����L�[���L��/����
	::CheckDlgButton( hwndDlg, IDC_CHECK_m_bHokanKey_TAB, m_Common.m_sHelper.m_bHokanKey_TAB );		//VK_TAB    �⊮����L�[���L��/����
	::CheckDlgButton( hwndDlg, IDC_CHECK_m_bHokanKey_RIGHT, m_Common.m_sHelper.m_bHokanKey_RIGHT );	//VK_RIGHT  �⊮����L�[���L��/����

	// �L�[���[�h�w���v �t�H���g	// 2013/4/24 Uchi
	m_hKeywordHelpFont = SetFontLabel( hwndDlg, IDC_STATIC_KEYWORDHELPFONT, m_Common.m_sHelper.m_lf, m_Common.m_sHelper.m_nPointSize);

	//migemo dict
	::DlgItem_SetText( hwndDlg, IDC_EDIT_MIGEMO_DLL, m_Common.m_sHelper.m_szMigemoDll);
	::DlgItem_SetText( hwndDlg, IDC_EDIT_MIGEMO_DICT, m_Common.m_sHelper.m_szMigemoDict);
}


/* �_�C�A���O�f�[�^�̎擾 Helper */
int CPropHelper::GetData( HWND hwndDlg )
{
	/* �⊮��⌈��L�[ */
	m_Common.m_sHelper.m_bHokanKey_RETURN = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_m_bHokanKey_RETURN );//VK_RETURN �⊮����L�[���L��/����
	m_Common.m_sHelper.m_bHokanKey_TAB = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_m_bHokanKey_TAB );		//VK_TAB    �⊮����L�[���L��/����
	m_Common.m_sHelper.m_bHokanKey_RIGHT = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_m_bHokanKey_RIGHT );	//VK_RIGHT  �⊮����L�[���L��/����

	//migemo dict
	::DlgItem_GetText( hwndDlg, IDC_EDIT_MIGEMO_DLL, m_Common.m_sHelper.m_szMigemoDll, _countof( m_Common.m_sHelper.m_szMigemoDll ));
	::DlgItem_GetText( hwndDlg, IDC_EDIT_MIGEMO_DICT, m_Common.m_sHelper.m_szMigemoDict, _countof( m_Common.m_sHelper.m_szMigemoDict ));

	return TRUE;
}
