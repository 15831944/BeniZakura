/*!	@file	CColor_Comment_Cpp.cpp
	@brief	C++ #if 0 �` #else �` #endif �R�����g����N���X

	@author	Suzuki Satoshi
*/
/*
	Copyright (C) 2012, 2013, 2014, Suzuki Satoshi

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
#include "view/CEditView.h"
#include "CColor_Comment_Cpp_If0.h"

/*!
	C++ �v���v���Z�b�T "#if 0" �J�n����

	@retval true: "#if 0" �����o����
	@retval false: ���o���Ȃ�����
 */
bool CColor_Comment_Cpp_If0::BeginColor( const CStringRef& rStr, int pos, ColorStrategyState& rColorStrategyState )
{
	if( !rStr.IsValid( ) ){
		return false;
	}

	const CEditDoc* pEditDoc = CEditDoc::GetInstance( 0 );
	const STypeConfig& rTypeSetting = pEditDoc->m_cDocType.GetDocumentAttribute( );

	if( rTypeSetting.m_ColorInfoArr[COLORIDX_COMMENT2].m_bDisp == false ){
		return false;
	}

	if( Match_CommentFrom( pos, rStr, rColorStrategyState ) ){
		this->commentEndPos = Match_CommentTo( pos + wcslen( L"#if 0" ), rStr, rColorStrategyState );

		return true;
	}

	return false;
}
/*!
	C++ �v���v���Z�b�T�ɂ��R�����g�A�E�g�̏I������

	@retval true: �R�����g�A�E�g�I�������o����
	@retval false: ���o���Ȃ�����
 */
bool CColor_Comment_Cpp_If0::EndColor( const CStringRef& rStr, int pos, ColorStrategyState& rColorStrategyState )
{
	if( this->commentEndPos == 0 ){
		this->commentEndPos = Match_CommentTo( pos, rStr, rColorStrategyState );
	}else if( pos == this->commentEndPos ){
		return true;
	}

	return false;
}

/*!
	"#if 0" �v���v���Z�b�T �R�����g�J�n����p�̕������r����

	@retval true: "#if 0" �����o����
	@retval false: ���o���Ȃ�����
*/
bool CColor_Comment_Cpp_If0::Match_CommentFrom
	(
		int pos,								//!< [in] �T���J�n�ʒu
		const CStringRef& rStr,					//!< [in] �T���Ώە����� ���T���J�n�ʒu�̃|�C���^�ł͂Ȃ����Ƃɒ���
		ColorStrategyState& rColorStrategyState //!< [in, out] ���( #if 0 �l�X�g���x��)
	)
{
	int len;

	len = wcslen( L"#if 0" );
	if( ( pos <= rStr.GetLength() - len )
	 && ( wmemicmp( &rStr.GetPtr()[pos], L"#if 0", len ) == 0 ) )
	{
		rColorStrategyState.cppPreprocessorIf0NestLevel = 1;
		return true;
	}

	return false;
}


/*!
	"#if 0" �v���v���Z�b�T �R�����g�I������p�̕������r����

	@return �R�����g�A�E�g�I���ʒu��Ԃ��B�I�����������o����Ȃ������ꍇ�ɂ́AnLineLen �����̂܂ܕԂ��B
*/
int CColor_Comment_Cpp_If0::Match_CommentTo
	(
		int pos,								//!< [in] �T���J�n�ʒu
		const CStringRef& rStr,					//!< [in] �T���Ώە����� ���T���J�n�ʒu�̃|�C���^�ł͂Ȃ����Ƃɒ���
		ColorStrategyState& rColorStrategyState	//!< [in, out] ���( #if 0 �l�X�g���x��)
	)
{
	int i;
	int targetLen;
	int len;

	targetLen = rStr.GetLength( );
	for( i = pos; i <= ( targetLen - (int)wcslen( L"//" ) ); i++ ){
		if( wmemicmp( &rStr.GetPtr( )[i], L"/*", 2 ) == 0 ){
			i += 2;
			for(; i <= ( targetLen - (int)wcslen( L"*/" ) ); i++ ){
				if( wmemicmp( &rStr.GetPtr( )[i], L"*/", 2 ) == 0 ){
					break;
				}
			}
		}

		if( wmemicmp( &rStr.GetPtr( )[i], L"//", 2 ) == 0 ){
			return targetLen;
		}

		len = wcslen( L"#if " );
		if( wmemicmp( &rStr.GetPtr( )[i], L"#if ", len ) == 0 ){
			rColorStrategyState.cppPreprocessorIf0NestLevel++;
		}

		len = wcslen( L"#if\t" );
		if( wmemicmp( &rStr.GetPtr( )[i], L"#if\t", len ) == 0 ){
			rColorStrategyState.cppPreprocessorIf0NestLevel++;
		}

		len = wcslen( L"#ifdef" );
		if( wmemicmp( &rStr.GetPtr( )[i], L"#ifdef", len ) == 0 ){
			rColorStrategyState.cppPreprocessorIf0NestLevel++;
		}

		len = wcslen( L"#ifndef" );
		if( wmemicmp( &rStr.GetPtr( )[i], L"#ifndef", len ) == 0 ){
			rColorStrategyState.cppPreprocessorIf0NestLevel++;
		}

		len = wcslen( L"$endif" );
		if( wmemicmp( &rStr.GetPtr( )[i], L"#endif", len ) == 0 ){
			rColorStrategyState.cppPreprocessorIf0NestLevel--;
			if (rColorStrategyState.cppPreprocessorIf0NestLevel == 0){
				return i + len;
			}
		}

		len = wcslen( L"#else" );
		if( wmemicmp( &rStr.GetPtr( )[i], L"#else", len ) == 0 ){
			if (rColorStrategyState.cppPreprocessorIf0NestLevel == 1){
				rColorStrategyState.cppPreprocessorIf0NestLevel = 0;
				return i + len;
			}
		}

		len = wcslen( L"#elif" );
		if( wmemicmp( &rStr.GetPtr( )[i], L"#elif", len ) == 0 ){
			if (rColorStrategyState.cppPreprocessorIf0NestLevel == 1){
				rColorStrategyState.cppPreprocessorIf0NestLevel = 0;
				return i + len;
			}
		}
	}

	return targetLen;
}
