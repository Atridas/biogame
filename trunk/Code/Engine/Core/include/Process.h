#pragma once
class CProcess
{
public:
	CProcess(void);
	virtual ~CProcess(void);

	virtual void Update() = 0;
	virtual void Render() = 0;

  virtual bool          Init() = 0;
  void                  Done() {if(IsOk()) Relase(); m_bIsOk=false;};
  bool                  IsOk() const {return m_bIsOk;};

protected:
	virtual void          Relase() = 0;
	bool                  m_bIsOk;
};

