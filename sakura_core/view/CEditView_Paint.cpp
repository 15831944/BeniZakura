/*
	Copyright (C) 2008, kobake

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
#include <vector>
#include <limits.h>
#include "view/CEditView_Paint.h"
#include "view/CEditView.h"
#include "view/CViewFont.h"
#include "view/CRuler.h"
#include "view/colors/CColorStrategy.h"
#include "view/colors/CColor_Found.h"
#include "view/figures/CFigureManager.h"
#include "types/CTypeSupport.h"
#include "doc/CEditDoc.h"
#include "doc/layout/CLayout.h"
#include "window/CEditWnd.h"
#include "parse/CWordParse.h"
#include "util/string_ex2.h"

void _DispWrap(CGraphics& gr, DispPos* pDispPos, const CEditView* pcView);

/*
	PAINT_LINENUMBER = (1<<0), //!< 行番号
	PAINT_RULER      = (1<<1), //!< ルーラー
	PAINT_BODY       = (1<<2), //!< 本文
*/

void CEditView_Paint::Call_OnPaint(
	int nPaintFlag,   //!< 描画する領域を選択する
	bool bUseMemoryDC //!< メモリDCを使用する
)
{
	CEditView* pView = GetEditView();

	//各要素
	CMyRect rcLineNumber(0,pView->GetTextArea().GetAreaTop(),pView->GetTextArea().GetAreaLeft(),pView->GetTextArea().GetAreaBottom());
	CMyRect rcRuler(pView->GetTextArea().GetAreaLeft(),0,pView->GetTextArea().GetAreaRight(),pView->GetTextArea().GetAreaTop());
	CMyRect rcBody(pView->GetTextArea().GetAreaLeft(),pView->GetTextArea().GetAreaTop(),pView->GetTextArea().GetAreaRight(),pView->GetTextArea().GetAreaBottom());

	//領域を作成 -> rc
	std::vector<CMyRect> rcs;
	if(nPaintFlag & PAINT_LINENUMBER)rcs.push_back(rcLineNumber);
	if(nPaintFlag & PAINT_RULER)rcs.push_back(rcRuler);
	if(nPaintFlag & PAINT_BODY)rcs.push_back(rcBody);
	if(rcs.size()==0)return;
	CMyRect rc=rcs[0];
	int nSize = (int)rcs.size();
	for(int i=1;i<nSize;i++)
		rc=MergeRect(rc,rcs[i]);

	//描画
	PAINTSTRUCT	ps;
	ps.rcPaint = rc;
	HDC hdc = pView->GetDC();
	pView->OnPaint( hdc, &ps, bUseMemoryDC );
	pView->ReleaseDC( hdc );
}



/* フォーカス移動時の再描画

	@date 2001/06/21 asa-o 「スクロールバーの状態を更新する」「カーソル移動」削除
*/
void CEditView::RedrawAll()
{
	// ウィンドウ全体を再描画
	PAINTSTRUCT	ps;
	HDC hdc = ::GetDC( GetHwnd() );
	::GetClientRect( GetHwnd(), &ps.rcPaint );
	OnPaint( hdc, &ps, FALSE );
	::ReleaseDC( GetHwnd(), hdc );

	// キャレットの表示
	GetCaret().ShowEditCaret();

	// キャレットの行桁位置を表示する
	GetCaret().ShowCaretPosInfo();

	// 親ウィンドウのタイトルを更新
	m_pcEditWnd->UpdateCaption();

	//	Jul. 9, 2005 genta	選択範囲の情報をステータスバーへ表示
	GetSelectionInfo().PrintSelectionInfoMsg();

	// スクロールバーの状態を更新する
	AdjustScrollBars();
}

// 2001/06/21 Start by asa-o 再描画
void CEditView::Redraw()
{
	HDC			hdc;
	PAINTSTRUCT	ps;

	hdc = ::GetDC( GetHwnd() );

	::GetClientRect( GetHwnd(), &ps.rcPaint );

	OnPaint( hdc, &ps, FALSE );

	::ReleaseDC( GetHwnd(), hdc );
}
// 2001/06/21 End

void MyFillRect(HDC hdc, RECT& re)
{
	::ExtTextOut(hdc, re.left, re.top, ETO_OPAQUE|ETO_CLIPPED, &re, _T(""), 0, NULL);
}

