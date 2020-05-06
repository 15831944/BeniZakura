/*!	@file
@brief CViewCommanderクラスのコマンド(検索系 アウトライン解析)関数群

	2012/12/17	CViewCommander.cppから分離
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2000-2001, jepro, genta
	Copyright (C) 2001, hor
	Copyright (C) 2002, YAZAKI
	Copyright (C) 2003, zenryaku
	Copyright (C) 2006, aroka
	Copyright (C) 2007, genta, kobake
	Copyright (C) 2009, genta
	Copyright (C) 2011, syat

	This source code is designed for sakura editor.
	Please contact the copyright holders to use this code for other purpose.
*/

#include "StdAfx.h"
#include "CViewCommander.h"
#include "CViewCommander_inline.h"

#include "outline/CFuncInfoArr.h"
#include "sakura_rc.h"


/*!	アウトライン解析
	
	2002/3/13 YAZAKI nOutlineTypeとnListTypeを統合。
*/
// トグル用のフラグに変更 20060201 aroka
BOOL CViewCommander::Command_FUNCLIST(
	int nAction,
	int _nOutlineType = OUTLINE_DEFAULT
)
{
	static bool bIsProcessing = false;	//アウトライン解析処理中フラグ

	//アウトラインプラグイン内でのEditor.Outline呼び出しによる再入を禁止する
	if( bIsProcessing )return FALSE;

	bIsProcessing = true;

	// 自プロセスが前面にいるかどうか調べる
	DWORD dwPid1, dwPid2;
	dwPid1 = ::GetCurrentProcessId();
	::GetWindowThreadProcessId( ::GetForegroundWindow(), &dwPid2 );
	bool bForeground = (dwPid1 == dwPid2);

	EOutlineType nOutlineType = (EOutlineType)_nOutlineType; //2007.11.29 kobake

//	if( bCheckOnly ){
//		return TRUE;
//	}

	static CFuncInfoArr	cFuncInfoArr;
//	int		nLine;
//	int		nListType;

	//	2001.12.03 hor & 2002.3.13 YAZAKI
	if( nOutlineType == OUTLINE_DEFAULT ){
		/* タイプ別に設定されたアウトライン解析方法 */
		nOutlineType = GetDocument()->m_cDocType.GetDocumentAttribute().m_eDefaultOutline;
//		if( nOutlineType == OUTLINE_CPP ){
//			if( CheckEXT( GetDocument()->m_cDocFile.GetFilePath(), _T("c") ) ){
//				nOutlineType = OUTLINE_C;	/* これでC関数一覧リストビューになる */
//			}
//		}
	}

	if( NULL != GetEditWindow()->m_cDlgFuncList.GetHwnd() ){
		switch( nAction ){
		case SHOW_TOGGLE: // 閉じる
			//	開いているものと種別が同じなら閉じる．異なれば再解析
			if( GetEditWindow()->m_cDlgFuncList.CheckListType( nOutlineType )){
				if( GetEditWindow()->m_cDlgFuncList.IsDocking() )
					::DestroyWindow( GetEditWindow()->m_cDlgFuncList.GetHwnd() );
				else
					::SendMessageAny( GetEditWindow()->m_cDlgFuncList.GetHwnd(), WM_CLOSE, 0, 0 );
				bIsProcessing = false;
				return TRUE;
			}
			break;
		case SHOW_NORMAL:
		case SHOW_RELOAD:
		default:
			break;
		}
	}

	/* 解析結果データを空にする */
	cFuncInfoArr.Empty();
	int		nListType = nOutlineType;			//2011.06.25 syat

	switch( nOutlineType ){
	case OUTLINE_C:			// C/C++ は MakeFuncList_C
	case OUTLINE_CPP:		GetDocument()->m_cDocOutline.MakeFuncList_C( &cFuncInfoArr );break;
	case OUTLINE_JAVA:		GetDocument()->m_cDocOutline.MakeFuncList_Java( &cFuncInfoArr );break;
	case OUTLINE_ASM:		GetDocument()->m_cDocOutline.MakeTopicList_asm( &cFuncInfoArr );break;
	case OUTLINE_WZTXT:		GetDocument()->m_cDocOutline.MakeTopicList_wztxt(&cFuncInfoArr);break;		// 2003.05.20 zenryaku 階層付テキスト アウトライン解析
	case OUTLINE_BOOKMARK:	GetDocument()->m_cDocOutline.MakeFuncList_BookMark( &cFuncInfoArr );break;	//	2001.12.03 hor
	case OUTLINE_FILE:		GetDocument()->m_cDocOutline.MakeFuncList_RuleFile( &cFuncInfoArr );break;	//	2002.04.01 YAZAKI アウトライン解析にルールファイルを導入
	case OUTLINE_PYTHON:	GetDocument()->m_cDocOutline.MakeFuncList_python(&cFuncInfoArr);break;		// 2007.02.08 genta
	case OUTLINE_TEXT:
		//	fall though
		//	ここには何も入れてはいけない 2007.02.28 genta 注意書き
	default:
		//それ以外
		GetDocument()->m_cDocOutline.MakeTopicList_txt( &cFuncInfoArr );
		break;
	}

	/* 解析対象ファイル名 */
	_tcscpy( cFuncInfoArr.m_szFilePath, GetDocument()->m_cDocFile.GetFilePath() );

	/* アウトライン ダイアログの表示 */
	CLayoutPoint poCaret = GetCaret().GetCaretLayoutPos();
	if( NULL == GetEditWindow()->m_cDlgFuncList.GetHwnd() ){
		GetEditWindow()->m_cDlgFuncList.DoModeless(
			G_AppInstance(),
			m_pCommanderView->GetHwnd(),
			(LPARAM)m_pCommanderView,
			&cFuncInfoArr,
			poCaret.GetY2() + CLayoutInt(1),
			poCaret.GetX2() + CLayoutInt(1),
			nOutlineType,
			nListType,
			GetDocument()->m_cDocType.GetDocumentAttribute().m_bLineNumIsCRLF	/* 行番号の表示 FALSE=折り返し単位／TRUE=改行単位 */
		);
	}else{
		/* アクティブにする */
		GetEditWindow()->m_cDlgFuncList.Redraw( nOutlineType, nListType, &cFuncInfoArr, poCaret.GetY2() + 1, poCaret.GetX2() + 1 );
		if( bForeground ){
			::SetFocus( GetEditWindow()->m_cDlgFuncList.GetHwnd() );
		}
	}

	bIsProcessing = false;
	return TRUE;
}
