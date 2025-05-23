
#include <swc.h>

#ifndef __ARIAENCRYPTION_H__
#define __ARIAENCRYPTION_H__

enum ARIA_ENC_KEY_TYPE {
	ARIA_ENC_KEY_128 = 128,	
	ARIA_ENC_KEY_192 = 192,
	ARIA_ENC_KEY_256 = 256
};

class CARIAEncryption
{
public:
	CARIAEncryption(ARIA_ENC_KEY_TYPE enKeyType = ARIA_ENC_KEY_256);
	~CARIAEncryption();
		
	int EncryptBlock(BYTE*& pOutputData, BYTE* pInputData, unsigned int nInputDataLen);
	int DecryptBlock(BYTE*& pOutputData, BYTE* pInputData, unsigned int nInputDataLen);

	WCString GetSaltSHA512(WCString strValue);

	WCString EncryptString(WCString strValue);
	WCString DecryptString(WCString strValue);

protected:
	ARIA_ENC_KEY_TYPE	m_enKeyType;
	BYTE*				m_pMasterKey;
	BYTE*				m_pEncRoundKey;
	BYTE*				m_pDecRoundKey;
	int					m_nEncRounds;
	int					m_nDecRounds;

	int GetMasterKeyByte(ARIA_ENC_KEY_TYPE enKeyType);
	int GetRoundKeyByte(ARIA_ENC_KEY_TYPE enKeyType);

	// Encryption and decryption rountine
	// p: plain text, e: round keys, c: ciphertext
	void Crypt(const BYTE *p, int R, const BYTE *e, BYTE *c);

	// Encryption round key generation rountine
	// w0 : master key, e : encryption round keys
	int EncKeySetup(const BYTE *w0, BYTE *e, int keyBits);

	// Decryption round key generation rountine
	// w0 : maskter key, d : decryption round keys
	int DecKeySetup(const BYTE *w0, BYTE *d, int keyBits);

	void DL(const BYTE *i, BYTE *o);
	void RotXOR(const BYTE *s, int n, BYTE *t);
};

#endif // __ARIAENCRYPTION_H__
