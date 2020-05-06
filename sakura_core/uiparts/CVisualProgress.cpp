#include "StdAfx.h"
#include "CVisualProgress.h"
#include "CWaitCursor.h"

#include "window/CEditWnd.h"


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CVisualProgress::CVisualProgress()
: m_pcWaitCursor(NULL)
{
}

CVisualProgress::~CVisualProgress()
{
	SAFE_DELETE(m_pcWaitCursor);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        ロード前後                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CVisualProgress::OnBeforeLoad(SLoadInfo* sLoadInfo)
{
	_Begin();
}

void CVisualProgress::OnAfterLoad(const SLoadInfo& sLoadInfo)
{
	_End();
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        セーブ前後                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CVisualProgress::OnBeforeSave(const SSaveInfo& sSaveInfo)
{
	_Begin();
}

void CVisualProgress::OnFinalSave(ESaveResult eSaveResult)
{
	_End();
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      プログレス受信                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CVisualProgress::OnProgress(int nPer)
{
	_Doing(nPer);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         実装補助                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CVisualProgress::_Begin()
{
	//砂時計
	if(!m_pcWaitCursor){
		m_pcWaitCursor = new CWaitCursor( CEditWnd::getInstance()->GetHwnd() );
	}

	//プログレスバー
	HWND hwndProgress = CEditWnd::getInstance()->m_cStatusBar.GetProgressHwnd();
	if( hwndProgress ){
		::ShowWindow( hwndProgress, SW_SHOW );
		//範囲設定・リセット
		Progress_SetRange( hwndProgress, 0, 100 );
		Progress_SetPos( hwndProgress, 0);
	}
}

void CVisualProgress::_Doing(int nPer)
{
	//プログレスバー更新
	HWND hwndProgress = CEditWnd::getInstance()->m_cStatusBar.GetProgressHwnd();
	if(hwndProgress)
		Progress_SetPos( hwndProgress, nPer );
}

void CVisualProgress::_End()
{
	//プログレスバー
	HWND hwndProgress = CEditWnd::getInstance()->m_cStatusBar.GetProgressHwnd();
	if( hwndProgress ){
		Progress_SetPos( hwndProgress, 0);
		::ShowWindow( hwndProgress, SW_HIDE );
	}

	//砂時計
	SAFE_DELETE(m_pcWaitCursor);
}

