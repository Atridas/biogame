#pragma once

#ifndef __ENTITY_DEFINES_H__
#define __ENTITY_DEFINES_H__

#include "base.h"

struct SEventInfo {
  enum { INT, FLOAT, VECTOR, STRING, PTR } Type;
  union {
    int i;
    float f;
    struct {float x,y,z;} v;
    const char* str;
    void* ptr;
  };
  void SetStr(const char* _cInput) { str = _cInput; };
};

#define EVENT_INFO_SIZE 5

struct SEvent {
  int Sender;
  int Receiver;
  
  double DispatchTime;

  SEventInfo Info[EVENT_INFO_SIZE];

  enum EEventType {
    REBRE_IMPACTE,
    REBRE_FORCE,
    MORIR,
    OBRIR,
    TANCAR,
    PICKUP,
    INTERACCIO
  } Msg;

  //agafar un element info
  SEventInfo GetInfo(int _iIndex) const
  {
    if(_iIndex >= 0 && _iIndex < EVENT_INFO_SIZE)
      return Info[_iIndex];
    else
      return Info[0];
  };

  //donar valor a un element info
  void SetInfo(int _iIndex, SEventInfo _Info)
  {
    if(_iIndex < 0 || _iIndex >= EVENT_INFO_SIZE)
      _iIndex = 0;

    Info[_iIndex].Type = _Info.Type;

    switch(_Info.Type)
    {
    case (SEventInfo::FLOAT):
      Info[_iIndex].f = _Info.f;
      break;
    case (SEventInfo::INT):
      Info[_iIndex].i = _Info.i;
      break;
    case (SEventInfo::STRING):
      Info[_iIndex].SetStr(_Info.str);
      break;
    case (SEventInfo::VECTOR):
      Info[_iIndex].v.x = _Info.v.x;
      Info[_iIndex].v.y = _Info.v.y;
      Info[_iIndex].v.z = _Info.v.z;
      break;
    case (SEventInfo::PTR):
      Info[_iIndex].ptr = _Info.ptr;
      break;
    default:
      break;
    }
  };

  SEvent():Sender(-1),Receiver(-1),DispatchTime(0) {};
};

#include "BaseComponent.h"
#include "GameEntity.h"
#include "EntityManager.h"

#endif