void CEditView::DrawBackImage(HDC hdc, RECT& rcPaint, HDC hdcBgImg)
{
#if 0
//	テスト背景パターン
	static int testColorIndex = 0;
	testColorIndex = testColorIndex % 7;
	COLORREF cols[7] = {RGB(255,255,255),
		RGB(200,255,255),RGB(255,200,255),RGB(255,255,200),
		RGB(200,200,255),RGB(255,200,200),RGB(200,255,200),
	};
	COLORREF colorOld = ::SetBkColor(hdc, cols[testColorIndex]);
	MyFillRect(hdc, rcPaint);
	::SetBkColor(hdc, colorOld);
	testColorIndex++;
#else
	CTypeSupport cTextType(this,COLORIDX_TEXT);
	COLORREF colorOld = ::SetBkColor(hdc, cTextType.GetBackColor());
	const CTextArea& area = GetTextArea();
	const CEditDoc& doc  = *m_pcEditDoc;
	const STypeConfig& typeConfig = doc.m_cDocType.GetDocumentAttribute();

	CMyRect rcImagePos;
	switch( typeConfig.m_backImgPos ){
	case BGIMAGE_TOP_LEFT:
	case BGIMAGE_BOTTOM_LEFT:
	case BGIMAGE_CENTER_LEFT:
		rcImagePos.left = area.GetAreaLeft();
		break;
	case BGIMAGE_TOP_RIGHT:
	case BGIMAGE_BOTTOM_RIGHT:
	case BGIMAGE_CENTER_RIGHT:
		rcImagePos.left = area.GetAreaRight() - doc.m_nBackImgWidth;
		break;
	case BGIMAGE_TOP_CENTER:
	case BGIMAGE_BOTTOM_CENTER:
	case BGIMAGE_CENTER:
		rcImagePos.left = area.GetAreaLeft() + area.GetAreaWidth()/2 - doc.m_nBackImgWidth/2;
		break;
	default:
		assert_warning(0 != typeConfig.m_backImgPos);
		break;
	}
	switch( typeConfig.m_backImgPos ){
	case BGIMAGE_TOP_LEFT:
	case BGIMAGE_TOP_RIGHT:
	case BGIMAGE_TOP_CENTER:
		rcImagePos.top  = area.GetAreaTop();
		break;
	case BGIMAGE_BOTTOM_LEFT:
	case BGIMAGE_BOTTOM_RIGHT:
	case BGIMAGE_BOTTOM_CENTER:
		rcImagePos.top  = area.GetAreaBottom() - doc.m_nBackImgHeight;
		break;
	case BGIMAGE_CENTER_LEFT:
	case BGIMAGE_CENTER_RIGHT:
	case BGIMAGE_CENTER:
		rcImagePos.top  = area.GetAreaTop() + area.GetAreaHeight()/2 - doc.m_nBackImgHeight/2;
		break;
	default:
		assert_warning(0 != typeConfig.m_backImgPos);
		break;
	}
	rcImagePos.left += typeConfig.m_backImgPosOffset.x;
	rcImagePos.top  += typeConfig.m_backImgPosOffset.y;
	// スクロール時の画面の端を作画するときの位置あたりへ移動
	if( typeConfig.m_backImgScrollX ){
		int tile = typeConfig.m_backImgRepeatX ? doc.m_nBackImgWidth : INT_MAX;
		Int posX = (area.GetViewLeftCol() % tile) * GetTextMetrics().GetHankakuDx();
		rcImagePos.left -= posX % tile;
	}
	if( typeConfig.m_backImgScrollY ){
		int tile = typeConfig.m_backImgRepeatY ? doc.m_nBackImgHeight : INT_MAX;
		Int posY = (area.GetViewTopLine() % tile) * GetTextMetrics().GetHankakuDy();
		rcImagePos.top -= posY % tile;
	}
	if( typeConfig.m_backImgRepeatX ){
		if( 0 < rcImagePos.left ){
			// rcImagePos.left = rcImagePos.left - (rcImagePos.left / doc.m_nBackImgWidth + 1) * doc.m_nBackImgWidth;
			rcImagePos.left = rcImagePos.left % doc.m_nBackImgWidth - doc.m_nBackImgWidth;
		}
	}
	if( typeConfig.m_backImgRepeatY ){
		if( 0 < rcImagePos.top ){
			// rcImagePos.top = rcImagePos.top - (rcImagePos.top / doc.m_nBackImgHeight + 1) * doc.m_nBackImgHeight;
			rcImagePos.top = rcImagePos.top % doc.m_nBackImgHeight - doc.m_nBackImgHeight;
		}
	}
	rcImagePos.SetSize(doc.m_nBackImgWidth, doc.m_nBackImgHeight);
	
	
	RECT rc = rcPaint;
	// rc.left = t_max((int)rc.left, area.GetAreaLeft());
	rc.top  = t_max((int)rc.top,  area.GetRulerHeight()); // ルーラーを除外
	const int nXEnd = area.GetAreaRight();
	const int nYEnd = area.GetAreaBottom();
	CMyRect rcBltAll;
	rcBltAll.SetLTRB(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);
	CMyRect rcImagePosOrg = rcImagePos;
	for(; rcImagePos.top <= nYEnd; ){
		for(; rcImagePos.left <= nXEnd; ){
			CMyRect rcBlt;
			if( ::IntersectRect(&rcBlt, &rc, &rcImagePos) ){
				::BitBlt(
					hdc,
					rcBlt.left,
					rcBlt.top,
					rcBlt.right  - rcBlt.left,
					rcBlt.bottom - rcBlt.top,
					hdcBgImg,
					rcBlt.left - rcImagePos.left,
					rcBlt.top - rcImagePos.top,
					SRCCOPY
				);
				rcBltAll.left   = t_min(rcBltAll.left,   rcBlt.left);
				rcBltAll.top    = t_min(rcBltAll.top,    rcBlt.top);
				rcBltAll.right  = t_max(rcBltAll.right,  rcBlt.right);
				rcBltAll.bottom = t_max(rcBltAll.bottom, rcBlt.bottom);
			}
			rcImagePos.left  += doc.m_nBackImgWidth;
			rcImagePos.right += doc.m_nBackImgWidth;
			if( !typeConfig.m_backImgRepeatX ){
				break;
			}
		}
		rcImagePos.left  = rcImagePosOrg.left;
		rcImagePos.right = rcImagePosOrg.right;
		rcImagePos.top    += doc.m_nBackImgHeight;
		rcImagePos.bottom += doc.m_nBackImgHeight;
		if( !typeConfig.m_backImgRepeatY ){
			break;
		}
	}
	if( rcBltAll.left != INT_MAX ){
		// 上下左右ななめの隙間を埋める
		CMyRect rcFill;
		LONG& x1 = rc.left;
		LONG& x2 = rcBltAll.left;
		LONG& x3 = rcBltAll.right;
		LONG& x4 = rc.right;
		LONG& y1 = rc.top;
		LONG& y2 = rcBltAll.top;
		LONG& y3 = rcBltAll.bottom;
		LONG& y4 = rc.bottom;
		if( y1 < y2 ){
			rcFill.SetLTRB(x1,y1, x4,y2); MyFillRect(hdc, rcFill);
		}
		if( x1 < x2 ){
			rcFill.SetLTRB(x1,y2, x2,y3); MyFillRect(hdc, rcFill);
		}
		if( x3 < x4 ){
			rcFill.SetLTRB(x3,y2, x4,y3); MyFillRect(hdc, rcFill);
		}
		if( y3 < y4 ){
			rcFill.SetLTRB(x1,y3, x4,y4); MyFillRect(hdc, rcFill);
		}
	}else{
		MyFillRect(hdc, rc);
	}
	::SetBkColor(hdc, colorOld);
#endif
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          色設定                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

/*! 指定位置の ColorIndex および colorStrategyState の取得
	CEditView::DrawLogicLineを元にしたためCEditView::DrawLogicLineに
	修正があった場合は、ここも修正が必要。
*/
EColorIndexType CEditView::GetColorIndex(
	const CLayout*		pcLayout,
	int					nIndex,
	ColorStrategyState& rColorStrategyState,
	bool				bPrev,				// 指定位置の色変更直前まで
	CColorStrategy**	ppStrategy,
	CColor_Found**		ppStrategyFound
)
{
	EColorIndexType eColor = COLORIDX_TEXT;

	if(!pcLayout){
		return COLORIDX_TEXT;
	}

	/* 論理行データの取得 */
	DispPos _sPos(0,0); // 注意：この値はダミー。DoChangeColorでの参照位置は不正確
	SColorStrategyInfo _sInfo;
	SColorStrategyInfo* pInfo = &_sInfo;
	pInfo->pcView = this;
	pInfo->pDispPos=&_sPos;
	{
		pInfo->pLineOfLogic = pcLayout->GetDocLineRef()->GetPtr();

		// 論理行の最初のレイアウト情報を取得 -> pcLayoutLineFirst
		const CLayout* pcLayoutLineFirst = pcLayout;
		while( 0 != pcLayoutLineFirst->GetLogicOffset() ){
			pcLayoutLineFirst = pcLayoutLineFirst->GetPrevLayout();
		}

		eColor = pcLayoutLineFirst->GetColorTypePrev();	/* 現在の色を指定 */
		pInfo->colorStrategyState = pcLayoutLineFirst->GetColorStrategyStatePrev();
		pInfo->nPosInLogic = pcLayoutLineFirst->GetLogicOffset();

		//CColorStrategyPool初期化
		CColorStrategyPool* pool = CColorStrategyPool::getInstance();
		pool->SetCurrentView(this);
		pool->NotifyOnStartScanLogic();
	}

	//文字列参照
	const CDocLine* pcDocLine = pcLayout->GetDocLineRef();
	CStringRef cLineStr(pcDocLine->GetPtr(),pcDocLine->GetLengthWithEOL());

	//color strategy
	CColorStrategyPool* pool = CColorStrategyPool::getInstance();
	pInfo->pStrategy = pool->GetStrategyByColor(eColor);
	if(pInfo->pStrategy)pInfo->pStrategy->InitStrategyStatus();

	int nPosTo = pcLayout->GetLogicOffset() + t_min(nIndex, (int)pcLayout->GetLengthWithEOL() - 1);
	CColor3Setting cColor;
	while( pInfo->nPosInLogic <= nPosTo ){
		if( bPrev && pInfo->nPosInLogic == nPosTo ){
			break;
		}

		//色切替
		pInfo->DoChangeColor(cLineStr, &cColor, pInfo->colorStrategyState);

		//1文字進む
		pInfo->nPosInLogic += CNativeW::GetSizeOfChar(
									cLineStr.GetPtr(),
									cLineStr.GetLength(),
									pInfo->nPosInLogic
								);
	}
	eColor = pInfo->GetCurrentColor2();

	if( ppStrategy ){
		*ppStrategy = pInfo->pStrategy;
	}
	if( ppStrategyFound ){
		*ppStrategyFound = pInfo->pStrategyFound;
	}

	rColorStrategyState = pInfo->colorStrategyState;
	return eColor;
}


/*! 現在の色を指定
	@param eColorIndex 現在の色

	@date 2013.05.08 novice 範囲外チェック削除
*/
void CEditView::SetCurrentColor( CGraphics& gr, EColorIndexType eColorIndex )
{
	//インデックス決定
	int		nColorIdx = ToColorInfoArrIndex(eColorIndex);

	//実際に色を設定
	const ColorInfo& info = m_pTypeData->m_ColorInfoArr[nColorIdx];
	gr.SetTextForeColor(info.m_colTEXT);
	gr.SetTextBackColor(info.m_colBACK);
	gr.SetMyFont(
		GetFontset().ChooseFontHandle(
			info.m_bBoldFont,
			info.m_bUnderLine
		)
	);
}

/*! 現在の色を指定
	@param eColorIndex   選択を含む現在の色
	@param eColorIndex2  選択以外の現在の色
	@param eColorIndexBg 背景色

	@date 2013.05.08 novice 範囲外チェック削除
*/
void CEditView::SetCurrentColor3( CGraphics& gr, EColorIndexType eColorIndex,  EColorIndexType eColorIndex2, EColorIndexType eColorIndexBg)
{
	//インデックス決定
	int		nColorIdx = ToColorInfoArrIndex(eColorIndex);
	int		nColorIdx2 = ToColorInfoArrIndex(eColorIndex2);
	int		nColorIdxBg = ToColorInfoArrIndex(eColorIndexBg);

	//実際に色を設定
	const ColorInfo& info  = m_pTypeData->m_ColorInfoArr[nColorIdx];
	const ColorInfo& info2 = m_pTypeData->m_ColorInfoArr[nColorIdx2];
	const ColorInfo& infoBg = m_pTypeData->m_ColorInfoArr[nColorIdxBg];
	gr.SetTextForeColor(GetTextColorByColorInfo2(info, info2));
	// 2012.11.21 背景色がテキストとおなじなら背景色はカーソル行背景
	const ColorInfo& info3 = (info2.m_colBACK == m_crBack ? infoBg : info2);
	if( nColorIdx == nColorIdx2 ){
		gr.SetTextBackColor(info3.m_colBACK);
	}else{
		gr.SetTextBackColor(GetBackColorByColorInfo2(info, info3));
	}
	gr.SetMyFont(
		GetFontset().ChooseFontHandle(
			info.m_colTEXT != info.m_colBACK ? info.m_bBoldFont  : info2.m_bBoldFont,
			info.m_colTEXT != info.m_colBACK ? info.m_bUnderLine : info2.m_bUnderLine
		)
	);
}

inline COLORREF MakeColor2(COLORREF a, COLORREF b, int alpha)
{
#ifdef USE_SSE
//	COLORREF result_mmx;
	// (a * alpha + b * (256 - alpha)) / 256 -> ((a - b) * alpha) / 256 + b
	_asm{
		pxor mm0, mm0; // mm0 = 0

		movd mm1, a;
		movd mm2, b;
		movd mm3, alpha;

		punpcklbw mm1, mm0; // a:a:a:a
		punpcklbw mm2, mm0; // b:b:b:b
		pshufw mm3, mm3, 0; // alpha:alpha:alpha:alpha

		psubw mm1, mm2;  // (a - b)
		pmullw mm1, mm3; // (a - b) * alpha
		psrlw mm1, 8;    // ((a - b) * alpha) / 256
		paddb mm1, mm2;  // ((a - b) * alpha) / 256 + b

		packuswb mm1, mm0;
//		movd result_mmx, mm1;
		movd eax, mm1;

		emms
	}
//	return result_mmx;
#else
	const int ap = alpha;
	const int bp = 256 - ap;
	BYTE valR = (BYTE)((GetRValue(a) * ap + GetRValue(b) * bp) / 256);
	BYTE valG = (BYTE)((GetGValue(a) * ap + GetGValue(b) * bp) / 256);
	BYTE valB = (BYTE)((GetBValue(a) * ap + GetBValue(b) * bp) / 256);
	return RGB(valR, valG, valB);
#endif
}

COLORREF CEditView::GetTextColorByColorInfo2(const ColorInfo& info, const ColorInfo& info2)
{
	if( info.m_colTEXT != info.m_colBACK ){
		return info.m_colTEXT;
	}
	// 反転表示
	if( info.m_colBACK == m_crBack ){
		return  info2.m_colTEXT ^ 0x00FFFFFF;
	}
	return  info2.m_colTEXT;
}

COLORREF CEditView::GetBackColorByColorInfo2(const ColorInfo& info, const ColorInfo& info2)
{
	if( info.m_colTEXT != info.m_colBACK ){
		return info.m_colBACK;
	}
	// 反転表示
	if( info.m_colBACK == m_crBack ){
		return  info2.m_colBACK ^ 0x00FFFFFF;
	}
	int alpha = 255 * 50 / 100; // 50%
	return MakeColor2(info.m_colBACK, info2.m_colBACK, alpha);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           描画                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

/*! 通常の描画処理 new 
	@param pPs  pPs.rcPaint は正しい必要がある
	@param bDrawFromComptibleBmp  TRUE 画面バッファからhdcに作画する(コピーするだけ)。
			TRUEの場合、pPs.rcPaint領域外は作画されないが、FALSEの場合は作画される事がある。
			互換DC/BMPが無い場合は、普通の作画処理をする。

	@date 2007.09.09 Moca 元々無効化されていた第三パラメータのbUseMemoryDCをbDrawFromComptibleBmpに変更。
	@date 2009.03.26 ryoji 行番号のみ描画を通常の行描画と分離（効率化）
*/
void CEditView::OnPaint( HDC _hdc, PAINTSTRUCT *pPs, BOOL bDrawFromComptibleBmp )
{
//	MY_RUNNINGTIMER( cRunningTimer, "CEditView::OnPaint" );
	CGraphics gr(_hdc);

	// 2004.01.28 Moca デスクトップに作画しないように
	if( (HDC)NULL == gr )return;

	if( !GetDrawSwitch() )return;
	//@@@
#if 0
	::MYTRACE( _T("OnPaint(%d,%d)-(%d,%d) : %d\n"),
		pPs->rcPaint.left,
		pPs->rcPaint.top,
		pPs->rcPaint.right,
		pPs->rcPaint.bottom,
		bDrawFromComptibleBmp
		);
#endif
	
	// From Here 2007.09.09 Moca 互換BMPによる画面バッファ
	// 互換BMPからの転送のみによる作画
	if( bDrawFromComptibleBmp
		&& m_hdcCompatDC && m_hbmpCompatBMP ){
		::BitBlt(
			gr,
			pPs->rcPaint.left,
			pPs->rcPaint.top,
			pPs->rcPaint.right - pPs->rcPaint.left,
			pPs->rcPaint.bottom - pPs->rcPaint.top,
			m_hdcCompatDC,
			pPs->rcPaint.left,
			pPs->rcPaint.top,
			SRCCOPY
		);
		if ( m_pcEditWnd->m_nActivePaneIndex == m_nMyIndex ){
			/* アクティブペインは、アンダーライン描画 */
			GetCaret().m_cUnderLine.CaretUnderLineON( true, false );
		}
		return;
	}
	if( m_hdcCompatDC && NULL == m_hbmpCompatBMP
		 || m_nCompatBMPWidth < (pPs->rcPaint.right - pPs->rcPaint.left)
		 || m_nCompatBMPHeight < (pPs->rcPaint.bottom - pPs->rcPaint.top) ){
		RECT rect;
		::GetWindowRect( this->GetHwnd(), &rect );
		CreateOrUpdateCompatibleBitmap( rect.right - rect.left, rect.bottom - rect.top );
	}
	// To Here 2007.09.09 Moca

	// キャレットを隠す
	bool bCaretShowFlag_Old = GetCaret().GetCaretShowFlag();	// 2008.06.09 ryoji
	GetCaret().HideCaret_( this->GetHwnd() ); // 2002/07/22 novice

	//	Aug. 14, 2005 genta 折り返し幅をLayoutMgrから取得するように
	const CLayoutInt nWrapKeta = m_pcEditDoc->m_cLayoutMgr.GetMaxLineKetas();

	RECT			rc;
	int				nLineHeight = GetTextMetrics().GetHankakuDy();
	int				nCharDx = GetTextMetrics().GetHankakuDx();

	//サポート
	CTypeSupport cTextType(this,COLORIDX_TEXT);

//@@@ 2001.11.17 add start MIK
	//変更があればタイプ設定を行う。
	if( m_pTypeData->m_bUseRegexKeyword || m_cRegexKeyword->m_bUseRegexKeyword ) //OFFなのに前回のデータが残ってる
	{
		//タイプ別設定をする。設定済みかどうかは呼び先でチェックする。
		m_cRegexKeyword->RegexKeySetTypes(m_pTypeData);
	}
//@@@ 2001.11.17 add end MIK

	bool bTransText = IsBkBitmap();
	// メモリＤＣを利用した再描画の場合は描画先のＤＣを切り替える
	HDC hdcOld = 0;
	// 2007.09.09 Moca bUseMemoryDCを有効化。
	// bUseMemoryDC = FALSE;
	BOOL bUseMemoryDC = (m_hdcCompatDC != NULL);
	assert_warning(gr != m_hdcCompatDC);
	if( bUseMemoryDC ){
		hdcOld = gr;
		gr = m_hdcCompatDC;
	}else{
		if( bTransText || pPs->rcPaint.bottom - pPs->rcPaint.top <= 2 || pPs->rcPaint.left - pPs->rcPaint.right <= 2 ){
			// 透過処理の場合フォントの輪郭が重ね塗りになるため自分でクリッピング領域を設定
			// 2以下はたぶんアンダーライン・カーソル行縦線の作画
			// MemoryDCの場合は転送が矩形クリッピングの代わりになっている
			gr.SetClipping(pPs->rcPaint);
		}
	}

	/* 03/02/18 対括弧の強調表示(消去) ai */
	if( !bUseMemoryDC ){
		// MemoryDCだとスクロール時に先に括弧だけ表示されて不自然なので後でやる。
		DrawBracketPair( false );
	}

	// 背景の表示
	if( bTransText ){
		HDC hdcBgImg = CreateCompatibleDC(gr);
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(hdcBgImg, m_pcEditDoc->m_hBackImg);
		DrawBackImage(gr, pPs->rcPaint, hdcBgImg);
		SelectObject(hdcBgImg, hOldBmp);
		DeleteObject(hdcBgImg);
	}

	/* ルーラーとテキストの間の余白 */
	//@@@ 2002.01.03 YAZAKI 余白が0のときは無駄でした。
	if ( GetTextArea().GetTopYohaku() ){
		if( !bTransText ){
			rc.left   = 0;
			rc.top    = GetTextArea().GetRulerHeight();
			rc.right  = GetTextArea().GetAreaRight();
			rc.bottom = GetTextArea().GetAreaTop();
			cTextType.FillBack(gr,rc);
		}
	}
	
	/* 行番号の表示 */
	//	From Here Sep. 7, 2001 genta
	//	Sep. 23, 2002 genta 行番号非表示でも行番号色の帯があるので隙間を埋める
	if( GetTextArea().GetTopYohaku() ){
		if( bTransText && m_pTypeData->m_ColorInfoArr[COLORIDX_GYOU].m_colBACK == cTextType.GetBackColor() ){
		}else{
			rc.left   = 0;
			rc.top    = GetTextArea().GetRulerHeight();
			rc.right  = GetTextArea().GetLineNumberWidth(); //	Sep. 23 ,2002 genta 余白はテキスト色のまま残す
			rc.bottom = GetTextArea().GetAreaTop();
			gr.SetTextBackColor(m_pTypeData->m_ColorInfoArr[COLORIDX_GYOU].m_colBACK);
			gr.FillMyRectTextBackColor(rc);
		}
	}
	//	To Here Sep. 7, 2001 genta

	::SetBkMode( gr, TRANSPARENT );

	cTextType.SetGraphicsState_WhileThisObj(gr);


	int nTop = pPs->rcPaint.top;

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//           描画開始レイアウト絶対行 -> nLayoutLine             //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	CLayoutInt nLayoutLine;
	if( 0 > nTop - GetTextArea().GetAreaTop() ){
		nLayoutLine = GetTextArea().GetViewTopLine(); //ビュー上部から描画
	}else{
		nLayoutLine = GetTextArea().GetViewTopLine() + CLayoutInt( ( nTop - GetTextArea().GetAreaTop() ) / nLineHeight ); //ビュー途中から描画
	}

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//          描画終了レイアウト絶対行 -> nLayoutLineTo            //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	CLayoutInt nLayoutLineTo = GetTextArea().GetViewTopLine()
		+ CLayoutInt( ( pPs->rcPaint.bottom - GetTextArea().GetAreaTop() + (nLineHeight - 1) ) / nLineHeight ) - 1;	// 2007.02.17 ryoji 計算を精密化


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                         描画座標                            //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	DispPos sPos(GetTextMetrics().GetHankakuDx(),GetTextMetrics().GetHankakuDy());
	sPos.InitDrawPos(CMyPoint(
		GetTextArea().GetAreaLeft() - (Int)GetTextArea().GetViewLeftCol() * nCharDx,
		GetTextArea().GetAreaTop() + (Int)( nLayoutLine - GetTextArea().GetViewTopLine() ) * nLineHeight
	));
	sPos.SetLayoutLineRef(nLayoutLine);


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                      全部の行を描画                         //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

	//必要な行を描画する	// 2009.03.26 ryoji 行番号のみ描画を通常の行描画と分離（効率化）
	if(pPs->rcPaint.right <= GetTextArea().GetAreaLeft()){
		while(sPos.GetLayoutLineRef() <= nLayoutLineTo)
		{
			if(!sPos.GetLayoutRef())
				break;

			//1行描画（行番号のみ）
			GetTextDrawer().DispLineNumber(
				gr,
				sPos.GetLayoutLineRef(),
				sPos.GetDrawPos().y
			);
			//行を進める
			sPos.ForwardDrawLine(1);		//描画Y座標＋＋
			sPos.ForwardLayoutLineRef(1);	//レイアウト行＋＋
		}
	}else{
		while(sPos.GetLayoutLineRef() <= nLayoutLineTo)
		{
			//描画X位置リセット
			sPos.ResetDrawCol();

			//1行描画
			bool bDispResult = DrawLogicLine(
				gr,
				&sPos,
				nLayoutLineTo
			);

			if(bDispResult){
				pPs->rcPaint.bottom += nLineHeight;	// EOF再描画対応
				break;
			}
		}
	}

	gr.ClearMyFont();


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//              テキストの無い部分の塗りつぶし                 //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	if( !bTransText && sPos.GetDrawPos().y < pPs->rcPaint.bottom ){
		RECT rcBack;
		rcBack.left   = pPs->rcPaint.left;
		rcBack.right  = pPs->rcPaint.right;
		rcBack.top    = sPos.GetDrawPos().y;
		rcBack.bottom = pPs->rcPaint.bottom;

		cTextType.FillBack(gr,rcBack);
	}
	{
		// 2006.04.29 行部分は行ごとに作画し、ここでは縦線の残りを作画
		GetTextDrawer().DispVerticalLines( gr, sPos.GetDrawPos().y, pPs->rcPaint.bottom, CLayoutInt(0), CLayoutInt(-1) );
		GetTextDrawer().DispWrapLine( gr, sPos.GetDrawPos().y, pPs->rcPaint.bottom );	// 2009.10.24 ryoji
	}

	cTextType.RewindGraphicsState(gr);


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                       ルーラー描画                          //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	if ( pPs->rcPaint.top < GetTextArea().GetRulerHeight() ) { // ルーラーが再描画範囲にあるときのみ再描画する 2002.02.25 Add By KK
		GetRuler().SetRedrawFlag(); //2002.02.25 Add By KK ルーラー全体を描画。
		GetRuler().DispRuler( gr );
	}

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                     その他後始末など                        //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	/* メモリＤＣを利用した再描画の場合はメモリＤＣに描画した内容を画面へコピーする */
	if( bUseMemoryDC ){
		// 2010.10.11 先に描くと背景固定のスクロールなどでの表示が不自然になる
		DrawBracketPair( false );

		::BitBlt(
			hdcOld,
			pPs->rcPaint.left,
			pPs->rcPaint.top,
			pPs->rcPaint.right - pPs->rcPaint.left,
			pPs->rcPaint.bottom - pPs->rcPaint.top,
			gr,
			pPs->rcPaint.left,
			pPs->rcPaint.top,
			SRCCOPY
		);
	}

	// From Here 2007.09.09 Moca 互換BMPによる画面バッファ
	//     アンダーライン描画をメモリDCからのコピー前処理から後に移動
	if ( m_pcEditWnd->m_nActivePaneIndex == m_nMyIndex ){
		/* アクティブペインは、アンダーライン描画 */
		GetCaret().m_cUnderLine.CaretUnderLineON( true, false );
	}
	// To Here 2007.09.09 Moca

	/* 03/02/18 対括弧の強調表示(描画) ai */
	DrawBracketPair( true );

	/* キャレットを現在位置に表示します */
	if( bCaretShowFlag_Old )	// 2008.06.09 ryoji
		GetCaret().ShowCaret_( this->GetHwnd() ); // 2002/07/22 novice
	return;
}

