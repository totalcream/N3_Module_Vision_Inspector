
#pragma once


// CPYH_Comm 명령 대상입니다.
#include<Windows.h>
#include<atlstr.h>

#define BUFFER_SIZE 128

class CPYH_Comm : public CCmdTarget
{
	DECLARE_DYNAMIC(CPYH_Comm)

private:
	HANDLE  m_hComm;
	DCB     m_dcb;
	COMMTIMEOUTS m_CommTimeouts;
	bool    m_bPortReady;
	bool    m_bWriteRC;
	bool    m_bReadRC;
	DWORD   m_iBytesWritten;
	DWORD   m_iBytesRead;
	DWORD   m_dwBytesRead;

public:
	CPYH_Comm();
	virtual ~CPYH_Comm();
	void ClosePort();
	bool ReadByte(BYTE &resp);
	bool ReadByte(BYTE* &resp, UINT size);
	bool WriteByte(char bybyte);
	bool OpenPort(CString portname);
	bool SetCommunicationTimeouts(DWORD ReadIntervalTimeout,
		DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant,
		DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant);
	bool ConfigurePort(DWORD BaudRate, BYTE ByteSize, DWORD fParity,
		BYTE  Parity, BYTE StopBits);

protected:
	DECLARE_MESSAGE_MAP()
};


