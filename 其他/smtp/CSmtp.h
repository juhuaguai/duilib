#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <string>
#include <istream>
#include <fstream>
#include <vector>
using namespace std;
//openssl
#include "openssl/sha.h"  
#include "openssl/rsa.h"  
#include "openssl/rand.h"  
#include "openssl/objects.h"  
#include "openssl/pem.h"  
#include "openssl/ssl.h"
#include "openssl/err.h"
#pragma comment(lib,"../Openssl/out32dll/ssleay32.lib")
#pragma comment(lib,"../Openssl/out32dll/libeay32.lib")

#include <assert.h>

class MD5 
{

public:
	// methods for controlled operation:
	MD5              ();  // simple initializer
	void  update     (unsigned char *input, unsigned int input_length);
	void  update     (istream& stream);
	void  update     (FILE *file);
	void  update     (ifstream& stream);
	void  finalize   ();

	// constructors for special circumstances.  All these constructors finalize
	// the MD5 context.
	MD5              (unsigned char *string); // digest string, finalize
	MD5              (istream& stream);       // digest stream, finalize
	MD5              (FILE *file);            // digest file, close, finalize
	MD5              (ifstream& stream);      // digest stream, close, finalize

	// methods to acquire finalized result
	unsigned char    *raw_digest ();  // digest as a 16-byte binary array
	char *            hex_digest ();  // digest as a 33-byte ascii-hex string
	friend ostream&   operator<< (ostream&, MD5 context);

private:
	// first, some types:
	typedef unsigned       int uint4; // assumes integer is 4 words long
	typedef unsigned short int uint2; // assumes short integer is 2 words long
	typedef unsigned      char uint1; // assumes char is 1 word long

	// next, the private data:
	uint4 state[4];
	uint4 count[2];     // number of *bits*, mod 2^64
	uint1 buffer[64];   // input buffer
	uint1 digest[16];
	uint1 finalized;

	// last, the private methods, mostly static:
	void init             ();               // called by all constructors
	void transform        (uint1 *buffer);  // does the real update work.  Note 
	// that length is implied to be 64.

	static void encode    (uint1 *dest, uint4 *src, uint4 length);
	static void decode    (uint4 *dest, uint1 *src, uint4 length);
	static void memcpy    (uint1 *dest, uint1 *src, uint4 length);
	static void memset    (uint1 *start, uint1 val, uint4 length);