/*!
	行のテキスト／選択状態の描画
	1回で1ロジック行分を作画する。

	@return EOFを作画したらtrue

	@date 2001.02.17 MIK
	@date 2001.12.21 YAZAKI 改行記号の描きかたを変更
	@date 2007.08.31 kobake 引数 bDispBkBitmap を削除
*/
bool CEditView::DrawLogicLine(
	HDC				_hdc,			//!< [in]     作画対象
	DispPos*		_pDispPos,		//!< [in/out] 描画する箇所、描画元ソース
	CLayoutInt		nLineTo			//!< [in]     作画終了するレイアウト行番号
)
{
//	MY_RUNNINGTIMER( cRunningTimer, "CEditView::DrawLogicLine" );
	bool bDispEOF = false;
	SColorStrategyInfo _sInfo;
	SColorStrategyInfo* pInfo = &_sInfo;
	pInfo->gr.Init(_hdc);
	pInfo->pDispPos = _pDispPos;
	pInfo->pcView = this;

//	if( !pcLayout ){
//		return true;
//	}

	//CColorStrategyPool初期化
	CColorStrategyPool* pool = CColorStrategyPool::getInstance();
	pool->SetCurrentView(this);
	pool->NotifyOnStartScanLogic();

	//DispPosを保存しておく
	pInfo->sDispPosBegin = *pInfo->pDispPos;

	//	Aug. 14, 2005 genta 折り返し幅をLayoutMgrから取得するように
	const CLayoutInt nWrapKeta = m_pcEditDoc->m_cLayoutMgr.GetMaxLineKetas();

	//処理する文字位置
	pInfo->nPosInLogic = CLogicInt(0); //☆開始

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//          論理行データの取得 -> pLine, pLineLen              //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	// 前行の最終設定色
	{
		const CLayout* pcLayout = pInfo->pDispPos->GetLayoutRef();
		ColorStrategyState colorStrategyState;
		EColorIndexType eType = GetColorIndex(pcLayout, 0, colorStrategyState, true, &pInfo->pStrategy, &pInfo->pStrategyFound);
		SetCurrentColor(pInfo->gr, eType);
		pInfo->colorStrategyState = colorStrategyState;
	}

	//開始ロジック位置を算出
	{
		const CLayout* pcLayout = pInfo->pDispPos->GetLayoutRef();
		pInfo->nPosInLogic = pcLayout?pcLayout->GetLogicOffset():CLogicInt(0);
	}

	//サポート
	CTypeSupport cTextType(this,COLORIDX_TEXT);

	for (;;) {
		//対象行が描画範囲外だったら終了
		if( GetTextArea().GetBottomLine() < pInfo->pDispPos->GetLayoutLineRef() ){
			pInfo->pDispPos->SetLayoutLineRef(nLineTo + CLayoutInt(1));
			break;
		}
		if( nLineTo < pInfo->pDispPos->GetLayoutLineRef() ){
			break;
		}

		//レイアウト行を1行描画
		bDispEOF = DrawLayoutLine(pInfo);

		//行を進める
		CLogicInt nOldLogicLineNo = pInfo->pDispPos->GetLayoutRef()->GetLogicLineNo();
		pInfo->pDispPos->ForwardDrawLine(1);		//描画Y座標＋＋
		pInfo->pDispPos->ForwardLayoutLineRef(1);	//レイアウト行＋＋

		// ロジック行を描画し終わったら抜ける
		if(pInfo->pDispPos->GetLayoutRef()->GetLogicLineNo()!=nOldLogicLineNo){
			break;
		}

		// nLineToを超えたら抜ける
		if(pInfo->pDispPos->GetLayoutLineRef() >= nLineTo + CLayoutInt(1)){
			break;
		}
	}

	return bDispEOF;
}

