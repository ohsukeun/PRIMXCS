/**
@file SNPHashList.h
@brief Key가 되는 스크링으로 정보를 검색하여 관리하는 리스트 클래스.
@date 2010/04/01
@version 1.0
@author hylee
*/



#pragma once
#include <afxmt.h>
#include <afxtempl.h>
#include <afxcoll.h>


#define	SIZE_HASH_53			53
#define SIZE_HASH_GENERAL		2003
#define SIZE_HASH_MEDIUM		5003
#define SIZE_HASH_LARGE			7001
#define SIZE_HASH_HUGE			19997


typedef int (__cdecl *SortComparisonFunc)(const void* elem1,const void* elem2); //( LPVOID elem1,LPVOID elem2);
//int (__cdecl *SortComparisonFunc )(const void *, const void *) 
/*
< 0	: elem1 less than elem2
0	: elem1 equivalent to elem2
> 0	: elem1 greater than elem2
*/


/// @class CSNPHashList
/// @brief Key가 되는 스크링으로 정보를 검색하여 관리하는 리스트 클래스.
template<class B_NODE>
class CSNPHashList
{
public:
	CSNPHashList(SortComparisonFunc pSortComparisonFunc = NULL, UINT HashSize = SIZE_HASH_GENERAL);
	virtual ~CSNPHashList(void);

	void SetSortFunc(SortComparisonFunc pSortComparisonFunc);
	void SetRemoveAllPoint();
	int GetCount();
	BOOL AddData(CString Key,B_NODE Data);
	B_NODE GetData(CString Key);
	BOOL DeleteData(CString Key);

	POSITION GetHeadPosition();
	B_NODE GetNext(POSITION& Pos);
	void Sort();

	BOOL FindKey(B_NODE Data,CString& strKey);

private:
	SortComparisonFunc					m_SortComparisonFunc;
	CMapStringToPtr						m_HashTable;
	CRITICAL_SECTION					m_Lock;
	CList<B_NODE,B_NODE>				m_List;

};

template<class B_NODE>
CSNPHashList<B_NODE>::CSNPHashList(SortComparisonFunc pSortComparisonFunc, UINT HashSize)
{
	m_SortComparisonFunc = pSortComparisonFunc;
	m_HashTable.InitHashTable(HashSize);
	InitializeCriticalSectionAndSpinCount(&m_Lock,4000);

}

template<class B_NODE>
CSNPHashList<B_NODE>::~CSNPHashList()
{
	SetRemoveAllPoint();
	DeleteCriticalSection(&m_Lock);
}

template<class B_NODE>
void CSNPHashList<B_NODE>::SetSortFunc(SortComparisonFunc pSortComparisonFunc)
{
	m_SortComparisonFunc = pSortComparisonFunc;
}


template<class B_NODE>
void CSNPHashList<B_NODE>::SetRemoveAllPoint()
{
	EnterCriticalSection(&m_Lock);
	POSITION  Pos = m_List.GetHeadPosition();
	while(NULL != Pos){
		B_NODE DelData = m_List.GetNext(Pos); 
		if(NULL != DelData){
			delete DelData;
			DelData = NULL;
		}
	}
	m_List.RemoveAll();
	m_HashTable.RemoveAll();
	LeaveCriticalSection(&m_Lock);

}

template<class B_NODE>
int CSNPHashList<B_NODE>::GetCount()
{
	return int(m_List.GetCount());
}

template<class B_NODE>
BOOL CSNPHashList<B_NODE>::AddData(CString Key,B_NODE Data)
{
	BOOL Re = FALSE;
	EnterCriticalSection(&m_Lock);
	if(NULL != m_List.AddTail(Data)){
		m_HashTable.SetAt(Key,Data);
		Re = TRUE;
	}
	LeaveCriticalSection(&m_Lock);
	return Re;
}

template<class B_NODE>
B_NODE CSNPHashList<B_NODE>::GetData(CString Key)
{
	B_NODE Data = NULL;
	EnterCriticalSection(&m_Lock);
	if(TRUE == m_HashTable.Lookup(Key,(LPVOID&)Data)){
		LeaveCriticalSection(&m_Lock);
		return Data;
	}
	LeaveCriticalSection(&m_Lock);
	return NULL;
}

template<class B_NODE>
BOOL CSNPHashList<B_NODE>::DeleteData(CString Key)
{
	B_NODE Data = NULL;
	EnterCriticalSection(&m_Lock);
	if(TRUE == m_HashTable.Lookup(Key,(LPVOID&)Data)){
		if(NULL != Data){
			POSITION Pos = m_List.Find(Data);
			if(NULL != Pos) m_List.RemoveAt(Pos);
			delete Data;
			Data = NULL;		
		}
		m_HashTable.RemoveKey(Key);
		LeaveCriticalSection(&m_Lock);
		return TRUE;
	}

	LeaveCriticalSection(&m_Lock);
	return FALSE;

}

template<class B_NODE>
POSITION CSNPHashList<B_NODE>::GetHeadPosition()
{
	return m_List.GetHeadPosition();

}

template<class B_NODE>
B_NODE CSNPHashList<B_NODE>::GetNext(POSITION& Pos)
{
	return m_List.GetNext(Pos);
}

template<class B_NODE>
BOOL CSNPHashList<B_NODE>::FindKey(B_NODE Data,CString& strKey)
{
	BOOL Re = FALSE;
	CString strFindKey;
	POSITION Pos = m_HashTable.GetStartPosition();
	while(NULL != Pos){
		void* tempEv = NULL;
		m_HashTable.GetNextAssoc(Pos,strFindKey,tempEv);
		if(tempEv == Data){
			strKey = strFindKey;
			Re = TRUE;
			break;
		}
	}
	return Re;
}

template<class B_NODE>
void CSNPHashList<B_NODE>::Sort()
{
	if(m_List.IsEmpty()) return;
	if(NULL == m_SortComparisonFunc) return;

	EnterCriticalSection(&m_Lock);

	CArray<B_NODE, B_NODE> vector;
	vector.SetSize(m_List.GetCount());
	int i = 0;
	POSITION pos = m_List.GetHeadPosition();
	while(pos != NULL){
		vector[i] = m_List.GetNext(pos);
		i++;
	}

	qsort(vector.GetData(),vector.GetSize(),sizeof(B_NODE),m_SortComparisonFunc);

	m_List.RemoveAll();

	INT_PTR n = vector.GetCount();
	for(i=0;i<n;i++){
		m_List.AddTail(vector[i]);
	}

	LeaveCriticalSection(&m_Lock);
}


