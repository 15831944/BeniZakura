/*!	@file
	@brief �L�[���蓖�ĂɊւ���N���X

	@author Norio Nakatani
	@date 1998/03/25 �V�K�쐬
	@date 1998/05/16 �N���X���Ƀf�[�^�������Ȃ��悤�ɕύX
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2001, jepro, genta
	Copyright (C) 2002, YAZAKI, aroka
	Copyright (C) 2007, ryoji
	Copyright (C) 2008, nasukoji

	This source code is designed for sakura editor.
	Please contact the copyright holder to use this code for other purpose.
*/

#include "StdAfx.h"
#include "func/CKeyBind.h"
#include "env/CShareData.h"
#include "env/DLLSHAREDATA.h"
#include "macro/CSMacroMgr.h"// 2002/2/10 aroka



CKeyBind::CKeyBind()
{
}


CKeyBind::~CKeyBind()
{
}




/*! Windows �A�N�Z�����[�^�̍쐬
	@date 2007.02.22 ryoji �f�t�H���g�@�\���蓖�ĂɊւ��鏈����ǉ�
*/
HACCEL CKeyBind::CreateAccerelator(
		int			nKeyNameArrNum,
		KeyData*	pKeyNameArr
)
{
	ACCEL*	pAccelArr;
	HACCEL	hAccel;
	int		j, k;

	// �@�\�����蓖�Ă��Ă���L�[�̐����J�E���g -> nAccelArrNum
	int nAccelArrNum = 0;
	for( int i = 0; i < nKeyNameArrNum; ++i ){
		if( 0 != pKeyNameArr[i].m_nKeyCode ){
			for( j = 0; j < 8; ++j ){
				if( 0 != GetFuncCodeAt( pKeyNameArr[i], j ) ){
					nAccelArrNum++;
				}
			}
		}
	}


	if( nAccelArrNum <= 0 ){
		/* �@�\���蓖�Ă��[�� */
		return NULL;
	}
	pAccelArr = new ACCEL[nAccelArrNum];
	k = 0;
	for( int i = 0; i < nKeyNameArrNum; ++i ){
		if( 0 != pKeyNameArr[i].m_nKeyCode ){
			for( j = 0; j < 8; ++j ){
				if( 0 != GetFuncCodeAt( pKeyNameArr[i], j ) ){
					pAccelArr[k].fVirt = FNOINVERT | FVIRTKEY;
					pAccelArr[k].fVirt |= ( j & _SHIFT ) ? FSHIFT   : 0;
					pAccelArr[k].fVirt |= ( j & _CTRL  ) ? FCONTROL : 0;
					pAccelArr[k].fVirt |= ( j & _ALT   ) ? FALT     : 0;

					pAccelArr[k].key = pKeyNameArr[i].m_nKeyCode;
					pAccelArr[k].cmd = pKeyNameArr[i].m_nKeyCode | (((WORD)j)<<8) ;

					k++;
				}
			}
		}
	}
	hAccel = ::CreateAcceleratorTable( pAccelArr, nAccelArrNum );
	delete [] pAccelArr;
	return hAccel;
}






/*! �A�N���Z���[�^���ʎq�ɑΉ�����R�}���h���ʎq��Ԃ��D
	�Ή�����A�N���Z���[�^���ʎq���Ȃ��ꍇ�܂��͋@�\�����蓖�Ă̏ꍇ��0��Ԃ��D

	@date 2007.02.22 ryoji �f�t�H���g�@�\���蓖�ĂɊւ��鏈����ǉ�
*/
EFunctionCode CKeyBind::GetFuncCode(
		WORD		nAccelCmd,
		int			nKeyNameArrNum,
		KeyData*	pKeyNameArr,
		BOOL		bGetDefFuncCode /* = TRUE */
)
{
	int nCmd = (int)( nAccelCmd & 0x00ff );
	int nSts = (int)( ( nAccelCmd & 0xff00 ) >> 8 );
	for( int i = 0; i < nKeyNameArrNum; ++i ){
		if( nCmd == pKeyNameArr[i].m_nKeyCode ){
			return GetFuncCodeAt( pKeyNameArr[i], nSts, bGetDefFuncCode );
		}
	}
	return F_DEFAULT;
}






/*!
	@return �@�\�����蓖�Ă��Ă���L�[�X�g���[�N�̐�
	
	@date Oct. 31, 2001 genta ���I�ȋ@�\���ɑΉ����邽�߈����ǉ�
	@date 2007.02.22 ryoji �f�t�H���g�@�\���蓖�ĂɊւ��鏈����ǉ�
*/
int CKeyBind::CreateKeyBindList(
	HINSTANCE		hInstance,		//!< [in] �C���X�^���X�n���h��
	int				nKeyNameArrNum,	//!< [in]
	KeyData*		pKeyNameArr,	//!< [out]
	CNativeW&		cMemList,		//!<
	CFuncLookup*	pcFuncLookup,	//!< [in] �@�\�ԍ������O�̑Ή������
	BOOL			bGetDefFuncCode //!< [in] ON:�f�t�H���g�@�\���蓖�Ă��g��/OFF:�g��Ȃ� �f�t�H���g:TRUE
)
{
	int		i;
	int		j;
	int		nValidKeys;
	WCHAR	pszStr[256];
	WCHAR	szFuncName[256];
	WCHAR	szFuncNameJapanese[256];

	nValidKeys = 0;
	cMemList.SetString(LTEXT(""));
	const WCHAR*	pszSHIFT = LTEXT("Shift+");
	const WCHAR*	pszCTRL  = LTEXT("Ctrl+");
	const WCHAR*	pszALT   = LTEXT("Alt+");
	const WCHAR*	pszTAB   = LTEXT("\t");
	const WCHAR*	pszCR    = LTEXT("\r\n");	//\r=0x0d=CR��ǉ�


	cMemList.AppendString( LTEXT("�L�[\t�@�\��\t�֐���\t�@�\�ԍ�\t�L�[�}�N���L�^��/�s��") );
	cMemList.AppendString( pszCR );
	cMemList.AppendString( LTEXT("-----\t-----\t-----\t-----\t-----") );
	cMemList.AppendString( pszCR );

	for( j = 0; j < 8; ++j ){
		for( i = 0; i < nKeyNameArrNum; ++i ){
			int iFunc = GetFuncCodeAt( pKeyNameArr[i], j, bGetDefFuncCode );

			if( 0 != iFunc ){
				nValidKeys++;
				if( j & _SHIFT ){
					cMemList.AppendString( pszSHIFT );
				}
				if( j & _CTRL ){
					cMemList.AppendString( pszCTRL );
				}
				if( j & _ALT ){
					cMemList.AppendString( pszALT );
				}
				cMemList.AppendString( to_wchar(pKeyNameArr[i].m_szKeyName) );
				//	Oct. 31, 2001 genta 
				if( !pcFuncLookup->Funccode2Name(
					iFunc,
					szFuncNameJapanese, 255 )){
					auto_strcpy( szFuncNameJapanese, LTEXT("---���O����`����Ă��Ȃ�-----") );
				}
				auto_strcpy( szFuncName, LTEXT("")/*"---unknown()--"*/ );

//				/* �@�\�����{�� */
//				::LoadString(
//					hInstance,
//					pKeyNameArr[i].m_nFuncCodeArr[j],
//					 szFuncNameJapanese, 255
//				);
				cMemList.AppendString( pszTAB );
				cMemList.AppendString( szFuncNameJapanese );

				/* �@�\ID���֐����C�@�\�����{�� */
				//@@@ 2002.2.2 YAZAKI �}�N����CSMacroMgr�ɓ���
				CSMacroMgr::GetFuncInfoByID(
					hInstance,
					iFunc,
					szFuncName,
					szFuncNameJapanese
				);

				/* �֐��� */
				cMemList.AppendString( pszTAB );
				cMemList.AppendString( szFuncName );

				/* �@�\�ԍ� */
				cMemList.AppendString( pszTAB );
				auto_sprintf( pszStr, LTEXT("%d"), iFunc );
				cMemList.AppendString( pszStr );

				/* �L�[�}�N���ɋL�^�\�ȋ@�\���ǂ����𒲂ׂ� */
				cMemList.AppendString( pszTAB );
				//@@@ 2002.2.2 YAZAKI �}�N����CSMacroMgr�ɓ���
				if( CSMacroMgr::CanFuncIsKeyMacro( iFunc ) ){
					cMemList.AppendString( LTEXT("��") );
				}else{
					cMemList.AppendString( LTEXT("�~") );
				}



				cMemList.AppendString( pszCR );
			}
		}
	}
	return nValidKeys;
}

