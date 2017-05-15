#ifndef D3DU_TEXT_H
#define D3DU_TEXT_H

#include <vector>
#include <time.h>

namespace d3du
{

class CText
{
public:
	CText();
	~CText();
	void Init(IDirect3DDevice9 *pDevice,int left,int top,int width,int height,LPCTSTR lpszText,LOGFONT *pLogfont,D3DCOLOR clr);
	void SetText(LPCTSTR lpszText);
	void SetTextColor(D3DCOLOR clr);
	void SetRect(int left,int right,int width,int height);
	void Display();
protected:
	IDirect3DDevice9 *m_pDevice;
	ID3DXFont *m_pFont;
	RECT m_rect;
	D3DCOLOR m_clr;
	char* m_lpszText;
};

class CFPS
{
public:
	CFPS(){m_clockBeg=clock();m_clockEnd=0;m_nFPS=0;m_nFPSTemp=0;}
	clock_t m_clockBeg;
	clock_t m_clockEnd;
	UINT m_nFPS;
	UINT m_nFPSTemp;
	void Calculate()
	{
		m_clockEnd=clock();
		if(m_clockEnd-m_clockBeg > CLOCKS_PER_SEC)
		{
			m_clockBeg=m_clockEnd;
			m_nFPS=m_nFPSTemp;
			m_nFPSTemp=0;
		}
		m_nFPSTemp++;
	}
};

}


#endif