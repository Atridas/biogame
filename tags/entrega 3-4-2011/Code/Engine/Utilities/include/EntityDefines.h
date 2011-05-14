#pragma once

#ifndef __ENTITY_DEFINES_H__
#define __ENTITY_DEFINES_H__

#include "base.h"


struct SEventInfo {
  enum { STI_INT, STI_FLOAT, STI_VECTOR, STI_STRING } Type;
  union {
    int i;
    float f;
    struct {float x,y,z;} v;
    const char* str;
  };
};

#define EVENT_INFO_SIZE 5

struct SEvent {
  int Sender;
  int Receiver;
  
  double DispatchTime;

  SEventInfo Info[EVENT_INFO_SIZE];

  enum EEventType {

  } Msg;
};

#include "BaseComponent.h"
#include "GameEntity.h"



#endif