/** �@�\�ɑΉ�����L�[���̃T�[�`(�⏕�֐�)

	�^����ꂽ�V�t�g��Ԃɑ΂��āC�w�肳�ꂽ�͈͂̃L�[�G���A����
	���Y�@�\�ɑΉ�����L�[�����邩�𒲂ׁC����������
	�Ή�����L�[��������Z�b�g����D
	
	�֐�����o��Ƃ��ɂ͌����J�n�ʒu(nKeyNameArrBegin)��
	���ɏ�������index��ݒ肷��D

	@param[in,out] nKeyNameArrBegin �����J�nINDEX (�I�����ɂ͎���̊J�nINDEX�ɏ�����������)
	@param[in] nKeyNameArrBegin �����I��INDEX + 1
	@param[in] pKeyNameArr �L�[�z��
	@param[in] nShiftState �V�t�g���
	@param[out] cMemList �L�[������ݒ��
	@param[in]	nFuncId �����Ώۋ@�\ID
	@param[in]	bGetDefFuncCode �W���@�\���擾���邩�ǂ���
*/
bool CKeyBind::GetKeyStrSub(
		int&		nKeyNameArrBegin,
		int			nKeyNameArrEnd,
		KeyData*	pKeyNameArr,
		int			nShiftState,
		CNativeT&	cMemList,
		int			nFuncId,
		BOOL		bGetDefFuncCode /* = TRUE */
)
{
	const TCHAR*	pszSHIFT = _T("Shift+");
	const TCHAR*	pszCTRL  = _T("Ctrl+");
	const TCHAR*	pszALT   = _T("Alt+");

	int i;
	for( i = nKeyNameArrBegin; i < nKeyNameArrEnd; ++i ){
		if( nFuncId == GetFuncCodeAt( pKeyNameArr[i], nShiftState, bGetDefFuncCode ) ){
			if( nShiftState & _SHIFT ){
				cMemList.AppendString( pszSHIFT );
			}
			if( nShiftState & _CTRL ){
				cMemList.AppendString( pszCTRL );
			}
			if( nShiftState & _ALT ){
				cMemList.AppendString( pszALT );
			}
			cMemList.AppendString( pKeyNameArr[i].m_szKeyName );
			nKeyNameArrBegin = i + 1;
			return true;
		}
	}
	nKeyNameArrBegin = i;
	return false;
}


/** �@�\�ɑΉ�����L�[���̎擾
	@date 2007.02.22 ryoji �f�t�H���g�@�\���蓖�ĂɊւ��鏈����ǉ�
	@date 2007.11.04 genta �}�E�X�N���b�N���L�[���蓖�Ă̗D��x���グ��
	@date 2007.11.04 genta ���ʋ@�\�̃T�u���[�`����
*/
int CKeyBind::GetKeyStr(
		HINSTANCE	hInstance,
		int			nKeyNameArrNum,
		KeyData*	pKeyNameArr,
		CNativeT&	cMemList,
		int			nFuncId,
		BOOL		bGetDefFuncCode /* = TRUE */
)
{
	int		i;
	int		j;
	cMemList.SetString(_T(""));

	//	��ɃL�[�����𒲍�����
	for( j = 0; j < 8; ++j ){
		for( i = MOUSEFUNCTION_KEYBEGIN; i < nKeyNameArrNum; /* 1�������Ă͂����Ȃ� */ ){
			if( GetKeyStrSub( i, nKeyNameArrNum, pKeyNameArr, j, cMemList, nFuncId, bGetDefFuncCode )){
				return 1;
			}
		}
	}

	//	��Ƀ}�E�X�����𒲍�����
	for( j = 0; j < 8; ++j ){
		for( i = 0; i < MOUSEFUNCTION_KEYBEGIN; /* 1�������Ă͂����Ȃ� */ ){
			if( GetKeyStrSub( i, nKeyNameArrNum, pKeyNameArr, j, cMemList, nFuncId, bGetDefFuncCode )){
				return 1;
			}
		}
	}
	return 0;
}


/** �@�\�ɑΉ�����L�[���̎擾(����)
	@date 2007.02.22 ryoji �f�t�H���g�@�\���蓖�ĂɊւ��鏈����ǉ�
	@date 2007.11.04 genta ���ʋ@�\�̃T�u���[�`����
*/
int CKeyBind::GetKeyStrList(
	HINSTANCE	hInstance,
	int			nKeyNameArrNum,
	KeyData*	pKeyNameArr,
	CNativeT***	pppcMemList,
	int			nFuncId,
	BOOL		bGetDefFuncCode /* = TRUE */
)
{
	int		i;
	int		j;
	int		nAssignedKeysNum;

	nAssignedKeysNum = 0;
	if( 0 == nFuncId ){
		return 0;
	}
	for( j = 0; j < 8; ++j ){
		for( i = 0; i < nKeyNameArrNum; ++i ){
			if( nFuncId == GetFuncCodeAt( pKeyNameArr[i], j, bGetDefFuncCode ) ){
				nAssignedKeysNum++;
			}
		}
	}
	if( 0 == nAssignedKeysNum ){
		return 0;
	}
	(*pppcMemList) = new CNativeT*[nAssignedKeysNum + 1];
	for( i = 0; i < nAssignedKeysNum; ++i ){
		(*pppcMemList)[i] = new CNativeT;
	}
	(*pppcMemList)[i] = NULL;


	nAssignedKeysNum = 0;
	for( j = 0; j < 8; ++j ){
		for( i = 0; i < nKeyNameArrNum; /* 1�������Ă͂����Ȃ� */ ){
			//	2007.11.04 genta ���ʋ@�\�̃T�u���[�`����
			if( GetKeyStrSub( i, nKeyNameArrNum, pKeyNameArr, j,
					*((*pppcMemList)[nAssignedKeysNum]), nFuncId, bGetDefFuncCode )){
				nAssignedKeysNum++;
			}
		}
	}
	return nAssignedKeysNum;
}




