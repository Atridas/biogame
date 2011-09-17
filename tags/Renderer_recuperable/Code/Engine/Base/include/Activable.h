#pragma once
#ifndef __ACTIVABLE_H__
#define __ACTIVABLE_H__

class CActivable
{
public:
  CActivable(): m_bActive(false) {};

  bool IsActive() const {return m_bActive;};
  void SetActive(bool _b) {m_bActive = _b;};
private:
  bool m_bActive;
};


#endif