/*!	@file
@brief CViewCommander�N���X�̃R�}���h(�x��)�֐��Q

	2012/12/16	CViewCommander_Hokan.cpp��cppCViewCommander.cpp����x���֘A�𕪗���CViewCommander_Support.cpp�ɖ��̕ύX
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2000-2001, jepro, genta
	Copyright (C) 2001, asa-o
	Copyright (C) 2002, genta, aroka
	Copyright (C) 2003, Moca
	Copyright (C) 2005, genta
	Copyright (C) 2006, ryoji, fon
	Copyright (C) 2007, kobake, ryoji
	Copyright (C) 2011, Moca
	Copyright (C) 2012, Moca

	This source code is designed for sakura editor.
	Please contact the copyright holders to use this code for other purpose.
*/

#include "StdAfx.h"
#include <HtmlHelp.h>
#include "CViewCommander.h"
#include "CViewCommander_inline.h"

#include "CPropertyManager.h"
#include "CEditApp.h"
#include "dlg/CDlgAbout.h"	//Dec. 24, 2000 JEPRO �ǉ�
#include "env/CHelpManager.h"
#include "util/module.h"
#include "util/shell.h"

/*!	���͕⊮
	Ctrl+Space�ł����ɓ����B
	CEditView::m_bHokan�F ���ݕ⊮�E�B���h�E���\������Ă��邩��\���t���O�B
	m_Common.m_sHelper.m_bUseHokan�F���ݕ⊮�E�B���h�E���\������Ă���ׂ����ۂ�������킷�t���O�B

    @date 2001/06/19 asa-o �p�啶���������𓯈ꎋ����
                     ��₪1�̂Ƃ��͂���Ɋm�肷��
	@date 2001/06/14 asa-o �Q�ƃf�[�^�ύX
	                 �J���v���p�e�B�V�[�g���^�C�v�ʂɕύXy
	@date 2000/09/15 JEPRO [Esc]�L�[��[x]�{�^���ł����~�ł���悤�ɕύX
	@date 2005/01/10 genta CEditView_Command����ړ�
*/
void CViewCommander::Command_HOKAN( void )
{
	if(!GetDllShareData().m_Common.m_sHelper.m_bUseHokan){
		GetDllShareData().m_Common.m_sHelper.m_bUseHokan = TRUE;
	}
#if 0
// 2011.06.24 Moca Plugin�����ɏ]�����ݒ�̊m�F����߂�
retry:;
	/* �⊮���ꗗ�t�@�C�����ݒ肳��Ă��Ȃ��Ƃ��́A�ݒ肷��悤�ɑ����B */
	// 2003.06.22 Moca �t�@�C�������猟������ꍇ�ɂ͕⊮�t�@�C���̐ݒ�͕K�{�ł͂Ȃ�
	if( GetDocument()->m_cDocType.GetDocumentAttribute().m_bUseHokanByFile == FALSE &&
		GetDocument()->m_cDocType.GetDocumentAttribute().m_bUseHokanByKeyword == false &&
		_T('\0') == GetDocument()->m_cDocType.GetDocumentAttribute().m_szHokanFile[0]
	){
		ConfirmBeep();
		if( IDYES == ::ConfirmMessage( GetMainWindow(),
			_T("�⊮���ꗗ�t�@�C�����ݒ肳��Ă��܂���B\n�������ݒ肵�܂���?")
		) ){
			/* �^�C�v�ʐݒ� �v���p�e�B�V�[�g */
			if( !CEditApp::getInstance()->m_pcPropertyManager->OpenPropertySheetTypes( 2, GetDocument()->m_cDocType.GetDocumentType() ) ){
				return;
			}
			goto retry;
		}
	}
#endif
	CNativeW	cmemData;
	/* �J�[�\�����O�̒P����擾 */
	if( 0 < m_pCommanderView->GetParser().GetLeftWord( &cmemData, 100 ) ){
		m_pCommanderView->ShowHokanMgr( cmemData, TRUE );
	}else{
		InfoBeep(); //2010.04.03 Error��Info
		m_pCommanderView->SendStatusMessage(_T("�⊮�Ώۂ�����܂���")); // 2010.05.29 �X�e�[�^�X�ŕ\��
		GetDllShareData().m_Common.m_sHelper.m_bUseHokan = FALSE;	//	���͕⊮�I���̂��m�点
	}
	return;
}



