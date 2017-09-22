#pragma once

#include "Poco/Task.h"

using Poco::Task;


class CToolTask : public Task
{
public:
	CToolTask(const std::string& name);
	virtual ~CToolTask(void);
};

/*	��������	*/
class CParseTask : public CToolTask
{
public:
	CParseTask();
	~CParseTask();

	virtual void runTask();
	
protected:
	static int AVInterruptCallBackFun(void* ctx);

private:
	bool	m_bD3dInited;
	static int64_t	m_dwLastFrameRealTime; // ����Ϊ��̬�������ӳ��������ڣ������ڳ����˳���AVInterruptCallBackFun����������δ���������µ��ñ��ĺ���ʧ�ܶ�����
};