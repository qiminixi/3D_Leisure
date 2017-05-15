#include "PreHeader.h"
#include "text.h"

using d3du::CText;

CText::CText()
{
	m_pDevice=NULL;
	m_pFont=NULL;
	m_lpszText=NULL;
}

CText::~CText()
{
	if(m_lpszText) delete m_lpszText;
	if(m_pFont) m_pFont->Release();
}

void CText::Init(IDirect3DDevice9 *pDevice,int left,int top,int width,int height,LPCTSTR lpszText,LOGFONT *pLogfont,D3DCOLOR clr)
{
	m_pDevice=pDevice;
	//
	m_rect.left=left;
	m_rect.top=top;
	m_rect.right=left+width;
	m_rect.bottom=top+height;
	m_clr=clr;
	//
	if(lpszText)
	{
		//int length=(int)wcsnlen(lpszText,0xFFFF);
		int length=(int)strlen(lpszText);
		m_lpszText=new char[length+1];
		memcpy(m_lpszText,lpszText,length);
		m_lpszText[length]=0;
	}
	//*
	D3DXFONT_DESC fdesc={0};
	fdesc.CharSet=GB2312_CHARSET;
	char facename[]=TEXT("Arial");
	memcpy(fdesc.FaceName,facename,sizeof(facename));
	fdesc.Height=14;
	fdesc.Italic=FALSE;
	fdesc.MipLevels=0;
	fdesc.OutputPrecision=OUT_DEFAULT_PRECIS;
	fdesc.PitchAndFamily=DEFAULT_PITCH;
	fdesc.Quality=DEFAULT_QUALITY;
	fdesc.Weight=400;
	//*/
	
	//if(pLogfont) 
		D3DXCreateFontIndirect(m_pDevice,&fdesc,&m_pFont);
}

void CText::Display()
{
	m_pFont->DrawText(NULL,m_lpszText,-1,&m_rect,DT_LEFT|DT_TOP,m_clr);
}

void CText::SetText(LPCTSTR lpszText)
{
	if(m_lpszText) delete m_lpszText;
	//int length=(int)wcsnlen(lpszText,0xFFFF);
	int length=strlen(lpszText);
	m_lpszText=new char[length+1];
	memcpy(m_lpszText,lpszText,length);
	m_lpszText[length]=0;
}