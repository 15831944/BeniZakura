#include "StdAfx.h"
#include "view/CEditView.h" // SColorStrategyInfo
#include "CColor_Numeric.h"
#include "parse/CWordParse.h"
#include "util/string_ex2.h"
#include "doc/layout/CLayout.h"
#include "types/CTypeSupport.h"

static int IsNumber( const CStringRef& cStr, int offset );

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         ���p���l                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool CColor_Numeric::BeginColor(const CStringRef& cStr, int nPos)
{
	if(!cStr.IsValid())return false;

	int	nnn;

	if( _IsPosKeywordHead(cStr,nPos) && m_pTypeData->m_ColorInfoArr[COLORIDX_DIGIT].m_bDisp
		&& (nnn = IsNumber(cStr, nPos)) > 0 )		/* ���p������\������ */
	{
		/* �L�[���[�h������̏I�[���Z�b�g���� */
		this->m_nCOMMENTEND = nPos + nnn;
		return true;	/* ���p���l�ł��� */
	}
	return false;
}


bool CColor_Numeric::EndColor(const CStringRef& cStr, int nPos)
{
	if( nPos == this->m_nCOMMENTEND ){
		return true;
	}
	return false;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         �����⏕                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

/*
 * ���l�Ȃ璷����Ԃ��B
 * 10�i���̐����܂��͏����B16�i��(����)�B
 * ������   ���l(�F����)
 * ---------------------
 * 123      123
 * 0123     0123
 * 0xfedc   0xfedc
 * -123     -123
 * -0x89a   0x89a
 * 0.5      0.5
 * 0.56.1   0.56 , 1
 * .5       5
 * -.5      5
 * 123.     123
 * 0x567.8  0x567 , 8
 */
/*
 * ���p���l
 *   1, 1.2, 1.2.3, .1, 0xabc, -.1, -1
 *   10�i��, 16�i��, ���������_��, ������
 *   IP�A�h���X�̃h�b�g�A��(�{���͐��l����Ȃ��񂾂��)
 */
static int IsNumber(const CStringRef& cStr, int offset)
{
	register const wchar_t* p;
	register const wchar_t* q;
	register int i = 0;
	register int d = 0;

	p = cStr.GetPtr() + offset;
	q = cStr.GetPtr() + cStr.GetLength();

	if( *p == L'0' )  /* 10�i��,C��16�i�� */
	{
		p++; i++;
		if( ( p < q ) && ( *p == L'x' ) )  /* C��16�i�� */
		{
			p++; i++;
			while( p < q )
			{
				if( ( *p >= L'0' && *p <= L'9' )
				 || ( *p >= L'A' && *p <= L'F' )
				 || ( *p >= L'a' && *p <= L'f' ) )
				{
					p++; i++;
				}
				else
				{
					break;
				}
			}
			/* "0x" �Ȃ� "0" ���������l */
			if( i == 2 ) return 1;
			return i;
		}
		else if( *p >= L'0' && *p <= L'9' )
		{
			p++; i++;
			while( p < q )
			{
				if( *p < L'0' || *p > L'9' )
				{
					if( *p == L'.' )
					{
						d++;
						if( d > 1 )
						{
							if( *(p - 1) == L'.' ) break;  /* "." ���A���Ȃ璆�f */
						}
					}
					else
					{
						break;
					}
				}
				p++; i++;
			}
			if( *(p - 1)  == L'.' ) return i - 1;  /* �Ōオ "." �Ȃ�܂߂Ȃ� */
			return i;
		}
		else if( *p == L'.' )
		{
			while( p < q )
			{
				if( *p < L'0' || *p > L'9' )
				{
					if( *p == L'.' )
					{
						d++;
						if( d > 1 )
						{
							if( *(p - 1) == L'.' ) break;  /* "." ���A���Ȃ璆�f */
						}
					}
					else
					{
						break;
					}
				}
				p++; i++;
			}
			if( *(p - 1)  == L'.' ) return i - 1;  /* �Ōオ "." �Ȃ�܂߂Ȃ� */
			return i;
		}
		/* "0" ���������l */
		return i;
	}

	else if( *p >= L'1' && *p <= L'9' )  /* 10�i�� */
	{
		p++; i++;
		while( p < q )
		{
			if( *p < L'0' || *p > L'9' )
			{
				if( *p == L'.' )
				{
					d++;
					if( d > 1 )
					{
						if( *(p - 1) == L'.' ) break;  /* "." ���A���Ȃ璆�f */
					}
				}
				else
				{
					break;
				}
			}
			p++; i++;
		}
		if( *(p - 1) == L'.' ) return i - 1;  /* �Ōオ "." �Ȃ�܂߂Ȃ� */
		return i;
	}

	else if( *p == L'-' )  /* �}�C�i�X */
	{
		p++; i++;
		while( p < q )
		{
			if( *p < L'0' || *p > L'9' )
			{
				if( *p == L'.' )
				{
					d++;
					if( d > 1 )
					{
						if( *(p - 1) == L'.' ) break;  /* "." ���A���Ȃ璆�f */
					}
				}
				else
				{
					break;
				}
			}
			p++; i++;
		}
		/* "-", "-." �����Ȃ琔�l�łȂ� */
		if( i == 1 ) return 0;
		if( *(p - 1) == L'.' )
		{
			i--;
			if( i == 1 ) return 0;
			return i;
		}
		return i;
	}

	else if( *p == L'.' )  /* �����_ */
	{
		d++;
		p++; i++;
		while( p < q )
		{
			if( *p < L'0' || *p > L'9' )
			{
				if( *p == L'.' )
				{
					d++;
					if( d > 1 )
					{
						if( *(p - 1) == L'.' ) break;  /* "." ���A���Ȃ璆�f */
					}
				}
				else
				{
					break;
				}
			}
			p++; i++;
		}
		/* "." �����Ȃ琔�l�łȂ� */
		if( i == 1 ) return 0;
		if( *(p - 1)  == L'.' ) return i - 1;  /* �Ōオ "." �Ȃ�܂߂Ȃ� */
		return i;
	}

	/* ���l�ł͂Ȃ� */
	return 0;
}
