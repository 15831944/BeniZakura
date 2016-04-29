/*!	@file
	@brief �@�\���ޒ�`

	@author Norio Nakatani
*/
/*
	Copyright (C) 1998-2001, Norio Nakatani
	Copyright (C) 2000, jepro
	Copyright (C) 2001, jepro, MIK, Stonee, asa-o, Misaka, genta, hor
	Copyright (C) 2002, aroka, minfu, MIK, ai, genta
	Copyright (C) 2003, genta, MIK, Moca
	Copyright (C) 2004, genta, isearch
	Copyright (C) 2005, genta, MIK
	Copyright (C) 2006, aroka, ryoji, fon
	Copyright (C) 2007, ryoji
	Copyright (C) 2008, nasukoji
	Copyright (C) 2009, ryoji

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

//	Sept. 14, 2000 Jepro note: functions & commands list
//	�L�[���[�h�F�R�}���h�ꗗ����
//	�����ɓo�^����Ă���R�}���h�����ʐݒ�̋@�\��ʂɕ\������A�L�[���蓖�Ăɂ��ݒ�ł���悤�ɂȂ�
//	���̃t�@�C���́u�R�}���h�ꗗ�v�̃��j���[�̏��Ԃ�\���ɂ��g���Ă���
//	sakura_rc.rc�t�@�C���̉��̂ق��ɂ���String Table���Q�Ƃ̂���

#include "StdAfx.h"
#include "func/Funccode.h"
#include "config/maxdata.h" //MAX_MRU
#include "env/CShareData.h"
#include "env/DLLSHAREDATA.h"
#include "doc/CEditDoc.h"
#include "_main/CAppMode.h"
#include "CEditApp.h"
#include "CGrepAgent.h"
#include "macro/CSMacroMgr.h"
#include "window/CEditWnd.h"
#include "docplus/CDiffManager.h"
#include "CMarkMgr.h"	// CAutoMarkMgr

//using namespace nsFuncCode;

const TCHAR* nsFuncCode::ppszFuncKind[] = {
//	"--����`--",	//Oct. 14, 2000 JEPRO �u--����`--�v��\�������Ȃ��悤�ɕύX
//	Oct. 16, 2000 JEPRO �\���̏��Ԃ����j���[�o�[�̂���ɍ��킹��悤�ɏ�������ւ���(���̌ʂ̂��̂��S��)
	_T("�t�@�C������n"),
	_T("�ҏW�n"),
	_T("�J�[�\���ړ��n"),
	_T("�I���n"),		//Oct. 15, 2000 JEPRO �u�J�[�\���ړ��n�v�������Ȃ����̂Łu�I���n�v�Ƃ��ēƗ���(�T�u���j���[���͍\����ł��Ȃ��̂�)
	_T("��`�I���n"),	//Oct. 17, 2000 JEPRO �u�I���n�v�Ɉꏏ�ɂ���Ƒ����Ȃ肷����̂Łu��`�I���n�v���Ɨ�������
	_T("�N���b�v�{�[�h�n"),
	_T("�}���n"),
	_T("�ϊ��n"),
	_T("�����n"),
	_T("���[�h�؂�ւ��n"),
	_T("�ݒ�n"),
	_T("�}�N���n"),
	//	Oct. 15, 2001 genta �J�X�^�����j���[�̕�������͓��I�ɕύX�\�ɂ��邽�߂�������͊O���D
//	_T("�J�X�^�����j���["),	//Oct. 21, 2000 JEPRO �u���̑��v����Ɨ�������
	_T("�E�B���h�E�n"),
	_T("�x��"),
	_T("���̑�")
};
const int nsFuncCode::nFuncKindNum = _countof(nsFuncCode::ppszFuncKind);


/* �t�@�C������n */
const EFunctionCode pnFuncList_File[] = {	//Oct. 16, 2000 JEPRO �ϐ����ύX(List5��List_File)
	F_FILENEW			,	//�V�K�쐬
	F_FILENEW_NEWWINDOW	,	//�V�K�E�C���h�E���J��
	F_FILEOPEN			,	//�J��
	F_FILEOPEN_DROPDOWN	,	//�J��(�h���b�v�_�E��)
	F_FILESAVE			,	//�㏑���ۑ�
	F_FILESAVEAS_DIALOG	,	//���O��t���ĕۑ�
	F_FILESAVEALL		,	//�S�ď㏑���ۑ�	// Jan. 24, 2005 genta
	F_FILECLOSE			,	//����(����)	//Oct. 17, 2000 jepro �u�t�@�C�������v�Ƃ����L���v�V������ύX
	F_FILECLOSE_OPEN	,	//���ĊJ��
	F_WINCLOSE			,	//�E�B���h�E�����	//Oct.17,2000 �R�}���h�{�Ƃ́u�E�B���h�E�n�v	//Feb. 18, 2001	JEPRO ������ړ�����
	F_FILESAVECLOSE		,	//�ۑ����ĕ��� Feb. 28, 2004 genta
	F_FILE_REOPEN		,	//�J������	//Dec. 4, 2002 genta
	F_FILE_REOPEN_SJIS		,//SJIS�ŊJ������
	F_FILE_REOPEN_JIS		,//JIS�ŊJ������
	F_FILE_REOPEN_EUC		,//EUC�ŊJ������
	F_FILE_REOPEN_LATIN1	,//Latin1�ŊJ������	// 2010/3/20 Uchi
	F_FILE_REOPEN_UNICODE	,//Unicode�ŊJ������
	F_FILE_REOPEN_UNICODEBE	,//UnicodeBE�ŊJ������
	F_FILE_REOPEN_UTF8		,//UTF-8�ŊJ������
	F_FILE_REOPEN_CESU8		,//CESU-8�ŊJ������
	F_FILE_REOPEN_UTF7		,//UTF-7�ŊJ������
	F_FILE_EXCLUSIVE_LOCK,
	F_FILE_SHARE_LOCK,
	F_FILE_UNLOCK,
	F_PRINT				,	//���
	F_PRINT_PREVIEW		,	//����v���r���[
	F_PRINT_PAGESETUP	,	//����y�[�W�ݒ�	//Sept. 14, 2000 jepro �u����̃y�[�W���C�A�E�g�̐ݒ�v����ύX
	F_OPEN_HfromtoC		,	//������C/C++�w�b�_(�\�[�X)���J��	//Feb. 7, 2001 JEPRO �ǉ�
//	F_OPEN_HHPP			,	//������C/C++�w�b�_�t�@�C�����J��	//Feb. 9, 2001 jepro�u.c�܂���.cpp�Ɠ�����.h���J���v����ύX		del 2008/6/23 Uchi
//	F_OPEN_CCPP			,	//������C/C++�\�[�X�t�@�C�����J��	//Feb. 9, 2001 jepro�u.h�Ɠ�����.c(�Ȃ����.cpp)���J���v����ύX	del 2008/6/23 Uchi
	F_ACTIVATE_SQLPLUS			,	/* Oracle SQL*Plus���A�N�e�B�u�\�� */	//Sept. 20, 2000 �u�R���p�C���vJEPRO �A�N�e�B�u�\������Ɉړ�����
	F_PLSQL_COMPILE_ON_SQLPLUS	,	/* Oracle SQL*Plus�Ŏ��s */	//Sept. 20, 2000 jepro �����́u�R���p�C���v���u���s�v�ɓ���
	F_BROWSE			,	//�u���E�Y
	F_VIEWMODE			,	//�r���[���[�h
	F_PROPERTY_FILE		,	/* �t�@�C���̃v���p�e�B */
	F_EXITALLEDITORS	,	//�ҏW�̑S�I��	// 2007.02.13 ryoji F_WIN_CLOSEALL��F_EXITALLEDITORS
	F_EXITALL				//�T�N���G�f�B�^�̑S�I��	//Dec. 27, 2000 JEPRO �ǉ�
};
const int nFincList_File_Num = _countof( pnFuncList_File );	//Oct. 16, 2000 JEPRO �z�񖼕ύX(FuncList5��FuncList_File)


