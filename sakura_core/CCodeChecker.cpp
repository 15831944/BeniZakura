#include "StdAfx.h"
#include "CCodeChecker.h"
#include "io/CIoBridge.h"
#include "charset/CCodeFactory.h" ////

#include "doc/CEditDoc.h"
#include "doc/logic/CDocLineMgr.h"
#include "window/CEditWnd.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     セーブ時チェック                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! CDocLineMgrが保持するデータに異なる改行コードが混在しているかどうか判定する
static bool _CheckSavingEolcode(const CDocLineMgr& pcDocLineMgr, CEol cEolType)
{
	bool bMix = false;
	if( cEolType == EOL_NONE ){	//改行コード変換なし
		CEol cEolCheck;	//比較対象EOL
		CDocLine* pcDocLine = pcDocLineMgr.GetDocLineTop();
		if( pcDocLine ){
			cEolCheck = pcDocLine->GetEol();
		}
		while( pcDocLine ){
			CEol cEol = pcDocLine->GetEol();
			if( cEol != cEolCheck && cEol != EOL_NONE ){
				bMix = true;
				break;
			}
			pcDocLine = pcDocLine->GetNextLine();
		}
	}
	return bMix;
}

//! CDocLineMgrが保持するデータを指定文字コードで安全に保存できるかどうか判定する
static EConvertResult _CheckSavingCharcode(const CDocLineMgr& pcDocLineMgr, ECodeType eCodeType)
{
	CDocLine*	pcDocLine = pcDocLineMgr.GetDocLineTop();
	CCodeBase* pCodeBase=CCodeFactory::CreateCodeBase(eCodeType,0);
	while( pcDocLine ){
		// コード変換 pcDocLine -> cmemTmp
		CMemory cmemTmp;
		EConvertResult e = CIoBridge::ImplToFile(
			pcDocLine->_GetDocLineDataWithEOL(),
			&cmemTmp,
			pCodeBase
		);
		if(e!=RESULT_COMPLETE){
			delete pCodeBase;
			return e;
		}

		//次の行へ
		pcDocLine = pcDocLine->GetNextLine();
	}
	delete pCodeBase;
	return RESULT_COMPLETE;
}


ECallbackResult CCodeChecker::OnCheckSave(SSaveInfo* pSaveInfo)
{
	CEditDoc* pcDoc = GetListeningDoc();

	//改行コードが混在しているかどうか判定
	bool bTmpResult = false;
	if( pcDoc->m_cDocType.GetDocumentType()->m_bChkEnterAtEnd ){
		bTmpResult = _CheckSavingEolcode(
			pcDoc->m_cDocLineMgr, pSaveInfo->cEol
		);
	}

	//ユーザ問い合わせ
	if (bTmpResult) {
		int nDlgResult = MYMESSAGEBOX(
			CEditWnd::getInstance()->GetHwnd(),
			MB_YESNOCANCEL | MB_ICONWARNING,
			GSTR_APPNAME,
			_T("改行コードが混在しています。\r\n")
			_T("現在の入力改行コード %ts に統一しますか？"),
			pcDoc->m_cDocEditor.GetNewLineCode().GetName()
		);
		switch(nDlgResult){
		case IDYES:		pSaveInfo->cEol = pcDoc->m_cDocEditor.GetNewLineCode(); break; //統一
		case IDNO:		break; //続行
		case IDCANCEL:	return CALLBACK_INTERRUPT; //中断
		}
	}

	//指定文字コードで安全に保存できるかどうか判定
	EConvertResult nTmpResult = _CheckSavingCharcode(
		pcDoc->m_cDocLineMgr, pSaveInfo->eCharCode
	);

	//ユーザ問い合わせ
	if(nTmpResult==RESULT_LOSESOME){
		int nDlgResult = MYMESSAGEBOX(
			CEditWnd::getInstance()->GetHwnd(),
			MB_YESNO | MB_ICONWARNING,
			GSTR_APPNAME,
			_T("文字エンコード %ts で保存しようとしていますが、\r\n")
			_T("文字コード変換により一部の文字情報が失われます。\r\n")
			_T("保存処理を続行しますか？"),
			CCodeTypeName(pSaveInfo->eCharCode).Normal()
		);
		switch(nDlgResult){
		case IDYES:		break; //続行
		case IDNO:		return CALLBACK_INTERRUPT; //中断
		case IDCANCEL:	return CALLBACK_INTERRUPT; //中断
		}
	}
	return CALLBACK_CONTINUE;
}

void CCodeChecker::OnFinalSave(ESaveResult eSaveResult)
{
	//カキコ結果
	if(eSaveResult==SAVED_LOSESOME){
		ErrorMessage(CEditWnd::getInstance()->GetHwnd(), _T("一部の文字情報が、セーブ時の変換により失われました"));
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     ロード時チェック                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CCodeChecker::OnFinalLoad(ELoadResult eLoadResult)
{
	if(eLoadResult==LOADED_LOSESOME){
		ErrorMessage(
			CEditWnd::getInstance()->GetHwnd(),
			_T("一部の文字情報が、ロード時の変換により失われました")
		);
	}
}
