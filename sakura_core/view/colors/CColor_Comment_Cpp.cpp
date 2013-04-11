/*!	@file	CColor_Comment_Cpp.cpp
	@brief	C++ �R�����g����N���X

	@author	Suzuki Satoshi
*/
/*
	Copyright (C) 2012, Suzuki Satoshi

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
#include "view/CEditView.h" // SColorStrategyInfo
#include "CColor_Comment_Cpp.h"
#include "doc/CLayout.h"


bool CColor_Comment_Cpp::BeginColor(const CStringRef& rStr, int pos, int& rCommentNestLevel)
{
	if( !rStr.IsValid() ){
		return false;
	}

	const CEditDoc* pEditDoc = CEditDoc::GetInstance(0);
	const STypeConfig& rTypeSetting = pEditDoc->m_cDocType.GetDocumentAttribute();

	if( rTypeSetting.m_ColorInfoArr[COLORIDX_COMMENT2].m_bDisp
	 && Match_CommentFrom( pos, rStr, rCommentNestLevel ) )
	{
		this->m_CommentEndPos = Match_CommentTo( pos + wcslen( L"#if 0" ), rStr, rCommentNestLevel);

		return true;
	}

	return false;
}


bool CColor_Comment_Cpp::EndColor(const CStringRef& rStr, int pos, int& rCommentNestLevel)
{
	if( this->m_CommentEndPos == 0 ){
		this->m_CommentEndPos = Match_CommentTo( pos, rStr, rCommentNestLevel );
	} else if( pos == this->m_CommentEndPos ){
		return true;
	}
	return false;
}

/*!
	n�Ԗڂ̃u���b�N�R�����g�́Apos����̕����񂪊J�n������(From)�ɓ��Ă͂܂邩�m�F����B

	@retval true  ��v����
	@retval false ��v���Ȃ�����
*/
bool CColor_Comment_Cpp::Match_CommentFrom
	(
		int					pos,		//!< [in] �T���J�n�ʒu
		const CStringRef&	rStr,		//!< [in] �T���Ώە����� ���T���J�n�ʒu�̃|�C���^�ł͂Ȃ����Ƃɒ���
		int& rCommentNestLevel			//!< [out] �l�X�g��
	)
{
	int len;

	len  = wcslen( L"#if 0" );
	if( ( pos <= rStr.GetLength() - len )
	 && ( wmemicmp( &rStr.GetPtr()[pos], L"#if 0", len ) == 0 ) )
	{
		rCommentNestLevel = 1;
		return true;
	}
	return false;
}


/*!
	n�Ԗڂ̃u���b�N�R�����g�́A���(To)�ɓ��Ă͂܂镶�����pos�ȍ~����T��

	@return ���Ă͂܂����ʒu��Ԃ����A���Ă͂܂�Ȃ������Ƃ��́AnLineLen�����̂܂ܕԂ��B
*/
int CColor_Comment_Cpp::Match_CommentTo
	(
		int					pos,		//!< [in] �T���J�n�ʒu
		const CStringRef&	rStr,		//!< [in] �T���Ώە����� ���T���J�n�ʒu�̃|�C���^�ł͂Ȃ����Ƃɒ���
		int& rCommentNestLevel			//!< [in, out] �l�X�g��
	)
{
	int i;
	int targetLen;
	int len;

	targetLen = rStr.GetLength();
	for( i = pos; i <= (targetLen - (int)wcslen( L"//" )); i++ ){
		if( wmemicmp( &rStr.GetPtr()[i], L"/*", 2 ) == 0 ){
			i += 2;
			for( ; i <= (targetLen - (int)wcslen( L"*/" )); i++ ){
				if( wmemicmp( &rStr.GetPtr()[i], L"*/", 2 ) == 0 ){
					break;
				}
			}
		}

		if( wmemicmp( &rStr.GetPtr()[i], L"//", 2 ) == 0 ){
			return targetLen;
		}

		len  = wcslen( L"#if " );
		if( wmemicmp( &rStr.GetPtr()[i], L"#if ", len ) == 0 ){
			rCommentNestLevel++;
		}

		len  = wcslen( L"#if\t" );
		if( wmemicmp( &rStr.GetPtr()[i], L"#if\t", len ) == 0 ){
			rCommentNestLevel++;
		}

		len  = wcslen( L"#ifdef" );
		if( wmemicmp( &rStr.GetPtr()[i], L"#ifdef", len ) == 0 ){
			rCommentNestLevel++;
		}

		len  = wcslen( L"#ifndef" );
		if( wmemicmp( &rStr.GetPtr()[i], L"#ifndef", len ) == 0 ){
			rCommentNestLevel++;
		}

		len  = wcslen( L"$endif" );
		if( wmemicmp( &rStr.GetPtr()[i], L"#endif", len ) == 0 ){
			rCommentNestLevel--;
			if( rCommentNestLevel == 0 ){
				return i + len;
			}
		}

		len  = wcslen( L"#else" );
		if( wmemicmp( &rStr.GetPtr()[i], L"#else", len ) == 0 ){
			if( rCommentNestLevel == 1 ){
				rCommentNestLevel = 0;
				return i + len;
			}
		}

		len  = wcslen( L"#elif" );
		if( wmemicmp( &rStr.GetPtr()[i], L"#elif", len ) == 0 ){
			if( rCommentNestLevel == 1 ){
				rCommentNestLevel = 0;
				return i + len;
			}
		}
	}

	return targetLen;
}