/* �ҏW�n */
const EFunctionCode pnFuncList_Edit[] = {	//Oct. 16, 2000 JEPRO �ϐ����ύX(List3��List_Edit)
	F_UNDO				,	//���ɖ߂�(Undo)
	F_REDO				,	//��蒼��(Redo)
	F_DELETE			,	//�폜
	F_DELETE_BACK		,	//�J�[�\���O���폜
	F_WordDeleteToStart	,	//�P��̍��[�܂ō폜
	F_WordDeleteToEnd	,	//�P��̉E�[�܂ō폜
	F_WordCopy			,	//�P��R�s�[
	F_WordCut			,	//�P��؂���
	F_WordDelete		,	//�P��폜
	F_LineCutToStart	,	//�s���܂Ő؂���(���s�P��)
	F_LineCutToEnd		,	//�s���܂Ő؂���(���s�P��)
	F_LineDeleteToStart	,	//�s���܂ō폜(���s�P��)
	F_LineDeleteToEnd	,	//�s���܂ō폜(���s�P��)
	F_CUT_LINE			,	//�s�؂���(�܂�Ԃ��P��)
	F_DELETE_LINE		,	//�s�폜(�܂�Ԃ��P��)
	F_DUPLICATELINE		,	//�s�̓�d��(�܂�Ԃ��P��)
	F_INDENT_TAB		,	//TAB�C���f���g
	F_UNINDENT_TAB		,	//�tTAB�C���f���g
	F_INDENT_SPACE		,	//SPACE�C���f���g
	F_UNINDENT_SPACE	,	//�tSPACE�C���f���g
	F_LTRIM				,	//��(�擪)�̋󔒂��폜	2001.12.03 hor
	F_RTRIM				,	//�E(����)�̋󔒂��폜	2001.12.03 hor
	F_SORT_ASC			,	//�I���s�̏����\�[�g	2001.12.06 hor
	F_SORT_DESC			,	//�I���s�̍~���\�[�g	2001.12.06 hor
	F_MERGE				,	//�I���s�̃}�[�W		2001.12.06 hor
	F_RECONVERT			,	//�ĕϊ� 				2002.04.09 minfu
	F_INSERT_NEWLINE		//���s
//		F_WORDSREFERENCE		//�P�ꃊ�t�@�����X
};
const int nFincList_Edit_Num = _countof( pnFuncList_Edit );	//Oct. 16, 2000 JEPRO �ϐ����ύX(List3��List_Edit)


/* �J�[�\���ړ��n */
const EFunctionCode pnFuncList_Move[] = {	//Oct. 16, 2000 JEPRO �ϐ����ύX(List1��List_Move)
	F_UP				,	//�J�[�\����ړ�
	F_DOWN				,	//�J�[�\�����ړ�
	F_LEFT				,	//�J�[�\�����ړ�
	F_RIGHT				,	//�J�[�\���E�ړ�
	F_UP2				,
	F_DOWN2				,
	F_WORDLEFT			,	//�P��̍��[�Ɉړ�
	F_WORDRIGHT			,	//�P��̉E�[�Ɉړ�
	F_GOLINETOP			,	//�s���Ɉړ�(�܂�Ԃ��P��)
	F_GOLINEEND			,	//�s���Ɉړ�(�܂�Ԃ��P��)
//	F_ROLLDOWN			,	//�X�N���[���_�E��
//	F_ROLLUP			,	//�X�N���[���A�b�v
	F_HalfPageUp		,	//���y�[�W�A�b�v	//Oct. 6, 2000 JEPRO ���̂�PC-AT�݊��@�n�ɕύX(ROLL��PAGE) //Oct. 10, 2000 JEPRO ���̕ύX
	F_HalfPageDown		,	//���y�[�W�_�E��	//Oct. 6, 2000 JEPRO ���̂�PC-AT�݊��@�n�ɕύX(ROLL��PAGE) //Oct. 10, 2000 JEPRO ���̕ύX
	F_1PageUp			,	//�P�y�[�W�A�b�v	//Oct. 10, 2000 JEPRO �]���̃y�[�W�A�b�v�𔼃y�[�W�A�b�v�Ɩ��̕ύX���P�y�[�W�A�b�v��ǉ�
	F_1PageDown			,	//�P�y�[�W�_�E��	//Oct. 10, 2000 JEPRO �]���̃y�[�W�_�E���𔼃y�[�W�_�E���Ɩ��̕ύX���P�y�[�W�_�E����ǉ�
	F_GOFILETOP			,	//�t�@�C���̐擪�Ɉړ�
	F_GOFILEEND			,	//�t�@�C���̍Ō�Ɉړ�
	F_CURLINECENTER		,	//�J�[�\���s���E�B���h�E������
	F_JUMP_DIALOG		,	//�w��s�w�W�����v	//Sept. 17, 2000 JEPRO �R�}���h�{�Ƃ́u�����n�v
	F_JUMP_SRCHSTARTPOS	,	//�����J�n�ʒu�֖߂�	// 02/06/26 ai �R�}���h�{�Ƃ͢�����n�
	F_JUMPHIST_PREV		,	//�ړ�����: �O��
	F_JUMPHIST_NEXT		,	//�ړ�����: ����
	F_JUMPHIST_SET		,	//���݈ʒu���ړ������ɓo�^
	F_WndScrollDown		,	//�e�L�X�g���P�s���փX�N���[��	// 2001/06/20 asa-o
	F_WndScrollUp		,	//�e�L�X�g���P�s��փX�N���[��	// 2001/06/20 asa-o
	F_GONEXTPARAGRAPH	,	//���̒i����
	F_GOPREVPARAGRAPH	,	//�O�̒i����
	F_AUTOSCROLL		,	//�I�[�g�X�N���[��
};
const int nFincList_Move_Num = _countof( pnFuncList_Move );	//Oct. 16, 2000 JEPRO �ϐ����ύX(List1��List_Move)


/* �I���n */	//Oct. 15, 2000 JEPRO �u�J�[�\���ړ��n�v����(�I��)���ړ�
const EFunctionCode pnFuncList_Select[] = {
	F_SELECTWORD			,	//���݈ʒu�̒P��I��
	F_SELECTALL				,	//���ׂđI��
	F_SELECTLINE			,	//1�s�I��	// 2007.10.06 nasukoji
	F_BEGIN_SEL				,	//�͈͑I���J�n
	F_UP_SEL				,	//(�͈͑I��)�J�[�\����ړ�
	F_DOWN_SEL				,	//(�͈͑I��)�J�[�\�����ړ�
	F_LEFT_SEL				,	//(�͈͑I��)�J�[�\�����ړ�
	F_RIGHT_SEL				,	//(�͈͑I��)�J�[�\���E�ړ�
	F_UP2_SEL				,
	F_DOWN2_SEL				,
	F_WORDLEFT_SEL			,	//(�͈͑I��)�P��̍��[�Ɉړ�
	F_WORDRIGHT_SEL			,	//(�͈͑I��)�P��̉E�[�Ɉړ�
	F_GOLINETOP_SEL			,	//(�͈͑I��)�s���Ɉړ�(�܂�Ԃ��P��)
	F_GOLINEEND_SEL			,	//(�͈͑I��)�s���Ɉړ�(�܂�Ԃ��P��)
//	F_ROLLDOWN_SEL			,	//(�͈͑I��)�X�N���[���_�E��
//	F_ROLLUP_SEL			,	//(�͈͑I��)�X�N���[���A�b�v
	F_HalfPageUp_Sel		,	//(�͈͑I��)���y�[�W�A�b�v	//Oct. 6, 2000 JEPRO ���̂�PC-AT�݊��@�n�ɕύX(ROLL��PAGE) //Oct. 10, 2000 JEPRO ���̕ύX
	F_HalfPageDown_Sel		,	//(�͈͑I��)���y�[�W�_�E��	//Oct. 6, 2000 JEPRO ���̂�PC-AT�݊��@�n�ɕύX(ROLL��PAGE) //Oct. 10, 2000 JEPRO ���̕ύX
	F_1PageUp_Sel			,	//(�͈͑I��)�P�y�[�W�A�b�v	//Oct. 10, 2000 JEPRO �]���̃y�[�W�A�b�v�𔼃y�[�W�A�b�v�Ɩ��̕ύX���P�y�[�W�A�b�v��ǉ�
	F_1PageDown_Sel			,	//(�͈͑I��)�P�y�[�W�_�E��	//Oct. 10, 2000 JEPRO �]���̃y�[�W�_�E���𔼃y�[�W�_�E���Ɩ��̕ύX���P�y�[�W�_�E����ǉ�
	F_GOFILETOP_SEL			,	//(�͈͑I��)�t�@�C���̐擪�Ɉړ�
	F_GOFILEEND_SEL			,	//(�͈͑I��)�t�@�C���̍Ō�Ɉړ�
	F_GONEXTPARAGRAPH_SEL	,	//(�͈͑I��)���̒i����
	F_GOPREVPARAGRAPH_SEL		//(�͈͑I��)�O�̒i����
};
const int nFincList_Select_Num = _countof( pnFuncList_Select );


