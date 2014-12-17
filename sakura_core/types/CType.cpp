#include "StdAfx.h"
#include "CType.h"
#include "view/Colors/EColorIndexType.h"
#include "env/CDocTypeManager.h"
#include "env/CShareData.h"
#include "env/DLLSHAREDATA.h"

void _DefaultConfig(STypeConfig* pType);

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        CTypeConfig                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
STypeConfig* CTypeConfig::GetTypeConfig()
{
	return &CDocTypeManager().GetTypeSetting(*this);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          CType                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CType::InitTypeConfig(int nIdx)
{
	//�K��l���R�s�[
	static STypeConfig sDefault;
	static bool bLoadedDefault = false;
	if(!bLoadedDefault){
		_DefaultConfig(&sDefault);
		bLoadedDefault=true;
	}
	CDocTypeManager().GetTypeSetting(CTypeConfig(nIdx)) = sDefault;

	//�C���f�b�N�X��ݒ�
	CTypeConfig(nIdx)->m_nIdx = nIdx;

	//�ʐݒ�
	InitTypeConfigImp(CTypeConfig(nIdx).GetTypeConfig());
}

/*!
 *	�L�[���[�h�Z�b�g�̃f�t�H���g�l��ǉ�����
 *
 *	@return CKeyWordSetMgr �ɓo�^�����L�[���[�h�Z�b�g�ԍ�
 */
int CType::AddDefaultKeywordSet(
	const wchar_t*	pSetName,			//!< [in] �Z�b�g��
	bool			bCaseSensitive,		//!< [in] �啶���������̋�ʁDtrue:����, false:����
	int				numOfKeyWords,		//!< [in] �o�^����L�[���[�h��
	const wchar_t*	pKeyWordSet[]		//!< [in] �o�^����L�[���[�h�̔z��
)
{
	DLLSHAREDATA* pShareData = &GetDllShareData();
	CKeyWordSetMgr* pKeyWordSetMgr = &pShareData->m_Common.m_sSpecialKeyword.m_CKeyWordSetMgr;
	int keyWordSetIdx;

	keyWordSetIdx = pKeyWordSetMgr->AddKeyWordSet( pSetName, bCaseSensitive );
	pKeyWordSetMgr->SetKeyWordArr( keyWordSetIdx, numOfKeyWords, pKeyWordSet );

	return keyWordSetIdx;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        �f�t�H���g                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void _DefaultConfig(STypeConfig* pType)
{
//�L�[���[�h�F�f�t�H���g�J���[�ݒ�
/************************/
/* �^�C�v�ʐݒ�̋K��l */
/************************/

	pType->m_nTextWrapMethod = WRAP_SETTING_WIDTH;	// �e�L�X�g�̐܂�Ԃ����@		// 2008.05.30 nasukoji
	pType->m_nMaxLineKetas = CLayoutInt(MAXLINEKETAS);	/* �܂�Ԃ����� */
	pType->m_nColumnSpace = 0;					/* �����ƕ����̌��� */
	pType->m_nLineSpace = 0;					/* �s�Ԃ̂����� */
	pType->m_nTabSpace = CLayoutInt(4);					/* TAB�̕����� */
	for( int i = 0; i < MAX_KEYWORDSET_PER_TYPE; i++ ){
		pType->m_nKeyWordSetIdx[i] = -1;
	}
	wcscpy( pType->m_szTabViewString, _EDITL("o.......") );	/* TAB�\�������� */
	pType->m_bTabArrow = false;				/* �^�u���\�� */	// 2001.12.03 hor
	pType->m_bInsSpace = FALSE;				/* �X�y�[�X�̑}�� */	// 2001.12.03 hor
	
	//@@@ 2002.09.22 YAZAKI �ȉ��Am_cLineComment��m_cBlockComments���g���悤�ɏC��
	pType->m_cLineComment.CopyTo(0, L"", -1);	/* �s�R�����g�f���~�^ */
	pType->m_cLineComment.CopyTo(1, L"", -1);	/* �s�R�����g�f���~�^2 */
	pType->m_cLineComment.CopyTo(2, L"", -1);	/* �s�R�����g�f���~�^3 */	//Jun. 01, 2001 JEPRO �ǉ�
	pType->m_cBlockComments[0].SetBlockCommentRule(L"", L"");	/* �u���b�N�R�����g�f���~�^ */
	pType->m_cBlockComments[1].SetBlockCommentRule(L"", L"");	/* �u���b�N�R�����g�f���~�^2 */

	pType->m_nStringType = 0;					/* �������؂�L���G�X�P�[�v���@ 0=[\"][\'] 1=[""][''] */
	wcscpy( pType->m_szIndentChars, L"" );		/* ���̑��̃C���f���g�Ώە��� */

	pType->m_nColorInfoArrNum = COLORIDX_LAST;

	// 2001/06/14 Start by asa-o
	_tcscpy( pType->m_szHokanFile, _T("") );		/* ���͕⊮ �P��t�@�C�� */
	// 2001/06/14 End

	pType->m_nHokanType = 0;

	// 2001/06/19 asa-o
	pType->m_bHokanLoHiCase = false;			// ���͕⊮�@�\�F�p�啶���������𓯈ꎋ����

	//	2003.06.23 Moca �t�@�C��������̓��͕⊮�@�\
	pType->m_bUseHokanByFile = true;			//! ���͕⊮ �J���Ă���t�@�C�����������T��
	pType->m_bUseHokanByKeyword = true;			// �����L�[���[�h������͕⊮

	// �����R�[�h�ݒ�
	pType->m_encoding.m_bPriorCesu8 = false;
	pType->m_encoding.m_eDefaultCodetype = CODE_SJIS;
	pType->m_encoding.m_eDefaultEoltype = EOL_CRLF;
	pType->m_encoding.m_bDefaultBom = false;

	//@@@2002.2.4 YAZAKI
	pType->m_szExtHelp[0] = L'\0';
	pType->m_szExtHtmlHelp[0] = L'\0';
	pType->m_bHtmlHelpIsSingle = true;

	pType->m_bAutoIndent = true;			/* �I�[�g�C���f���g */
	pType->m_bAutoIndent_ZENSPACE = true;	/* ���{��󔒂��C���f���g */
	pType->m_bRTrimPrevLine = false;		// 2005.10.11 ryoji ���s���ɖ����̋󔒂��폜

	pType->m_nIndentLayout = 0;	/* �܂�Ԃ���2�s�ڈȍ~���������\�� */

	GetDefaultColorInfo( pType->m_ColorInfoArr, COLOR_THEME_BENIZAKURA );

	pType->m_szBackImgPath[0] = '\0';
	pType->m_backImgPos = BGIMAGE_TOP_LEFT;
	pType->m_backImgRepeatX = true;
	pType->m_backImgRepeatY = true;
	pType->m_backImgScrollX = true;
	pType->m_backImgScrollY = true;
	{
		POINT pt ={0,0};
		pType->m_backImgPosOffset = pt;
	}
	pType->m_bLineNumIsCRLF = true;					// �s�ԍ��̕\�� FALSE=�܂�Ԃ��P�ʁ^TRUE=���s�P��
	pType->m_nLineTermType = 1;						// �s�ԍ���؂� 0=�Ȃ� 1=�c�� 2=�C��
	pType->m_cLineTermChar = L':';					// �s�ԍ���؂蕶��
	pType->m_bWordWrap = false;						// �p�����[�h���b�v������
	pType->m_nCurrentPrintSetting = 0;				// ���ݑI�����Ă������ݒ�
	pType->m_bOutlineDockDisp = false;				// �A�E�g���C����͕\���̗L��
	pType->m_eOutlineDockSide = DOCKSIDE_FLOAT;		// �A�E�g���C����̓h�b�L���O�z�u
	pType->m_cxOutlineDockLeft = 0;					// �A�E�g���C���̍��h�b�L���O��
	pType->m_cyOutlineDockTop = 0;					// �A�E�g���C���̏�h�b�L���O��
	pType->m_cxOutlineDockRight = 0;				// �A�E�g���C���̉E�h�b�L���O��
	pType->m_cyOutlineDockBottom = 0;				// �A�E�g���C���̉��h�b�L���O��
	pType->m_eDefaultOutline = OUTLINE_TEXT;		/* �A�E�g���C����͕��@ */
	pType->m_nOutlineSortCol = 0;					/* �A�E�g���C����̓\�[�g��ԍ� */
	pType->m_bOutlineSortDesc = false;				// �A�E�g���C����̓\�[�g�~��
	pType->m_nOutlineSortType = 0;					/* �A�E�g���C����̓\�[�g� */
	pType->m_eSmartIndent = SMARTINDENT_NONE;		/* �X�}�[�g�C���f���g��� */
	pType->m_nImeState = IME_CMODE_NOCONVERSION;	/* IME���� */

	pType->m_szOutlineRuleFilename[0] = L'\0';		//Dec. 4, 2000 MIK
	pType->m_bKinsokuHead = false;					// �s���֑�				//@@@ 2002.04.08 MIK
	pType->m_bKinsokuTail = false;					// �s���֑�				//@@@ 2002.04.08 MIK
	pType->m_bKinsokuRet  = false;					// ���s�������Ԃ牺����	//@@@ 2002.04.13 MIK
	pType->m_bKinsokuKuto = false;					// ��Ǔ_���Ԃ牺����	//@@@ 2002.04.17 MIK
	wcscpy( pType->m_szKinsokuHead, L"" );			// �s���֑�				//@@@ 2002.04.08 MIK
	wcscpy( pType->m_szKinsokuTail, L"" );			// �s���֑�				//@@@ 2002.04.08 MIK
	wcscpy( pType->m_szKinsokuKuto, L"�A�B�C�D��,." );	// ��Ǔ_�Ԃ牺������	// 2009.08.07 ryoji

	pType->m_bUseDocumentIcon = false;				// �����Ɋ֘A�Â���ꂽ�A�C�R�����g��

//@@@ 2001.11.17 add start MIK
	for(int i = 0; i < _countof(pType->m_RegexKeywordArr); i++)
	{
		pType->m_RegexKeywordArr[i].m_nColorIndex = COLORIDX_REGEX1;
	}
	pType->m_RegexKeywordList[0] = L'\0';
	pType->m_bUseRegexKeyword = false;
//		pType->m_nRegexKeyMagicNumber = 1;
//@@@ 2001.11.17 add end MIK

//@@@ 2006.04.10 fon ADD-start
	for(int i = 0; i < MAX_KEYHELP_FILE; i++){
		pType->m_KeyHelpArr[i].m_bUse = false;
		pType->m_KeyHelpArr[i].m_szAbout[0] = _T('\0');
		pType->m_KeyHelpArr[i].m_szPath[0] = _T('\0');
	}
	pType->m_bUseKeyWordHelp = false;		// �����I���@�\�̎g�p��
	pType->m_nKeyHelpNum = 0;				// �o�^������
	pType->m_bUseKeyHelpAllSearch = false;	// �q�b�g�������̎���������(&A)
	pType->m_bUseKeyHelpKeyDisp = false;	// 1�s�ڂɃL�[���[�h���\������(&W)
	pType->m_bUseKeyHelpPrefix = false;		// �I��͈͂őO����v����(&P)
//@@@ 2006.04.10 fon ADD-end

	// 2005.11.08 Moca �w��ʒu�c���̐ݒ�
	for(int i = 0; i < MAX_VERTLINES; i++ ){
		pType->m_nVertLineIdx[i] = CLayoutInt(0);
	}

	//  �I�����A���s�̈�v����������	2013/4/14 Uchi
	pType->m_bChkEnterAtEnd = true;
}
