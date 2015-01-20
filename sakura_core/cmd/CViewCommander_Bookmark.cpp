/*!	@file
@brief CViewCommander�N���X�̃R�}���h(�W�����v&�u�b�N�}�[�N)�֐��Q

	2012/12/17	CViewCommander.cpp,CViewCommander_New.cpp���番��
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2000-2001, genta
	Copyright (C) 2002, hor, YAZAKI, MIK
	Copyright (C) 2006, genta

	This source code is designed for sakura editor.
	Please contact the copyright holders to use this code for other purpose.
*/

#include "StdAfx.h"
#include "CViewCommander.h"
#include "CViewCommander_inline.h"


//	from CViewCommander_New.cpp
/*!	�����J�n�ʒu�֖߂�
	@author	ai
	@date	02/06/26
*/
void CViewCommander::Command_JUMP_SRCHSTARTPOS(void)
{
	if( m_pCommanderView->m_ptSrchStartPos_PHY.BothNatural() )
	{
		CLayoutPoint pt;
		/* �͈͑I�𒆂� */
		GetDocument()->m_cLayoutMgr.LogicToLayout(
			m_pCommanderView->m_ptSrchStartPos_PHY,
			&pt
		);
		//	2006.07.09 genta �I����Ԃ�ۂ�
		m_pCommanderView->MoveCursorSelecting( pt, m_pCommanderView->GetSelectionInfo().m_bSelectingLock );
	}
	else
	{
		ErrorBeep();
	}
	return;
}



/*! �w��s�փW�����v�_�C�A���O�̕\��
	2002.2.2 YAZAKI
*/
void CViewCommander::Command_JUMP_DIALOG( void )
{
	if( !GetEditWindow()->m_cDlgJump.DoModal(
		G_AppInstance(), m_pCommanderView->GetHwnd(), (LPARAM)GetDocument()
	) ){
		return;
	}
}



/* �w��s�w�W�����v */
void CViewCommander::Command_JUMP( void )
{
	if( 0 == GetDocument()->m_cLayoutMgr.GetLineCount() ){
		ErrorBeep();
		return;
	}

	/* �s�ԍ� */
	int	nLineNum; //$$ �P�ʍ���
	nLineNum = GetEditWindow()->m_cDlgJump.m_nLineNum;

	/* �s�ԍ��̕\�� FALSE=�܂�Ԃ��P�ʁ^TRUE=���s�P�� */
	if( GetDllShareData().m_bLineNumIsCRLF_ForJump ){
		if( CLogicInt(0) >= nLineNum ){
			nLineNum = CLogicInt(1);
		}
		/*
		  �J�[�\���ʒu�ϊ�
		  ���W�b�N�ʒu(�s������̃o�C�g���A�܂�Ԃ������s�ʒu)
		  ��
		  ���C�A�E�g�ʒu(�s������̕\�����ʒu�A�܂�Ԃ�����s�ʒu)
		*/
		CLayoutPoint ptPosXY;
		GetDocument()->m_cLayoutMgr.LogicToLayout(
			CLogicPoint(0, nLineNum - 1),
			&ptPosXY
		);
		nLineNum = (Int)ptPosXY.y + 1;
	}
	else{
		if( 0 >= nLineNum ){
			nLineNum = 1;
		}
		if( nLineNum > GetDocument()->m_cLayoutMgr.GetLineCount() ){
			nLineNum = (Int)GetDocument()->m_cLayoutMgr.GetLineCount();
		}
	}
	m_pCommanderView->AddCurrentLineToHistory();
	m_pCommanderView->MoveCursorSelecting( CLayoutPoint(0, nLineNum - 1), m_pCommanderView->GetSelectionInfo().m_bSelectingLock, _CARETMARGINRATE / 3 );
}



//	from CViewCommander_New.cpp
//! �u�b�N�}�[�N�̐ݒ�E�������s��(�g�O������)
void CViewCommander::Command_BOOKMARK_SET(void)
{
	CDocLine*	pCDocLine;
	if( m_pCommanderView->GetSelectionInfo().IsTextSelected() && m_pCommanderView->GetSelectionInfo().m_sSelect.GetFrom().y<m_pCommanderView->GetSelectionInfo().m_sSelect.GetTo().y ){
		CLogicPoint ptFrom;
		CLogicPoint ptTo;
		GetDocument()->m_cLayoutMgr.LayoutToLogic(
			CLayoutPoint(CLayoutInt(0), m_pCommanderView->GetSelectionInfo().m_sSelect.GetFrom().y),
			&ptFrom
		);
		GetDocument()->m_cLayoutMgr.LayoutToLogic(
			CLayoutPoint(CLayoutInt(0), m_pCommanderView->GetSelectionInfo().m_sSelect.GetTo().y  ),
			&ptTo
		);
		for(CLogicInt nY=ptFrom.GetY2();nY<=ptTo.y;nY++){
			pCDocLine=GetDocument()->m_cDocLineMgr.GetLine( nY );
			CBookmarkSetter cBookmark(pCDocLine);
			if(pCDocLine)cBookmark.SetBookmark(!cBookmark.IsBookmarked());
		}
	}
	else{
		pCDocLine=GetDocument()->m_cDocLineMgr.GetLine( GetCaret().GetCaretLogicPos().GetY2() );
		CBookmarkSetter cBookmark(pCDocLine);
		if(pCDocLine)cBookmark.SetBookmark(!cBookmark.IsBookmarked());
	}

	// 2002.01.16 hor ���������r���[���X�V
	GetEditWindow()->Views_Redraw();
}