/* ��`�I���n */	//Oct. 17, 2000 JEPRO (��`�I��)���V�݂��ꎟ�悱���ɂ���
const EFunctionCode pnFuncList_Box[] = {
//	F_BOXSELALL			,	//��`�ł��ׂđI��
	F_BEGIN_BOX			,	//��`�͈͑I���J�n
	F_UP_BOX			,	//(��`�I��)�J�[�\����ړ�
	F_DOWN_BOX			,	//(��`�I��)�J�[�\�����ړ�
	F_LEFT_BOX			,	//(��`�I��)�J�[�\�����ړ�
	F_RIGHT_BOX			,	//(��`�I��)�J�[�\���E�ړ�
	F_UP2_BOX			,
	F_DOWN2_BOX			,
	F_WORDLEFT_BOX		,	//(��`�I��)�P��̍��[�Ɉړ�
	F_WORDRIGHT_BOX		,	//(��`�I��)�P��̉E�[�Ɉړ�
	F_GOLOGICALLINETOP_BOX	,	//(��`�I��)�s���Ɉړ�(���s�P��)
	F_GOLINETOP_BOX		,	//(��`�I��)�s���Ɉړ�(�܂�Ԃ��P��)
	F_GOLINEEND_BOX		,	//(��`�I��)�s���Ɉړ�(�܂�Ԃ��P��)
	F_HalfPageUp_BOX	,	//(��`�I��)���y�[�W�A�b�v
	F_HalfPageDown_BOX	,	//(��`�I��)���y�[�W�_�E��
	F_1PageUp_BOX		,	//(��`�I��)�P�y�[�W�A�b�v
	F_1PageDown_BOX		,	//(��`�I��)�P�y�[�W�_�E��
	F_GOFILETOP_BOX		,	//(��`�I��)�t�@�C���̐擪�Ɉړ�
	F_GOFILEEND_BOX			//(��`�I��)�t�@�C���̍Ō�Ɉړ�
};
const int nFincList_Box_Num = _countof( pnFuncList_Box );


/* �N���b�v�{�[�h�n */
const EFunctionCode pnFuncList_Clip[] = {	//Oct. 16, 2000 JEPRO �ϐ����ύX(List2��List_Clip)
	F_CUT						,	//�؂���(�I��͈͂��N���b�v�{�[�h�ɃR�s�[���č폜)
	F_COPY						,	//�R�s�[(�I��͈͂��N���b�v�{�[�h�ɃR�s�[)
	F_COPY_ADDCRLF				,	//�܂�Ԃ��ʒu�ɉ��s�����ăR�s�[(�I��͈͂��N���b�v�{�[�h�ɃR�s�[)
	F_COPY_CRLF					,	//CRLF���s�ŃR�s�[
	F_PASTE						,	//�\��t��(�N���b�v�{�[�h����\��t��)
	F_PASTEBOX					,	//��`�\��t��(�N���b�v�{�[�h�����`�\��t��)
//	F_INSTEXT_W					,	//�e�L�X�g��\��t��		//Oct. 22, 2000 JEPRO �����ɒǉ�������������@�\�Ȃ̂��s���Ȃ̂ŃR�����g�A�E�g�ɂ��Ă���
//	F_ADDTAIL_W					,	//�Ō�Ƀe�L�X�g��ǉ�		//Oct. 22, 2000 JEPRO �����ɒǉ�������������@�\�Ȃ̂��s���Ȃ̂ŃR�����g�A�E�g�ɂ��Ă���
	F_COPYLINES					,	//�I��͈͓��S�s�R�s�[
	F_COPYLINESASPASSAGE		,	//�I��͈͓��S�s���p���t���R�s�[
	F_COPYLINESWITHLINENUMBER	,	//�I��͈͓��S�s�s�ԍ��t���R�s�[
	F_COPY_COLOR_HTML			,	//�I��͈͓��F�t��HTML�R�s�[
	F_COPY_COLOR_HTML_LINENUMBER,	//�I��͈͓��s�ԍ��F�t��HTML�R�s�[
	F_COPYFNAME					,	//���̃t�@�C�������N���b�v�{�[�h�ɃR�s�[ //2002/2/3 aroka
	F_COPYPATH					,	//���̃t�@�C���̃p�X�����N���b�v�{�[�h�ɃR�s�[
	F_COPYTAG					,	//���̃t�@�C���̃p�X���ƃJ�[�\���ʒu���R�s�[	//Sept. 14, 2000 JEPRO ���j���[�ɍ��킹�ĉ��Ɉړ�
	F_CREATEKEYBINDLIST				//�L�[���蓖�Ĉꗗ���R�s�[	//Sept. 15, 2000 JEPRO IDM_TEST�̂܂܂ł͂��܂������Ȃ��̂�F�ɕς��ēo�^	//Dec. 25, 2000 ����
};
const int nFincList_Clip_Num = _countof( pnFuncList_Clip );	//Oct. 16, 2000 JEPRO �ϐ����ύX(List1��List_Move)


/* �}���n */
const EFunctionCode pnFuncList_Insert[] = {
	F_INS_DATE				,	// ���t�}��
	F_INS_TIME				,	// �����}��
	F_CTRL_CODE_DIALOG			//�R���g���[���R�[�h�̓���
};
const int nFincList_Insert_Num = _countof( pnFuncList_Insert );


/* �ϊ��n */
const EFunctionCode pnFuncList_Convert[] = {	//Oct. 16, 2000 JEPRO �ϐ����ύX(List6��List_Convert)
	F_TOLOWER				,	//������
	F_TOUPPER				,	//�啶��
	F_TOHANKAKU				,	/* �S�p�����p */
	// From Here 2007.01.24 maru ���я��ύX
	F_TOZENKAKUKATA			,	/* ���p�{�S�Ђ灨�S�p�E�J�^�J�i */	//Sept. 17, 2000 jepro �������u���p���S�p�J�^�J�i�v����ύX
	F_TOZENKAKUHIRA			,	/* ���p�{�S�J�^���S�p�E�Ђ炪�� */	//Sept. 17, 2000 jepro �������u���p���S�p�Ђ炪�ȁv����ύX
	F_TOZENEI				,	/* ���p�p�����S�p�p�� */			//July. 30, 2001 Misaka
	F_TOHANEI				,	/* �S�p�p�������p�p�� */
	F_TOHANKATA				,	/* �S�p�J�^�J�i�����p�J�^�J�i */	//Aug. 29, 2002 ai
	// To Here 2007.01.24 maru ���я��ύX
	F_HANKATATOZENKATA		,	/* ���p�J�^�J�i���S�p�J�^�J�i */
	F_HANKATATOZENHIRA		,	/* ���p�J�^�J�i���S�p�Ђ炪�� */
	F_TABTOSPACE			,	/* TAB���� */
	F_SPACETOTAB			,	/* �󔒁�TAB */  //---- Stonee, 2001/05/27
	F_CODECNV_AUTO2SJIS		,	/* �������ʁ�SJIS�R�[�h�ϊ� */
	F_CODECNV_EMAIL			,	//E-Mail(JIS��SJIS)�R�[�h�ϊ�
	F_CODECNV_EUC2SJIS		,	//EUC��SJIS�R�[�h�ϊ�
	F_CODECNV_UNICODE2SJIS	,	//Unicode��SJIS�R�[�h�ϊ�
	F_CODECNV_UNICODEBE2SJIS	,	//Unicode��SJIS�R�[�h�ϊ�
	F_CODECNV_UTF82SJIS		,	/* UTF-8��SJIS�R�[�h�ϊ� */
	F_CODECNV_UTF72SJIS		,	/* UTF-7��SJIS�R�[�h�ϊ� */
	F_CODECNV_SJIS2JIS		,	/* SJIS��JIS�R�[�h�ϊ� */
	F_CODECNV_SJIS2EUC		,	/* SJIS��EUC�R�[�h�ϊ� */
	F_CODECNV_SJIS2UTF8		,	/* SJIS��UTF-8�R�[�h�ϊ� */
	F_CODECNV_SJIS2UTF7		,	/* SJIS��UTF-7�R�[�h�ϊ� */
	F_BASE64DECODE			,	//Base64�f�R�[�h���ĕۑ�
	F_UUDECODE					//uudecode���ĕۑ�
	//Sept. 30, 2000JEPRO �R�����g�A�E�g����Ă������̂𕜊�������(���삵�Ȃ��̂����H)
	//Oct. 17, 2000 jepro �������u�I�𕔕���UUENCODE�f�R�[�h�v����ύX
};
const int nFincList_Convert_Num = _countof( pnFuncList_Convert );	//Oct. 16, 2000 JEPRO �ϐ����ύX(List6��List_Convert)