/*!
	レイアウト行を1行描画
*/
//改行記号を描画した場合はtrueを返す？
bool CEditView::DrawLayoutLine(SColorStrategyInfo* pInfo)
{
	bool bDispEOF = false;
	CTypeSupport cTextType(this,COLORIDX_TEXT);

	const CLayout* pcLayout = pInfo->pDispPos->GetLayoutRef(); //m_pcEditDoc->m_cLayoutMgr.SearchLineByLayoutY( pInfo->pDispPos->GetLayoutLineRef() );

	// レイアウト情報
	if( pcLayout ){
		pInfo->pLineOfLogic					= pcLayout->GetDocLineRef()->GetPtr();
	}
	else{
		pInfo->pLineOfLogic					= NULL;
	}

	//文字列参照
	const CDocLine* pcDocLine = pInfo->GetDocLine();
	CStringRef cLineStr = pcDocLine->GetStringRefWithEOL();

	// レイアウト先頭行の場合には、 color を初期化する.
	if(pcLayout && pcLayout->GetLogicOffset()==0){			/* */
		CColorStrategyPool* pool = CColorStrategyPool::getInstance();
		pInfo->pStrategy = pool->GetStrategyByColor(pcLayout->GetColorTypePrev());
		if(pInfo->pStrategy)pInfo->pStrategy->InitStrategyStatus();
		SetCurrentColor(pInfo->gr, pcLayout->GetColorTypePrev());
	}

	// 描画範囲外の場合は色切替だけで抜ける
	if(pInfo->pDispPos->GetDrawPos().y < GetTextArea().GetAreaTop()){
		if(pcLayout){
			bool bChange = false;
			CColor3Setting cColor;
			while(pInfo->nPosInLogic < pcLayout->GetLogicOffset() + pcLayout->GetLengthWithEOL()){
				//色切替
				bChange |= pInfo->DoChangeColor(cLineStr, &cColor, pInfo->colorStrategyState);

				//1文字進む
				pInfo->nPosInLogic += CNativeW::GetSizeOfChar(
											cLineStr.GetPtr(),
											cLineStr.GetLength(),
											pInfo->nPosInLogic
										);
			}
			if( bChange ){
				SetCurrentColor3(pInfo->gr, cColor.eColorIndex, cColor.eColorIndex2, cColor.eColorIndexBg);
			}
		}
		return false;
	}

	// コンフィグ
	int nLineHeight = GetTextMetrics().GetHankakuDy();  //行の縦幅？
	CTypeSupport	cCaretLineBg(this, COLORIDX_CARETLINEBG);
	CTypeSupport&	cBackType = (cCaretLineBg.IsDisp() &&
		GetCaret().GetCaretLayoutPos().GetY() == pInfo->pDispPos->GetLayoutLineRef() ?  cCaretLineBg : cTextType);
	bool bTransText = IsBkBitmap();
	if( bTransText ){
		bTransText = cBackType.GetBackColor() == cTextType.GetBackColor();
	}

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                        行番号描画                           //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	GetTextDrawer().DispLineNumber(
		pInfo->gr,
		pInfo->pDispPos->GetLayoutLineRef(),
		pInfo->pDispPos->GetDrawPos().y
	);


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                       本文描画開始                          //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	pInfo->pDispPos->ResetDrawCol();


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                 行頭(インデント)背景描画                    //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	if(pcLayout && pcLayout->GetIndent()!=0)
	{
		RECT rcClip;
		if(!bTransText && GetTextArea().GenerateClipRect(&rcClip,*pInfo->pDispPos,(Int)pcLayout->GetIndent())){
			cBackType.FillBack(pInfo->gr,rcClip);
		}
		//描画位置進める
		pInfo->pDispPos->ForwardDrawCol((Int)pcLayout->GetIndent());
	}


	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                         本文描画                            //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//行終端または折り返しに達するまでループ
	if(pcLayout){
		while(pInfo->nPosInLogic < pcLayout->GetLogicOffset() + pcLayout->GetLengthWithEOL()){
			//色切替
			CColor3Setting cColor;
			if( pInfo->DoChangeColor(cLineStr, &cColor, pInfo->colorStrategyState) ){
				SetCurrentColor3(pInfo->gr, cColor.eColorIndex, cColor.eColorIndex2, cColor.eColorIndexBg);
			}

			//1文字情報取得 $$高速化可能
			CFigure& cFigure = CFigureManager::getInstance()->GetFigure(&cLineStr.GetPtr()[pInfo->GetPosInLogic()]);

			//1文字描画
			cFigure.DrawImp(pInfo);
		}
	}

	// 必要ならEOF描画
	void _DispEOF( CGraphics& gr, DispPos* pDispPos, const CEditView* pcView);
	if(pcLayout && pcLayout->GetNextLayout()==NULL && pcLayout->GetLayoutEol().GetLen()==0){
		// 有文字行のEOF
		_DispEOF(pInfo->gr,pInfo->pDispPos,this);
		bDispEOF = true;
	}
	else if(!pcLayout && pInfo->pDispPos->GetLayoutLineRef()==m_pcEditDoc->m_cLayoutMgr.GetLineCount()){
		// 空行のEOF
		CLayout* pBottom = m_pcEditDoc->m_cLayoutMgr.GetBottomLayout();
		if(pBottom==NULL || (pBottom && pBottom->GetLayoutEol().GetLen())){
			_DispEOF(pInfo->gr,pInfo->pDispPos,this);
			bDispEOF = true;
		}
	}

	// 必要なら折り返し記号描画
	if(pcLayout && pcLayout->GetLayoutEol().GetLen()==0 && pcLayout->GetNextLayout()!=NULL){
		_DispWrap(pInfo->gr,pInfo->pDispPos,this);
	}

	// 行末背景描画
	RECT rcClip;
	bool rcClipRet = GetTextArea().GenerateClipRectRight(&rcClip,*pInfo->pDispPos);
	if(rcClipRet){
		if( !bTransText ){
			cBackType.FillBack(pInfo->gr,rcClip);
		}
		CTypeSupport cSelectType(this, COLORIDX_SELECT);
		if( GetSelectionInfo().IsTextSelected() && cSelectType.IsDisp() ){
			// 選択範囲の指定色：必要ならテキストのない部分の矩形選択を作画
			CLayoutRange selectArea = GetSelectionInfo().GetSelectAreaLine(pInfo->pDispPos->GetLayoutLineRef(), pcLayout);
			// 2010.10.04 スクロール分の足し忘れ
			int nSelectFromPx = GetTextMetrics().GetHankakuDx() * (Int)(selectArea.GetFrom().x - GetTextArea().GetViewLeftCol());
			int nSelectToPx   = GetTextMetrics().GetHankakuDx() * (Int)(selectArea.GetTo().x - GetTextArea().GetViewLeftCol());
			if( nSelectFromPx < nSelectToPx && selectArea.GetTo().x != INT_MAX ){
				RECT rcSelect; // Pixel
				rcSelect.top    = pInfo->pDispPos->GetDrawPos().y;
				rcSelect.bottom = pInfo->pDispPos->GetDrawPos().y + GetTextMetrics().GetHankakuDy();
				rcSelect.left   = GetTextArea().GetAreaLeft() + nSelectFromPx;
				rcSelect.right  = GetTextArea().GetAreaLeft() + nSelectToPx;
				RECT rcDraw;
				if( ::IntersectRect(&rcDraw, &rcClip, &rcSelect) ){
					COLORREF color = GetBackColorByColorInfo2(cSelectType.GetColorInfo(), cBackType.GetColorInfo());
					if( color != cBackType.GetBackColor() ){
						pInfo->gr.FillSolidMyRect(rcDraw, color);
					}
				}
			}
		}
	}

	// 指定桁縦線描画
	GetTextDrawer().DispVerticalLines(
		pInfo->gr,
		pInfo->pDispPos->GetDrawPos().y,
		pInfo->pDispPos->GetDrawPos().y + nLineHeight,
		CLayoutInt(0),
		CLayoutInt(-1)
	);

	// 折り返し桁縦線描画
	GetTextDrawer().DispWrapLine(
		pInfo->gr,
		pInfo->pDispPos->GetDrawPos().y,
		pInfo->pDispPos->GetDrawPos().y + nLineHeight
	);

	if(pcLayout){
		DispZeroWidthSelected(
			pInfo->gr,
			pInfo->pDispPos->GetLayoutLineRef(),
			CMyPoint(pInfo->sDispPosBegin.GetDrawPos().x, pInfo->pDispPos->GetDrawPos().y)
		);
	}

	// 反転描画
	if( pcLayout && GetSelectionInfo().IsTextSelected() ){
		DispTextSelected(
			pInfo->gr,
			pInfo->pDispPos->GetLayoutLineRef(),
			CMyPoint(pInfo->sDispPosBegin.GetDrawPos().x, pInfo->pDispPos->GetDrawPos().y),
			pcLayout->CalcLayoutWidth(CEditDoc::GetInstance(0)->m_cLayoutMgr) + CLayoutInt(pcLayout->GetLayoutEol().GetLen()?1:0)
		);
	}

	return bDispEOF;
}








