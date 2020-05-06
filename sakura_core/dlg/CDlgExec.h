/*!	@file
	@brief 外部コマンド実行ダイアログ

	@author Norio Nakatani
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2002, YAZAKI
	Copyright (C) 2009, ryoji

	This source code is designed for sakura editor.
	Please contact the copyright holder to use this code for other purpose.
*/
#include "dlg/CDialog.h"

#ifndef _CDLGEXEC_H_
#define _CDLGEXEC_H_

/*-----------------------------------------------------------------------
クラスの宣言
-----------------------------------------------------------------------*/
class CDlgExec : public CDialog
{
public:
	/*
	||  Constructors
	*/
	CDlgExec();
	/*
	||  Attributes & Operations
	*/
	int DoModal( HINSTANCE, HWND, LPARAM );	/* モーダルダイアログの表示 */

	TCHAR	m_szCommand[1024 + 1];	/* コマンドライン */
	SFilePath	m_szCurDir;	/* カレントディレクトリ */
	bool	m_bEditable;			/* 編集ウィンドウへの入力可能 */	// 2009.02.21 ryoji


protected:
	/* オーバーライド? */
	virtual BOOL OnInitDialog( HWND, WPARAM wParam, LPARAM lParam );
	int GetData( void );	/* ダイアログデータの取得 */
	void SetData( void );	/* ダイアログデータの設定 */
	BOOL OnBnClicked( int );


};



///////////////////////////////////////////////////////////////////////
#endif /* _CDLGEXEC_H_ */