/* �����n */
const EFunctionCode pnFuncList_Search[] = {	//Oct. 16, 2000 JEPRO �ϐ����ύX(List4��List_Search)
	F_SEARCH_WORD		,	//���݈ʒu�̒P��Ō���
	F_SEARCH_WORDEND	,	//���݈ʒu����P�ꖖ���܂ł̕�����Ō���
	F_SEARCH_DIALOG		,	//����(�P�ꌟ���_�C�A���O)
	F_SEARCH_BOX		,	//����(�{�b�N�X)
	F_SEARCH_NEXT		,	//��������	//Sept. 16, 2000 JEPRO "��"��"�O"�̑O�Ɉړ�
	F_SEARCH_PREV		,	//�O������
	F_REPLACE_DIALOG	,	//�u��
	F_SEARCH_CLEARMARK	,	//�����}�[�N�̃N���A
	F_JUMP_SRCHSTARTPOS	,	//�����J�n�ʒu�֖߂�	// 02/06/26 ai
	F_GREP_DIALOG		,	//Grep
	F_JUMP_DIALOG		,	//�w��s�w�W�����v
	F_OUTLINE			,	//�A�E�g���C�����
	F_OUTLINE_TOGGLE	,	//�A�E�g���C�����(toggle) // 20060201 aroka
	F_TAGJUMP			,	//�^�O�W�����v�@�\
	F_TAGJUMP_CLOSE		,	//���ă^�O�W�����v(���E�B���h�E�����) // Apr. 03, 2003 genta
	F_TAGJUMPBACK		,	//�^�O�W�����v�o�b�N�@�\
	F_TAGS_MAKE			,	//�^�O�t�@�C���̍쐬	//@@@ 2003.04.13 MIK
	F_DIRECT_TAGJUMP	,	//�_�C���N�g�^�O�W�����v	//@@@ 2003.04.15 MIK
	F_SYMBOL_SEARCH		,	//�V���{������(�L�[���[�h�w��^�O�W�����v)
	F_COMPARE			,	//�t�@�C�����e��r
	F_DIFF_DIALOG		,	//DIFF�����\��(�_�C�A���O)
	F_DIFF_NEXT			,	//���̍�����
	F_DIFF_PREV			,	//�O�̍�����
	F_DIFF_RESET		,	//�����̑S����
	F_BRACKETPAIR		,	//�Ί��ʂ̌���
	F_BOOKMARK_SET		,	//�u�b�N�}�[�N�ݒ�E����
	F_BOOKMARK_NEXT		,	//���̃u�b�N�}�[�N��
	F_BOOKMARK_PREV		,	//�O�̃u�b�N�}�[�N��
	F_BOOKMARK_RESET	,	//�u�b�N�}�[�N�̑S����
	F_BOOKMARK_VIEW		,	//�u�b�N�}�[�N�̈ꗗ
	F_ISEARCH_NEXT	    ,   //�O���C���N�������^���T�[�` //2004.10.13 isearch
	F_ISEARCH_PREV		,	//����C���N�������^���T�[�` //2004.10.13 isearch
	F_ISEARCH_REGEXP_NEXT,	//�O�����K�\���C���N�������^���T�[�`  //2004.10.13 isearch
	F_ISEARCH_REGEXP_PREV,	//������K�\���C���N�������^���T�[�`  //2004.10.13 isearch
	F_ISEARCH_MIGEMO_NEXT,	//�O��MIGEMO�C���N�������^���T�[�`    //2004.10.13 isearch
	F_ISEARCH_MIGEMO_PREV	//���MIGEMO�C���N�������^���T�[�`    //2004.10.13 isearch
};
const int nFincList_Search_Num = _countof( pnFuncList_Search );	//Oct. 16, 2000 JEPRO �ϐ����ύX(List4��List_Search)


/* ���[�h�؂�ւ��n */	//Oct. 16, 2000 JEPRO �ϐ����ύX(List8��List_Mode)
const EFunctionCode pnFuncList_Mode[] = {
	F_CHGMOD_INS		,	//�}���^�㏑�����[�h�؂�ւ�
	F_CHG_CHARSET		,	//�����R�[�h�Z�b�g�w��		2010/6/14 Uchi
	F_CHGMOD_EOL_CRLF	,	//���͉��s�R�[�h�w��(CRLF)	2003.06.23 Moca
	F_CHGMOD_EOL_LF		,	//���͉��s�R�[�h�w��(LF)	2003.06.23 Moca
	F_CHGMOD_EOL_CR		,	//���͉��s�R�[�h�w��(CR)	2003.06.23 Moca
	F_CANCEL_MODE			//�e�탂�[�h�̎�����
};
const int nFincList_Mode_Num = _countof( pnFuncList_Mode );	//Oct. 16, 2000 JEPRO �ϐ����ύX(List8��List_Mode)


/* �ݒ�n */
const EFunctionCode pnFuncList_Set[] = {	//Oct. 16, 2000 JEPRO �ϐ����ύX(List9��List_Set)
	F_SHOWTOOLBAR		,	/* �c�[���o�[�̕\�� */
	F_SHOWFUNCKEY		,	/* �t�@���N�V�����L�[�̕\�� */	//Sept. 14, 2000 JEPRO �t�@���N�V�����L�[�ƃX�e�[�^�X�o�[�����ւ�
	F_SHOWTAB			,	/* �^�u�̕\�� */	//@@@ 2003.06.10 MIK
	F_SHOWSTATUSBAR		,	/* �X�e�[�^�X�o�[�̕\�� */
	F_TYPE_LIST			,	/* �^�C�v�ʐݒ�ꗗ */			//Sept. 17, 2000 JEPRO �ݒ�n�ɓ����ĂȂ������̂Œǉ�
	F_OPTION_TYPE		,	/* �^�C�v�ʐݒ� */
	F_OPTION			,	/* ���ʐݒ� */
	F_FONT				,	/* �t�H���g�ݒ� */
	F_FONT_ZOOM_IN		,	/* �t�H���g�̊g�� */
	F_FONT_ZOOM_OUT		,	/* �t�H���g�̏k�� */
	F_FONT_ZOOM_RESET	,	/* �t�H���g�T�C�Y�̃��Z�b�g */
	F_FONT_MINIMIZE		,	/* �t�H���g�T�C�Y�ŏ��� */
	F_SHOW_TAB_CHAR		,	/* TAB�L���̕\��(�g�O��) */
	F_SHOW_EOL_CHAR		,	/* ���s�L���̕\��(�g�O��) */
	F_TAB_WIDTH_2		,	/* TAB �� 2 */
	F_TAB_WIDTH_4		,	/* TAB �� 4 */
	F_TAB_WIDTH_8		,	/* TAB �� 8 */
	F_WRAPWINDOWWIDTH	,	/* ���݂̃E�B���h�E���Ő܂�Ԃ� */	//Oct. 7, 2000 JEPRO WRAPWINDIWWIDTH �� WRAPWINDOWWIDTH �ɕύX
	F_PRINT_PAGESETUP	,	//����y�[�W�ݒ�				//Sept. 14, 2000 JEPRO �u����̃y�[�W���C�A�E�g�̐ݒ�v���u����y�[�W�ݒ�v�ɕύX	//Oct. 17, 2000 �R�}���h�{�Ƃ́u�t�@�C������n�v
	F_FAVORITE			,	//�����̊Ǘ�	//@@@ 2003.04.08 MIK
	F_TMPWRAPNOWRAP		,	//�܂�Ԃ��Ȃ��i�ꎞ�ݒ�j			// 2008.05.30 nasukoji
	F_TMPWRAPSETTING	,	//�w�茅�Ő܂�Ԃ��i�ꎞ�ݒ�j		// 2008.05.30 nasukoji
	F_TMPWRAPWINDOW		,	//�E�[�Ő܂�Ԃ��i�ꎞ�ݒ�j		// 2008.05.30 nasukoji
	F_SELECT_COUNT_MODE		//�����J�E���g�ݒ�	// 2009.07.06 syat
};
int		nFincList_Set_Num = _countof( pnFuncList_Set );	//Oct. 16, 2000 JEPRO �ϐ����ύX(List9��List_Set)


/* �}�N���n */
const EFunctionCode pnFuncList_Macro[] = {	//Oct. 16, 2000 JEPRO �ϐ����ύX(List10��List_Macro)
	F_RECKEYMACRO	,	/* �L�[�}�N���̋L�^�J�n�^�I�� */
	F_SAVEKEYMACRO	,	/* �L�[�}�N���̕ۑ� */
	F_LOADKEYMACRO	,	/* �L�[�}�N���̓ǂݍ��� */
	F_EXECKEYMACRO	,	/* �L�[�}�N���̎��s */
	F_EXECEXTMACRO	,	/* ���O���w�肵�ă}�N�����s */
//	From Here Sept. 20, 2000 JEPRO ����CMMAND��COMMAND�ɕύX
//	F_EXECCMMAND		/* �O���R�}���h���s */
	F_EXECMD_DIALOG		/* �O���R�}���h���s */
//	To Here Sept. 20, 2000

};
const int nFincList_Macro_Num = _countof( pnFuncList_Macro);	//Oct. 16, 2000 JEPRO �ϐ����ύX(List10��List_Macro)


