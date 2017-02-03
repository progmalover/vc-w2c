#pragma once

class CIEComCtrlSink;
class TIESinkManager
{
public:
	static  CIEComCtrlSink * CreateSinkInstance();
	static  BOOL ReleaseSinkInstance();
	static	CIEComCtrlSink * SinkInstance();
protected:
	static CIEComCtrlSink *m_pCtrlSink;
 
};