	static inline uint4  rotate_left(uint4 x, uint4 n);
	static inline uint4  Ffunc(uint4 x, uint4 y, uint4 z);
	static inline uint4  Gfunc(uint4 x, uint4 y, uint4 z);
	static inline uint4  Hfunc(uint4 x, uint4 y, uint4 z);
	static inline uint4  Ifunc(uint4 x, uint4 y, uint4 z);
	static inline void   FFfunc(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void   GGfunc(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void   HHfunc(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void   IIfunc(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum CSmptXPriority
{
	XPRIORITY_HIGH = 2,
	XPRIORITY_NORMAL = 3,
	XPRIORITY_LOW = 4
};

class ECSmtp
{
public:
	enum CSmtpError
	{
		CSMTP_NO_ERROR = 0,
		WSA_STARTUP = 100, // WSAGetLastError()
		WSA_VER,
		WSA_SEND,
		WSA_RECV,
		WSA_CONNECT,
		WSA_GETHOSTBY_NAME_ADDR,
		WSA_INVALID_SOCKET,
		WSA_HOSTNAME,
		WSA_IOCTLSOCKET,
		WSA_SELECT,
		BAD_IPV4_ADDR,
		UNDEF_MSG_HEADER = 200,
		UNDEF_MAIL_FROM,
		UNDEF_SUBJECT,
		UNDEF_RECIPIENTS,
		UNDEF_LOGIN,
		UNDEF_PASSWORD,
		BAD_LOGIN_PASSWORD,
		BAD_DIGEST_RESPONSE,
		BAD_SERVER_NAME,
		UNDEF_RECIPIENT_MAIL,
		COMMAND_MAIL_FROM = 300,
		COMMAND_EHLO,
		COMMAND_AUTH_PLAIN,
		COMMAND_AUTH_LOGIN,
		COMMAND_AUTH_CRAMMD5,
		COMMAND_AUTH_DIGESTMD5,
		COMMAND_DIGESTMD5,
		COMMAND_DATA,
		COMMAND_QUIT,
		COMMAND_RCPT_TO,
		MSG_BODY_ERROR,
		CONNECTION_CLOSED = 400, // by server
		SERVER_NOT_READY, // remote server
		SERVER_NOT_RESPONDING,
		SELECT_TIMEOUT,
		FILE_NOT_EXIST,
		MSG_TOO_BIG,
		BAD_LOGIN_PASS,
		UNDEF_XYZ_RESPONSE,
		LACK_OF_MEMORY,
		TIME_ERROR,
		RECVBUF_IS_EMPTY,
		SENDBUF_IS_EMPTY,
		OUT_OF_MSG_RANGE,
		COMMAND_EHLO_STARTTLS,
		SSL_PROBLEM,
		COMMAND_DATABLOCK,
		STARTTLS_NOT_SUPPORTED,
		LOGIN_NOT_SUPPORTED
	};
	ECSmtp(CSmtpError err_,int reply_code_) : ErrorCode(err_),ReplyCode(reply_code_) {}
	CSmtpError GetErrorNum(void) const {return ErrorCode;}
	std::string GetErrorText(void) const;
	int GetReplyCode() const {return ReplyCode;}

private:
	CSmtpError ErrorCode;
	int ReplyCode;
};

enum SMTP_COMMAND
{
	command_INIT,
	command_EHLO,
	command_AUTHPLAIN,
	command_AUTHLOGIN,
	command_AUTHCRAMMD5,
	command_AUTHDIGESTMD5,
	command_DIGESTMD5,
	command_USER,
	command_PASSWORD,
	command_MAILFROM,
	command_RCPTTO,
	command_DATA,
	command_DATABLOCK,
	command_DATAEND,
	command_QUIT,
	command_STARTTLS
};

// TLS/SSL extension
enum SMTP_SECURITY_TYPE
{
	NO_SECURITY,
	USE_TLS,
	USE_SSL,
	DO_NOT_SET
};

typedef struct tagCommand_Entry
{
	SMTP_COMMAND       command;
	int                send_timeout;	 // 0 means no send is required
	int                recv_timeout;	 // 0 means no recv is required
	int                valid_reply_code; // 0 means no recv is required, so no reply code
	ECSmtp::CSmtpError error;
}Command_Entry;

class CSmtp  
{
public:
	CSmtp();
	virtual ~CSmtp();

	void AddRecipient(const char *email, const char *name=NULL);
	void AddBCCRecipient(const char *email, const char *name=NULL);	//ÃÜËÍ
	void AddCCRecipient(const char *email, const char *name=NULL);  //³­ËÍ
	void AddAttachment(const wchar_t *path);   
	void AddMsgLine(const char* text);
	void ClearMessage();
	bool ConnectRemoteServer(const char* szServer, const unsigned short nPort_=0,
							 SMTP_SECURITY_TYPE securityType=DO_NOT_SET,
		                     bool authenticate=true, const char* login=NULL,
							 const char* password=NULL);
	void DisconnectRemoteServer();
	void DelRecipients(void);
	void DelBCCRecipients(void);
	void DelCCRecipients(void);
	void DelAttachments(void);
	void DelMsgLines(void);
	void DelMsgLine(unsigned int line);
	void ModMsgLine(unsigned int line,const char* text);
	unsigned int GetBCCRecipientCount() const;    
	unsigned int GetCCRecipientCount() const;
	unsigned int GetRecipientCount() const;    
	const char* GetLocalHostIP() const;
	const char* GetLocalHostName();
	const char* GetMsgLineText(unsigned int line) const;
	unsigned int GetMsgLines(void) const;
	const char* GetReplyTo() const;
	const char* GetMailFrom() const;
	const char* GetSenderName() const;
	const char* GetSubject() const;
	const char* GetXMailer() const;
	CSmptXPriority GetXPriority() const;
	void Send();
	void SetCharSet(const char *sCharSet);
	void SetLocalHostName(const char *sLocalHostName);
	void SetSubject(const char*);
	void SetSenderName(const char*);
	void SetSenderMail(const char*);
	void SetReplyTo(const char*);
	void SetReadReceipt(bool requestReceipt=true);
	void SetXMailer(const char*);
	void SetLogin(const char*);
	void SetPassword(const char*);
	void SetXPriority(CSmptXPriority);
	void SetSMTPServer(const char* server, const unsigned short port=0, bool authenticate=true);

private:	
	std::string m_sLocalHostName;
	std::string m_sMailFrom;
	std::string m_sNameFrom;
	std::string m_sSubject;
	std::string m_sCharSet;
	std::string m_sXMailer;
	std::string m_sReplyTo;
	bool m_bReadReceipt;
	std::string m_sIPAddr;
	std::string m_sLogin;
	std::string m_sPassword;
	std::string m_sSMTPSrvName;
	unsigned short m_iSMTPSrvPort;
	bool m_bAuthenticate;
	CSmptXPriority m_iXPriority;
	char *SendBuf;
	char *RecvBuf;
	
	SOCKET hSocket;
	bool m_bConnected;

	struct Recipient
	{
		std::string Name;
		std::string Mail;
	};

	std::vector<Recipient> Recipients;
	std::vector<Recipient> CCRecipients;
	std::vector<Recipient> BCCRecipients;
	std::vector<std::wstring> Attachments;
	std::vector<std::string> MsgBody;
 
	void ReceiveData(Command_Entry* pEntry);
	void SendData(Command_Entry* pEntry);
	void FormatHeader(char*);
	int SmtpXYZdigits();
	void SayHello();
	void SayQuit();

// TLS/SSL extension
public:
	SMTP_SECURITY_TYPE GetSecurityType() const
	{ return m_type; }
	void SetSecurityType(SMTP_SECURITY_TYPE type)
	{ m_type = type; }
	bool m_bHTML;

private:
	SMTP_SECURITY_TYPE m_type;
	SSL_CTX*      m_ctx;
	SSL*          m_ssl;

	void ReceiveResponse(Command_Entry* pEntry);
	void InitOpenSSL();
	void OpenSSLConnect();
	void CleanupOpenSSL();
	void ReceiveData_SSL(SSL* ssl, Command_Entry* pEntry);
	void SendData_SSL(SSL* ssl, Command_Entry* pEntry);
	void StartTls();
};