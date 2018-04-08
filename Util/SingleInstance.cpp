#include "SingleInstance.h"

SECURITY_ATTRIBUTES *InitSecurityAttribute(SECURITY_ATTRIBUTES &SA, SECURITY_DESCRIPTOR &SD)
{
	if (!InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION))
		return NULL;
	if (!SetSecurityDescriptorDacl(&SD, true, NULL, false))
		return NULL;

	SA.nLength = sizeof(SA);
	SA.lpSecurityDescriptor = &SD;
	SA.bInheritHandle = true;
	return &SA;
};


CSingleInstance::CSingleInstance(const string& strMutexName)
{
	// be sure to use a name that is unique for this application otherwise
	// two apps may think they are the same if they are using same name for
	// 3rd parm to CreateMutex
	SECURITY_ATTRIBUTES SA; 
	SECURITY_DESCRIPTOR SD;
	SECURITY_ATTRIBUTES *pSA = InitSecurityAttribute(SA, SD);
	//   Create   a   Semaphore   with   a   name   of   application   exename   
	m_hMutex = CreateSemaphoreA(pSA, 1, 1, strMutexName.c_str());
	m_dwLastError = ::GetLastError(); //save for use later...
}
  
CSingleInstance::~CSingleInstance()
{
	if (m_hMutex)  //don't forget to close handles...
	{
		::CloseHandle(m_hMutex); //do as late as possible
		m_hMutex = NULL;       //good habit to be in
	}
}

BOOL CSingleInstance::IsAnotherInstanceRunning()
{
	return (ERROR_ALREADY_EXISTS == m_dwLastError);
}