/* �J�X�^�����j���[ */	//Oct. 21, 2000 JEPRO �u���̑��v���番���Ɨ���
#if 0
//	From Here Oct. 15, 2001 genta
//	�J�X�^�����j���[�̕�����𓮓I�ɕύX�\�ɂ��邽�߂���͍폜�D
const EFunctionCode pnFuncList_Menu[] = {
	F_MENU_RBUTTON				,	/* �E�N���b�N���j���[ */
	F_CUSTMENU_1				,	/* �J�X�^�����j���[1 */
	F_CUSTMENU_2				,	/* �J�X�^�����j���[2 */
	F_CUSTMENU_3				,	/* �J�X�^�����j���[3 */
	F_CUSTMENU_4				,	/* �J�X�^�����j���[4 */
	F_CUSTMENU_5				,	/* �J�X�^�����j���[5 */
	F_CUSTMENU_6				,	/* �J�X�^�����j���[6 */
	F_CUSTMENU_7				,	/* �J�X�^�����j���[7 */
	F_CUSTMENU_8				,	/* �J�X�^�����j���[8 */
	F_CUSTMENU_9				,	/* �J�X�^�����j���[9 */
	F_CUSTMENU_10				,	/* �J�X�^�����j���[10 */
	F_CUSTMENU_11				,	/* �J�X�^�����j���[11 */
	F_CUSTMENU_12				,	/* �J�X�^�����j���[12 */
	F_CUSTMENU_13				,	/* �J�X�^�����j���[13 */
	F_CUSTMENU_14				,	/* �J�X�^�����j���[14 */
	F_CUSTMENU_15				,	/* �J�X�^�����j���[15 */
	F_CUSTMENU_16				,	/* �J�X�^�����j���[16 */
	F_CUSTMENU_17				,	/* �J�X�^�����j���[17 */
	F_CUSTMENU_18				,	/* �J�X�^�����j���[18 */
	F_CUSTMENU_19				,	/* �J�X�^�����j���[19 */
	F_CUSTMENU_20				,	/* �J�X�^�����j���[20 */
	F_CUSTMENU_21				,	/* �J�X�^�����j���[21 */
	F_CUSTMENU_22				,	/* �J�X�^�����j���[22 */
	F_CUSTMENU_23				,	/* �J�X�^�����j���[23 */
	F_CUSTMENU_24				 	/* �J�X�^�����j���[24 */
};
const int nFincList_Menu_Num = _countof( pnFuncList_Menu );	//Oct. 21, 2000 JEPRO �u���̑��v���番���Ɨ���
#endif

/* �E�B���h�E�n */
const EFunctionCode pnFuncList_Win[] = {	//Oct. 16, 2000 JEPRO �ϐ����ύX(List7��List_Win)
	F_SPLIT_V			,	//�㉺�ɕ���	//Sept. 17, 2000 jepro �����́u�c�v���u�㉺�Ɂv�ɕύX
	F_SPLIT_H			,	//���E�ɕ���	//Sept. 17, 2000 jepro �����́u���v���u���E�Ɂv�ɕύX
	F_SPLIT_VH			,	//�c���ɕ���	//Sept. 17, 2000 jepro �����Ɂu�Ɂv��ǉ�
	F_WINCLOSE			,	//�E�B���h�E�����
	F_WIN_CLOSEALL		,	//���ׂẴE�B���h�E�����	//Oct. 17, 2000 JEPRO ���O��ύX(F_FILECLOSEALL��F_WIN_CLOSEALL)
	F_GREP_WIN_ALL_CLOSE	,	// Grep �E�B���h�E�����ׂĕ���
	F_TAB_CLOSEOTHER	,	//���̃^�u�ȊO�����	// 2008.11.22 syat
	F_NEXTWINDOW		,	//���̃E�B���h�E
	F_PREVWINDOW		,	//�O�̃E�B���h�E
 	F_WINLIST			,	//�J���Ă���E�B���h�E�ꗗ�|�b�v�A�b�v�\��	// 2006.03.23 fon
	F_CASCADE			,	//�d�˂ĕ\��
	F_TILE_V			,	//�㉺�ɕ��ׂĕ\��
	F_TILE_H			,	//���E�ɕ��ׂĕ\��
	F_TOPMOST			,	//��Ɏ�O�ɕ\��
	F_BIND_WINDOW		,	//�������ĕ\��	// 2006.04.22 ryoji
	F_GROUPCLOSE		,	//�O���[�v�����	// 2007.06.20 ryoji
	F_NEXTGROUP			,	//���̃O���[�v	// 2007.06.20 ryoji
	F_PREVGROUP			,	//�O�̃O���[�v	// 2007.06.20 ryoji
	F_TAB_MOVERIGHT		,	//�^�u���E�Ɉړ�	// 2007.06.20 ryoji
	F_TAB_MOVELEFT		,	//�^�u�����Ɉړ�	// 2007.06.20 ryoji
	F_TAB_SEPARATE		,	//�V�K�O���[�v	// 2007.06.20 ryoji
	F_TAB_JOINTNEXT		,	//���̃O���[�v�Ɉړ�	// 2007.06.20 ryoji
	F_TAB_JOINTPREV		,	//�O�̃O���[�v�Ɉړ�	// 2007.06.20 ryoji
	F_TAB_CLOSELEFT 	,	//�������ׂĕ���		// 2008.11.22 syat
	F_TAB_CLOSERIGHT	,	//�E�����ׂĕ���		// 2008.11.22 syat
	F_MAXIMIZE			,	//�E�B���h�E���ő剻
	F_MAXIMIZE_V		,	//�c�����ɍő剻
	F_MAXIMIZE_H		,	//�������ɍő剻 //2001.02.10 by MIK
	F_MINIMIZE_ALL		,	//���ׂčŏ���	//Sept. 17, 2000 jepro �����́u�S�āv���u���ׂāv�ɓ���
	F_REDRAW			,	//�ĕ`��
	F_WIN_OUTPUT		,	//�A�E�g�v�b�g�E�B���h�E�\��
};
const int nFincList_Win_Num = _countof( pnFuncList_Win );	//Oct. 16, 2000 JEPRO �ϐ����ύX(List7��List_Win)


/* �x�� */
const EFunctionCode pnFuncList_Support[] = {	//Oct. 16, 2000 JEPRO �ϐ����ύX(List11��List_Support)
	F_HOKAN						,	/* ���͕⊮ */
	F_TOGGLE_KEY_SEARCH			,	/* �L�����b�g�ʒu�̒P���������������@�\ON/OFF */	// 2006.03.24 fon
	F_MENU_ALLFUNC				,	/* �R�}���h�ꗗ */
	F_ABOUT							/* �o�[�W������� */	//Dec. 24, 2000 JEPRO �ǉ�
};
const int nFincList_Support_Num = _countof( pnFuncList_Support );	//Oct. 16, 2000 JEPRO �ϐ����ύX(List11��List_Support)


/* ���̑� */	//Oct. 16, 2000 JEPRO �ϐ����ύX(List12��List_Others)
const EFunctionCode pnFuncList_Others[] = {
	F_EXEC_CALC,
	F_EXEC_CMD_PROMPT,
	F_EXEC_EXPLORER,
	F_EXEC_THG_ANNOTATE
};
const int nFincList_Others_Num = _countof( pnFuncList_Others );	//Oct. 16, 2000 JEPRO �ϐ����ύX(List12��List_Others)





const int nsFuncCode::pnFuncListNumArr[] = {
//	nFincList_Undef_Num,	//Oct. 14, 2000 JEPRO �u--����`--�v��\�������Ȃ��悤�ɕύX	//Oct. 16, 2000 JEPRO �ϐ����ύX(List0��List_Undef)
	nFincList_File_Num,		/* �t�@�C������n */	//Oct. 16, 2000 JEPRO �ϐ����ύX(List5��List_File)
	nFincList_Edit_Num,		/* �ҏW�n */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List3��List_Edit)
	nFincList_Move_Num,		/* �J�[�\���ړ��n */	//Oct. 16, 2000 JEPRO �ϐ����ύX(List1��List_Move)
	nFincList_Select_Num,	/* �I���n */			//Oct. 15, 2000 JEPRO �u�J�[�\���ړ��n�v����(�I��)���ړ�
	nFincList_Box_Num,		/* ��`�I���n */		//Oct. 17, 2000 JEPRO (��`�I��)���V�݂��ꎟ�悱���ɂ���
	nFincList_Clip_Num,		/* �N���b�v�{�[�h�n */	//Oct. 16, 2000 JEPRO �ϐ����ύX(List2��List_Clip)
	nFincList_Insert_Num,	/* �}���n */
	nFincList_Convert_Num,	/* �ϊ��n */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List6��List_Convert)
	nFincList_Search_Num,	/* �����n */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List4��List_Search)
	nFincList_Mode_Num,		/* ���[�h�؂�ւ��n */	//Oct. 16, 2000 JEPRO �ϐ����ύX(List8��List_Mode)
	nFincList_Set_Num,		/* �ݒ�n */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List9��List_Set)
	nFincList_Macro_Num,	/* �}�N���n */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List10��List_Macro)
