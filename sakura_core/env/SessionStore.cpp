/*!
 * \file SessionStore.cpp
 * \brief �I�����ɕҏW���̃t�@�C���Ɋւ�������L�^���A�ċN����ɕ�������.
 *
 * \author Suzuki Satoshi
 */
/*
 *  Copyright (C) 2015, Suzuki Satoshi
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose, 
 *  including commercial applications, and to alter it and redistribute it 
 *  freely, subject to the following restrictions:
 *
 *      1. The origin of this software must not be misrepresented;
 *         you must not claim that you wrote the original software.
 *         If you use this software in a product, an acknowledgment
 *         in the product documentation would be appreciated but is
 *         not required.
 *
 *      2. Altered source versions must be plainly marked as such, 
 *         and must not be misrepresented as being the original software.
 *
 *      3. This notice may not be removed or altered from any source
 *         distribution.
 */

#include "StdAfx.h"
#include "CShareData.h"
#include "EditInfo.h"
#include "CAppNodeManager.h"
#include "window/CEditWnd.h"
#include "_main/CControlTray.h"
#include "_main/CMutex.h"
#include "_main/CProcess.h"
#include "util/file.h"
#include "util/module.h"

#include "SessionStore.h"

#include "stdio.h"


SessionStore* SessionStore::pUniqInstance = NULL;

/*!
 * SessionStore �N���X�̗B��̃C���X�^���X�𐶐�����.
 */
void SessionStore::createUniqInstance()
{
	if(pUniqInstance == NULL){
		pUniqInstance = new SessionStore;
		pUniqInstance->pMutex = new CMutex(FALSE, L"MutexBenizakuraSessionStore");
	}
}

/*!
 * SessionStore �N���X�̗B��̃C���X�^���X��j������.
 */
void SessionStore::destroyUniqInstance()
{
	delete pUniqInstance->pMutex;
	delete pUniqInstance;
	pUniqInstance = NULL;
}

/*!
 * �ҏW���t�@�C���̈ꗗ���Z�b�V�������t�@�C���ɕۑ�����.
 */
void SessionStore::save(void)
{
	pMutex->Lock();

	DLLSHAREDATA* pShareData = CShareData::getInstance()->GetShareData();
	if(pShareData->m_sFlags.sessionSaved == true){
		pMutex->Unlock();
		return;
	}
	pShareData->m_sFlags.sessionSaved = true;

	EditNode* pEditNodeArr;
	int winNum = CAppNodeManager::getInstance()->GetOpenedWindowArr(&pEditNodeArr, TRUE);
	{
		fileOpenForSave();
		for(int i = 0; i < winNum; i++){
			::SendMessage(pEditNodeArr[i].GetHwnd(), MYWM_GETFILEINFO, 0, 0);
			EditInfo* pEditInfo = (EditInfo*)&pShareData->m_sWorkBuffer.m_EditInfo_MYWM_GETFILEINFO;
			if(wcscmp(pEditInfo->m_szPath, L"") != 0){
				fileOneRecordWrite(pEditInfo->m_szPath);
			}
		}
		fileClose();
	}
	delete [] pEditNodeArr;

	pShareData->m_sFlags.sessionRestored = false;

	pMutex->Unlock();
}

/*!
 * �Z�b�V�������t�@�C�������ɁA�Z�b�V�����̕����i�����̃G�f�B�^�v���Z�X�����j���s��.
 */
void SessionStore::restore(CEditWnd* pEditWnd, EditInfo* pEditInfo)
{
	DLLSHAREDATA* pShareData = CShareData::getInstance()->GetShareData();

	if(pShareData->m_sFlags.sessionRestored == true){
		return;
	}

	pMutex->Lock();
	if(pShareData->m_sFlags.sessionRestored == true){
		pMutex->Unlock();
		return;
	}
	pShareData->m_sFlags.sessionRestored = true;

	if(fileOpenForRestore() == false){
		pShareData->m_sFlags.sessionSaved = false;
		pMutex->Unlock();
		return;
	}

	EditInfo resoreFileInfo = *pEditInfo;
	wchar_t fullPathFileName[_MAX_PATH + 1];
	bool succeeded;
	succeeded = fileOneRecordRead(fullPathFileName);
	while(succeeded == true){
		if(IsFileExists(fullPathFileName, true) == true){
			wcscpy_s(resoreFileInfo.m_szPath, _MAX_PATH, fullPathFileName);
			CControlTray::OpenNewEditor2(
										CProcess::getInstance()->GetProcessInstance(), pEditWnd->GetHwnd(),
										&resoreFileInfo, false, true, false
										);
		}
		succeeded = fileOneRecordRead(fullPathFileName);
	}
	fileClose();

	/* �t�@�C�������w�肵�ċN�����ꂽ�ꍇ�ɂ́A�w��t�@�C���̃E�B���h�E���A�N�e�B�u�ɕύX���� */
	if(pEditInfo->m_szPath[0] != L'\0'){
		HWND hwndOwner;
		if(CShareData::getInstance()->IsPathOpened(pEditInfo->m_szPath, &hwndOwner)){
			ActivateFrameWindow(hwndOwner);
		}
	}

	pShareData->m_sFlags.sessionSaved = false;

	pMutex->Unlock();
}


#define STORE_FILE_NAME L"BeniZakura.PreviouslyOpenedFiles.txt"
/*!
 * �������ݗp�ɃZ�b�V�������t�@�C�����J��.
 */
void SessionStore::fileOpenForSave(void)
{
	wchar_t fullPathFileName[_MAX_PATH + 1];

	GetInidir(fullPathFileName, STORE_FILE_NAME);

	fp = _wfopen(fullPathFileName, L"w");
}

/*!
 * �Z�b�V�������t�@�C���ɂP���R�[�h��������.
 */
void SessionStore::fileOneRecordWrite(const wchar_t* pRecord)
{
	fwprintf(fp, L"%s\n", pRecord);
}

/*!
 * �ǂݍ��ݗp�ɃZ�b�V�������t�@�C�����J��.
 */
bool SessionStore::fileOpenForRestore(void)
{
	wchar_t fullPathFileName[_MAX_PATH + 1];

	GetInidir(fullPathFileName, STORE_FILE_NAME);

	fp = _wfopen(fullPathFileName, L"r");
	if(fp == NULL){
		return false;
	}

	return true;
}

/*!
 * �Z�b�V�������t�@�C������P���R�[�h�ǂݍ���.
 */
bool SessionStore::fileOneRecordRead(wchar_t* pRecord)
{
	wchar_t *p;

	p = fgetws(pRecord, _MAX_PATH, fp);
	if(p == NULL){
		return false;
	}

	p = wcschr(pRecord, L'\n');
	if(p != NULL){
		*p = L'\0';
	}

	return true;
}

/*!
 * �Z�b�V�������t�@�C�������.
 */
void SessionStore::fileClose(void)
{
	fclose(fp);
}

