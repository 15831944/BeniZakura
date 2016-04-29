/*!	@file
	@brief �^�O�W�����v���X�g�_�C�A���O�{�b�N�X

	@author MIK
	@date 2003.4.13
	@date 2005.03.31 MIK �L�[���[�h�w��TagJump�Ή��̂��ߑ啝�ɕύX
*/
/*
	Copyright (C) 2003, MIK
	Copyright (C) 2005, MIK
	Copyright (C) 2006, genta, ryoji
	Copyright (C) 2010, Moca

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
#include "dlg/CDlgTagJumpList.h"
#include "CSortedTagJumpList.h"
#include "func/Funccode.h"
#include "env/DLLSHAREDATA.h"
#include "recent/CRecent.h"
#include "util/container.h"
#include "util/shell.h"
#include "util/file.h"
#include "util/window.h"
#include "sakura_rc.h"


static const SAnchorList anchorList[] = {
	{IDC_STATIC_BASEDIR,	ANCHOR_BOTTOM},
	{IDC_LIST_TAGJUMP,		ANCHOR_ALL},
};


//�^�O�t�@�C���̃t�H�[�}�b�g	//	@@ 2005.03.31 MIK �萔��
//	@@ 2005.04.03 MIK �L�[���[�h�ɋ󔒂��܂܂��ꍇ�̍l��
#define TAG_FORMAT_2_A       "%[^\t\r\n]\t%[^\t\r\n]\t%d;\"\t%s\t%s"
#define TAG_FORMAT_1_A       "%[^\t\r\n]\t%[^\t\r\n]\t%d"
#define TAG_FILE_INFO_A      "%[^\t\r\n]\t%[^\t\r\n]\t%[^\t\r\n]"
// #define TAG_FORMAT_E_FILE_A  "%[^\t\r\n,],%d"
// #define TAG_FORMAT_E_NAME_A  "%[^\x7f\r\n]\x7f%[^\x7ff\r\n\x01]\x01%d,%d"

//	@@ 2005.03.31 MIK
//�L�[���[�h����͂��ĊY���������\������܂ł̎���(�~���b)
#define TAGJUMP_TIMER_DELAY 700
#define TAGJUMP_TIMER_DELAY_SHORT 50

#define TAGJUMP_LIST_MAX 32

/*
	ctags.exe ���o�͂���A�g���q�ƑΉ�������
*/
static const TCHAR *p_extentions[] = {
	/*asm*/			_T("asm,s"),								_T("d=define,l=label,m=macro,t=type,F=file"),
	/*asp*/			_T("asp,asa"),								_T("f=function,s=sub"),
	/*awk*/			_T("awk,gawk,mawk"),						_T("f=function"),
	/*beta*/		_T("bet"),									_T("f=fragment,p=pattern,s=slot,v=virtual"),
	/*c*/			_T("c,h"),									_T("c=class,d=macro,e=enumerator,f=function,g=enum,m=member,n=namespace,p=prototype,s=struct,t=typedef,u=union,v=variable,x=externvar,F=file"),
	/*c++*/			_T("c++,cc,cp,cpp,cxx,h++,hh,hp,hpp,hxx"),	_T("c=class,d=macro,e=enumerator,f=function,g=enum,m=member,n=namespace,p=prototype,s=struct,t=typedef,u=union,v=variable,x=externvar,F=file"),
	/*java*/		_T("java"),									_T("c=class,d=macro,e=enumerator,f=function,g=enum,m=member,n=namespace,p=prototype,s=struct,t=typedef,u=union,v=variable,x=externvar,F=file"),
	/*vera*/		_T("vr,vri,vrh"),							_T("c=class,d=macro,e=enumerator,f=function,g=enum,m=member,n=namespace,p=prototype,s=struct,t=typedef,u=union,v=variable,x=externvar,F=file"),
	/*cobol*/		_T("cbl,cob"),								_T("d=data,f=file,g=group,p=paragraph,P=program,s=section"),
	/*eiffel*/		_T("e"),									_T("c=class,f=feature,l=local"),
	/*fortran*/		_T("f,for,ftn,f77,f90,f95"),				_T("b=block data,c=common,e=entry,f=function,i=interface,k=component,l=label,L=local,m=module,n=namelist,p=program,s=subroutine,t=type,v=variable"),
	/*lisp*/		_T("cl,clisp,el,l,lisp,lsp,ml"),			_T("f=function"),
	/*lua*/			_T("lua"),									_T("f=function"),
	/*makefile*/	_T("mak"),									_T("m=macro"),
	/*pascal*/		_T("p,pas"),								_T("f=function,p=procedure"),
	/*perl*/		_T("pl,pm,perl"),							_T("s=subroutine,p=package"),
	/*php*/			_T("php,php3,phtml"),						_T("c=class,f=function"),
	/*python*/		_T("py,python"),							_T("c=class,f=function,m=member"),
	/*rexx*/		_T("cmd,rexx,rx"),							_T("s=subroutine"),
	/*ruby*/		_T("rb"),									_T("c=class,f=method,F=singleton method,m=mixin"),
	/*scheme*/		_T("sch,scheme,scm,sm"),					_T("f=function,s=set"),
	/*sh*/			_T("sh,bsh,bash,ksh,zsh"),					_T("f=function"),
	/*slang*/		_T("sl"),									_T("f=function,n=namespace"),
	/*sql*/			_T("sql"),									_T("c=cursor,d=prototype,f=function,F=field,l=local,P=package,p=procedure,r=record,s=subtype,t=table,T=trigger,v=variable"),
	/*tcl*/			_T("tcl,tk,wish,itcl"),						_T("p=procedure,c=class,f=method"),
	/*verilog*/		_T("v"),									_T("f=function,m=module,P=parameter,p=port,r=reg,t=task,v=variable,w=wire"),
	/*vim*/			_T("vim"),									_T("f=function,v=variable"),
	/*yacc*/		_T("y"),									_T("l=label"),
//	/*vb*/			_T("bas,cls,ctl,dob,dsr,frm,pag"),			_T("a=attribute,c=class,f=function,l=label,s=procedure,v=variable"),
					NULL,									NULL
};

inline bool CDlgTagJumpList::IsDirectTagJump(){
	return m_bDirectTagJump;
}