// �A�N�Z�X�L�[�t���̕�����̍쐬
TCHAR*	CKeyBind::MakeMenuLabel(const TCHAR* sName, const TCHAR* sKey)
{
	static	TCHAR	sLabel[300];
	const	TCHAR*	p;

	if (sKey == NULL || sKey[0] == L'\0') {
		return const_cast<TCHAR*>( to_tchar(sName) );
	}
	else {
		if (!CShareData::getInstance()->GetShareData()->m_Common.m_sMainMenu.m_bMainMenuKeyParentheses
			  && (p = auto_strchr( sName, sKey[0] )) != NULL) {
			// �������A�g�p���Ă��镶�����A�N�Z�X�L�[��
			auto_strcpy_s( sLabel, _countof(sLabel), sName );
			sLabel[p-sName] = _T('&');
			auto_strcpy_s( sLabel + (p-sName) + 1, _countof(sLabel), p );
		}
		else if( (p = auto_strchr( sName, _T('(') )) != NULL
			  && (p = auto_strchr( p, sKey[0] )) != NULL) {
			// (�t���̌�ɃA�N�Z�X�L�[
			auto_strcpy_s( sLabel, _countof(sLabel), sName );
			sLabel[p-sName] = _T('&');
			auto_strcpy_s( sLabel + (p-sName) + 1, _countof(sLabel), p );
		}
		else if (_tcscmp( sName + _tcslen(sName) - 3, _T("...") ) == 0) {
			// ����...
			auto_strcpy_s( sLabel, _countof(sLabel), sName );
			sLabel[_tcslen(sName) - 3] = '\0';						// ������...�����
			auto_strcat_s( sLabel, _countof(sLabel), _T("(&") );
			auto_strcat_s( sLabel, _countof(sLabel), sKey );
			auto_strcat_s( sLabel, _countof(sLabel), _T(")...") );
		}
		else {
			auto_sprintf_s( sLabel, _countof(sLabel), _T("%ts(&%ts)"), sName, sKey );
		}

		return sLabel;
	}
}

/*! ���j���[���x���̍쐬
	@date 2007.02.22 ryoji �f�t�H���g�@�\���蓖�ĂɊւ��鏈����ǉ�
	2010/5/17	�A�N�Z�X�L�[�̒ǉ�
*/
TCHAR* CKeyBind::GetMenuLabel(
		HINSTANCE	hInstance,
		int			nKeyNameArrNum,
		KeyData*	pKeyNameArr,
		int			nFuncId,
		TCHAR*      pszLabel,   //!< [in,out] �o�b�t�@��256�ȏ�Ɖ���
		const TCHAR*	pszKey,
		BOOL		bKeyStr,
		BOOL		bGetDefFuncCode /* = TRUE */
)
{
	const int LABEL_MAX = 256;

	if( _T('\0') == pszLabel[0] ){
		::LoadString( hInstance, nFuncId, pszLabel, LABEL_MAX );
	}
	if( _T('\0') == pszLabel[0] ){
		_tcscpy( pszLabel, _T("-- undefined name --") );
	}
	// �A�N�Z�X�L�[�̒ǉ�	2010/5/17 Uchi
	_tcsncpy_s( pszLabel, LABEL_MAX, MakeMenuLabel( pszLabel, pszKey ), _TRUNCATE );

	/* �@�\�ɑΉ�����L�[����ǉ����邩 */
	if( bKeyStr ){
		CNativeT    cMemAccessKey;
		// 2010.07.11 Moca ���j���[���x���́u\t�v�̕t�������ύX
		// [�t�@�C��/�t�H���_/�E�B���h�E�ꗗ�ȊO]����[�A�N�Z�X�L�[������Ƃ��̂�]�ɕt������悤�ɕύX
		/* �@�\�ɑΉ�����L�[���̎擾 */
		if( GetKeyStr( hInstance, nKeyNameArrNum, pKeyNameArr, cMemAccessKey, nFuncId, bGetDefFuncCode ) ){
			// �o�b�t�@������Ȃ��Ƃ��͓���Ȃ�
			if( _tcslen( pszLabel ) + (Int)cMemAccessKey.GetStringLength() + 1 < LABEL_MAX ){
				_tcscat( pszLabel, _T("\t") );
				_tcscat( pszLabel, cMemAccessKey.GetStringPtr() );
			}
		}
	}
	return pszLabel;
}


/*! �L�[�̃f�t�H���g�@�\���擾����

	@param nKeyCode [in] �L�[�R�[�h
	@param nState [in] Shift,Ctrl,Alt�L�[���

	@return �@�\�ԍ�

	@date 2007.02.22 ryoji �V�K�쐬
*/
EFunctionCode CKeyBind::GetDefFuncCode( int nKeyCode, int nState )
{
	DLLSHAREDATA* pShareData = CShareData::getInstance()->GetShareData();
	if( pShareData == NULL )
		return F_DEFAULT;

	EFunctionCode nDefFuncCode = F_DEFAULT;
	if( nKeyCode == VK_F4 ){
		if( nState == _CTRL ){
			nDefFuncCode = F_FILECLOSE;	// ����(����)
			if( pShareData->m_Common.m_sTabBar.m_bDispTabWnd && !pShareData->m_Common.m_sTabBar.m_bDispTabWndMultiWin ){
				nDefFuncCode = F_WINCLOSE;	// ����
			}
		}
		else if( nState == _ALT ){
			nDefFuncCode = F_WINCLOSE;	// ����
			if( pShareData->m_Common.m_sTabBar.m_bDispTabWnd && !pShareData->m_Common.m_sTabBar.m_bDispTabWndMultiWin ){
				if( !pShareData->m_Common.m_sTabBar.m_bTab_CloseOneWin ){
					nDefFuncCode = F_GROUPCLOSE;	// �O���[�v�����	// 2007.06.20 ryoji
				}
			}
		}
	}
	return nDefFuncCode;
}


/*! ����̃L�[��񂩂�@�\�R�[�h���擾����

	@param KeyData [in] �L�[���
	@param nState [in] Shift,Ctrl,Alt�L�[���
	@param bGetDefFuncCode [in] �f�t�H���g�@�\���擾���邩�ǂ���

	@return �@�\�ԍ�

	@date 2007.03.07 ryoji �C�����C���֐�����ʏ�̊֐��ɕύX�iBCC�̍œK���o�O�΍�j
*/
EFunctionCode CKeyBind::GetFuncCodeAt( KeyData& rKeyData, int nState, BOOL bGetDefFuncCode )
{
	if( 0 != rKeyData.m_nFuncCodeArr[nState] )
		return rKeyData.m_nFuncCodeArr[nState];
	if( bGetDefFuncCode )
		return GetDefFuncCode( rKeyData.m_nKeyCode, nState );
	return F_DEFAULT;
}