/*! �L�����b�g�ʒu�̒P�����������ON-OFF

	@date 2006.03.24 fon �V�K�쐬
*/
void CViewCommander::Command_ToggleKeySearch( void )
{	/* ���ʐݒ�_�C�A���O�̐ݒ���L�[���蓖�Ăł��؂�ւ�����悤�� */
	if( GetDllShareData().m_Common.m_sSearch.m_bUseCaretKeyWord ){
		GetDllShareData().m_Common.m_sSearch.m_bUseCaretKeyWord = FALSE;
	}else{
		GetDllShareData().m_Common.m_sSearch.m_bUseCaretKeyWord = TRUE;
	}
}



/* �R�}���h�ꗗ */
void CViewCommander::Command_MENU_ALLFUNC( void )
{

	UINT	uFlags;
	POINT	po;
	RECT	rc;
	HMENU	hMenu;
	HMENU	hMenuPopUp;
	int		i;
	int		j;
	int		nId;

//	From Here Sept. 15, 2000 JEPRO
//	�T�u���j���[�A���Ɂu���̑��v�̃R�}���h�ɑ΂��ăX�e�[�^�X�o�[�ɕ\�������L�[�A�T�C�����
//	���j���[�ŉB��Ȃ��悤�ɉE�ɂ��炵��
//	(�{���͂��́u�R�}���h�ꗗ�v���j���[���_�C�A���O�ɕύX���o�[���܂�Ŏ��R�Ɉړ��ł���悤�ɂ�����)
//	po.x = 0;
	po.x = 540;
//	To Here Sept. 15, 2000 (Oct. 7, 2000 300��500; Nov. 3, 2000 500��540)
	po.y = 0;

	CEditWnd*	pCEditWnd = GetDocument()->m_pcEditWnd;	//	Sep. 10, 2002 genta
	::GetClientRect( pCEditWnd->GetHwnd(), &rc );
	po.x = t_min( po.x, rc.right );
	::ClientToScreen( pCEditWnd->GetHwnd(), &po );
	::GetWindowRect( pCEditWnd->m_cSplitterWnd.GetHwnd() , &rc );
	po.y = rc.top;

	pCEditWnd->GetMenuDrawer().ResetContents();

	//	Oct. 3, 2001 genta
	CFuncLookup& FuncLookup = GetDocument()->m_cFuncLookup;

	hMenu = ::CreatePopupMenu();
//Oct. 14, 2000 JEPRO �u--����`--�v��\�������Ȃ��悤�ɕύX�������Ƃ�1��(�J�[�\���ړ��n)���O�ɃV�t�g���ꂽ(���̕ύX�ɂ���� i=1��i=0 �ƕύX)
	//	Oct. 3, 2001 genta
	for( i = 0; i < FuncLookup.GetCategoryCount(); i++ ){
		hMenuPopUp = ::CreatePopupMenu();
		for( j = 0; j < FuncLookup.GetItemCount(i); j++ ){
			//	Oct. 3, 2001 genta
			int code = FuncLookup.Pos2FuncCode( i, j, false );	// 2007.11.02 ryoji ���o�^�}�N����\���𖾎��w��
			if( code != 0 ){
				WCHAR	szLabel[300];
				FuncLookup.Pos2FuncName( i, j, szLabel, 256 );
				uFlags = MF_BYPOSITION | MF_STRING | MF_ENABLED;
				//	Oct. 3, 2001 genta
				pCEditWnd->GetMenuDrawer().MyAppendMenu( hMenuPopUp, uFlags, code, szLabel, L"" );
			}
		}
		//	Oct. 3, 2001 genta
		pCEditWnd->GetMenuDrawer().MyAppendMenu( hMenu, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT)hMenuPopUp , FuncLookup.Category2Name(i) , _T(""));
//		pCEditWnd->GetMenuDrawer().MyAppendMenu( hMenu, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT)hMenuPopUp , nsFuncCode::ppszFuncKind[i] );
	}

	nId = ::TrackPopupMenu(
		hMenu,
		TPM_TOPALIGN
		| TPM_LEFTALIGN
		| TPM_RETURNCMD
		| TPM_LEFTBUTTON
		,
		po.x,
		po.y,
		0,
		GetMainWindow()/*GetHwnd()*/,
		NULL
	);
	::DestroyMenu( hMenu );
	if( 0 != nId ){
		/* �R�}���h�R�[�h�ɂ�鏈���U�蕪�� */
//		HandleCommand( nFuncID, true, 0, 0, 0, 0 );
		::PostMessageCmd( GetMainWindow(), WM_COMMAND, MAKELONG( nId, 0 ), (LPARAM)NULL );
	}
	return;
}



/* �o�[�W������� */
void CViewCommander::Command_ABOUT( void )
{
	CDlgAbout cDlgAbout;
	cDlgAbout.DoModal( G_AppInstance(), m_pCommanderView->GetHwnd() );
	return;
}
