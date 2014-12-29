// 2000.10.08 JEPRO  �w�i�F��^����RGB(255,255,255)��(255,251,240)�ɕύX(ῂ�������������)
// 2000.12.09 Jepro  note: color setting (�ڍׂ� CshareData.h ���Q�Ƃ̂���)
// 2000.09.04 JEPRO  �V���O���N�H�[�e�[�V����������ɐF�����蓖�Ă邪�F�����\���͂��Ȃ�
// 2000.10.17 JEPRO  �F�����\������悤�ɕύX(�ŏ���FALSE��TRUE)
// 2008.03.27 kobake �吮��

#include "StdAfx.h"
#include "CDocTypeSetting.h"
#include "view/Colors/EColorIndexType.h"


//! �F�ݒ�(�ۑ��p)
struct ColorInfoIni {
	const TCHAR*	m_pszName;			//!< ���ږ�
	ColorInfoBase	m_sColorInfoBase;		//!< �F�ݒ�
};

static ColorInfoIni ColorInfo_DEFAULT[COLOR_THEME_MAX][COLORIDX_LAST] = {
	/* �g�� */
	{
		//	���ږ�,								�\��,		����,		����,		�����F,					�w�i�F,
		_T("�e�L�X�g"),							TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("���[���["),							TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x00,0x50,0x00 ),
		_T("�J�[�\��"),							TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("�J�[�\��(IME ON)"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x30,0x30 ),	RGB( 0x00,0x00,0x00 ),
		_T("�J�[�\���s�̔w�i�F"),				FALSE,		FALSE,		FALSE,		RGB( 0x00,0x00,0x00 ),	RGB( 0x08,0x08,0x20 ),
		_T("�J�[�\���s�A���_�[���C��"),			TRUE,		FALSE,		FALSE,		RGB( 0x00,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("�J�[�\���ʒu�c��"),					FALSE,		FALSE,		FALSE,		RGB( 0x00,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("�s�ԍ�"),							TRUE,		FALSE,		FALSE,		RGB( 0x80,0x80,0xa0 ),	RGB( 0x00,0x00,0x00 ),
		_T("�s�ԍ�(�ύX�s)"),					TRUE,		FALSE,		FALSE,		RGB( 0x00,0xff,0xff ),	RGB( 0x40,0x40,0xa0 ),
		_T("TAB�L��"),							TRUE,		FALSE,		FALSE,		RGB( 0x40,0x10,0x80 ),	RGB( 0x00,0x00,0x00 ),
		_T("���p��")		,					FALSE,		FALSE,		FALSE,		RGB( 0xc0,0xc0,0xc0 ),	RGB( 0x00,0x00,0x00 ),
		_T("���{���"),						TRUE,		FALSE,		FALSE,		RGB( 0xc0,0xc0,0xc0 ),	RGB( 0x00,0x00,0x00 ),
		_T("�R���g���[���R�[�h"),				TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0x00 ),	RGB( 0x00,0x00,0x00 ),
		_T("���s�L��"),							TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0x00 ),	RGB( 0x00,0x00,0x00 ),
		_T("�܂�Ԃ��L��"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0x00,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("�w�茅�c��"),						FALSE,		FALSE,		FALSE,		RGB( 0xc0,0xc0,0xc0 ),	RGB( 0x00,0x00,0x00 ),
		_T("EOF�L��"),							TRUE,		FALSE,		FALSE,		RGB( 0x00,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("���p���l"),							TRUE,		FALSE,		FALSE,		RGB( 0xff,0x30,0x30 ),	RGB( 0x00,0x00,0x00 ),
		_T("�Ί��ʂ̋����\��"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0xff,0x30,0x30 ),
		_T("�I��͈�"),							TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0x00 ),	RGB( 0xff,0xff,0x00 ),
		_T("����������"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0x00 ),	RGB( 0x00,0x00,0xff ),
		_T("����������2"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0x00 ),	RGB( 0x00,0x00,0xff ),
		_T("����������3"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0x00 ),	RGB( 0x00,0x00,0xff ),
		_T("����������4"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0x00 ),	RGB( 0x00,0x00,0xff ),
		_T("����������5"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0x00 ),	RGB( 0x00,0x00,0xff ),
		_T("�R�����g"),							TRUE,		FALSE,		FALSE,		RGB( 0x00,0xc0,0x00 ),	RGB( 0x00,0x00,0x00 ),
		_T("C/C++ �v���v���Z�b�T�R�����g"),		TRUE,		FALSE,		FALSE,		RGB( 0x80,0x80,0x80 ),	RGB( 0x00,0x00,0x00 ),
		_T("�V���O���N�H�[�e�[�V����������"),	TRUE,		FALSE,		FALSE,		RGB( 0xff,0x30,0x30 ),	RGB( 0x00,0x00,0x00 ),
		_T("�_�u���N�H�[�e�[�V����������"),		TRUE,		FALSE,		FALSE,		RGB( 0xff,0x30,0x30 ),	RGB( 0x00,0x00,0x00 ),
		_T("URL"),								TRUE,		FALSE,		TRUE,		RGB( 0x80,0x80,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("�����L�[���[�h1"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x80,0x00 ),	RGB( 0x00,0x00,0x00 ),
		_T("�����L�[���[�h2"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x60,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("�����L�[���[�h3"),					TRUE,		FALSE,		FALSE,		RGB( 0x40,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("�����L�[���[�h4"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x30,0x30 ),	RGB( 0x00,0x00,0x00 ),
		_T("�����L�[���[�h5"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x00,0x80 ),	RGB( 0x00,0x00,0x00 ),
		_T("�����L�[���[�h6"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x80,0x00 ),	RGB( 0x00,0x00,0x00 ),
		_T("�����L�[���[�h7"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x80,0x00 ),	RGB( 0x00,0x00,0x00 ),
		_T("�����L�[���[�h8"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x80,0x00 ),	RGB( 0x00,0x00,0x00 ),
		_T("�����L�[���[�h9"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x80,0x00 ),	RGB( 0x00,0x00,0x00 ),
		_T("�����L�[���[�h10"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x80,0x00 ),	RGB( 0x00,0x00,0x00 ),
		_T("���K�\���L�[���[�h1"),				TRUE,		FALSE,		FALSE,		RGB( 0x98,0xfb,0x98 ),	RGB( 0x00,0x00,0x00 ),
		_T("���K�\���L�[���[�h2"),				FALSE,		FALSE,		FALSE,		RGB( 0x40,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("���K�\���L�[���[�h3"),				FALSE,		FALSE,		FALSE,		RGB( 0x40,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("���K�\���L�[���[�h4"),				FALSE,		FALSE,		FALSE,		RGB( 0x40,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("���K�\���L�[���[�h5"),				FALSE,		FALSE,		FALSE,		RGB( 0x40,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("���K�\���L�[���[�h6"),				FALSE,		FALSE,		FALSE,		RGB( 0x40,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("���K�\���L�[���[�h7"),				FALSE,		FALSE,		FALSE,		RGB( 0x40,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("���K�\���L�[���[�h8"),				FALSE,		FALSE,		FALSE,		RGB( 0x40,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("���K�\���L�[���[�h9"),				FALSE,		FALSE,		FALSE,		RGB( 0x40,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("���K�\���L�[���[�h10"),				FALSE,		FALSE,		FALSE,		RGB( 0x40,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("DIFF�����\��(�ǉ�)"),				FALSE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("DIFF�����\��(�ύX)"),				FALSE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("DIFF�����\��(�폜)"),				FALSE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x00,0x00,0x00 ),
		_T("�u�b�N�}�[�N"),						TRUE ,		FALSE,		FALSE,		RGB( 0x00,0x00,0x00 ),	RGB( 0xff,0xff,0x00 ),
	},

	/* �T�N���G�f�B�^ */
	{
		//	���ږ�,								�\��,		����,		����,		�����F,					�w�i�F,
		_T("�e�L�X�g"),							TRUE,		FALSE,		FALSE,		RGB(   0,   0,   0 ),	RGB( 255, 251, 240 ),
		_T("���[���["),							TRUE,		FALSE,		FALSE,		RGB(   0,   0,   0 ),	RGB( 239, 239, 239 ),
		_T("�J�[�\��"),							TRUE,		FALSE,		FALSE,		RGB(   0,   0,   0 ),	RGB( 255, 251, 240 ),
		_T("�J�[�\��(IME ON)"),					TRUE,		FALSE,		FALSE,		RGB( 255,   0,   0 ),	RGB( 255, 251, 240 ),
		_T("�J�[�\���s�̔w�i�F"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0,   0 ),	RGB( 255, 255, 128 ),
		_T("�J�[�\���s�A���_�[���C��"),			TRUE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("�J�[�\���ʒu�c��"),					FALSE,		FALSE,		FALSE,		RGB( 128, 128, 255 ),	RGB( 255, 251, 240 ),
		_T("�s�ԍ�"),							TRUE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 239, 239, 239 ),
		_T("�s�ԍ�(�ύX�s)"),					TRUE,		TRUE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 239, 239, 239 ),
		_T("TAB�L��"),							TRUE,		FALSE,		FALSE,		RGB( 128, 128, 128 ),	RGB( 255, 251, 240 ),
		_T("���p��")		,					FALSE,		FALSE,		FALSE,		RGB( 192, 192, 192 ),	RGB( 255, 251, 240 ),
		_T("���{���"),						TRUE,		FALSE,		FALSE,		RGB( 192, 192, 192 ),	RGB( 255, 251, 240 ),
		_T("�R���g���[���R�[�h"),				TRUE,		FALSE,		FALSE,		RGB( 255, 255,   0 ),	RGB( 255, 251, 240 ),
		_T("���s�L��"),							TRUE,		FALSE,		FALSE,		RGB(   0, 128, 255 ),	RGB( 255, 251, 240 ),
		_T("�܂�Ԃ��L��"),						TRUE,		FALSE,		FALSE,		RGB( 255,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("�w�茅�c��"),						FALSE,		FALSE,		FALSE,		RGB( 192, 192, 192 ),	RGB( 255, 251, 240 ),
		_T("EOF�L��"),							TRUE,		FALSE,		FALSE,		RGB(   0, 255, 255 ),	RGB(   0,   0,   0 ),
		_T("���p���l"),							TRUE,		FALSE,		FALSE,		RGB( 235,   0,   0 ),	RGB( 255, 251, 240 ),
		_T("�Ί��ʂ̋����\��"),					TRUE,		TRUE,		FALSE,		RGB( 128,   0,   0 ),	RGB( 255, 251, 240 ),
		_T("�I��͈�"),							TRUE,		FALSE,		FALSE,		RGB(  49, 106, 197 ),	RGB(  49, 106, 197 ),
		_T("����������"),						TRUE,		FALSE,		FALSE,		RGB(   0,   0,   0 ),	RGB( 255, 255,   0 ),
		_T("����������2"),						TRUE,		FALSE,		FALSE,		RGB(   0,   0,   0 ),	RGB( 160, 255, 255 ),
		_T("����������3"),						TRUE,		FALSE,		FALSE,		RGB(   0,   0,   0 ),	RGB( 153, 255, 153 ),
		_T("����������4"),						TRUE,		FALSE,		FALSE,		RGB(   0,   0,   0 ),	RGB( 255, 153, 153 ),
		_T("����������5"),						TRUE,		FALSE,		FALSE,		RGB(   0,   0,   0 ),	RGB( 255, 102, 255 ),
		_T("�R�����g"),							TRUE,		FALSE,		FALSE,		RGB(   0, 128,   0 ),	RGB( 255, 251, 240 ),
		_T("C/C++ �v���v���Z�b�T�R�����g"),		TRUE,		FALSE,		FALSE,		RGB( 128, 128, 128 ),	RGB( 255, 251, 240 ),
		_T("�V���O���N�H�[�e�[�V����������"),	TRUE,		FALSE,		FALSE,		RGB(  64, 128, 128 ),	RGB( 255, 251, 240 ),
		_T("�_�u���N�H�[�e�[�V����������"),		TRUE,		FALSE,		FALSE,		RGB( 128,   0,  64 ),	RGB( 255, 251, 240 ),
		_T("URL"),								TRUE,		FALSE,		TRUE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("�����L�[���[�h1"),					TRUE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("�����L�[���[�h2"),					TRUE,		FALSE,		FALSE,		RGB( 128,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("�����L�[���[�h3"),					TRUE,		FALSE,		FALSE,		RGB(   0,  64, 128 ),	RGB( 255, 251, 240 ),
		_T("�����L�[���[�h4"),					TRUE,		FALSE,		FALSE,		RGB( 235,   0,   0 ),	RGB( 255, 251, 240 ),
		_T("�����L�[���[�h5"),					TRUE,		FALSE,		FALSE,		RGB( 255, 128,   0 ),	RGB( 255, 251, 240 ),
		_T("�����L�[���[�h6"),					TRUE,		FALSE,		FALSE,		RGB( 255, 128,   0 ),	RGB( 255, 251, 240 ),
		_T("�����L�[���[�h7"),					TRUE,		FALSE,		FALSE,		RGB( 255, 128,   0 ),	RGB( 255, 251, 240 ),
		_T("�����L�[���[�h8"),					TRUE,		FALSE,		FALSE,		RGB( 255, 128,   0 ),	RGB( 255, 251, 240 ),
		_T("�����L�[���[�h9"),					TRUE,		FALSE,		FALSE,		RGB( 255, 128,   0 ),	RGB( 255, 251, 240 ),
		_T("�����L�[���[�h10"),					TRUE,		FALSE,		FALSE,		RGB( 255, 128,   0 ),	RGB( 255, 251, 240 ),
		_T("���K�\���L�[���[�h1"),				TRUE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("���K�\���L�[���[�h2"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("���K�\���L�[���[�h3"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("���K�\���L�[���[�h4"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("���K�\���L�[���[�h5"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("���K�\���L�[���[�h6"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("���K�\���L�[���[�h7"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("���K�\���L�[���[�h8"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("���K�\���L�[���[�h9"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("���K�\���L�[���[�h10"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0, 255 ),	RGB( 255, 251, 240 ),
		_T("DIFF�����\��(�ǉ�)"),				FALSE,		FALSE,		FALSE,		RGB(   0,   0, 210 ),	RGB( 162, 208, 255 ),
		_T("DIFF�����\��(�ύX)"),				FALSE,		FALSE,		FALSE,		RGB(   0, 111,   0 ),	RGB( 189, 253, 192 ),
		_T("DIFF�����\��(�폜)"),				FALSE,		FALSE,		FALSE,		RGB( 213, 106,   0 ),	RGB( 255, 233, 172 ),
		_T("�u�b�N�}�[�N"),						TRUE ,		FALSE,		FALSE,		RGB( 255, 251, 240 ),   RGB(   0, 128, 192 )
	},

	/* Visual Studio 2012 Dark */
	{
		//	���ږ�,								�\��,		����,		����,		�����F,					�w�i�F,
		_T("�e�L�X�g"),							TRUE,		FALSE,		FALSE,		RGB( 0xdc,0xdc,0xdc ),	RGB( 0x1e,0x1e,0x1e ), // "TXT"
		_T("���[���["),							TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x33,0x33,0x33 ), // "RUL"
		_T("�J�[�\��"),							TRUE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x00,0x00,0x00 ), // "CAR"
		_T("�J�[�\��(IME ON)"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0x30,0x30 ),	RGB( 0x00,0x00,0x00 ), // "IME"
		_T("�J�[�\���s�̔w�i�F"),				TRUE,		FALSE,		FALSE,		RGB( 0x00,0x00,0x00 ),	RGB( 0x00,0x00,0x00 ), // "CBK"
		_T("�J�[�\���s�A���_�[���C��"),			TRUE,		FALSE,		FALSE,		RGB( 0x33,0x33,0x33 ),	RGB( 0x1e,0x1e,0x1e ), // "UND"
		_T("�J�[�\���ʒu�c��"),					FALSE,		FALSE,		FALSE,		RGB( 0x33,0x33,0x33 ),	RGB( 0x00,0x00,0x00 ), // "CVL"
		_T("�s�ԍ�"),							TRUE,		FALSE,		FALSE,		RGB( 0x2b,0x91,0xaf ),	RGB( 0x1e,0x1e,0x1e ), // "LNO"
		_T("�s�ԍ�(�ύX�s)"),					TRUE,		FALSE,		FALSE,		RGB( 0xff,0xd8,0x00 ),	RGB( 0x1e,0x1e,0x1e ), // "MOD"
		_T("TAB�L��"),							FALSE,		FALSE,		FALSE,		RGB( 0x33,0x33,0x33 ),	RGB( 0x1e,0x1e,0x1e ), // "TAB"
		_T("���p��")		,					FALSE,		FALSE,		FALSE,		RGB( 0x9b,0x9b,0x9b ),	RGB( 0x1e,0x1e,0x1e ), // "SPC"
		_T("���{���"),						TRUE,		FALSE,		FALSE,		RGB( 0x9b,0x9b,0x9b ),	RGB( 0x1e,0x1e,0x1e ), // "ZEN"
		_T("�R���g���[���R�[�h"),				TRUE,		FALSE,		FALSE,		RGB( 0xff,0xd8,0x00 ),	RGB( 0x1e,0x1e,0x1e ), // "CTL"
		_T("���s�L��"),							FALSE,		FALSE,		FALSE,		RGB( 0xff,0xd8,0x00 ),	RGB( 0x1e,0x1e,0x1e ), // "EOL"
		_T("�܂�Ԃ��L��"),						TRUE,		FALSE,		FALSE,		RGB( 0xd6,0x9d,0x85 ),	RGB( 0x1e,0x1e,0x1e ), // "RAP"
		_T("�w�茅�c��"),						FALSE,		FALSE,		FALSE,		RGB( 0x14,0x48,0x50 ),	RGB( 0x1e,0x1e,0x1e ), // "VER"
		_T("EOF�L��"),							TRUE,		FALSE,		FALSE,		RGB( 0x56,0x9c,0xd6 ),	RGB( 0x1e,0x1e,0x1e ), // "EOF"
		_T("���p���l"),							TRUE,		FALSE,		FALSE,		RGB( 0xb5,0xce,0xa8 ),	RGB( 0x1e,0x1e,0x1e ), // "NUM"
		_T("�Ί��ʂ̋����\��"),					TRUE,		FALSE,		FALSE,		RGB( 0x00,0x00,0x00 ),	RGB( 0x0e,0x45,0x83 ), // "BRC"
		_T("�I��͈�"),							TRUE,		FALSE,		FALSE,		RGB( 0x33,0x99,0xff ),	RGB( 0x33,0x99,0xff ), // "SEL"
		_T("����������"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0xd8,0x00 ),	RGB( 0x77,0x38,0x00 ), // "FND"
		_T("����������2"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0xd8,0x00 ),	RGB( 0x77,0x38,0x00 ), // "FN2"
		_T("����������3"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0xd8,0x00 ),	RGB( 0x77,0x38,0x00 ), // "FN3"
		_T("����������4"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0xd8,0x00 ),	RGB( 0x77,0x38,0x00 ), // "FN4"
		_T("����������5"),						TRUE,		FALSE,		FALSE,		RGB( 0xff,0xd8,0x00 ),	RGB( 0x77,0x38,0x00 ), // "FN5"
		_T("�R�����g"),							TRUE,		FALSE,		FALSE,		RGB( 0x57,0xa6,0x4a ),	RGB( 0x1e,0x1e,0x1e ), // "CMT"
		_T("C/C++ �v���v���Z�b�T�R�����g"),		TRUE,		FALSE,		FALSE,		RGB( 0x60,0x60,0x60 ),	RGB( 0x1e,0x1e,0x1e ), // "CM2"
		_T("�V���O���N�H�[�e�[�V����������"),	TRUE,		FALSE,		FALSE,		RGB( 0xd6,0x9d,0x85 ),	RGB( 0x1e,0x1e,0x1e ), // "SQT"
		_T("�_�u���N�H�[�e�[�V����������"),		TRUE,		FALSE,		FALSE,		RGB( 0xd6,0x9d,0x85 ),	RGB( 0x1e,0x1e,0x1e ), // "WQT"
		_T("URL"),								TRUE,		FALSE,		TRUE,		RGB( 0x52,0x63,0xda ),	RGB( 0x1e,0x1e,0x1e ), // "URL"
		_T("�����L�[���[�h1"),					TRUE,		FALSE,		FALSE,		RGB( 0x56,0x9c,0xd6 ),	RGB( 0x1e,0x1e,0x1e ), // "KW1"
		_T("�����L�[���[�h2"),					TRUE,		FALSE,		FALSE,		RGB( 0x9b,0x9b,0x9b ),	RGB( 0x1e,0x1e,0x1e ), // "KW2"
		_T("�����L�[���[�h3"),					TRUE,		FALSE,		FALSE,		RGB( 0x56,0x9c,0xd6 ),	RGB( 0x1e,0x1e,0x1e ), // "KW3"
		_T("�����L�[���[�h4"),					TRUE,		FALSE,		FALSE,		RGB( 0xb5,0xce,0xa8 ),	RGB( 0x1e,0x1e,0x1e ), // "KW4"
		_T("�����L�[���[�h5"),					TRUE,		FALSE,		FALSE,		RGB( 0xd6,0x9d,0x85 ),	RGB( 0x1e,0x1e,0x1e ), // "KW5"
		_T("�����L�[���[�h6"),					TRUE,		FALSE,		FALSE,		RGB( 0xd6,0x9d,0x85 ),	RGB( 0x1e,0x1e,0x1e ), // "KW6"
		_T("�����L�[���[�h7"),					TRUE,		FALSE,		FALSE,		RGB( 0xd6,0x9d,0x85 ),	RGB( 0x1e,0x1e,0x1e ), // "KW7"
		_T("�����L�[���[�h8"),					TRUE,		FALSE,		FALSE,		RGB( 0xd6,0x9d,0x85 ),	RGB( 0x1e,0x1e,0x1e ), // "KW8"
		_T("�����L�[���[�h9"),					TRUE,		FALSE,		FALSE,		RGB( 0xd6,0x9d,0x85 ),	RGB( 0x1e,0x1e,0x1e ), // "KW9"
		_T("�����L�[���[�h10"),					TRUE,		FALSE,		FALSE,		RGB( 0xd6,0x9d,0x85 ),	RGB( 0x1e,0x1e,0x1e ), // "KWA"
		_T("���K�\���L�[���[�h1"),				TRUE,		FALSE,		FALSE,		RGB( 0x4e,0xc9,0xb0 ),	RGB( 0x1e,0x1e,0x1e ), // "RK1"
		_T("���K�\���L�[���[�h2"),				FALSE,		FALSE,		FALSE,		RGB( 0x4e,0xc9,0xb0 ),	RGB( 0x1e,0x1e,0x1e ), // "RK2"
		_T("���K�\���L�[���[�h3"),				FALSE,		FALSE,		FALSE,		RGB( 0x4e,0xc9,0xb0 ),	RGB( 0x1e,0x1e,0x1e ), // "RK3"
		_T("���K�\���L�[���[�h4"),				FALSE,		FALSE,		FALSE,		RGB( 0x4e,0xc9,0xb0 ),	RGB( 0x1e,0x1e,0x1e ), // "RK4"
		_T("���K�\���L�[���[�h5"),				FALSE,		FALSE,		FALSE,		RGB( 0x4e,0xc9,0xb0 ),	RGB( 0x1e,0x1e,0x1e ), // "RK5"
		_T("���K�\���L�[���[�h6"),				FALSE,		FALSE,		FALSE,		RGB( 0x4e,0xc9,0xb0 ),	RGB( 0x1e,0x1e,0x1e ), // "RK6"
		_T("���K�\���L�[���[�h7"),				FALSE,		FALSE,		FALSE,		RGB( 0x4e,0xc9,0xb0 ),	RGB( 0x1e,0x1e,0x1e ), // "RK7"
		_T("���K�\���L�[���[�h8"),				FALSE,		FALSE,		FALSE,		RGB( 0x4e,0xc9,0xb0 ),	RGB( 0x1e,0x1e,0x1e ), // "RK8"
		_T("���K�\���L�[���[�h9"),				FALSE,		FALSE,		FALSE,		RGB( 0x4e,0xc9,0xb0 ),	RGB( 0x1e,0x1e,0x1e ), // "RK9"
		_T("���K�\���L�[���[�h10"),				FALSE,		FALSE,		FALSE,		RGB( 0x4e,0xc9,0xb0 ),	RGB( 0x1e,0x1e,0x1e ), // "RKA"
		_T("DIFF�����\��(�ǉ�)"),				FALSE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x1e,0x1e,0x1e ), // "DFA"
		_T("DIFF�����\��(�ύX)"),				FALSE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x1e,0x1e,0x1e ), // "DFC"
		_T("DIFF�����\��(�폜)"),				FALSE,		FALSE,		FALSE,		RGB( 0xff,0xff,0xff ),	RGB( 0x1e,0x1e,0x1e ), // "DFD"
		_T("�u�b�N�}�[�N"),						TRUE ,		FALSE,		FALSE,		RGB( 0x00,0x00,0x00 ),	RGB( 0x7e,0x65,0x51 ), // "MRK"
	}
};

/*!
	SetDefaultColorInfo - ��ʕ\���F�Ɋւ�������w��e�[�}�̏����l�ɐݒ肷��

	@param [out] ColorInfo* pColorInfo
	@param [in]  ColorTheme themeNum
	@retval �Ȃ�
 */
void SetDefaultColorInfo(ColorInfo* pColorInfo, ColorTheme themeNum)
{
	for( int i = 0; i < COLORIDX_LAST; i++ ){
		ColorInfoBase* p = pColorInfo;
		*p = ColorInfo_DEFAULT[themeNum][i].m_sColorInfoBase;
		_tcscpy(pColorInfo->m_szName, ColorInfo_DEFAULT[themeNum][i].m_pszName);
		pColorInfo->m_nColorIdx = i;
		pColorInfo++;
	}
}

/*!
	SetColorTheme - ��ʕ\���F���w��̃e�[�}�z�F�ɐݒ肷��

	@param [out] ColorInfo* pColorInfo
	@param [in]  ColorTheme themeNum
	@retval �Ȃ�
 */
void SetColorTheme(ColorInfo* pColorInfo, ColorTheme themeNum)
{
	for( int i = 0; i < COLORIDX_LAST; i++ ){
		pColorInfo->m_bBoldFont = ColorInfo_DEFAULT[themeNum][i].m_sColorInfoBase.m_bBoldFont;
		pColorInfo->m_bUnderLine = ColorInfo_DEFAULT[themeNum][i].m_sColorInfoBase.m_bUnderLine;
		pColorInfo->m_colTEXT = ColorInfo_DEFAULT[themeNum][i].m_sColorInfoBase.m_colTEXT;
		pColorInfo->m_colBACK = ColorInfo_DEFAULT[themeNum][i].m_sColorInfoBase.m_colBACK;
		pColorInfo++;
	}
}