//	�J�X�^�����j���[�̕�����𓮓I�ɕύX�\�ɂ��邽�߂���͍폜
//	nFincList_Menu_Num,		/* �J�X�^�����j���[ */	//Oct. 21, 2000 JEPRO �u���̑��v���番���Ɨ���
	nFincList_Win_Num,		/* �E�B���h�E�n */		//Oct. 16, 2000 JEPRO �ϐ����ύX(List7��List_Win)
	nFincList_Support_Num,	/* �x�� */				//Oct. 16, 2000 JEPRO �ϐ����ύX(List11��List_Support)
	nFincList_Others_Num	/* ���̑� */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List12��List_Others)
};
const EFunctionCode* nsFuncCode::ppnFuncListArr[] = {
//	pnFuncList_Undef,	//Oct. 14, 2000 JEPRO �u--����`--�v��\�������Ȃ��悤�ɕύX	//Oct. 16, 2000 JEPRO �ϐ����ύX(List0��List_Undef)
	pnFuncList_File,	/* �t�@�C������n */	//Oct. 16, 2000 JEPRO �ϐ����ύX(List5��List_File)
	pnFuncList_Edit,	/* �ҏW�n */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List3��List_Edit)
	pnFuncList_Move,	/* �J�[�\���ړ��n */	//Oct. 16, 2000 JEPRO �ϐ����ύX(List1��List_Move)
	pnFuncList_Select,/* �I���n */			//Oct. 15, 2000 JEPRO �u�J�[�\���ړ��n�v����(�I��)���ړ�  (��`�I��)�͐V�݂��ꎟ�悱���ɂ���
	pnFuncList_Box,	/* ��`�I���n */		//Oct. 17, 2000 JEPRO (��`�I��)���V�݂��ꎟ�悱���ɂ���
	pnFuncList_Clip,	/* �N���b�v�{�[�h�n */	//Oct. 16, 2000 JEPRO �ϐ����ύX(List2��List_Clip)
	pnFuncList_Insert,/* �}���n */
	pnFuncList_Convert,/* �ϊ��n */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List6��List_Convert)
	pnFuncList_Search,/* �����n */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List4��List_Search)
	pnFuncList_Mode,	/* ���[�h�؂�ւ��n */	//Oct. 16, 2000 JEPRO �ϐ����ύX(List8��List_Mode)
	pnFuncList_Set,	/* �ݒ�n */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List9��List_Set)
	pnFuncList_Macro,	/* �}�N���n */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List10��List_Macro)
//	�J�X�^�����j���[�̕�����𓮓I�ɕύX�\�ɂ��邽�߂���͍폜
//	pnFuncList_Menu,	/* �J�X�^�����j���[ */	//Oct. 21, 2000 JEPRO�u���̑��v���番���Ɨ���
	pnFuncList_Win,	/* �E�B���h�E�n */		//Oct. 16, 2000 JEPRO �ϐ����ύX(List7��List_Win)
	pnFuncList_Support,/* �x�� */				//Oct. 16, 2000 JEPRO �ϐ����ύX(List11��List_Support)
	pnFuncList_Others	/* ���̑� */			//Oct. 16, 2000 JEPRO �ϐ����ύX(List12��List_Others)
};
const int nsFuncCode::nFincListNumArrNum = _countof( nsFuncCode::pnFuncListNumArr );