/*!
	ゼロ幅矩形選択の直線表示
 */
void CEditView::DispZeroWidthSelected(
	CGraphics& gr,			//!< 作画するウィンドウのDC
	CLayoutInt nLineNum,	//!< 処理対象レイアウト行番号(0開始)
	const CMyPoint&	ptXY	//!< 相対レイアウト0桁目の左端座標, 対象行の上端座標
)
{
	if(GetSelectionInfo().IsBoxSelecting() == false){
		return;
	}

	CLayoutRange& sSelect = GetSelectionInfo().m_sSelect;
	// 対象行が選択範囲外の場合はリターン
	if(nLineNum < sSelect.GetFrom().y || nLineNum > sSelect.GetTo().y){
		return;
	}
	// 選択範囲がゼロ幅の可能性がない場合にはリターン
	if(sSelect.GetFrom().x != sSelect.GetTo().x){
		return;
	}

	const CLayout* pcLayout = m_pcEditDoc->m_cLayoutMgr.SearchLineByLayoutY(nLineNum);
	CLayoutRange selectArea = GetSelectionInfo().GetSelectAreaLine(nLineNum, pcLayout);
	CLayoutInt nSelectFrom = selectArea.GetFrom().x;
	CLayoutInt nSelectTo = selectArea.GetTo().x;

	// 選択範囲がゼロ幅ではない場合にはリターン
	if(nSelectFrom != nSelectTo){
		return;
	}
	// 選択範囲の X 位置が画面外の場合にはリターン
	if(nSelectFrom > GetTextArea().GetRightCol()
	|| nSelectFrom < GetTextArea().GetViewLeftCol()){
		return;
	}

	// 描画
	int nCharWidth = GetTextMetrics().GetHankakuDx();
	int nLineHeight = GetTextMetrics().GetHankakuDy();
	RECT rcClip;

	rcClip.left = ptXY.x + (Int)nSelectFrom * nCharWidth;
	rcClip.right = rcClip.left + t_max(nCharWidth / 6, 1);
	rcClip.top = ptXY.y;
	rcClip.bottom = ptXY.y + nLineHeight;

#if 0
	// αブレンドあり
	CTypeSupport cTextType(this, COLORIDX_TEXT);
	CTypeSupport cSelectType(this, COLORIDX_SELECT);
	COLORREF crSelect = GetBackColorByColorInfo2(cSelectType.GetColorInfo(), cTextType.GetColorInfo());
#else
	// αブレンドなし
	CTypeSupport cSelectType(this, COLORIDX_SELECT);
	COLORREF crSelect = cSelectType.GetBackColor();
#endif

	HBRUSH hbrSelect = ::CreateSolidBrush(crSelect);
	::FillRect(gr, &rcClip, hbrSelect);
}


