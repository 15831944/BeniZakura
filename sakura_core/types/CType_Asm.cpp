#include "StdAfx.h"
#include "types/CType.h"
#include "doc/CEditDoc.h"
#include "doc/CDocOutline.h"
#include "doc/logic/CDocLine.h"
#include "outline/CFuncInfoArr.h"
#include "view/Colors/EColorIndexType.h"

/*!
 *	GNU Assembler �L�[���[�h
 */
static const wchar_t* g_defaultKeywordSetGas[] = {
	L".abort",
	L".align",
	L".ascii",
	L".asciz",
	L".balign",
	L".byte",
	L".comm",
	L".data",
	L".def",
	L".desc",
	L".dim",
	L".double",
	L".eject",
	L".else",
	L".elseif",
	L".end",
	L".endef",
	L".endfunc",
	L".endif",
	L".equ",
	L".equiv",
	L".err",
	L".exitm",
	L".extern",
	L".fail",
	L".file",
	L".fill",
	L".float",
	L".func",
	L".global",
	L".globl",
	L".hidden",
	L".hword",
	L".ident",
	L".if",
	L".incbin",
	L".include",
	L".int",
	L".internal",
	L".irp",
	L".irpc",
	L".lcomm",
	L".lflags",
	L".line",
	L".ln",
	L".linkonce",
	L".list",
	L".long",
	L".macro",
	L".mri",
	L".nolist",
	L".octa",
	L".org",
	L".p2align",
	L".popsection",
	L".previous",
	L".print",
	L".protected",
	L".psize",
	L".purgem",
	L".pushsection",
	L".quad",
	L".rept",
	L".sbttl",
	L".scl",
	L".section",
	L".set",
	L".short",
	L".single",
	L".size",
	L".skip",
	L".sleb128",
	L".space",
	L".stabd",
	L".stabn",
	L".stabs",
	L".string",
	L".struct",
	L".subsection",
	L".symver",
	L".tag",
	L".text",
	L".title",
	L".type",
	L".uleb128",
	L".val",
	L".version",
	L".vtable_entry",
	L".vtable_inherit",
	L".weak",
	L".word"
};

/*!
 *	GNU Assembler �v���v���Z�b�T �L�[���[�h
 */
static const wchar_t* g_defaultKeywordSetGasPreprocessor[] = {
	L"#define",
	L"#elif",
	L"#else",
	L"#endif",
	L"#error",
	L"#if",
	L"#ifdef",
	L"#ifndef",
	L"#include",
	L"#line",
	L"#pragma",
	L"#undef"
};

/*!
 *	�A�Z���u�� �^�C�v�ʐݒ�̃f�t�H���g�l��ݒ肷��
 *
 *	@return �Ȃ�
 */
void CType_Asm::InitTypeConfigImp(STypeConfig* pType)
{
	//���O�Ɗg���q
	_tcscpy( pType->m_szTypeName, _T("�A�Z���u��") );
	_tcscpy( pType->m_szTypeExts, _T("asm,s") );

	//�ݒ�
	pType->m_cLineComment.CopyTo( 0, L";", -1 );							/* �s�R�����g�f���~�^ */
	pType->m_cBlockComments[0].SetBlockCommentRule( L"/*", L"*/" );			/* �u���b�N�R�����g�f���~�^ */
	pType->m_eDefaultOutline = OUTLINE_ASM;									/* �A�E�g���C����͕��@ */
	pType->m_eSmartIndent = SMARTINDENT_NONE;								/* �X�}�[�g�C���f���g��� */
	pType->m_ColorInfoArr[COLORIDX_DIGIT].m_bDisp = true;					/* ���p���l��F�����\�� */
	pType->m_ColorInfoArr[COLORIDX_COMMENT2].m_bDisp = true;				/* C/C++ �v���v���Z�b�T�ɂ��R�����g�A�E�g�u���b�N */

	pType->m_nKeyWordSetIdx[0] = AddDefaultKeywordSet(
										L"Assembler",
										true,
										_countof(g_defaultKeywordSetGas),
										g_defaultKeywordSetGas
									);

	pType->m_nKeyWordSetIdx[1] = AddDefaultKeywordSet(
										L"GNU Assembler Preprocessor",
										true,
										_countof(g_defaultKeywordSetGasPreprocessor),
										g_defaultKeywordSetGasPreprocessor
									);
}