/* �@�\�����p�\�����ׂ� */
bool IsFuncEnable( CEditDoc* pcEditDoc, DLLSHAREDATA* pShareData, EFunctionCode nId )
{
	/* ���������֎~�̂Ƃ����ꊇ�`�F�b�N */
	if( pcEditDoc->IsModificationForbidden( nId ) )
		return false;

	switch( nId ){
	case F_RECKEYMACRO:	/* �L�[�}�N���̋L�^�J�n�^�I�� */
		if( pShareData->m_sFlags.m_bRecordingKeyMacro ){	/* �L�[�{�[�h�}�N���̋L�^�� */
			if( pShareData->m_sFlags.m_hwndRecordingKeyMacro == CEditWnd::getInstance()->GetHwnd() ){	/* �L�[�{�[�h�}�N�����L�^���̃E�B���h�E */
				return true;
			}else{
				return false;
			}
		}else{
			return true;
		}
	case F_SAVEKEYMACRO:	/* �L�[�}�N���̕ۑ� */
		//	Jun. 16, 2002 genta
		//	�L�[�}�N���G���W���ȊO�̃}�N����ǂݍ���ł���Ƃ���
		//	���s�͂ł��邪�ۑ��͂ł��Ȃ��D
		if( pShareData->m_sFlags.m_bRecordingKeyMacro ){	/* �L�[�{�[�h�}�N���̋L�^�� */
			if( pShareData->m_sFlags.m_hwndRecordingKeyMacro == CEditWnd::getInstance()->GetHwnd() ){	/* �L�[�{�[�h�}�N�����L�^���̃E�B���h�E */
				return true;
			}else{
				return false;
			}
		}else{
			return CEditApp::getInstance()->m_pcSMacroMgr->IsSaveOk();
		}
	case F_EXECKEYMACRO:	/* �L�[�}�N���̎��s */
		if( pShareData->m_sFlags.m_bRecordingKeyMacro ){	/* �L�[�{�[�h�}�N���̋L�^�� */
			if( pShareData->m_sFlags.m_hwndRecordingKeyMacro == CEditWnd::getInstance()->GetHwnd() ){	/* �L�[�{�[�h�}�N�����L�^���̃E�B���h�E */
				return true;
			}else{
				return false;
			}
		}else{
			//@@@ 2002.1.24 YAZAKI m_szKeyMacroFileName�Ƀt�@�C�������R�s�[����Ă��邩�ǂ����B
			if (pShareData->m_Common.m_sMacro.m_szKeyMacroFileName[0] ) {
				return true;
			}else{
				return false;
			}
		}
	case F_LOADKEYMACRO:	/* �L�[�}�N���̓ǂݍ��� */
		if( pShareData->m_sFlags.m_bRecordingKeyMacro ){	/* �L�[�{�[�h�}�N���̋L�^�� */
			if( pShareData->m_sFlags.m_hwndRecordingKeyMacro == CEditWnd::getInstance()->GetHwnd() ){	/* �L�[�{�[�h�}�N�����L�^���̃E�B���h�E */
				return true;
			}else{
				return false;
			}
		}else{
			return true;
		}
	case F_EXECEXTMACRO:	/* ���O���w�肵�ă}�N�����s */
		return true;

	case F_SEARCH_CLEARMARK:	//�����}�[�N�̃N���A
		return true;

	// 02/06/26 ai Start
	case F_JUMP_SRCHSTARTPOS:	// �����J�n�ʒu�֖߂�
		if( pcEditDoc->m_pcEditWnd->GetActiveView().m_ptSrchStartPos_PHY.BothNatural() ){
			return true;
		}else{
			return false;
		}
	// 02/06/26 ai End

	case F_COMPARE:	/* �t�@�C�����e��r */
		if( 2 <= pShareData->m_sNodes.m_nEditArrNum ){
			return true;
		}else{
			return false;
		}

	case F_DIFF_NEXT:	/* ���̍����� */	//@@@ 2002.05.25 MIK
	case F_DIFF_PREV:	/* �O�̍����� */	//@@@ 2002.05.25 MIK
	case F_DIFF_RESET:	/* �����̑S���� */	//@@@ 2002.05.25 MIK
		if( !CDiffManager::getInstance()->IsDiffUse() ) return false;
		return true;
	case F_DIFF_DIALOG:	/* DIFF�����\�� */	//@@@ 2002.05.25 MIK
		//if( pcEditDoc->IsModified() ) return false;
		//if( ! pcEditDoc->m_cDocFile.GetFilePathClass().IsValidPath() ) return false;
		return true;

	case F_BEGIN_BOX:	//��`�͈͑I���J�n
	case F_UP_BOX:
	case F_DOWN_BOX:
	case F_LEFT_BOX:
	case F_RIGHT_BOX:
	case F_UP2_BOX:
	case F_DOWN2_BOX:
	case F_WORDLEFT_BOX:
	case F_WORDRIGHT_BOX:
	case F_GOLOGICALLINETOP_BOX:
	case F_GOLINETOP_BOX:
	case F_GOLINEEND_BOX:
	case F_HalfPageUp_BOX:
	case F_HalfPageDown_BOX:
	case F_1PageUp_BOX:
	case F_1PageDown_BOX:
	case F_GOFILETOP_BOX:
	case F_GOFILEEND_BOX:
		if( pShareData->m_Common.m_sView.m_bFontIs_FIXED_PITCH ){	/* ���݂̃t�H���g�͌Œ蕝�t�H���g�ł��� */
			return true;
		}else{
			return false;
		}
	case F_PASTEBOX:
		/* �N���b�v�{�[�h����\��t���\���H */
		if( pcEditDoc->m_cDocEditor.IsEnablePaste() && pShareData->m_Common.m_sView.m_bFontIs_FIXED_PITCH ){
			return true;
		}else{
			return false;
		}
	case F_PASTE:
		/* �N���b�v�{�[�h����\��t���\���H */
		if( pcEditDoc->m_cDocEditor.IsEnablePaste() ){
			return true;
		}else{
			return false;
		}

	case F_FILENEW:	/* �V�K�쐬 */
	case F_GREP_DIALOG:	/* Grep */
		/* �ҏW�E�B���h�E�̏���`�F�b�N */
		if( pShareData->m_sNodes.m_nEditArrNum >= MAX_EDITWINDOWS ){	//�ő�l�C��	//@@@ 2003.05.31 MIK
			return false;
		}else{
			return true;
		}

	case F_FILESAVE:	/* �㏑���ۑ� */
		if( !CAppMode::getInstance()->IsViewMode() ){	/* �r���[���[�h */
			if( pcEditDoc->m_cDocEditor.IsModified() ){	/* �ύX�t���O */
				return true;
			}
			else if (pcEditDoc->m_cDocFile.IsChgCodeSet()) {	// �����R�[�h�̕ύX
				return true;
			}
			else {
				/* ���ύX�ł��㏑�����邩 */
				if( !pShareData->m_Common.m_sFile.m_bEnableUnmodifiedOverwrite ){
					return false;
				}else{
					return true;
				}
			}
		}else{
			return false;
		}
	case F_COPYLINES:				//�I��͈͓��S�s�R�s�[
	case F_COPYLINESASPASSAGE:		//�I��͈͓��S�s���p���t���R�s�[
	case F_COPYLINESWITHLINENUMBER:	//�I��͈͓��S�s�s�ԍ��t���R�s�[
	case F_COPY_COLOR_HTML:				//�I��͈͓��F�t��HTML�R�s�[
	case F_COPY_COLOR_HTML_LINENUMBER:	//�I��͈͓��s�ԍ��F�t��HTML�R�s�[
		//�e�L�X�g���I������Ă����true
		return pcEditDoc->m_pcEditWnd->GetActiveView().GetSelectionInfo().IsTextSelected();

	case F_TOLOWER:					/* ������ */
	case F_TOUPPER:					/* �啶�� */
	case F_TOHANKAKU:				/* �S�p�����p */
	case F_TOHANKATA:				/* �S�p�J�^�J�i�����p�J�^�J�i */	//Aug. 29, 2002 ai
	case F_TOZENEI:					/* ���p�p�����S�p�p�� */			//July. 30, 2001 Misaka
	case F_TOHANEI:					/* �S�p�p�������p�p�� */
	case F_TOZENKAKUKATA:			/* ���p�{�S�Ђ灨�S�p�E�J�^�J�i */	//Sept. 17, 2000 jepro �������u���p���S�p�J�^�J�i�v����ύX
	case F_TOZENKAKUHIRA:			/* ���p�{�S�J�^���S�p�E�Ђ炪�� */	//Sept. 17, 2000 jepro �������u���p���S�p�Ђ炪�ȁv����ύX
	case F_HANKATATOZENKATA:		/* ���p�J�^�J�i���S�p�J�^�J�i */
	case F_HANKATATOZENHIRA:		/* ���p�J�^�J�i���S�p�Ђ炪�� */
	case F_TABTOSPACE:				/* TAB���� */
	case F_SPACETOTAB:				/* �󔒁�TAB */  //---- Stonee, 2001/05/27
	case F_CODECNV_AUTO2SJIS:		/* �������ʁ�SJIS�R�[�h�ϊ� */
	case F_CODECNV_EMAIL:			/* E-Mail(JIS��SJIS)�R�[�h�ϊ� */
	case F_CODECNV_EUC2SJIS:		/* EUC��SJIS�R�[�h�ϊ� */
	case F_CODECNV_UNICODE2SJIS:	/* Unicode��SJIS�R�[�h�ϊ� */
	case F_CODECNV_UNICODEBE2SJIS:	/* UnicodeBE��SJIS�R�[�h�ϊ� */
	case F_CODECNV_UTF82SJIS:		/* UTF-8��SJIS�R�[�h�ϊ� */
	case F_CODECNV_UTF72SJIS:		/* UTF-7��SJIS�R�[�h�ϊ� */
	case F_CODECNV_SJIS2JIS:		/* SJIS��JIS�R�[�h�ϊ� */
	case F_CODECNV_SJIS2EUC:		/* SJIS��EUC�R�[�h�ϊ� */
	case F_CODECNV_SJIS2UTF8:		/* SJIS��UTF-8�R�[�h�ϊ� */
	case F_CODECNV_SJIS2UTF7:		/* SJIS��UTF-7�R�[�h�ϊ� */
	case F_BASE64DECODE:			/* Base64�f�R�[�h���ĕۑ� */
	case F_UUDECODE:				//uudecode���ĕۑ�	//Oct. 17, 2000 jepro �������u�I�𕔕���UUENCODE�f�R�[�h�v����ύX
		// �e�L�X�g���I������Ă����true
		return pcEditDoc->m_pcEditWnd->GetActiveView().GetSelectionInfo().IsTextSelected();

	case F_CUT_LINE:	//�s�؂���(�܂�Ԃ��P��)
	case F_DELETE_LINE:	//�s�폜(�܂�Ԃ��P��)
		// �e�L�X�g���I������Ă��Ȃ����true
		return !pcEditDoc->m_pcEditWnd->GetActiveView().GetSelectionInfo().IsTextSelected();

	case F_UNDO:		return pcEditDoc->m_cDocEditor.IsEnableUndo();	/* Undo(���ɖ߂�)�\�ȏ�Ԃ��H */
	case F_REDO:		return pcEditDoc->m_cDocEditor.IsEnableRedo();	/* Redo(��蒼��)�\�ȏ�Ԃ��H */

	case F_COPYPATH:
	case F_COPYTAG:
	case F_COPYFNAME:					// 2002/2/3 aroka
	case F_OPEN_HfromtoC:				//������C/C++�w�b�_(�\�[�X)���J��	//Feb. 7, 2001 JEPRO �ǉ�
//	case F_OPEN_HHPP:					//������C/C++�w�b�_�t�@�C�����J��	//Feb. 9, 2001 jepro�u.c�܂���.cpp�Ɠ�����.h���J���v����ύX		del 2008/6/23 Uchi
//	case F_OPEN_CCPP:					//������C/C++�\�[�X�t�@�C�����J��	//Feb. 9, 2001 jepro�u.h�Ɠ�����.c(�Ȃ����.cpp)���J���v����ύX	del 2008/6/23 Uchi
	case F_PLSQL_COMPILE_ON_SQLPLUS:	/* Oracle SQL*Plus�Ŏ��s */
	case F_BROWSE:						//�u���E�Y
	//case F_VIEWMODE:					//�r���[���[�h	//	Sep. 10, 2002 genta ��Ɏg����悤��
	//case F_PROPERTY_FILE:				//�t�@�C���̃v���p�e�B	// 2009.04.11 ryoji �R�����g�A�E�g
		return pcEditDoc->m_cDocFile.GetFilePathClass().IsValidPath();	// ���ݕҏW���̃t�@�C���̃p�X�����N���b�v�{�[�h�ɃR�s�[�ł��邩

	case F_JUMPHIST_PREV:	//	�ړ�����: �O��
		if( pcEditDoc->m_pcEditWnd->GetActiveView().m_cHistory->CheckPrev() )
			return true;
		else
			return false;
	case F_JUMPHIST_NEXT:	//	�ړ�����: ����
		if( pcEditDoc->m_pcEditWnd->GetActiveView().m_cHistory->CheckNext() )
			return true;
		else
			return false;
	case F_JUMPHIST_SET:	//	���݈ʒu���ړ������ɓo�^
		return true;
	// 20100402 Moca (����)���_�C���N�g�^�O�W�����v�ł���悤��
	case F_DIRECT_TAGJUMP:	//�_�C���N�g�^�O�W�����v	//@@@ 2003.04.15 MIK
	case F_SYMBOL_SEARCH:	//�V���{������(�L�[���[�h�w��^�O�W�����v)
	//	2003.05.12 MIK �^�O�t�@�C���쐬���I�ׂ�悤�ɂ����̂ŁA��ɍ쐬�\�Ƃ���
//	case F_TAGS_MAKE:	//�^�O�t�@�C���̍쐬	//@@@ 2003.04.13 MIK
		if( false == CEditApp::getInstance()->m_pcGrepAgent->m_bGrepMode
			&& pcEditDoc->m_cDocFile.GetFilePathClass().IsValidPath() ){
			return true;
		}else{
			return false;
		}
	
	//�^�u���[�h���̓E�C���h�E���בւ��֎~�ł��B	@@@ 2003.06.12 MIK
	case F_TILE_H:
	case F_TILE_V:
	case F_CASCADE:
		//Start 2004.07.15 Kazika �^�u�E�B���h�������s�\
		return true;
		//End 2004.07.15 Kazika
	case F_BIND_WINDOW:	//2004.07.14 Kazika �V�K�ǉ�
	case F_TAB_MOVERIGHT:	// 2007.06.20 ryoji �ǉ�
	case F_TAB_MOVELEFT:	// 2007.06.20 ryoji �ǉ�
	case F_TAB_CLOSELEFT:	// 2009.12.26 syat �ǉ�
	case F_TAB_CLOSERIGHT:	// 2009.12.26 syat �ǉ�
		//��^�u���[�h���̓E�B���h�E���������ĕ\���ł��Ȃ�
		return pShareData->m_Common.m_sTabBar.m_bDispTabWnd != FALSE;
	case F_GROUPCLOSE:		// 2007.06.20 ryoji �ǉ�
	case F_NEXTGROUP:		// 2007.06.20 ryoji �ǉ�
	case F_PREVGROUP:		// 2007.06.20 ryoji �ǉ�
		return ( pShareData->m_Common.m_sTabBar.m_bDispTabWnd && !pShareData->m_Common.m_sTabBar.m_bDispTabWndMultiWin );
	case F_TAB_SEPARATE:	// 2007.06.20 ryoji �ǉ�
	case F_TAB_JOINTNEXT:	// 2007.06.20 ryoji �ǉ�
	case F_TAB_JOINTPREV:	// 2007.06.20 ryoji �ǉ�
	case F_FILENEW_NEWWINDOW:	// 2011.11.15 syat �ǉ�
		return ( pShareData->m_Common.m_sTabBar.m_bDispTabWnd && !pShareData->m_Common.m_sTabBar.m_bDispTabWndMultiWin );
	}
	return true;
}