//	from CViewCommander_New.cpp
//! ���̃u�b�N�}�[�N��T���C����������ړ�����
void CViewCommander::Command_BOOKMARK_NEXT(void)
{
	int			nYOld;				// hor
	BOOL		bFound	=	FALSE;	// hor
	BOOL		bRedo	=	TRUE;	// hor

	CLogicPoint	ptXY(0, GetCaret().GetCaretLogicPos().y);
	CLogicInt tmp_y;

	nYOld=ptXY.y;					// hor

re_do:;								// hor
	if(CBookmarkManager(&GetDocument()->m_cDocLineMgr).SearchBookMark(ptXY.GetY2(), SEARCH_FORWARD, &tmp_y)){
		ptXY.y = tmp_y;
		bFound = TRUE;
		CLayoutPoint ptLayout;
		GetDocument()->m_cLayoutMgr.LogicToLayout(ptXY,&ptLayout);
		//	2006.07.09 genta �V�K�֐��ɂ܂Ƃ߂�
		m_pCommanderView->MoveCursorSelecting( ptLayout, m_pCommanderView->GetSelectionInfo().m_bSelectingLock );
	}
    // 2002.01.26 hor
	if(GetDllShareData().m_Common.m_sSearch.m_bSearchAll){
		if(!bFound	&&		// ������Ȃ�����
			bRedo			// �ŏ��̌���
		){
			ptXY.y=-1;	//	2002/06/01 MIK
			bRedo=FALSE;
			goto re_do;		// �擪����Č���
		}
	}
	if(bFound){
		if(nYOld >= ptXY.y)m_pCommanderView->SendStatusMessage(_T("���擪����Č������܂���"));
	}else{
		m_pCommanderView->SendStatusMessage(_T("��������܂���ł���"));
	}
	return;
}



//	from CViewCommander_New.cpp
//! �O�̃u�b�N�}�[�N��T���C����������ړ�����D
void CViewCommander::Command_BOOKMARK_PREV(void)
{
	int			nYOld;				// hor
	BOOL		bFound	=	FALSE;	// hor
	BOOL		bRedo	=	TRUE;	// hor

	CLogicPoint	ptXY(0,GetCaret().GetCaretLogicPos().y);
	CLogicInt tmp_y;

	nYOld=ptXY.y;						// hor

re_do:;								// hor
	if(CBookmarkManager(&GetDocument()->m_cDocLineMgr).SearchBookMark(ptXY.GetY2(), SEARCH_BACKWARD, &tmp_y)){
		ptXY.y = tmp_y;
		bFound = TRUE;				// hor
		CLayoutPoint ptLayout;
		GetDocument()->m_cLayoutMgr.LogicToLayout(ptXY,&ptLayout);
		//	2006.07.09 genta �V�K�֐��ɂ܂Ƃ߂�
		m_pCommanderView->MoveCursorSelecting( ptLayout, m_pCommanderView->GetSelectionInfo().m_bSelectingLock );
	}
    // 2002.01.26 hor
	if(GetDllShareData().m_Common.m_sSearch.m_bSearchAll){
		if(!bFound	&&	// ������Ȃ�����
			bRedo		// �ŏ��̌���
		){
			// 2011.02.02 m_cLayoutMgr��m_cDocLineMgr
			ptXY.y= GetDocument()->m_cDocLineMgr.GetLineCount();	// 2002/06/01 MIK
			bRedo=FALSE;
			goto re_do;	// ��������Č���
		}
	}
	if(bFound){
		if(nYOld <= ptXY.y)m_pCommanderView->SendStatusMessage(_T("����������Č������܂���"));
	}else{
		m_pCommanderView->SendStatusMessage(_T("��������܂���ł���"));
	}
	return;
}



//	from CViewCommander_New.cpp
//! �u�b�N�}�[�N���N���A����
void CViewCommander::Command_BOOKMARK_RESET(void)
{
	CBookmarkManager(&GetDocument()->m_cDocLineMgr).ResetAllBookMark();
	// 2002.01.16 hor ���������r���[���X�V
	GetEditWindow()->Views_Redraw();
}



//	from CViewCommander_New.cpp
//�w��p�^�[���Ɉ�v����s���}�[�N 2002.01.16 hor
//�L�[�}�N���ŋL�^�ł���悤��	2002.02.08 hor
void CViewCommander::Command_BOOKMARK_PATTERN( void )
{
	//����or�u���_�C�A���O����Ăяo���ꂽ
	if( !m_pCommanderView->ChangeCurRegexp(false) ) return;
	
	CBookmarkManager(&GetDocument()->m_cDocLineMgr).MarkSearchWord(
		m_pCommanderView->m_strCurSearchKey.c_str(),		// ��������
		m_pCommanderView->m_sCurSearchOption,	// ��������
		&m_pCommanderView->m_CurRegexp							// ���K�\���R���p�C���f�[�^
	);
	// 2002.01.16 hor ���������r���[���X�V
	GetEditWindow()->Views_Redraw();
}