CDlgTagJumpList::CDlgTagJumpList(bool bDirectTagJump)
	: m_pcList( NULL ),
	  m_bDirectTagJump(bDirectTagJump),
	  m_nIndex( -1 ),
	  m_nLoop( -1 ),
	  m_pszFileName( NULL ),
	  m_pszKeyword( NULL ),
	  m_nTimerId( 0 )
{
	/* �T�C�Y�ύX���Ɉʒu�𐧌䂷��R���g���[���� */
	assert( _countof(anchorList) == _countof(m_rcItems) );

	m_pcList = new CSortedTagJumpList(TAGJUMP_LIST_MAX);
	m_ptDefaultSize.x = -1;
	m_ptDefaultSize.y = -1;
}

CDlgTagJumpList::~CDlgTagJumpList()
{
	Empty();

	if( m_pszFileName ) free( m_pszFileName );
	m_pszFileName = NULL;
	if( m_pszKeyword ) free( m_pszKeyword );
	m_pszKeyword = NULL;

	StopTimer();
	SAFE_DELETE( m_pcList );
}

/*!
	�^�C�}�[��~

	@author MIK
	@date 2005.03.31 �V�K�쐬
*/
void CDlgTagJumpList::StopTimer( void )
{
	if( m_nTimerId != 0 ){
		::KillTimer( GetHwnd(), m_nTimerId );
		m_nTimerId = 0;
	}
}

/*!
	�^�C�}�[�J�n
	
	�L�[���[�h�w�莞�C�����ԕ������͂��Ȃ���΃��X�g���X�V���邽��
	�u�����ԁv���v��^�C�}�[���K�v

	@author MIK
	@date 2005.03.31 �V�K�쐬
*/
void CDlgTagJumpList::StartTimer( int nDelay = TAGJUMP_TIMER_DELAY )
{
	StopTimer();
	m_nTimerId = ::SetTimer( GetHwnd(), 12345, nDelay, NULL );
}

/*!
	���X�g�̃N���A

	@author MIK
	@date 2005.03.31 �V�K�쐬
*/
void CDlgTagJumpList::Empty( void )
{
	m_nIndex = -1;
	m_pcList->Empty();
}

/*
	���[�_���_�C�A���O�̕\��

	@param[in] lParam 0=�_�C���N�g�^�O�W�����v, 1=�V���{������(�L�[���[�h�w��^�O�W�����v)
*/
int CDlgTagJumpList::DoModal(
	HINSTANCE	hInstance,
	HWND		hwndParent,
	LPARAM		lParam
)
{
	int ret = (int)CDialog::DoModal( hInstance, hwndParent, IDD_TAGJUMPLIST, lParam );
	StopTimer();
	return ret;
}

/* �_�C�A���O�f�[�^�̐ݒ� */
void CDlgTagJumpList::SetData( void )
{
	if( IsDirectTagJump() ){
		if( m_pszKeyword != NULL ){
			::DlgItem_SetText( GetHwnd(), IDC_KEYWORD, m_pszKeyword );
		}
	}
	//	From Here 2005.04.03 MIK �ݒ�l�̓ǂݍ���
	else{
		HWND hwndKey;
		hwndKey = ::GetDlgItem( GetHwnd(), IDC_KEYWORD );
		Combo_LimitText( hwndKey, _MAX_PATH-1 );
		CRecentTagjumpKeyword cRecentTagJump;
		for( int i = 0; i < cRecentTagJump.GetItemCount(); i++ ){
			Combo_AddString( hwndKey, cRecentTagJump.GetItemText(i) );
		}
		if( m_pszKeyword != NULL ){
			::DlgItem_SetText( GetHwnd(), IDC_KEYWORD, m_pszKeyword );
		}
		cRecentTagJump.Terminate();
	}
	//	To Here 2005.04.03 MIK �ݒ�l�̓ǂݍ���
	

	SetTextDir();

	UpdateData(true);

	// �O�̂��ߏォ��UpdateData�̌�Ɉړ�
	if( ! IsDirectTagJump() ){
		StartTimer( TAGJUMP_TIMER_DELAY_SHORT ); // �ŏ��͋K�莞�ԑ҂��Ȃ�
	}
}