/* テキスト反転

	@param hdc      
	@param nLineNum 
	@param x        
	@param y        
	@param nX       

	@note
	CCEditView::DrawLogicLine() での作画(WM_PAINT)時に、1レイアウト行をまとめて反転処理するための関数。
	範囲選択の随時更新は、CEditView::DrawSelectArea() が選択・反転解除を行う。
	
*/
void CEditView::DispTextSelected(
	HDC				hdc,		//!< 作画対象ビットマップを含むデバイス
	CLayoutInt		nLineNum,	//!< 反転処理対象レイアウト行番号(0開始)
	const CMyPoint&	ptXY,		//!< (相対レイアウト0桁目の左端座標, 対象行の上端座標)
	CLayoutInt		nX_Layout	//!< 対象行の終了桁位置。　[ABC\n]なら改行の後ろで4
)
{
	CLayoutInt	nSelectFrom;
	CLayoutInt	nSelectTo;
	RECT		rcClip;
	int			nLineHeight = GetTextMetrics().GetHankakuDy();
	int			nCharWidth = GetTextMetrics().GetHankakuDx();
	HRGN		hrgnDraw;
	const CLayout* pcLayout = m_pcEditDoc->m_cLayoutMgr.SearchLineByLayoutY( nLineNum );
	CLayoutRange& sSelect = GetSelectionInfo().m_sSelect;

	/* 選択範囲内の行かな */
//	if( IsTextSelected() ){
		if( nLineNum >= sSelect.GetFrom().y && nLineNum <= sSelect.GetTo().y ){
			CLayoutRange selectArea = GetSelectionInfo().GetSelectAreaLine(nLineNum, pcLayout);
			nSelectFrom = selectArea.GetFrom().x;
			nSelectTo   = selectArea.GetTo().x;
			if( nSelectFrom == INT_MAX ){
				nSelectFrom = nX_Layout;
			}
			if( nSelectTo == INT_MAX ){
				nSelectTo = nX_Layout;
			}

			// 2006.03.28 Moca 表示域外なら何もしない
			if( GetTextArea().GetRightCol() < nSelectFrom ){
				return;
			}
			if( nSelectTo < GetTextArea().GetViewLeftCol() ){	// nSelectTo == GetTextArea().GetViewLeftCol()のケースは後で０文字マッチでないことを確認してから抜ける
				return;
			}

			if( nSelectFrom < GetTextArea().GetViewLeftCol() ){
				nSelectFrom = GetTextArea().GetViewLeftCol();
			}
			rcClip.left   = ptXY.x + (Int)nSelectFrom * nCharWidth;
			rcClip.right  = ptXY.x + (Int)nSelectTo   * nCharWidth;
			rcClip.top    = ptXY.y;
			rcClip.bottom = ptXY.y + nLineHeight;

			bool bOMatch = false;

			// 2005/04/02 かろと ０文字マッチだと反転幅が０となり反転されないので、1/3文字幅だけ反転させる
			// 2005/06/26 zenryaku 選択解除でキャレットの残骸が残る問題を修正
			// 2005/09/29 ryoji スクロール時にキャレットのようなゴミが表示される問題を修正
			if (GetSelectionInfo().IsTextSelected() && rcClip.right == rcClip.left &&
				sSelect.IsLineOne() &&
				sSelect.GetFrom().x >= GetTextArea().GetViewLeftCol())
			{
				HWND hWnd = ::GetForegroundWindow();
				if( hWnd && (hWnd == m_pcEditWnd->m_cDlgFind.GetHwnd() || hWnd == m_pcEditWnd->m_cDlgReplace.GetHwnd()) ){
					rcClip.right = rcClip.left + (nCharWidth/3 == 0 ? 1 : nCharWidth/3);
					bOMatch = true;
				}
			}
			if( rcClip.right == rcClip.left ){
				return;	//０文字マッチによる反転幅拡張なし
			}

			// 2006.03.28 Moca ウィンドウ幅が大きいと正しく反転しない問題を修正
			if( rcClip.right > GetTextArea().GetAreaRight() ){
				rcClip.right = GetTextArea().GetAreaRight();
			}
			
			// 選択色表示なら反転しない
			if( !bOMatch && CTypeSupport(this, COLORIDX_SELECT).IsDisp() ){
				return;
			}
			
			HBRUSH hBrush    = ::CreateSolidBrush( SELECTEDAREA_RGB );

			int    nROP_Old  = ::SetROP2( hdc, SELECTEDAREA_ROP2 );
			HBRUSH hBrushOld = (HBRUSH)::SelectObject( hdc, hBrush );
			hrgnDraw = ::CreateRectRgn( rcClip.left, rcClip.top, rcClip.right, rcClip.bottom );
			::PaintRgn( hdc, hrgnDraw );
			::DeleteObject( hrgnDraw );

			SetROP2( hdc, nROP_Old );
			SelectObject( hdc, hBrushOld );
			DeleteObject( hBrush );
		}
//	}
	return;
}







// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       画面バッファ                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


/*!
	画面の互換ビットマップを作成または更新する。
		必要の無いときは何もしない。
	
	@param cx ウィンドウの高さ
	@param cy ウィンドウの幅
	@return true: ビットマップを利用可能 / false: ビットマップの作成・更新に失敗

	@date 2007.09.09 Moca CEditView::OnSizeから分離。
		単純に生成するだけだったものを、仕様変更に従い内容コピーを追加。
		サイズが同じときは何もしないように変更

	@par 互換BMPにはキャレット・カーソル位置横縦線・対括弧以外の情報を全て書き込む。
		選択範囲変更時の反転処理は、画面と互換BMPの両方を別々に変更する。
		カーソル位置横縦線変更時には、互換BMPから画面に元の情報を復帰させている。

*/
bool CEditView::CreateOrUpdateCompatibleBitmap( int cx, int cy )
{
	if( NULL == m_hdcCompatDC ){
		return false;
	}
	// サイズを64の倍数で整列
	int nBmpWidthNew  = ((cx + 63) & (0x7fffffff - 63));
	int nBmpHeightNew = ((cy + 63) & (0x7fffffff - 63));
	if( nBmpWidthNew != m_nCompatBMPWidth || nBmpHeightNew != m_nCompatBMPHeight ){
#if 0
	MYTRACE( _T("CEditView::CreateOrUpdateCompatibleBitmap( %d, %d ): resized\n"), cx, cy );
#endif
		HDC	hdc = ::GetDC( GetHwnd() );
		HBITMAP hBitmapNew = NULL;
		if( m_hbmpCompatBMP ){
			// BMPの更新
			HDC hdcTemp = ::CreateCompatibleDC( hdc );
			hBitmapNew = ::CreateCompatibleBitmap( hdc, nBmpWidthNew, nBmpHeightNew );
			if( hBitmapNew ){
				HBITMAP hBitmapOld = (HBITMAP)::SelectObject( hdcTemp, hBitmapNew );
				// 前の画面内容をコピーする
				::BitBlt( hdcTemp, 0, 0,
					t_min( nBmpWidthNew,m_nCompatBMPWidth ),
					t_min( nBmpHeightNew, m_nCompatBMPHeight ),
					m_hdcCompatDC, 0, 0, SRCCOPY );
				::SelectObject( hdcTemp, hBitmapOld );
				::SelectObject( m_hdcCompatDC, m_hbmpCompatBMPOld );
				::DeleteObject( m_hbmpCompatBMP );
			}
			::DeleteDC( hdcTemp );
		}else{
			// BMPの新規作成
			hBitmapNew = ::CreateCompatibleBitmap( hdc, nBmpWidthNew, nBmpHeightNew );
		}
		if( hBitmapNew ){
			m_hbmpCompatBMP = hBitmapNew;
			m_nCompatBMPWidth = nBmpWidthNew;
			m_nCompatBMPHeight = nBmpHeightNew;
			m_hbmpCompatBMPOld = (HBITMAP)::SelectObject( m_hdcCompatDC, m_hbmpCompatBMP );
		}else{
			// 互換BMPの作成に失敗
			// 今後も失敗を繰り返す可能性が高いので
			// m_hdcCompatDCをNULLにすることで画面バッファ機能をこのウィンドウのみ無効にする。
			//	2007.09.29 genta 関数化．既存のBMPも解放
			UseCompatibleDC(FALSE);
		}
		::ReleaseDC( GetHwnd(), hdc );
	}
	return NULL != m_hbmpCompatBMP;
}