const KeyData BenizakuraDefaultKeyBindTable[] = {

	// key binding
	// ���Ԃ�2�i�ŉ���3�r�b�g[Alt][Ctrl][Shift]�̑g�����̏�(�����2���������l)
	// 0,		1,		 			2(000),				3(001),				4(010),						5(011),				6(100),					7(101),				8(110),					9(111)			

	/* �}�E�X�{�^�� */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ 0,_T("�_�u���N���b�N"),		F_SEARCH_WORD,		F_SELECTWORD,		F_SELECTWORD,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("�E�N���b�N"),			F_MENU_RBUTTON,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("���N���b�N"),			F_AUTOSCROLL,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("���T�C�h�N���b�N"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("�E�T�C�h�N���b�N"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("�g���v���N���b�N"),		F_SELECTLINE,		F_SELECTLINE,		F_SELECTLINE,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("�N�A�h���v���N���b�N"),	F_SELECTALL,		F_SELECTALL,		F_SELECTALL,				F_0,				F_0,					F_0,				F_0,					F_0				},

	/* �t�@���N�V�����L�[ */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ VK_F1,	_T("F1" ),			F_CUSTMENU_1,		F_RECKEYMACRO,		F_EXECMD_DIALOG,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F2,	_T("F2" ),			F_CUSTMENU_2,		F_EXECKEYMACRO,		F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F3,	_T("F3" ),			F_CUSTMENU_3,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F4,	_T("F4" ),			F_CUSTMENU_4,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F5,	_T("F5" ),			F_CUSTMENU_5,		F_BOOKMARK_VIEW,	F_OPTION_TYPE,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F6,	_T("F6" ),			F_BEGIN_SEL,		F_BEGIN_BOX,		F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F7,	_T("F7" ),			F_CUT,				F_EXEC_CALC,		F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F8,	_T("F8" ),			F_COPY,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F9,	_T("F9" ),			F_PASTE,			F_PASTEBOX,			F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F10,	_T("F10"),			F_TAGJUMP,			F_TAGJUMPBACK,		F_SYMBOL_SEARCH,			F_0,				F_TAGS_MAKE,			F_0,				F_0,					F_0				},
	{ VK_F11,	_T("F11"),			F_OUTLINE,			F_OUTLINE_TOGGLE,	F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F12,	_T("F12"),			F_BOOKMARK_VIEW,	F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F13,	_T("F13"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F14,	_T("F14"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F15,	_T("F15"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F16,	_T("F16"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F17,	_T("F17"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F18,	_T("F18"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F19,	_T("F19"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F20,	_T("F20"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F21,	_T("F21"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F22,	_T("F22"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F23,	_T("F23"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F24,	_T("F24"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* ����L�[ */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ VK_TAB,	_T("Tab"),			F_INDENT_TAB,		F_UNINDENT_TAB,		F_NEXTWINDOW,				F_PREVWINDOW,		F_0,					F_0,				F_0,					F_0				},
	{ VK_RETURN,_T("Enter"),		F_0,				F_0,				F_0,						F_0,				F_MAXIMIZE,				F_0,				F_0,					F_0				},
	{ VK_ESCAPE,_T("Esc"),			F_CANCEL_MODE,		F_UNDO,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_BACK,	_T("BkSp"),			F_DELETE_BACK,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_INSERT,_T("Ins"),			F_CHGMOD_INS,		F_PASTE,			F_PASTE,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_DELETE,_T("Del"),			F_DELETE,			F_WordDelete,		F_WordDelete,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_HOME,	_T("Home"),			F_NEXTWINDOW,		F_PREVWINDOW,		F_PREVWINDOW,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_END,	_T("End(Help)"),	F_PREVWINDOW,		F_NEXTWINDOW,		F_NEXTWINDOW,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_PAUSE,	_T("Pause"),		F_NEXTWINDOW,		F_PREVWINDOW,		F_PREVWINDOW,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_LEFT,	_T("��"),			F_LEFT,				F_GOLINETOP,		F_WORDLEFT,					F_WORDLEFT_SEL,		F_LEFT_BOX,				F_0,				F_0,					F_0				},
	{ VK_UP,	_T("��"),			F_UP,				F_UP_SEL,			F_UP2,						F_UP2_SEL,			F_UP_BOX,				F_0,				F_0,					F_0				},
	{ VK_RIGHT,	_T("��"),			F_RIGHT,			F_GOLINEEND,		F_WORDRIGHT,				F_WORDRIGHT_SEL,	F_RIGHT_BOX,			F_0,				F_0,					F_0				},
	{ VK_DOWN,	_T("��"),			F_DOWN,				F_DOWN_SEL,			F_DOWN2,					F_DOWN2_SEL,		F_DOWN_BOX,				F_0,				F_0,					F_0				},
	{ VK_NEXT,	_T("PgDn(RollUp)"),	F_HalfPageDown,		F_1PageDown,		F_NEXTWINDOW,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_PRIOR,	_T("PgUp(RollDn)"),	F_HalfPageUp,		F_1PageUp,			F_PREVWINDOW,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_SPACE,	_T("Space"),		F_INDENT_SPACE,		F_UNINDENT_SPACE,	F_HOKAN,					F_0,				F_0,					F_0,				F_0,					F_0				},

	/* ���� */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ '0',		_T("0"),			F_0,				F_0,				F_FONT_ZOOM_RESET,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '1',		_T("1"),			F_0,				F_0,				F_FONT_MINIMIZE,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '2',		_T("2"),			F_0,				F_0,				F_TAB_WIDTH_2,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '3',		_T("3"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '4',		_T("4"),			F_0,				F_0,				F_TAB_WIDTH_4,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '5',		_T("5"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '6',		_T("6"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '7',		_T("7"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '8',		_T("8"),			F_0,				F_0,				F_TAB_WIDTH_8,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '9',		_T("9"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* �A���t�@�x�b�g */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ 'A',		_T("A"),			F_0,				F_0,				F_SELECTALL,				F_EXEC_THG_ANNOTATE,F_0,					F_0,				F_0,					F_0				},
	{ 'B',		_T("B"),			F_0,				F_0,				F_DUPLICATELINE,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'C',		_T("C"),			F_0,				F_0,				F_COPY,						F_GOFILEEND,		F_0,					F_0,				F_0,					F_0				},
	{ 'D',		_T("D"),			F_0,				F_0,				F_WordDelete,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'E',		_T("E"),			F_0,				F_0,				F_WINLIST,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'F',		_T("F"),			F_0,				F_0,				F_SEARCH_DIALOG,			F_EXEC_EXPLORER,	F_0,					F_0,				F_0,					F_0				},
	{ 'G',		_T("G"),			F_0,				F_0,				F_GREP_DIALOG,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'H',		_T("H"),			F_0,				F_0,				F_OPEN_HfromtoC,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'I',		_T("I"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'J',		_T("J"),			F_0,				F_0,				F_JUMP_DIALOG,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'K',		_T("K"),			F_0,				F_0,				F_BRACKETPAIR,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'L',		_T("L"),			F_0,				F_0,				F_SEARCH_WORD,				F_0,				F_SEARCH_WORDEND,		F_0,				F_0,					F_0				},
	{ 'M',		_T("M"),			F_0,				F_0,				F_BOOKMARK_SET,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'N',		_T("N"),			F_0,				F_0,				F_SEARCH_NEXT,				F_0,				F_NEXTWINDOW,			F_0,				F_0,					F_0				},
	{ 'O',		_T("O"),			F_0,				F_0,				F_SYMBOL_SEARCH,			F_SYMBOL_SEARCH,	F_0,					F_0,				F_0,					F_0				},
	{ 'P',		_T("P"),			F_0,				F_0,				F_SEARCH_PREV,				F_0,				F_PREVWINDOW,			F_0,				F_0,					F_0				},
	{ 'Q',		_T("Q"),			F_0,				F_0,				F_CUSTMENU_6,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'R',		_T("R"),			F_0,				F_0,				F_REPLACE_DIALOG,			F_GOFILETOP,		F_0,					F_0,				F_0,					F_0				},
	{ 'S',		_T("S"),			F_0,				F_0,				F_FILESAVE,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'T',		_T("T"),			F_0,				F_0,				F_LineDeleteToEnd,			F_LineDeleteToEnd,	F_0,					F_0,				F_0,					F_0				},
	{ 'U',		_T("U"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'V',		_T("V"),			F_0,				F_0,				F_PASTE,					F_LineDeleteToStart,F_0,					F_0,				F_0,					F_0				},
	{ 'W',		_T("W"),			F_0,				F_0,				F_WINCLOSE,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'X',		_T("X"),			F_0,				F_0,				F_CUT,						F_EXEC_CMD_PROMPT,	F_0,					F_0,				F_0,					F_0				},
	{ 'Y',		_T("Y"),			F_0,				F_0,				F_CUT_LINE,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'Z',		_T("Z"),			F_0,				F_0,				F_UNDO,						F_REDO,				F_REDO,					F_0,				F_0,					F_0				},

	/* �L�� */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ 0x00bd,	_T("-"),			F_0,				F_0,				F_FONT_ZOOM_OUT,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00de,	_T("^(�p��')"),		F_0,				F_0,				F_COPYTAG,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00dc,	_T("\\"),			F_0,				F_0,				F_COPYPATH,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00c0,	_T("@(�p��`)"),		F_0,				F_0,				F_COPYLINES,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00db,	_T("["),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00bb,	_T(";"),			F_0,				F_0,				F_FONT_ZOOM_IN,				F_FONT_ZOOM_IN,		F_0,					F_0,				F_0,					F_0				},
	{ 0x00ba,	_T(":"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00dd,	_T("]"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00bc,	_T(","),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00be,	_T("."),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00bf,	_T("/"),			F_0,				F_0,				F_HOKAN,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00e2,	_T("_"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00df,	_T("_(PC-98)"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_APPS,	_T("�A�v���L�["),	F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* �I�[�}�[�N */
	{ -1,		_T(""),				F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				}
};

#define _SQL_RUN F_PLSQL_COMPILE_ON_SQLPLUS
#define _COPYWITHLINENUM F_COPYLINESWITHLINENUMBER

const KeyData SakuraDefaultKeyBindTable[] = {

	// key binding
	// ���Ԃ�2�i�ŉ���3�r�b�g[Alt][Ctrl][Shift]�̑g�����̏�(�����2���������l)
	// 0,		1,		 			2(000),				3(001),				4(010),						5(011),				6(100),					7(101),				8(110),					9(111)			

	/* �}�E�X�{�^�� */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ 0,_T("�_�u���N���b�N"),		F_SELECTWORD,		F_SELECTWORD,		F_SELECTWORD,				F_SELECTWORD,		F_SELECTWORD,			F_SELECTWORD,		F_SELECTWORD,			F_SELECTWORD	},
	{ 0,_T("�E�N���b�N"),			F_MENU_RBUTTON,		F_MENU_RBUTTON,		F_MENU_RBUTTON,				F_MENU_RBUTTON,		F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("���N���b�N"),			F_AUTOSCROLL,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("���T�C�h�N���b�N"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("�E�T�C�h�N���b�N"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("�g���v���N���b�N"),		F_SELECTLINE,		F_SELECTLINE,		F_SELECTLINE,				F_SELECTLINE,		F_SELECTLINE,			F_SELECTLINE,		F_SELECTLINE,			F_SELECTLINE	},
	{ 0,_T("�N�A�h���v���N���b�N"),	F_SELECTALL,		F_SELECTALL,		F_SELECTALL,				F_SELECTALL,		F_SELECTALL,			F_SELECTALL,		F_SELECTALL,			F_SELECTALL		},

	/* �t�@���N�V�����L�[ */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ VK_F1,	_T("F1" ),			F_0,				F_MENU_ALLFUNC,		F_0,						F_ABOUT,			F_0,					F_0,				F_0,					F_0				},
	{ VK_F2,	_T("F2" ),			F_BOOKMARK_NEXT,	F_BOOKMARK_PREV,	F_BOOKMARK_SET,				F_BOOKMARK_RESET,	F_BOOKMARK_VIEW,		F_0,				F_0,					F_0				},
	{ VK_F3,	_T("F3" ),			F_SEARCH_NEXT,		F_SEARCH_PREV,		F_SEARCH_CLEARMARK,			F_JUMP_SRCHSTARTPOS,F_0,					F_0,				F_0,					F_0				},
	{ VK_F4,	_T("F4" ),			F_SPLIT_V,			F_SPLIT_H,			F_0,						F_FILECLOSE_OPEN,	F_0,					F_EXITALLEDITORS,	F_EXITALL,				F_0				},
	{ VK_F5,	_T("F5" ),			F_REDRAW,			F_0,				F_EXECMD_DIALOG,			F_0,				F_UUDECODE,				F_0,				F_TABTOSPACE,			F_SPACETOTAB	},
	{ VK_F6,	_T("F6" ),			F_BEGIN_SEL,		F_BEGIN_BOX,		F_TOLOWER,					F_0,				F_BASE64DECODE,			F_0,				F_0,					F_0				},
	{ VK_F7,	_T("F7" ),			F_CUT,				F_0,				F_TOUPPER,					F_0,				F_CODECNV_UTF72SJIS,	F_CODECNV_SJIS2UTF7,F_FILE_REOPEN_UTF7,		F_0				},
	{ VK_F8,	_T("F8" ),			F_COPY,				F_COPY_CRLF,		F_TOHANKAKU,				F_0,				F_CODECNV_UTF82SJIS,	F_CODECNV_SJIS2UTF8,F_FILE_REOPEN_UTF8,		F_0				},
	{ VK_F9,	_T("F9" ),			F_PASTE,			F_PASTEBOX,			F_TOZENKAKUKATA,			F_0,				F_CODECNV_UNICODE2SJIS,	F_0,				F_FILE_REOPEN_UNICODE,	F_0				},
	{ VK_F10,	_T("F10"),			_SQL_RUN,			F_DUPLICATELINE,	F_TOZENKAKUHIRA,			F_0,				F_CODECNV_EUC2SJIS,		F_CODECNV_SJIS2EUC,	F_FILE_REOPEN_EUC,		F_0				},
	{ VK_F11,	_T("F11"),			F_OUTLINE,			F_ACTIVATE_SQLPLUS,	F_HANKATATOZENKATA,			F_0,				F_CODECNV_EMAIL,		F_CODECNV_SJIS2JIS,	F_FILE_REOPEN_JIS,		F_0				},
	{ VK_F12,	_T("F12"),			F_TAGJUMP,			F_TAGJUMPBACK,		F_HANKATATOZENHIRA,			F_0,				F_CODECNV_AUTO2SJIS,	F_0,				F_FILE_REOPEN_SJIS,		F_0				},
	{ VK_F13,	_T("F13"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F14,	_T("F14"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F15,	_T("F15"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F16,	_T("F16"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F17,	_T("F17"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F18,	_T("F18"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F19,	_T("F19"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F20,	_T("F20"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F21,	_T("F21"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F22,	_T("F22"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F23,	_T("F23"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F24,	_T("F24"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* ����L�[ */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ VK_TAB,	_T("Tab"),			F_INDENT_TAB,		F_UNINDENT_TAB,		F_NEXTWINDOW,				F_PREVWINDOW,		F_0,					F_0,				F_0,					F_0				},
	{ VK_RETURN,_T("Enter"),		F_0,				F_0,				F_COMPARE,					F_0,				F_PROPERTY_FILE,		F_0,				F_0,					F_0				},
	{ VK_ESCAPE,_T("Esc"),			F_CANCEL_MODE,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_BACK,	_T("BkSp"),			F_DELETE_BACK,		F_0,				F_WordDeleteToStart,		F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_INSERT,_T("Ins"),			F_CHGMOD_INS,		F_PASTE,			F_COPY,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_DELETE,_T("Del"),			F_DELETE,			F_CUT,				F_WordDeleteToEnd,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_HOME,	_T("Home"),			F_GOLINETOP,		F_GOLINETOP_SEL,	F_GOFILETOP,				F_GOFILETOP_SEL,	F_GOLINETOP_BOX,		F_0,				F_GOFILETOP_BOX,		F_0				},
	{ VK_END,	_T("End(Help)"),	F_GOLINEEND,		F_GOLINEEND_SEL,	F_GOFILEEND,				F_GOFILEEND_SEL,	F_GOLINEEND_BOX,		F_0,				F_GOFILEEND_BOX,		F_0				},
	{ VK_PAUSE,	_T("Pause"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_LEFT,	_T("��"),			F_LEFT,				F_LEFT_SEL,			F_WORDLEFT,					F_WORDLEFT_SEL,		F_LEFT_BOX,				F_0,				F_WORDLEFT_BOX,			F_0				},
	{ VK_UP,	_T("��"),			F_UP,				F_UP_SEL,			F_WndScrollDown,			F_UP2_SEL,			F_UP_BOX,				F_0,				F_UP2_BOX,				F_MAXIMIZE_V	},
	{ VK_RIGHT,	_T("��"),			F_RIGHT,			F_RIGHT_SEL,		F_WORDRIGHT,				F_WORDRIGHT_SEL,	F_RIGHT_BOX,			F_0,				F_WORDRIGHT_BOX,		F_MAXIMIZE_H	},
	{ VK_DOWN,	_T("��"),			F_DOWN,				F_DOWN_SEL,			F_WndScrollUp,				F_DOWN2_SEL,		F_DOWN_BOX,				F_0,				F_DOWN2_BOX,			F_MINIMIZE_ALL	},
	{ VK_NEXT,	_T("PgDn(RollUp)"),	F_1PageDown,		F_1PageDown_Sel,	F_HalfPageDown,				F_HalfPageDown_Sel,	F_1PageDown_BOX,		F_0,				F_HalfPageDown_BOX,		F_0				},
	{ VK_PRIOR,	_T("PgUp(RollDn)"),	F_1PageUp,			F_1PageUp_Sel,		F_HalfPageUp,				F_HalfPageUp_Sel,	F_1PageUp_BOX,			F_0,				F_HalfPageUp_BOX,		F_0				},
	{ VK_SPACE,	_T("Space"),		F_INDENT_SPACE,		F_UNINDENT_SPACE,	F_HOKAN,					F_0,				F_0,					F_0,				F_0,					F_0				},

	/* ���� */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ '0',		_T("0"),			F_0,				F_0,				F_0,						F_0,				F_CUSTMENU_10,			F_CUSTMENU_20,		F_0,					F_0				},
	{ '1',		_T("1"),			F_0,				F_0,				F_SHOWTOOLBAR,				F_CUSTMENU_21,		F_CUSTMENU_1,			F_CUSTMENU_11,		F_0,					F_0				},
	{ '2',		_T("2"),			F_0,				F_0,				F_SHOWFUNCKEY,				F_CUSTMENU_22,		F_CUSTMENU_2,			F_CUSTMENU_12,		F_0,					F_0				},
	{ '3',		_T("3"),			F_0,				F_0,				F_SHOWSTATUSBAR,			F_CUSTMENU_23,		F_CUSTMENU_3,			F_CUSTMENU_13,		F_0,					F_0				},
	{ '4',		_T("4"),			F_0,				F_0,				F_TYPE_LIST,				F_CUSTMENU_24,		F_CUSTMENU_4,			F_CUSTMENU_14,		F_0,					F_0				},
	{ '5',		_T("5"),			F_0,				F_0,				F_OPTION_TYPE,				F_0,				F_CUSTMENU_5,			F_CUSTMENU_15,		F_0,					F_0				},
	{ '6',		_T("6"),			F_0,				F_0,				F_OPTION,					F_0,				F_CUSTMENU_6,			F_CUSTMENU_16,		F_0,					F_0				},
	{ '7',		_T("7"),			F_0,				F_0,				F_FONT,						F_0,				F_CUSTMENU_7,			F_CUSTMENU_17,		F_0,					F_0				},
	{ '8',		_T("8"),			F_0,				F_0,				F_0,						F_0,				F_CUSTMENU_8,			F_CUSTMENU_18,		F_0,					F_0				},
	{ '9',		_T("9"),			F_0,				F_0,				F_0,						F_0,				F_CUSTMENU_9,			F_CUSTMENU_19,		F_0,					F_0				},

	/* �A���t�@�x�b�g */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ 'A',		_T("A"),			F_0,				F_0,				F_SELECTALL,				F_0,				F_SORT_ASC,				F_0,				F_0,					F_0				},
	{ 'B',		_T("B"),			F_0,				F_0,				F_BROWSE,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'C',		_T("C"),			F_0,				F_0,				F_COPY,						F_OPEN_HfromtoC,	F_0,					F_0,				F_0,					F_0				},
	{ 'D',		_T("D"),			F_0,				F_0,				F_WordCut,					F_WordDelete,		F_SORT_DESC,			F_0,				F_0,					F_0				},
	{ 'E',		_T("E"),			F_0,				F_0,				F_CUT_LINE,					F_DELETE_LINE,		F_0,					F_0,				F_CASCADE,				F_0				},
	{ 'F',		_T("F"),			F_0,				F_0,				F_SEARCH_DIALOG,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'G',		_T("G"),			F_0,				F_0,				F_GREP_DIALOG,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'H',		_T("H"),			F_0,				F_0,				F_CURLINECENTER,			F_OPEN_HfromtoC,	F_0,					F_0,				F_TILE_V,				F_0				},
	{ 'I',		_T("I"),			F_0,				F_0,				F_DUPLICATELINE,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'J',		_T("J"),			F_0,				F_0,				F_JUMP_DIALOG,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'K',		_T("K"),			F_0,				F_0,				F_LineCutToEnd,				F_LineDeleteToEnd,	F_0,					F_0,				F_0,					F_0				},
	{ 'L',		_T("L"),			F_0,				F_0,				F_LOADKEYMACRO,				F_EXECKEYMACRO,		F_LTRIM,				F_0,				F_TOLOWER,				F_TOUPPER		},
	{ 'M',		_T("M"),			F_0,				F_0,				F_SAVEKEYMACRO,				F_RECKEYMACRO,		F_MERGE,				F_0,				F_0,					F_0				},
	{ 'N',		_T("N"),			F_0,				F_0,				F_FILENEW,					F_0,				F_JUMPHIST_NEXT,		F_0,				F_0,					F_0				},
	{ 'O',		_T("O"),			F_0,				F_0,				F_FILEOPEN,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'P',		_T("P"),			F_0,				F_0,				F_PRINT,					F_PRINT_PREVIEW,	F_JUMPHIST_PREV,		F_0,				F_PRINT_PAGESETUP,		F_0				},
	{ 'Q',		_T("Q"),			F_0,				F_0,				F_CREATEKEYBINDLIST,		F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'R',		_T("R"),			F_0,				F_0,				F_REPLACE_DIALOG,			F_0,				F_RTRIM,				F_0,				F_0,					F_0				},
	{ 'S',		_T("S"),			F_0,				F_0,				F_FILESAVE,					F_FILESAVEAS_DIALOG,F_0,					F_0,				F_TMPWRAPSETTING,		F_0				},
	{ 'T',		_T("T"),			F_0,				F_0,				F_TAGJUMP,					F_TAGJUMPBACK,		F_0,					F_0,				F_TILE_H,				F_0				},
	{ 'U',		_T("U"),			F_0,				F_0,				F_LineCutToStart,			F_LineDeleteToStart,F_0,					F_0,				F_WRAPWINDOWWIDTH,		F_0				},
	{ 'V',		_T("V"),			F_0,				F_0,				F_PASTE,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'W',		_T("W"),			F_0,				F_0,				F_SELECTWORD,				F_0,				F_0,					F_0,				F_TMPWRAPWINDOW,		F_0				},
	{ 'X',		_T("X"),			F_0,				F_0,				F_CUT,						F_0,				F_0,					F_0,				F_TMPWRAPNOWRAP,		F_0				},
	{ 'Y',		_T("Y"),			F_0,				F_0,				F_REDO,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'Z',		_T("Z"),			F_0,				F_0,				F_UNDO,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* �L�� */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ 0x00bd,	_T("-"),			F_0,				F_0,				F_COPYFNAME,				F_SPLIT_V,			F_0,					F_0,				F_0,					F_0				},
	{ 0x00de,	_T("^(�p��')"),		F_0,				F_0,				F_COPYTAG,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00dc,	_T("\\"),			F_0,				F_0,				F_COPYPATH,					F_SPLIT_H,			F_0,					F_0,				F_0,					F_0				},
	{ 0x00c0,	_T("@(�p��`)"),		F_0,				F_0,				F_COPYLINES,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00db,	_T("["),			F_0,				F_0,				F_BRACKETPAIR,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00bb,	_T(";"),			F_0,				F_0,				F_0,						F_SPLIT_VH,			F_INS_DATE,				F_0,				F_0,					F_0				},
	{ 0x00ba,	_T(":"),			F_0,				F_0,				_COPYWITHLINENUM,			F_0,				F_INS_TIME,				F_0,				F_0,					F_0				},
	{ 0x00dd,	_T("]"),			F_0,				F_0,				F_BRACKETPAIR,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00bc,	_T(","),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00be,	_T("."),			F_0,				F_0,				F_COPYLINESASPASSAGE,		F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00bf,	_T("/"),			F_0,				F_0,				F_HOKAN,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00e2,	_T("_"),			F_0,				F_0,				F_UNDO,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00df,	_T("_(PC-98)"),		F_0,				F_0,				F_UNDO,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_APPS,	_T("�A�v���L�["),	F_MENU_RBUTTON,		F_MENU_RBUTTON,		F_MENU_RBUTTON,				F_MENU_RBUTTON,		F_MENU_RBUTTON,			F_MENU_RBUTTON,		F_MENU_RBUTTON,			F_MENU_RBUTTON	},

	/* �I�[�}�[�N */
	{ -1,		_T(""),				F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				}
};

const KeyData HidemaruDefaultKeyBindTable[] = {
	/* �}�E�X�{�^�� */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ 0,_T("�_�u���N���b�N"),		F_SELECTWORD,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("�E�N���b�N"),			F_MENU_RBUTTON,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("���N���b�N"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("���T�C�h�N���b�N"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("�E�T�C�h�N���b�N"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("�g���v���N���b�N"),		F_SELECTLINE,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0,_T("�N�A�h���v���N���b�N"),	F_SELECTALL,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* �t�@���N�V�����L�[ */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ VK_F1,	_T("F1" ),			F_0,				F_RECKEYMACRO,		F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F2,	_T("F2" ),			F_0,				F_EXECKEYMACRO,		F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F3,	_T("F3" ),			F_SEARCH_NEXT,		F_SEARCH_PREV,		F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F4,	_T("F4" ),			/* N/A */ F_0, 		/* N/A */ F_0,		F_FILECLOSE,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F5,	_T("F5" ),			F_WordCopy,			F_COPYLINES,		F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F6,	_T("F6" ),			/* N/A */ F_0,		F_BEGIN_SEL,		F_NEXTWINDOW,				F_PREVWINDOW,		F_0,					F_0,				F_0,					F_0				},
	{ VK_F7,	_T("F7" ),			F_CUT,				/* N/A */ F_0,		F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F8,	_T("F8" ),			F_COPY,				/* N/A */ F_0,		F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F9,	_T("F9" ),			F_PASTE,			F_PASTEBOX,			F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F10,	_T("F10"),			F_TAGJUMP,			F_MENU_RBUTTON,		F_DIRECT_TAGJUMP,			F_TAGJUMPBACK,		F_0,					F_0,				F_0,					F_0				},
	{ VK_F11,	_T("F11"),			F_OUTLINE,			F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F12,	_T("F12"),			/* N/A */ F_0,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F13,	_T("F13"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F14,	_T("F14"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F15,	_T("F15"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F16,	_T("F16"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F17,	_T("F17"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F18,	_T("F18"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F19,	_T("F19"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F20,	_T("F20"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F21,	_T("F21"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F22,	_T("F22"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F23,	_T("F23"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_F24,	_T("F24"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* ����L�[ */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ VK_TAB,	_T("Tab"),			F_INDENT_TAB,		F_UNINDENT_TAB,		F_NEXTWINDOW,				F_PREVWINDOW,		F_0,					F_0,				F_0,					F_0				},
	{ VK_RETURN,_T("Enter"),		F_0,				/* N/A */ F_0,		F_DELETE_LINE,				F_0,				F_HOKAN,				F_0,				F_0,					F_0				},
	{ VK_ESCAPE,_T("Esc"),			F_CANCEL_MODE,		F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_BACK,	_T("BkSp"),			F_DELETE_BACK,		F_0,				F_DELETE_BACK,				F_0,				F_UNDO,					F_REDO,				F_0,					F_0				},
	{ VK_INSERT,_T("Ins"),			F_CHGMOD_INS,		F_PASTE,			F_COPY,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_DELETE,_T("Del"),			F_DELETE,			F_CUT,				F_WordDeleteToEnd,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_HOME,	_T("Home"),			F_GOLINETOP,		F_GOLINETOP_SEL,	F_GOFILETOP,				F_GOFILETOP_SEL,	F_0,					F_0,				F_0,					F_0				},
	{ VK_END,	_T("End(Help)"),	F_GOLINEEND,		F_GOLINEEND_SEL,	F_GOFILEEND,				F_GOFILEEND_SEL,	F_0,					F_0,				F_0,					F_0				},
	{ VK_PAUSE,	_T("Pause"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_LEFT,	_T("��"),			F_LEFT,				F_LEFT_SEL,			F_WORDLEFT,					F_WORDLEFT_SEL,		/* N/A */ F_0,			F_0,				F_0,					F_0				},
	{ VK_UP,	_T("��"),			F_UP,				F_UP_SEL,			F_UP2,						F_UP2_SEL,			F_GOPREVPARAGRAPH,		F_0,				F_0,					F_0				},
	{ VK_RIGHT,	_T("��"),			F_RIGHT,			F_RIGHT_SEL,		F_WORDRIGHT,				F_WORDRIGHT_SEL,	/* N/A */ F_0,			F_0,				F_0,					F_0				},
	{ VK_DOWN,	_T("��"),			F_DOWN,				F_DOWN_SEL,			F_DOWN2,					F_DOWN2_SEL,		F_GONEXTPARAGRAPH,		F_0,				F_0,					F_0				},
	{ VK_NEXT,	_T("PgDn(RollUp)"),	F_1PageDown,		F_1PageDown_Sel,	F_0,						F_1PageDown_Sel,	F_0,					F_0,				F_0,					F_0				},
	{ VK_PRIOR,	_T("PgUp(RollDn)"),	F_1PageUp,			F_1PageUp_Sel,		F_0,						F_1PageUp_Sel,		F_0,					F_0,				F_0,					F_0				},
	{ VK_SPACE,	_T("Space"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* ���� */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ '0',		_T("0"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '1',		_T("1"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '2',		_T("2"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '3',		_T("3"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '4',		_T("4"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '5',		_T("5"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '6',		_T("6"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '7',		_T("7"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '8',		_T("8"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ '9',		_T("9"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* �A���t�@�x�b�g */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ 'A',		_T("A"),			F_0,				F_0,				F_SELECTALL,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'B',		_T("B"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'C',		_T("C"),			F_0,				F_0,				F_COPY,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'D',		_T("D"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'E',		_T("E"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'F',		_T("F"),			F_0,				F_0,				F_SEARCH_DIALOG,			F_GREP_DIALOG,		F_0,					F_0,				F_0,					F_0				},
	{ 'G',		_T("G"),			F_0,				F_0,				F_JUMP_DIALOG,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'H',		_T("H"),			F_0,				F_0,				F_DELETE_BACK,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'I',		_T("I"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'J',		_T("J"),			F_0,				F_0,				F_INSERT_NEWLINE,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'K',		_T("K"),			F_0,				F_0,				F_BRACKETPAIR,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'L',		_T("L"),			F_0,				F_0,				/* N/A */ F_0,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'M',		_T("M"),			F_0,				F_0,				F_INSERT_NEWLINE,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'N',		_T("N"),			F_0,				F_0,				F_FILENEW,					/* N/A */ F_0,		F_0,					F_0,				F_0,					F_0				},
	{ 'O',		_T("O"),			F_0,				F_0,				F_FILEOPEN,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'P',		_T("P"),			F_0,				F_0,				F_PRINT,					/* N/A */ F_0,		F_0,					F_0,				F_0,					F_0				},
	{ 'Q',		_T("Q"),			F_0,				F_0,				F_CUSTMENU_6,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'R',		_T("R"),			F_0,				F_0,				F_REPLACE_DIALOG,			F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'S',		_T("S"),			F_0,				F_0,				F_FILESAVE,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'T',		_T("T"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'U',		_T("U"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'V',		_T("V"),			F_0,				F_0,				F_PASTE,					F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'W',		_T("W"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'X',		_T("X"),			F_0,				F_0,				F_CUT,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'Y',		_T("Y"),			F_0,				F_0,				F_REDO,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 'Z',		_T("Z"),			F_0,				F_0,				F_UNDO,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* �L�� */
	//keycode,	keyname,			�Ȃ�,				Shitf+,				Ctrl+,						Shift+Ctrl+,		Alt+,					Shit+Alt+,			Ctrl+Alt+,				Shift+Ctrl+Alt+
	{ 0x00bd,	_T("-"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00de,	_T("^(�p��')"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00dc,	_T("\\"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00c0,	_T("@(�p��`)"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00db,	_T("["),			F_0,				F_0,				F_BRACKETPAIR,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00bb,	_T(";"), /* + */	F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00ba,	_T(":"), /* * */	F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00dd,	_T("]"),			F_0,				F_0,				F_BRACKETPAIR,				F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00bc,	_T(","), /* < */	F_0,				F_0,				F_WORDLEFT,					/* N/A */ F_0,		F_0,					F_0,				F_0,					F_0				},
	{ 0x00be,	_T("."), /* > */	F_0,				F_0,				F_WORDRIGHT,				/* N/A */ F_0,		F_0,					F_0,				F_0,					F_0				},
	{ 0x00bf,	_T("/"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00e2,	_T("_"),			F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ 0x00df,	_T("_(PC-98)"),		F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},
	{ VK_APPS,	_T("�A�v���L�["),	F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				},

	/* �I�[�}�[�N */
	{ -1,		_T(""),				F_0,				F_0,				F_0,						F_0,				F_0,					F_0,				F_0,					F_0				}
};

bool CKeyBind::InitKeyAssign(DLLSHAREDATA* pShareData)
{
	SetDefaultKeyBind(&pShareData->m_Common.m_sKeyBind, &BenizakuraDefaultKeyBindTable[0]);
	return true;
}


void CKeyBind::SetDefaultKeyBindToBenizakura(struct CommonSetting_KeyBind* pKeyBind)
{
	SetDefaultKeyBind(pKeyBind, &BenizakuraDefaultKeyBindTable[0]);
}


void CKeyBind::SetDefaultKeyBindToSakura(struct CommonSetting_KeyBind* pKeyBind)
{
	SetDefaultKeyBind(pKeyBind, &SakuraDefaultKeyBindTable[0]);
}


void CKeyBind::SetDefaultKeyBindToHidemaru(struct CommonSetting_KeyBind* pKeyBind)
{
	SetDefaultKeyBind(pKeyBind, &HidemaruDefaultKeyBindTable[0]);
}


void CKeyBind::SetDefaultKeyBind(struct CommonSetting_KeyBind* pKeyBind, const KeyData* pKeyData)
{
	int i;

	for( i = 0; i < MAX_KEY_BIND; i++ ){
		if( pKeyData->m_nKeyCode == -1 ){
			break;
		}
		pKeyBind->m_pKeyNameArr[i].m_nKeyCode = pKeyData->m_nKeyCode;
		_tcscpy( pKeyBind->m_pKeyNameArr[i].m_szKeyName, pKeyData->m_szKeyName );
		pKeyBind->m_pKeyNameArr[i].m_nFuncCodeArr[0] = pKeyData->m_nFuncCodeArr[0];
		pKeyBind->m_pKeyNameArr[i].m_nFuncCodeArr[1] = pKeyData->m_nFuncCodeArr[1];
		pKeyBind->m_pKeyNameArr[i].m_nFuncCodeArr[2] = pKeyData->m_nFuncCodeArr[2];
		pKeyBind->m_pKeyNameArr[i].m_nFuncCodeArr[3] = pKeyData->m_nFuncCodeArr[3];
		pKeyBind->m_pKeyNameArr[i].m_nFuncCodeArr[4] = pKeyData->m_nFuncCodeArr[4];
		pKeyBind->m_pKeyNameArr[i].m_nFuncCodeArr[5] = pKeyData->m_nFuncCodeArr[5];
		pKeyBind->m_pKeyNameArr[i].m_nFuncCodeArr[6] = pKeyData->m_nFuncCodeArr[6];
		pKeyBind->m_pKeyNameArr[i].m_nFuncCodeArr[7] = pKeyData->m_nFuncCodeArr[7];

		pKeyData++;
	}
	pKeyBind->m_nKeyNameArrNum = i;
}