/*! �A�Z���u�� �A�E�g���C�����

	@author MIK
	@date 2004.04.12 ��蒼��
*/
void CDocOutline::MakeTopicList_asm( CFuncInfoArr* pcFuncInfoArr )
{
	CLogicInt nTotalLine;

	nTotalLine = m_pcDocRef->m_cDocLineMgr.GetLineCount();

	for( CLogicInt nLineCount = CLogicInt(0); nLineCount < nTotalLine; nLineCount++ ){
		const WCHAR* pLine;
		CLogicInt nLineLen;
		WCHAR* pTmpLine;
		int length;
		int offset;
#define MAX_ASM_TOKEN 2
		WCHAR* token[MAX_ASM_TOKEN];
		int j;
		WCHAR* p;

		//1�s�擾����B
		pLine = m_pcDocRef->m_cDocLineMgr.GetLine(nLineCount)->GetDocLineStrWithEOL(&nLineLen);
		if( pLine == NULL ) break;

		//��Ɨp�ɃR�s�[���쐬����B�o�C�i�����������炻�̌��͒m��Ȃ��B
		pTmpLine = wcsdup( pLine );
		if( pTmpLine == NULL ) break;
		if( wcslen( pTmpLine ) >= (unsigned int)nLineLen ){	//�o�C�i�����܂�ł�����Z���Ȃ�̂�...
			pTmpLine[ nLineLen ] = L'\0';	//�w�蒷�Ő؂�l��
		}

		//�s�R�����g�폜
		p = wcsstr( pTmpLine, L";" );
		if( p ) *p = L'\0';

		length = wcslen( pTmpLine );
		offset = 0;

		//�g�[�N���ɕ���
		for( j = 0; j < MAX_ASM_TOKEN; j++ ) token[ j ] = NULL;
		for( j = 0; j < MAX_ASM_TOKEN; j++ ){
			token[ j ] = my_strtok<WCHAR>( pTmpLine, length, &offset, L" \t\r\n" );
			if( token[ j ] == NULL ) break;
			//�g�[�N���Ɋ܂܂��ׂ������łȂ����H
			if( wcsstr( token[ j ], L"\"") != NULL
			 || wcsstr( token[ j ], L"\\") != NULL
			 || wcsstr( token[ j ], L"'" ) != NULL ){
				token[ j ] = NULL;
				break;
			}
		}

		if( token[ 0 ] != NULL ){	//�g�[�N����1�ȏ゠��
			int nFuncId = -1;
			WCHAR* entry_token = NULL;

			length = wcslen( token[ 0 ] );
			if( length >= 2
			 && token[ 0 ][ length - 1 ] == L':' ){	//���x��
				token[ 0 ][ length - 1 ] = L'\0';
				nFuncId = 51;
				entry_token = token[ 0 ];
			}
			else if( token[ 1 ] != NULL ){	//�g�[�N����2�ȏ゠��
				if( wcsicmp( token[ 1 ], L"proc" ) == 0 ){	//�֐�
					nFuncId = 50;
					entry_token = token[ 0 ];
				}else
				if( wcsicmp( token[ 1 ], L"endp" ) == 0 ){	//�֐��I��
					nFuncId = 52;
					entry_token = token[ 0 ];
				//}else
				//if( my_stricmp( token[ 1 ], _T("macro") ) == 0 ){	//�}�N��
				//	nFuncId = -1;
				//	entry_token = token[ 0 ];
				//}else
				//if( my_stricmp( token[ 1 ], _T("struc") ) == 0 ){	//�\����
				//	nFuncId = -1;
				//	entry_token = token[ 0 ];
				}
			}

			if( nFuncId >= 0 ){
				/*
				  �J�[�\���ʒu�ϊ�
				  �����ʒu(�s������̃o�C�g���A�܂�Ԃ������s�ʒu)
				  ��
				  ���C�A�E�g�ʒu(�s������̕\�����ʒu�A�܂�Ԃ�����s�ʒu)
				*/
				CLayoutPoint ptPos;
				m_pcDocRef->m_cLayoutMgr.LogicToLayout(
					CLogicPoint(0, nLineCount),
					&ptPos
				);
				pcFuncInfoArr->AppendData( nLineCount + CLogicInt(1), ptPos.GetY2() + CLayoutInt(1), entry_token, nFuncId );
			}
		}

		free( pTmpLine );
	}

	return;
}