/* �@�\���`�F�b�N��Ԃ����ׂ� */
bool IsFuncChecked( CEditDoc* pcEditDoc, DLLSHAREDATA*	pShareData, EFunctionCode nId )
{
	CEditWnd* pCEditWnd;
	// Modified by KEITA for WIN64 2003.9.6
	pCEditWnd = ( CEditWnd* )::GetWindowLongPtr( CEditWnd::getInstance()->GetHwnd(), GWLP_USERDATA );
//@@@ 2002.01.14 YAZAKI ����v���r���[��CPrintPreview�ɓƗ����������Ƃɂ��A�v���r���[����폜
	ECodeType eDocCode = pcEditDoc->GetDocumentEncoding();
	switch( nId ){
	case F_FILE_REOPEN_SJIS:		return CODE_SJIS == eDocCode;
	case F_FILE_REOPEN_JIS:			return CODE_JIS == eDocCode;
	case F_FILE_REOPEN_EUC:			return CODE_EUC == eDocCode;
	case F_FILE_REOPEN_LATIN1:		return CODE_LATIN1 == eDocCode;		// 2010/3/20 Uchi
	case F_FILE_REOPEN_UNICODE:		return CODE_UNICODE == eDocCode;
	case F_FILE_REOPEN_UNICODEBE:	return CODE_UNICODEBE == eDocCode;
	case F_FILE_REOPEN_UTF8:		return CODE_UTF8 == eDocCode;
	case F_FILE_REOPEN_CESU8:		return CODE_CESU8 == eDocCode;
	case F_FILE_REOPEN_UTF7:		return CODE_UTF7 == eDocCode;
	case F_RECKEYMACRO:	/* �L�[�}�N���̋L�^�J�n�^�I�� */
		if( pShareData->m_sFlags.m_bRecordingKeyMacro ){	/* �L�[�{�[�h�}�N���̋L�^�� */
			if( pShareData->m_sFlags.m_hwndRecordingKeyMacro == CEditWnd::getInstance()->GetHwnd() ){	/* �L�[�{�[�h�}�N�����L�^���̃E�B���h�E */
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	case F_SHOWTOOLBAR:			return pCEditWnd->m_cToolbar.GetToolbarHwnd() != NULL;
	case F_SHOWFUNCKEY:			return pCEditWnd->m_CFuncKeyWnd.GetHwnd() != NULL;
	case F_SHOWTAB:				return pCEditWnd->m_cTabWnd.GetHwnd() != NULL;	//@@@ 2003.06.10 MIK
	case F_SHOWSTATUSBAR:		return pCEditWnd->m_cStatusBar.GetStatusHwnd() != NULL;
	// 2008.05.30 nasukoji	�e�L�X�g�̐܂�Ԃ����@
	case F_TMPWRAPNOWRAP:		return ( pcEditDoc->m_nTextWrapMethodCur == WRAP_NO_TEXT_WRAP );		// �܂�Ԃ��Ȃ�
	case F_TMPWRAPSETTING:		return ( pcEditDoc->m_nTextWrapMethodCur == WRAP_SETTING_WIDTH );		// �w�茅�Ő܂�Ԃ�
	case F_TMPWRAPWINDOW:		return ( pcEditDoc->m_nTextWrapMethodCur == WRAP_WINDOW_WIDTH );		// �E�[�Ő܂�Ԃ�
	// 2009.07.06 syat  �����J�E���g���@
	case F_SELECT_COUNT_MODE:	return ( pCEditWnd->m_nSelectCountMode == SELECT_COUNT_TOGGLE ?
											pShareData->m_Common.m_sStatusbar.m_bDispSelCountByByte == TRUE :
											pCEditWnd->m_nSelectCountMode == SELECT_COUNT_BY_BYTE );
	// Mar. 6, 2002 genta
	case F_VIEWMODE:			return CAppMode::getInstance()->IsViewMode(); //�r���[���[�h
	//	From Here 2003.06.23 Moca
	case F_CHGMOD_EOL_CRLF:		return EOL_CRLF == pcEditDoc->m_cDocEditor.GetNewLineCode();
	case F_CHGMOD_EOL_LF:		return EOL_LF == pcEditDoc->m_cDocEditor.GetNewLineCode();
	case F_CHGMOD_EOL_CR:		return EOL_CR == pcEditDoc->m_cDocEditor.GetNewLineCode();
	//	To Here 2003.06.23 Moca
	//	2003.07.21 genta
	case F_CHGMOD_INS:			return pcEditDoc->m_cDocEditor.IsInsMode();	//	Oct. 2, 2005 genta �}�����[�h�̓h�L�������g���ɕ⊮����悤�ɕύX����
	case F_TOGGLE_KEY_SEARCH:	return pShareData->m_Common.m_sSearch.m_bUseCaretKeyWord != FALSE;	//	2007.02.03 genta �L�[���[�h�|�b�v�A�b�v��ON/OFF��Ԃ𔽉f����
	case F_BIND_WINDOW:			return ((pShareData->m_Common.m_sTabBar.m_bDispTabWnd) && !(pShareData->m_Common.m_sTabBar.m_bDispTabWndMultiWin));	//2004.07.14 Kazika �ǉ�
	case F_TOPMOST:				return ((DWORD)::GetWindowLongPtr( pCEditWnd->GetHwnd(), GWL_EXSTYLE ) & WS_EX_TOPMOST) != 0;	// 2004.09.21 Moca
	// Jan. 10, 2004 genta �C���N�������^���T�[�`
	case F_ISEARCH_NEXT:
	case F_ISEARCH_PREV:
	case F_ISEARCH_REGEXP_NEXT:
	case F_ISEARCH_REGEXP_PREV:
	case F_ISEARCH_MIGEMO_NEXT:
	case F_ISEARCH_MIGEMO_PREV:
		return pcEditDoc->m_pcEditWnd->GetActiveView().IsISearchEnabled( nId );
	case F_OUTLINE_TOGGLE: // 20060201 aroka �A�E�g���C���E�B���h�E
		// ToDo:�u�b�N�}�[�N���X�g���o�Ă���Ƃ����ւ���ł��܂��B
		return pcEditDoc->m_pcEditWnd->m_cDlgFuncList.GetHwnd() != NULL;
	}
	//End 2004.07.14 Kazika

	return false;
}
