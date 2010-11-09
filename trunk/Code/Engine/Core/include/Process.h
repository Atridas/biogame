#pragma once
class CProcess
{
public:
	CProcess(void);
	virtual ~CProcess(void);

	virtual void Update() = 0;
	virtual void Render() = 0;
};

