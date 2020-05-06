/*!	@file
	@brief テキストのレイアウト情報

	@author Norio Nakatani
	@date 1998/3/11 新規作成
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2002, YAZAKI

	This source code is designed for sakura editor.
	Please contact the copyright holder to use this code for other purpose.
*/
#include "StdAfx.h"
#include "CLayout.h"
#include "CLayoutMgr.h"
#include "charset/charcode.h"
#include "extmodule/CBregexp.h" // CLayoutMgrの定義で必要



CLayout::~CLayout()
{
	return;
}

void CLayout::DUMP( void )
{
	DEBUG_TRACE( _T("\n\n■CLayout::DUMP()======================\n") );
	DEBUG_TRACE( _T("m_ptLogicPos.y=%d\t\t対応する論理行番号\n"), m_ptLogicPos.y );
	DEBUG_TRACE( _T("m_ptLogicPos.x=%d\t\t対応する論理行の先頭からのオフセット\n"), m_ptLogicPos.x );
	DEBUG_TRACE( _T("m_nLength=%d\t\t対応する論理行のハイト数\n"), (int)m_nLength );
	DEBUG_TRACE( _T("m_colorIndexPrev=%d\t\tColorIndexType\n"), m_colorIndexPrev );
	DEBUG_TRACE( _T("======================\n") );
	return;
}

//!レイアウト幅を計算。改行は含まない。
//2007.10.11 kobake 作成
//2007.11.29 kobake タブ幅が計算されていなかったのを修正
//2011.12.26 Moca インデントは含むように変更(座標変換バグ修正)
CLayoutInt CLayout::CalcLayoutWidth(const CLayoutMgr& cLayoutMgr) const
{
	//ソース
	const wchar_t* pText    = m_pCDocLine->GetPtr();
	CLogicInt      nTextLen = m_pCDocLine->GetLengthWithoutEOL();

	//計算
	CLayoutInt nWidth = GetIndent();
	for(CLogicInt i=m_ptLogicPos.GetX2();i<m_ptLogicPos.GetX2()+m_nLength;i++){
		if(pText[i]==WCODE::TAB){
			nWidth += cLayoutMgr.GetActualTabSpace(nWidth);
		}
		else{
			nWidth += CNativeW::GetKetaOfChar(pText,nTextLen,i);
		}
	}
	return nWidth;
}

//! オフセット値をレイアウト単位に変換して取得。2007.10.17 kobake
CLayoutInt CLayout::CalcLayoutOffset(const CLayoutMgr& cLayoutMgr) const
{
	CLayoutInt nRet(0);
	if(this->GetLogicOffset()){
		const wchar_t* pLine = this->m_pCDocLine->GetPtr();
		int nLineLen = this->m_pCDocLine->GetLengthWithEOL();
		for(int i=0;i<GetLogicOffset();i++){
			if(pLine[i]==WCODE::TAB){
				nRet+=cLayoutMgr.GetActualTabSpace(nRet);
			}
			else{
				nRet+=CNativeW::GetKetaOfChar(pLine,nLineLen,i);
			}
		}
	}
	return nRet;
}



