/*!	@file
	@brief �w��s�ւ̃W�����v�_�C�A���O�{�b�N�X

	@author Norio Nakatani
	@date	1998/05/31 �쐬
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2000, jepro
	Copyright (C) 2001, jepro, Stonee
	Copyright (C) 2002, aroka, MIK, YAZAKI
	Copyright (C) 2004, genta
	Copyright (C) 2006, ryoji

	This source code is designed for sakura editor.
	Please contact the copyright holder to use this code for other purpose.
*/
#include "StdAfx.h"
#include "dlg/CDlgJump.h"
#include "doc/CEditDoc.h"
#include "func/Funccode.h"
#include "outline/CFuncInfo.h"
#include "outline/CFuncInfoArr.h"
#include "util/shell.h"
#include "window/CEditWnd.h"
#include "sakura_rc.h"
#include "sakura.hh"

// �W�����v CDlgJump.cpp
const DWORD p_helpids[] = {	//12800
	IDC_BUTTON_JUMP,				HIDC_JUMP_BUTTON_JUMP,			//�W�����v
	IDCANCEL,						HIDCANCEL_JUMP,					//�L�����Z��
	IDC_EDIT_LINENUM,				HIDC_JUMP_EDIT_LINENUM,			//�s�ԍ�
	IDC_RADIO_LINENUM_LAYOUT,		HIDC_JUMP_RADIO_LINENUM_LAYOUT,	//�܂�Ԃ��P��
	IDC_RADIO_LINENUM_CRLF,			HIDC_JUMP_RADIO_LINENUM_CRLF,	//���s�P��
	IDC_SPIN_LINENUM,				HIDC_JUMP_EDIT_LINENUM,			//12870,	//
	0, 0
};



CDlgJump::CDlgJump()
{
	m_nLineNum = 0;			/* �s�ԍ� */

	return;
}



/* ���[�_���_�C�A���O�̕\�� */
int CDlgJump::DoModal(
	HINSTANCE	hInstance,
	HWND		hwndParent,
	LPARAM		lParam
)
{
	return CDialog::DoModal( hInstance, hwndParent, IDD_JUMP, lParam );
}



BOOL CDlgJump::OnNotify( WPARAM wParam, LPARAM lParam )
{
	NM_UPDOWN*		pMNUD;
	int				idCtrl;
	int				nData;

	idCtrl = (int)wParam;
	pMNUD  = (NM_UPDOWN*)lParam;
	/* �X�s���R���g���[���̏��� */
	switch( idCtrl ){
		case IDC_SPIN_LINENUM:
			/* �W�����v�������s�ԍ��̎w�� */
			nData = ::GetDlgItemInt( GetHwnd(), IDC_EDIT_LINENUM, NULL, FALSE );
			if( pMNUD->iDelta < 0 ){
				++nData;
			}else
			if( pMNUD->iDelta > 0 ){
				nData--;
			}
			if( nData < 1 ){
				nData = 1;
			}
			::SetDlgItemInt( GetHwnd(), IDC_EDIT_LINENUM, nData, FALSE );
			break;

		default:
			break;
	}

	return TRUE;
}



BOOL CDlgJump::OnBnClicked( int wID )
{
	switch( wID ){
		case IDC_BUTTON_JUMP:			/* �w��s�փW�����v */
			if( 0 < GetData() ){
				CloseDialog( 1 );
			}else{
				OkMessage( GetHwnd(), _T("�������s�ԍ�����͂��Ă��������B") );
			}
			{
				CEditDoc*		pCEditDoc = (CEditDoc*)m_lParam;
				pCEditDoc->m_pcEditWnd->GetActiveView().GetCommander().HandleCommand(F_JUMP, true, 0, 0, 0, 0);	//	�W�����v�R�}���h���s
			}
			return TRUE;

		case IDCANCEL:
			::EndDialog( GetHwnd(), FALSE );

			return TRUE;
	}

	/* ���N���X�����o */
	return CDialog::OnBnClicked( wID );
}



/* �_�C�A���O�f�[�^�̐ݒ� */
void CDlgJump::SetData( void )
{
	if( 0 == m_nLineNum ){
		::DlgItem_SetText( GetHwnd(), IDC_EDIT_LINENUM, _T("") );	/* �s�ԍ� */
	}else{
		::SetDlgItemInt( GetHwnd(), IDC_EDIT_LINENUM, m_nLineNum, FALSE );	/* �O��̍s�ԍ� */
	}

	::EnableWindow( ::GetDlgItem( GetHwnd(), IDC_RADIO_LINENUM_LAYOUT ), TRUE );
	::EnableWindow( ::GetDlgItem( GetHwnd(), IDC_RADIO_LINENUM_CRLF ), TRUE );

	/* �s�ԍ��̕\�� FALSE=�܂�Ԃ��P�ʁ^TRUE=���s�P�� */
	if( m_pShareData->m_bLineNumIsCRLF_ForJump ){
		::CheckDlgButton( GetHwnd(), IDC_RADIO_LINENUM_LAYOUT, FALSE );
		::CheckDlgButton( GetHwnd(), IDC_RADIO_LINENUM_CRLF, TRUE );
	}else{
		::CheckDlgButton( GetHwnd(), IDC_RADIO_LINENUM_LAYOUT, TRUE );
		::CheckDlgButton( GetHwnd(), IDC_RADIO_LINENUM_CRLF, FALSE );
	}

	return;
}



/* �_�C�A���O�f�[�^�̎擾 */
/*   TRUE==����   FALSE==���̓G���[  */
int CDlgJump::GetData( void )
{
	BOOL	pTranslated;

	/* �s�ԍ��̕\�� FALSE=�܂�Ԃ��P�ʁ^TRUE=���s�P�� */
	if( ::IsDlgButtonChecked( GetHwnd(), IDC_RADIO_LINENUM_LAYOUT ) ){
		m_pShareData->m_bLineNumIsCRLF_ForJump = false;
	}else{
		m_pShareData->m_bLineNumIsCRLF_ForJump = true;
	}

	/* �s�ԍ� */
	m_nLineNum = ::GetDlgItemInt( GetHwnd(), IDC_EDIT_LINENUM, &pTranslated, FALSE );
	if( m_nLineNum == 0 && !pTranslated ){
		return FALSE;
	}
	return TRUE;
}



LPVOID CDlgJump::GetHelpIdTable(void)
{
	return (LPVOID)p_helpids;
}



