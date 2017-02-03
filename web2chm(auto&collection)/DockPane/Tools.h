#pragma once
/*
	          ����ģ��
	��¼���к���:
	ExtractFileName - ���ļ�·���н������ļ���
	DrawButton - ����Ŧ���漰�߿�
	Div - ����������ԭ�����������
	Lighten - ����ɫ������߰�
*/
BOOL FileExist(const CString &FileName);
CString ExtractFileName(const CString &strPath);
void DrawButton(CDC *pdc, const CRect &rect, COLORREF cr, BOOL Pushed);
COLORREF Lighten(COLORREF cr, double scale = 0.8);
int Div(int x, int y);


#define TOP_LEFT		 1
#define TOP_UP			 2
#define TOP_RIGHT		 3
#define TOP_BOTTOM	 4
void DrawTrigon( CDC *pDC,int width,int height,COLORREF color,POINT point,DWORD top);  //zxy  2007.11