/*!
	互換メモリBMPを削除

	@note 分割ビューが非表示になった場合と
		親ウィンドウが非表示・最小化された場合に削除される。
	@date 2007.09.09 Moca 新規作成 
*/
void CEditView::DeleteCompatibleBitmap()
{
	if( m_hbmpCompatBMP ){
		::SelectObject( m_hdcCompatDC, m_hbmpCompatBMPOld );
		::DeleteObject( m_hbmpCompatBMP );
		m_hbmpCompatBMP = NULL;
		m_hbmpCompatBMPOld = NULL;
		m_nCompatBMPWidth = -1;
		m_nCompatBMPHeight = -1;
	}
}



/** 画面キャッシュ用CompatibleDCを用意する

	@param[in] TRUE: 画面キャッシュON

	@date 2007.09.30 genta 関数化
*/
void CEditView::UseCompatibleDC(BOOL fCache)
{
	// From Here 2007.09.09 Moca 互換BMPによる画面バッファ
	if( fCache ){
		if( m_hdcCompatDC == NULL ){
			HDC			hdc;
			hdc = ::GetDC( GetHwnd() );
			m_hdcCompatDC = ::CreateCompatibleDC( hdc );
			::ReleaseDC( GetHwnd(), hdc );
			DEBUG_TRACE(_T("CEditView::UseCompatibleDC: Created\n"), fCache);
		}
		else {
			DEBUG_TRACE(_T("CEditView::UseCompatibleDC: Reused\n"), fCache);
		}
	}
	else {
		//	CompatibleBitmapが残っているかもしれないので最初に削除
		DeleteCompatibleBitmap();
		if( m_hdcCompatDC != NULL ){
			::DeleteDC( m_hdcCompatDC );
			DEBUG_TRACE(_T("CEditView::UseCompatibleDC: Deleted.\n"));
			m_hdcCompatDC = NULL;
		}
	}
}