/*! @brief Jump���̍X�V

	@date 2005.03.31 MIK 
		�_�C�A���OOpen���ȊO�ɂ��X�V���K�v�Ȃ���SetData()��蕪��
*/
void CDlgTagJumpList::UpdateData( bool bInit )
{
	HWND	hwndList;
	LV_ITEM	lvi;
	int		nIndex;
	int		count;

	hwndList = ::GetDlgItem( GetHwnd(), IDC_LIST_TAGJUMP );
	ListView_DeleteAllItems( hwndList );

	count = m_pcList->GetCount();

	const int SymbolNameMax = 64;
	wchar_t symbolName[SymbolNameMax + 1];
	symbolName[SymbolNameMax] = 0;
	TCHAR	tmp[32];
	for( nIndex = 0; nIndex < count; nIndex++ )
	{
		CSortedTagJumpList::TagJumpInfo* item;
		item = m_pcList->GetPtr( nIndex );
		if( NULL == item ) break;

		TCHAR	*p;
		p = GetNameByType( item->type, item->filename );
		{
			if( wcscmp( p, L"function" ) == 0 ){
				_snwprintf( symbolName, SymbolNameMax, L"%s()", item->keyword );
			}else if( (wcscmp( p, L"struct" ) == 0) || (wcscmp( p, L"class" ) == 0) ){
				_snwprintf( symbolName, SymbolNameMax, L"%s::", item->keyword );
			}else if( wcscmp( p, L"prototype" ) == 0 ){
				_snwprintf( symbolName, SymbolNameMax, L"%s();", item->keyword );
			}else{
				_snwprintf( symbolName, SymbolNameMax, L"%s", item->keyword );
			}

			lvi.mask     = LVIF_TEXT;
			lvi.iItem    = nIndex;
			lvi.iSubItem = 0;
			lvi.pszText  = symbolName;
			ListView_InsertItem( hwndList, &lvi );
			ListView_SetItemText( hwndList, nIndex, 3, p );
		}
		free( p );

		ListView_SetItemText( hwndList, nIndex, 1, item->filename );

		auto_sprintf( tmp, _T("%d"), item->no );
		ListView_SetItemText( hwndList, nIndex, 2, tmp );

		ListView_SetItemText( hwndList, nIndex, 4, item->note );

		ListView_SetItemState( hwndList, nIndex, 0, LVIS_SELECTED | LVIS_FOCUSED );
	}

	const TCHAR* pszMsgText = NULL;

	//	������������ꍇ�͐؂�̂Ă��|�𖖔��ɑ}��
//	if( m_pcList->IsOverflow() ){
		// 2010.04.03 �u���v�u�O�v�{�^���ǉ����� Overflow���Ȃ��Ȃ���
//		pszMsgText = _T("(����ȍ~�͐؂�̂Ă܂���)");
//	}
	if( (! bInit) && m_pcList->GetCount() == 0 ){
		pszMsgText = _T("(���͌�����܂���ł���)");
	}
	if( pszMsgText ){
		lvi.mask     = LVIF_TEXT | LVIF_PARAM;
		lvi.iItem    = nIndex;
		lvi.iSubItem = 0;
		lvi.pszText  = _T("(�ʒm)");
		lvi.lParam   = -1;
		ListView_InsertItem( hwndList, &lvi );
//		ListView_SetItemText( hwndList, nIndex, 1, _T("") );
//		ListView_SetItemText( hwndList, nIndex, 2, _T("") );
//		ListView_SetItemText( hwndList, nIndex, 3, _T("") );
		ListView_SetItemText( hwndList, nIndex, 1, const_cast<TCHAR*>(pszMsgText) );
//		ListView_SetItemText( hwndList, nIndex, 5, _T("") );
	}

	m_nIndex = SearchBestTag();
	if( m_nIndex != -1 )
	{
		ListView_SetItemState( hwndList, m_nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
		ListView_EnsureVisible( hwndList, m_nIndex, FALSE );
	}
	ListView_SetColumnWidth( hwndList, 0, LVSCW_AUTOSIZE_USEHEADER );
	ListView_SetColumnWidth( hwndList, 1, LVSCW_AUTOSIZE_USEHEADER );
	ListView_SetColumnWidth( hwndList, 2, LVSCW_AUTOSIZE_USEHEADER );
	ListView_SetColumnWidth( hwndList, 3, LVSCW_AUTOSIZE_USEHEADER );
	ListView_SetColumnWidth( hwndList, 4, LVSCW_AUTOSIZE_USEHEADER );
	ListView_SetColumnWidth( hwndList, 5, LVSCW_AUTOSIZE_USEHEADER );

	return;
}

/*!	�_�C�A���O�f�[�^�̎擾

	@return TRUE: ����, FALSE: ���̓G���[

	@date 2005.04.03 MIK �ݒ�l�̕ۑ������ǉ�
*/
int CDlgTagJumpList::GetData( void )
{
	HWND	hwndList;

	hwndList = ::GetDlgItem( GetHwnd(), IDC_LIST_TAGJUMP );
	m_nIndex = ListView_GetNextItem( hwndList, -1, LVIS_SELECTED );

	//	From Here 2005.04.03 MIK �ݒ�l�̕ۑ�
	if( !IsDirectTagJump() )
	{
		// 2010.07.22 ��₪��ł��W�����v�ŕ����Ƃ��́A�I�v�V������ۑ�����
		if( m_nIndex == -1 || m_nIndex >= m_pcList->GetCapacity() ){
			return FALSE;
		}
		wchar_t	tmp[MAX_TAG_STRING_LENGTH];
		wcscpy( tmp, L"" );
		::DlgItem_GetText( GetHwnd(), IDC_KEYWORD, tmp, _countof( tmp ) );
		SetKeyword( tmp );

		//�ݒ��ۑ�
		CRecentTagjumpKeyword cRecentTagJumpKeyword;
		cRecentTagJumpKeyword.AppendItem( m_pszKeyword );
		cRecentTagJumpKeyword.Terminate();
	}
	//	To Here 2005.04.03 MIK
	if( m_nIndex == -1 || m_nIndex >= m_pcList->GetCapacity() ) return FALSE;

	return TRUE;
}

/*!
	@date 2005.03.31 MIK
		�K�w�J�����̒ǉ��D�L�[���[�h�w�藓�̒ǉ�
*/
BOOL CDlgTagJumpList::OnInitDialog( HWND hwndDlg, WPARAM wParam, LPARAM lParam )
{
	HWND		hwndList;
	LV_COLUMN	col;
	RECT		rc;
	long		lngStyle;

	_SetHwnd( hwndDlg );
	::SetWindowLongPtr( GetHwnd(), DWLP_USER, lParam );

	CreateSizeBox();
	CDialog::OnSize();
	
	::GetWindowRect( hwndDlg, &rc );
	m_ptDefaultSize.x = rc.right - rc.left;
	m_ptDefaultSize.y = rc.bottom - rc.top;

	for( int i = 0; i < _countof(anchorList); i++ ){
		GetItemClientRect( anchorList[i].id, m_rcItems[i] );
	}

	RECT rcDialog = GetDllShareData().m_Common.m_sOthers.m_rcTagJumpDialog;
	if( rcDialog.left != 0 ||
		rcDialog.bottom != 0 ){
		m_xPos = rcDialog.left;
		m_yPos = rcDialog.top;
		m_nWidth = rcDialog.right - rcDialog.left;
		m_nHeight = rcDialog.bottom - rcDialog.top;
	}

	// �E�B���h�E�̃��T�C�Y
	SetDialogPosSize();

	//���X�g�r���[�̕\���ʒu���擾����B
	hwndList = ::GetDlgItem( hwndDlg, IDC_LIST_TAGJUMP );

	// �t�H���g�ݒ�
	HFONT hFontOld = (HFONT)::SendMessageAny( hwndList, WM_GETFONT, 0, 0 );
	HFONT hFont = SetMainFontAndFontSize( hwndList );
	m_listViewFont.SetFont( hFontOld, hFont, hwndList );

	//ListView_DeleteAllItems( hwndList );
	rc.left = rc.top = rc.right = rc.bottom = 0;
	::GetWindowRect( hwndList, &rc );
	
	int nWidth = (rc.right - rc.left) - ::GetSystemMetrics( SM_CXHSCROLL ) - CTextWidthCalc::WIDTH_MARGIN_SCROLLBER;

	col.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt      = LVCFMT_LEFT;
	col.cx       = nWidth * 20 / 100;
	col.pszText  = _T("�V���{��");
	col.iSubItem = 0;
	ListView_InsertColumn( hwndList, 0, &col );

	col.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt      = LVCFMT_LEFT;
	col.cx       = nWidth * 35 / 100;
	col.pszText  = _T("�t�@�C����");
	col.iSubItem = 1;
	ListView_InsertColumn( hwndList, 1, &col );

	col.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt      = LVCFMT_RIGHT;
	col.cx       = nWidth * 8 / 100;
	col.pszText  = _T("�s�ԍ�");
	col.iSubItem = 2;
	ListView_InsertColumn( hwndList, 2, &col );

	col.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt      = LVCFMT_LEFT;
	col.cx       = nWidth * 9 / 100;
	col.pszText  = _T("���");
	col.iSubItem = 3;
	ListView_InsertColumn( hwndList, 3, &col );

	col.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	col.fmt      = LVCFMT_LEFT;
	col.cx       = nWidth * 21 / 100;
	col.pszText  = _T("���l");
	col.iSubItem = 4;
	ListView_InsertColumn( hwndList, 4, &col );

	/* �s�I�� */
	lngStyle = ListView_GetExtendedListViewStyle( hwndList );
	lngStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle( hwndList, lngStyle );

	// �_�C���N�g�^�u�W�����v�̎��́A�L�[���[�h�� Disable �ɁB
	HWND hwndKey = GetItemHwnd( IDC_KEYWORD );
	if( IsDirectTagJump() ){
		::EnableWindow( hwndKey, FALSE );
		::SetFocus( hwndList );
	}else{
		::SetFocus( hwndKey );
	}

	/* ���N���X�����o */
	CDialog::OnInitDialog( GetHwnd(), wParam, lParam );

	return FALSE;
}

BOOL CDlgTagJumpList::OnBnClicked( int wID )
{
	switch( wID )
	{
	case IDOK:			/* ���E�ɕ\�� */
		{
			BOOL succeeded = GetData();
			if( succeeded == TRUE ){
				StopTimer();
				::EndDialog( GetHwnd(), succeeded );
				return TRUE;
			}
			return FALSE;
		}
		break;

	case IDCANCEL:
		StopTimer();
		::EndDialog( GetHwnd(), FALSE );
		return TRUE;
	}

	/* ���N���X�����o */
	return CDialog::OnBnClicked( wID );
}



INT_PTR CDlgTagJumpList::DispatchEvent( HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam )
{
	INT_PTR result;
	result = CDialog::DispatchEvent( hWnd, wMsg, wParam, lParam );

	if( wMsg == WM_GETMINMAXINFO ){
		return OnMinMaxInfo( lParam );
	}
	return result;
}



BOOL CDlgTagJumpList::OnSize( WPARAM wParam, LPARAM lParam )
{
	/* ���N���X�����o */
	CDialog::OnSize( wParam, lParam );

	::GetWindowRect( GetHwnd(), &GetDllShareData().m_Common.m_sOthers.m_rcTagJumpDialog );

	RECT  rc;
	POINT ptNew;
	::GetWindowRect( GetHwnd(), &rc );
	ptNew.x = rc.right - rc.left;
	ptNew.y = rc.bottom - rc.top;

	for( int i = 0 ; i < _countof(anchorList); i++ ){
		ResizeItem( GetItemHwnd(anchorList[i].id), m_ptDefaultSize, ptNew, m_rcItems[i], anchorList[i].anchor );
	}
	::InvalidateRect( GetHwnd(), NULL, TRUE );
	return TRUE;
}



BOOL CDlgTagJumpList::OnMove( WPARAM wParam, LPARAM lParam )
{
	::GetWindowRect( GetHwnd(), &GetDllShareData().m_Common.m_sOthers.m_rcTagJumpDialog );

	return CDialog::OnMove( wParam, lParam );
}



BOOL CDlgTagJumpList::OnMinMaxInfo( LPARAM lParam )
{
	LPMINMAXINFO lpmmi = (LPMINMAXINFO) lParam;
	if( m_ptDefaultSize.x < 0 ){
		return 0;
	}
	lpmmi->ptMinTrackSize.x = m_ptDefaultSize.x;
	lpmmi->ptMinTrackSize.y = m_ptDefaultSize.y;
	lpmmi->ptMaxTrackSize.x = m_ptDefaultSize.x*2;
	lpmmi->ptMaxTrackSize.y = m_ptDefaultSize.y*3;
	return 0;
}



BOOL CDlgTagJumpList::OnNotify( WPARAM wParam, LPARAM lParam )
{
	NMHDR*	pNMHDR;
	HWND	hwndList;

	pNMHDR = (NMHDR*)lParam;

	hwndList = GetDlgItem( GetHwnd(), IDC_LIST_TAGJUMP );

	//	���ꗗ���X�g�{�b�N�X
	if( hwndList == pNMHDR->hwndFrom )
	{
		switch( pNMHDR->code )
		{
		case NM_DBLCLK:
			StopTimer();
			::EndDialog( GetHwnd(), GetData() );
			return TRUE;
		}
	}

	/* ���N���X�����o */
	return CDialog::OnNotify( wParam, lParam );
}

/*!
	�^�C�}�[�o��

	�^�C�}�[���~���C��⃊�X�g���X�V����
*/
BOOL CDlgTagJumpList::OnTimer( WPARAM wParam )
{
	StopTimer();

	FindNext( true );

	return TRUE;
}

/*!
	�^�C�}�[�o��

	�^�C�}�[���J�n���C��⃊�X�g���X�V���鏀��������
*/
BOOL CDlgTagJumpList::OnCbnEditChange( HWND hwndCtl, int wID )
{
	StartTimer();

	/* ���N���X�����o */
	return CDialog::OnCbnEditChange( hwndCtl, wID );
}

BOOL CDlgTagJumpList::OnCbnSelChange( HWND hwndCtl, int wID )
{
	StartTimer();

	/* ���N���X�����o */
	return CDialog::OnCbnSelChange( hwndCtl, wID );
}

bool CDlgTagJumpList::GetSelectedParam( TCHAR *s0, TCHAR *s1, int *n2, TCHAR *s3, TCHAR *s4, int *depth, TCHAR *baseDir )
{
	if( 1 != m_pcList->GetCount() ){
		if( -1 == m_nIndex || m_nIndex >= m_pcList->GetCount() ) return false;
	}else{
		m_nIndex = 0;
	}

	m_pcList->GetParam( m_nIndex, s0, s1, n2, &s3[0], s4, depth, baseDir );

	return true;
}

bool CDlgTagJumpList::GetSelectedFullPathAndLine( TCHAR *fullPath, int count, int *lineNum, int *depth )
{
	TCHAR path[1024];
	TCHAR fileName[1024];
	TCHAR dirFileName[1024];
	int tempDepth = 0;
	SplitPath_FolderAndFile( GetFilePath(), path, NULL );
	AddLastYenFromDirectoryPath( path );
	
	if( false == GetSelectedParam( NULL, fileName, lineNum, NULL, NULL, &tempDepth, dirFileName ) ){
		return false;
	}
	if( depth ){
		*depth = tempDepth;
	}
	const TCHAR* fileNamePath;
	// �t�@�C�����A�f�B���N�g���w��A��t�@�C���p�X�A�̏��ɓK�p�B�r���Ńt���p�X�Ȃ炻�̂܂܁B
	if( dirFileName[0] ){
		AddLastYenFromDirectoryPath( dirFileName );
		const TCHAR	*p = fileName;
		if( p[0] == _T('\\') ){
			if( p[1] == _T('\\') ){
				auto_strcpy( dirFileName, p );
			}else{
				auto_strcpy( dirFileName, p );
			}
		}else if( _istalpha( p[0] ) && p[1] == _T(':') ){
			auto_strcpy( dirFileName, p );
		}else{
			// ���΃p�X�F�A������
			auto_strcat( dirFileName, p );
		}
		fileNamePath = dirFileName;
	}else{
		fileNamePath = fileName;
	}
	bool ret = NULL != GetFullPathFromDepth( fullPath, count, path, fileNamePath, tempDepth );
	if(ret){
		DEBUG_TRACE( _T("jump to: %ts\n"), static_cast<const TCHAR*>(fullPath) );
	}else{
		DEBUG_TRACE( _T("jump to: error\n") );
	}
	return ret;
}

/*!
	@return �u.ext�v�`���̃^�C�v���B free���邱��
*/
TCHAR *CDlgTagJumpList::GetNameByType( const TCHAR type, const TCHAR *name )
{
	const TCHAR	*p;
	TCHAR	*token;
	int		i;
	//	2005.03.31 MIK
	TCHAR	tmp[MAX_TAG_STRING_LENGTH];

	p = _tcsrchr( name, _T('.') );
	if( ! p ) p = _T(".c");	//������Ȃ��Ƃ��� ".c" �Ƒz�肷��B
	p++;

	for( i = 0; p_extentions[i]; i += 2 )
	{
		_tcscpy( tmp, p_extentions[i] );
		token = _tcstok( tmp, _T(",") );
		while( token )
		{
			if( _tcsicmp( p, token ) == 0 )
			{
				_tcscpy( tmp, p_extentions[i+1] );
				token = _tcstok( tmp, _T(",") );
				while( token )
				{
					if( token[0] == type )
					{
						return _tcsdup( &token[2] );
					}

					token = _tcstok( NULL, _T(",") );
				}

				return _tcsdup( _T("") );
			}

			token = _tcstok( NULL, _T(",") );
		}
	}

	return _tcsdup( _T("") );
}

/*!
	��t�@�C������ݒ�
*/
void CDlgTagJumpList::SetFileName( const TCHAR *pszFileName )
{
	assert_warning( pszFileName );
	if( NULL == pszFileName ) return;

	if( m_pszFileName ) free( m_pszFileName );

	m_pszFileName = _tcsdup( pszFileName );
	
	m_nLoop = CalcMaxUpDirectory( m_pszFileName );
}

/*!
	�����L�[���[�h�̐ݒ�

*/
void CDlgTagJumpList::SetKeyword( const wchar_t *pszKeyword )
{
	if( NULL == pszKeyword ) return;

	if( m_pszKeyword ) free( m_pszKeyword );

	m_pszKeyword = wcsdup( pszKeyword );

	return;
}

/*!
	����ꂽ��₩��ł����҂ɋ߂��Ǝv������̂�
	�I�яo���D(�����I���ʒu����̂���)

	@return �I�����ꂽ�A�C�e����index

*/
int CDlgTagJumpList::SearchBestTag( void )
{
	if( m_pcList->GetCount() <= 0 ) return -1;	//�I�ׂ܂���B
	if( NULL == m_pszFileName ) return 0;

	TCHAR	szFileSrc[1024];
	TCHAR	szFileDst[1024];
	TCHAR	szExtSrc[1024];
	TCHAR	szExtDst[1024];
	int		nMatch = -1;
	int		i;
	int		count;

	_tcscpy( szFileSrc, _T("") );
	_tcscpy( szExtSrc,  _T("") );
	_tsplitpath( m_pszFileName, NULL, NULL, szFileSrc, szExtSrc );

	count = m_pcList->GetCount();

	for( i = 0; i < count; i++ )
	{
		CSortedTagJumpList::TagJumpInfo* item;
		item = m_pcList->GetPtr( i );

		_tcscpy( szFileDst, _T("") );
		_tcscpy( szExtDst,  _T("") );
		_tsplitpath( item->filename, NULL, NULL, szFileDst, szExtDst );
		
		if( _tcsicmp( szFileSrc, szFileDst ) == 0 )
		{
			if( _tcsicmp( szExtSrc, szExtDst ) == 0 ) return i;
			if( nMatch == -1 ) nMatch = i;
		}
	}

	if( nMatch != -1 ) return nMatch;

	return 0;
}

/*!
	@param bNewFind �V������������(���E�O�̂Ƃ�false)
*/
void CDlgTagJumpList::FindNext( bool bNewFind )
{
	wchar_t	szKey[ MAX_TAG_STRING_LENGTH ];
	wcscpy( szKey, L"" );
	::DlgItem_GetText( GetHwnd(), IDC_KEYWORD, szKey, _countof( szKey ) );
	find_key( szKey );
	UpdateData( false );
}

/*!
	�_�C���N�g�^�O�W�����v����(DoModal�O�Ɏ��s)
*/
int CDlgTagJumpList::FindDirectTagJump()
{
	return find_key_core(
		m_pszKeyword,
		true,  // ���S��v
		true  // �������[�h
	);
}

void CDlgTagJumpList::find_key( const wchar_t* keyword )
{
	find_key_core(
		keyword,
		IsDirectTagJump(),
		IsDirectTagJump()
	);
}

/*!
	�^�O�t�@�C������L�[���[�h�Ƀ}�b�`����f�[�^�𒊏o���Cm_cList�ɐݒ肷��

	@date 2007.03.13 genta �o�b�t�@�I�[�o�[�����b��Ώ��Ńo�b�t�@�T�C�Y�ύX
	@date 2010.04.02 Moca ���낢��ύX�BSJIS�œǂށB�y�[�W���O, format=1�̉��߁A�^�O�t�@�C�����̗��p
		�u�Stags�̌������ʂ��\�[�g���Đ擪����Capaticy�܂Łv���utags�t�@�C����(=depth)���A�L�[���[�h���v�ɕύX
	@date 2010.07.21 find_key��find_key_core�ɂ��āACViewCommander::Command_TagJumpByTagsFile�Ɠ���
*/
int CDlgTagJumpList::find_key_core(
	const wchar_t* keyword,
	bool bTagJumpExactMatch, // ���S��v
	bool bTagJumpICaseByTags // Tag�t�@�C�����̃\�[�g�ɏ]��
){
	// to_achar�͈ꎞ�o�b�t�@�Ŕj�󂳂��\��������̂ŃR�s�[
	CNativeA cmemKeyA = CNativeA(to_achar(keyword));
	const ACHAR* paszKeyword = cmemKeyA.GetStringPtr();
	int	length = cmemKeyA.GetStringLength();

	Empty();

	m_bNextItem = false;

	if( length == 0 ){
		return -1;
	}
	// �����ϊ����Ă݂Ĉ�v���Ȃ�������A�����L�[�ɂ͈�v���Ȃ��Ƃ������Ƃɂ���
	if( 0 != wcscmp( to_wchar( paszKeyword ), keyword ) ){
		return -1;
	}
	STagFindState state;
	state.m_nDepth    = 0;
	state.m_nMatchAll = 0;
	state.m_szCurPath[0] = 0;

	// �t�@�C�������R�s�[�������ƁA�f�B���N�g��(�Ō�\)�݂̂ɂ���
	_tcscpy( state.m_szCurPath, GetFilePath() );
	state.m_szCurPath[ GetFileName() - GetFilePath() ] = _T('\0');

	TCHAR	szTagFile[1024];		//�^�O�t�@�C��
	vector_ex<std::tstring> seachDirs;

	// �p�X��Jump�ŏz���Ă���ꍇ�ɍő�l���K������
	for( ; state.m_nDepth <= m_nLoop && state.m_nDepth < (_MAX_PATH/2); state.m_nDepth++ )
	{
		std::tstring curPath = state.m_szCurPath;
		if( seachDirs.exist( curPath ) ){
			// �����ς� =>�I��
			break;
		}
		seachDirs.push_back( curPath );

		//�^�O�t�@�C�������쐬����B
		auto_sprintf( szTagFile, _T("%ts%ts"), state.m_szCurPath, TAG_FILENAME_T );
		DEBUG_TRACE( _T("tag: %ts\n"), szTagFile );

		if( bTagJumpExactMatch == true ){
			int matchNum = SearchKeywordInTagFile( &szTagFile[0], paszKeyword, length,
												   false, bTagJumpExactMatch, false, bTagJumpICaseByTags,
												   0, state );
		}else{
			int matchNum = SearchKeywordInTagFile( &szTagFile[0], paszKeyword, length,
												   false, true, false, false,
												   0, state );
			if( matchNum == 0 ){
				matchNum = SearchKeywordInTagFile( &szTagFile[0], paszKeyword, length,
												   false, false, false, false,
												   0, state );
				if( matchNum < TAGJUMP_LIST_MAX ){
					matchNum = SearchKeywordInTagFile( &szTagFile[0], paszKeyword, length,
													   true, false, false, false,
													   0, state );
				}
				if( matchNum < TAGJUMP_LIST_MAX ){
					matchNum = SearchKeywordInTagFile( &szTagFile[0], paszKeyword, length,
													   false, false, true, false,
													   0, state );
				}
				if( matchNum < TAGJUMP_LIST_MAX ){
					matchNum = SearchKeywordInTagFile( &szTagFile[0], paszKeyword, length,
													   true, false, true, false,
													   0, state );
				}
				if( matchNum < TAGJUMP_LIST_MAX ){
					matchNum = FuzzyMatchInTagFile( &szTagFile[0], paszKeyword, length,
													false, bTagJumpExactMatch, false, bTagJumpICaseByTags,
													0, state );
				}
				if( matchNum < TAGJUMP_LIST_MAX ){
					matchNum = FuzzyMatchInTagFile( &szTagFile[0], paszKeyword, length,
													false, bTagJumpExactMatch, true, bTagJumpICaseByTags,
													0, state );
				}
			}
		}
		//�J�����g�p�X��1�K�w��ցB
		DirUp( state.m_szCurPath );

		if( m_bNextItem ){
			break;
		}
	}
	return state.m_nMatchAll;
}

int CDlgTagJumpList::SearchKeywordInTagFile
	(
		TCHAR* pTagFileName,
		const ACHAR* paszKeyword,
		int length,
		bool bTagJumpAnyWhere,
		bool bTagJumpExactMatch,
		bool bTagJumpICase,
		bool bTagJumpICaseByTags,
		int nTop, // TODO ���g�p
		struct STagFindState& rState
	)
{
	ACHAR	szLineData[1024];		//�s�o�b�t�@
	ACHAR	s[4][1024];
	int		n2;
	CSortedTagJumpList& cList = *m_pcList;
	const int listCapacity = cList.GetCapacity();

	//�^�O�t�@�C�����J���B
	FILE* fp = _tfopen( pTagFileName, _T("rb") );
	if(fp == NULL){
		return 0;
	}

	bool bSorted = true;
	bool bFoldcase = false;
	int  nTagFormat = 2; // 2��1���ǂ߂�̂Ńf�t�H���g��2
	int  nLines = 0;
	int  baseDirId = 0;

	// �o�b�t�@�̌�납��2�����ڂ�\0���ǂ����ŁA�s���܂œǂݍ��񂾂��m�F����
	const int LINEDATA_LAST_CHAR_INDEX = _countof( szLineData ) - 2;
	szLineData[LINEDATA_LAST_CHAR_INDEX] = '\0';
	while( fgets( szLineData, _countof( szLineData ), fp ) )
	{
		nLines++;
		int  nRet;
		// fgets���s���ׂĂ�ǂݍ��߂Ă��Ȃ��ꍇ�̍l��
		if( '\0' != szLineData[LINEDATA_LAST_CHAR_INDEX]
		    && '\n' != szLineData[LINEDATA_LAST_CHAR_INDEX] ){
			// ���s�R�[�h�܂ł��̂Ă�
			int ch = fgetc( fp );
			while( ch != '\n' && ch != EOF ){
				ch = fgetc( fp );
			}
		}
		if( 1 == nLines && szLineData[0] == '\x0c' ){
			// etags�Ȃ̂Ŏ��̃t�@�C��
			break;
		}
		if( '!' == szLineData[0] ){
			if( 0 == strncmp_literal( szLineData + 1, "_TAG_" ) ){
				s[0][0] = s[1][0] = s[2][0] = 0;
				nRet = sscanf(
					szLineData, 
					TAG_FILE_INFO_A,	//tags�t�@�C�����
					s[0], s[1], s[2]
				);
				if( nRet < 2 ) goto next_line;
				const ACHAR* pTag = s[0] + 6;
				if( 0 == strncmp_literal(pTag , "FILE_FORMAT" ) ){
					n2 = atoi( s[1] );
					if( 1 <=n2 && n2 <= 2 ){
						nTagFormat = n2;
					}
				}else if( 0 == strncmp_literal( pTag, "FILE_SORTED" ) ){
					n2 = atoi( s[1] );
					bSorted   = (1 == n2);
					bFoldcase = (2 == n2);
					if( bTagJumpICaseByTags ){
						bTagJumpICase = bFoldcase;
					}
				}
			}
			goto next_line;	//�R�����g�Ȃ�X�L�b�v
		}
		if( szLineData[0] < '!' ) goto next_line;

		s[0][0] = s[1][0] = s[2][0] = s[3][0] = '\0';
		n2 = 0;
		if( 2 == nTagFormat ){
			nRet = sscanf(
				szLineData, 
				TAG_FORMAT_2_A,	//�g��tags�t�H�[�}�b�g
				s[0], s[1], &n2, s[2], s[3]
				);
			if( nRet < 3 ) goto next_line;
			if( n2 <= 0 ) goto next_line;	//�s�ԍ��s��(-excmd=n���w�肳��ĂȂ�����)
		}else{
			nRet = sscanf(
				szLineData, 
				TAG_FORMAT_1_A,	//tags�t�H�[�}�b�g
				s[0], s[1], &n2
				);
			if( nRet < 2 ) goto next_line;
			if( n2 <= 0 ) goto next_line;
		}

		int  cmp;
		if( bTagJumpAnyWhere ){
			if( bTagJumpICase ){
				if(stristr_j(s[0], paszKeyword) == NULL){
					cmp = -1;
				}else{
					cmp = 0;
				}
			}else{
				if(strstr_j(s[0], paszKeyword) == NULL){
					cmp = -1;
				}else{
					cmp = 0;
				}
			}
		}else{
			if( bTagJumpExactMatch ){
				// ���S��v
				if( bTagJumpICase ){
					cmp = auto_stricmp( s[0], paszKeyword );
				}else{
					cmp = auto_strcmp( s[0], paszKeyword );
				}
			}else{
				// �O����v
				if( bTagJumpICase ){
					cmp = my_strnicmp( s[0], paszKeyword, length );
				}else{
					cmp = strncmp( s[0], paszKeyword, length );
				}
			}
		}
		if(cmp == 0){
			cList.AddParamA( s[0], s[1], n2, s[2][0], s[3], rState.m_nDepth, baseDirId );
			rState.m_nMatchAll++;
			if(rState.m_nMatchAll >= TAGJUMP_LIST_MAX ){
				// �T���ł��؂�
				m_bNextItem = true;
				break;
			}
		}
		else if(cmp > 0){
			//	tags�̓\�[�g����Ă���̂ŁC�擪�����case sensitive��
			//	��r���ʂɂ���Č����̎��͏����̑ł��؂肪�\
			if( (!bTagJumpICase) && bSorted && (!bTagJumpAnyWhere) ) break;
			// 2010.07.21 Foldcase�����ł��؂�B������tags�ƃT�N�����̃\�[�g���������łȂ���΂Ȃ�Ȃ�
			if( bTagJumpICase  && bFoldcase && (!bTagJumpAnyWhere) ) break;
		}
next_line:
		;
		szLineData[LINEDATA_LAST_CHAR_INDEX] = '\0';
	}

	//�t�@�C�������B
	fclose( fp );

	return rState.m_nMatchAll;
}


int CDlgTagJumpList::FuzzyMatchInTagFile
	(
		TCHAR* pTagFileName,
		const ACHAR* paszKeyword,
		int length,
		bool bTagJumpAnyWhere, // TODO false �����n����Ȃ�
		bool bTagJumpExactMatch, // TODO ���g�p
		bool bTagJumpICase,
		bool bTagJumpICaseByTags,
		int nTop, // TODO ���g�p
		struct STagFindState& rState
	)
{
	ACHAR	szLineData[1024];		//�s�o�b�t�@
	ACHAR	s[4][1024];
	int		n2;
	CSortedTagJumpList& cList = *m_pcList;
	const int listCapacity = cList.GetCapacity();

	//�^�O�t�@�C�����J���B
	FILE* fp = _tfopen( pTagFileName, _T("rb") );
	if(fp == NULL){
		return 0;
	}

	bool bSorted = true;
	bool bFoldcase = false;
	int  nTagFormat = 2; // 2��1���ǂ߂�̂Ńf�t�H���g��2
	int  nLines = 0;
	int  baseDirId = 0;

	// �o�b�t�@�̌�납��2�����ڂ�\0���ǂ����ŁA�s���܂œǂݍ��񂾂��m�F����
	const int LINEDATA_LAST_CHAR_INDEX = _countof( szLineData ) - 2;
	szLineData[LINEDATA_LAST_CHAR_INDEX] = '\0';
	while( fgets( szLineData, _countof( szLineData ), fp ) )
	{
		nLines++;
		int  nRet;
		// fgets���s���ׂĂ�ǂݍ��߂Ă��Ȃ��ꍇ�̍l��
		if( '\0' != szLineData[LINEDATA_LAST_CHAR_INDEX]
		    && '\n' != szLineData[LINEDATA_LAST_CHAR_INDEX] ){
			// ���s�R�[�h�܂ł��̂Ă�
			int ch = fgetc( fp );
			while( ch != '\n' && ch != EOF ){
				ch = fgetc( fp );
			}
		}
		if( 1 == nLines && szLineData[0] == '\x0c' ){
			// etags�Ȃ̂Ŏ��̃t�@�C��
			break;
		}
		if( '!' == szLineData[0] ){
			if( 0 == strncmp_literal( szLineData + 1, "_TAG_" ) ){
				s[0][0] = s[1][0] = s[2][0] = 0;
				nRet = sscanf(
					szLineData, 
					TAG_FILE_INFO_A,	//tags�t�@�C�����
					s[0], s[1], s[2]
				);
				if( nRet < 2 ) goto next_line;
				const ACHAR* pTag = s[0] + 6;
				if( 0 == strncmp_literal(pTag , "FILE_FORMAT" ) ){
					n2 = atoi( s[1] );
					if( 1 <=n2 && n2 <= 2 ){
						nTagFormat = n2;
					}
				}else if( 0 == strncmp_literal( pTag, "FILE_SORTED" ) ){
					n2 = atoi( s[1] );
					bSorted   = (1 == n2);
					bFoldcase = (2 == n2);
					if( bTagJumpICaseByTags ){
						bTagJumpICase = bFoldcase;
					}
				}
			}
			goto next_line;	//�R�����g�Ȃ�X�L�b�v
		}
		if( szLineData[0] < '!' ) goto next_line;

		s[0][0] = s[1][0] = s[2][0] = s[3][0] = '\0';
		n2 = 0;
		if( 2 == nTagFormat ){
			nRet = sscanf(
				szLineData, 
				TAG_FORMAT_2_A,	//�g��tags�t�H�[�}�b�g
				s[0], s[1], &n2, s[2], s[3]
				);
			if( nRet < 3 ) goto next_line;
			if( n2 <= 0 ) goto next_line;	//�s�ԍ��s��(-excmd=n���w�肳��ĂȂ�����)
		}else{
			nRet = sscanf(
				szLineData, 
				TAG_FORMAT_1_A,	//tags�t�H�[�}�b�g
				s[0], s[1], &n2
				);
			if( nRet < 2 ) goto next_line;
			if( n2 <= 0 ) goto next_line;
		}

		if( bTagJumpICase ){
			if( isIncludedCharsByCaseInsensitive(s[0], paszKeyword) == true ){
				cList.AddParamA( s[0], s[1], n2, s[2][0], s[3], rState.m_nDepth, baseDirId );
				rState.m_nMatchAll++;
				if(rState.m_nMatchAll >= TAGJUMP_LIST_MAX ){
					// �T���ł��؂�
					m_bNextItem = true;
					break;
				}
			}
		}else{
			if( isIncludedCharsByCaseSensitive(s[0], paszKeyword) == true ){
				cList.AddParamA( s[0], s[1], n2, s[2][0], s[3], rState.m_nDepth, baseDirId );
				rState.m_nMatchAll++;
				if(rState.m_nMatchAll >= TAGJUMP_LIST_MAX ){
					// �T���ł��؂�
					m_bNextItem = true;
					break;
				}
			}
		}

next_line:
		;
		szLineData[LINEDATA_LAST_CHAR_INDEX] = '\0';
	}

	//�t�@�C�������B
	fclose( fp );

	return rState.m_nMatchAll;
}


/*!
	�p�X����t�@�C���������݂̂����o���D(2�o�C�g�Ή�)
*/
const TCHAR* CDlgTagJumpList::GetFileName( void )
{
	return GetFileTitlePointer(GetFilePath());
}


void CDlgTagJumpList::SetTextDir()
{
	if( GetHwnd() ){
		DlgItem_SetText( GetHwnd(), IDC_STATIC_BASEDIR, _T("") );
		if( GetFileName() ){
			std::tstring strPath = GetFilePath();
			strPath[ GetFileName() - GetFilePath() ] = _T('\0');
			DlgItem_SetText( GetHwnd(), IDC_STATIC_BASEDIR, strPath.c_str() );
		}
	}
}

int CDlgTagJumpList::CalcMaxUpDirectory( const TCHAR* p )
{
	int loop = CalcDirectoryDepth( p );
	if( loop <  0 ) loop =  0;
	if( loop > (_MAX_PATH/2) ) loop = (_MAX_PATH/2);	//\A\B\C...�̂悤�ȂƂ�1�t�H���_��2���������̂�...
	return loop;
}

/*!
	
	@date 2010.04.02 Moca Command_TagJumpByTagsFileKeyword���番���E�ړ�
	@param basePath [in,out] \�t�f�B���N�g���p�X��΃p�X�����B���������̂ɒ���
	@param fileName [in] ���΁E��΃t�@�C�����p�X
	@param depth    [in] fineName����΃p�X�̎������B1==1��̃f�B���N�g��
	@retval pszOutput ���� �uC:\dir1\filename.txt�v�̌`��(..\�t���͔p�~)
	@retval NULL   ���s
*/
TCHAR* CDlgTagJumpList::GetFullPathFromDepth( TCHAR* pszOutput, int count,
	TCHAR* basePath, const TCHAR* fileName, int depth )
{
	DEBUG_TRACE( _T("base  %ts\n"), basePath );
	DEBUG_TRACE( _T("file  %ts\n"), fileName );
	DEBUG_TRACE( _T("depth %d\n"),  depth );
	//���S�p�X�����쐬����B
	const TCHAR	*p = fileName;
	if( p[0] == _T('\\') ){	//�h���C�u�Ȃ���΃p�X���H
		if( p[1] == _T('\\') ){	//�l�b�g���[�N�p�X���H
			_tcscpy( pszOutput, p );	//�������H���Ȃ��B
		}else{
			//�h���C�u���H�����ق����悢�H
			_tcscpy( pszOutput, p );	//�������H���Ȃ��B
		}
	}else if( _istalpha( p[0] ) && p[1] == _T(':') ){	//��΃p�X���H
		_tcscpy( pszOutput, p );	//�������H���Ȃ��B
	}else{
		for( int i = 0; i < depth; i++ ){
			//_tcscat( basePath, _T("..\\") );
			DirUp( basePath );
		}
		if( -1 == auto_snprintf_s( pszOutput, count, _T("%ts%ts"), basePath, p ) ){
			return NULL;
		}
	}
	return pszOutput;
}

/*!
	�f�B���N�g���ƃf�B���N�g����A������
*/
TCHAR* CDlgTagJumpList::CopyDirDir( TCHAR* dest, const TCHAR* target, const TCHAR* base )
{
	if( _IS_REL_PATH( target ) ){
		auto_strcpy( dest, base );
		AddLastYenFromDirectoryPath( dest );
		auto_strcat( dest, target );
	}else{
		auto_strcpy( dest, target );
	}
	AddLastYenFromDirectoryPath( dest );
	return dest;
}

/*
	@param dir [in,out] �t�H���_�̃p�X 
	in == C:\dir\subdir\
	out == C:\dir\
*/
TCHAR* CDlgTagJumpList::DirUp( TCHAR* dir )
{
	CutLastYenFromDirectoryPath( dir );
	const TCHAR *p = GetFileTitlePointer(dir); //�Ō��\�̎��̕������擾 last_index_of('\\') + 1;
	if( 0 < p - dir){
		dir[p - dir] = '\0';
	}
	return dir;
}
