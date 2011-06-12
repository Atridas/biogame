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
    REBRE_IMPACTE,
    MORIR
  } Msg;

  SEvent():Sender(-1),Receiver(-1),DispatchTime(0) {};
};

enum ECollisionGroup {
  ECG_ESCENARI = 1,
  ECG_PERSONATGE = 2,
  ECG_ENEMICS = 4,
  ECG_TRIGGERS = 8,
  ECG_COBERTURES = 16,
  ECG_OBJECTES_DINAMICS = 32,
  ECG_EXPLOSIONS = 64
};

#include "BaseComponent.h"
#include "GameEntity.h"
#include "EntityManager.h"

#endif