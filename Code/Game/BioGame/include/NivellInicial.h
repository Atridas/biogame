#pragma once
#ifndef __NIVELL_INICIAL__
#define __NIVELL_INICIAL__

#define BIOGAME_VERSIO_FINAL
//#define BIOGAME_NIVELLS_NIVELL_MENYS_2
//#define BIOGAME_NIVELLS_NIVELL_MENYS_1
//#define BIOGAME_NIVELLS_HANGAR



#ifdef BIOGAME_VERSIO_FINAL
#ifdef BIOGAME_NIVELLS_NIVELL_MENYS_2
#error 2 nivell definits a la vegada!
#endif
#ifdef BIOGAME_NIVELLS_NIVELL_MENYS_1
#error 2 nivell definits a la vegada!
#endif
#ifdef BIOGAME_NIVELLS_HANGAR
#error 2 nivell definits a la vegada!
#endif
#endif

#ifdef BIOGAME_NIVELLS_NIVELL_MENYS_2
#ifdef BIOGAME_NIVELLS_NIVELL_MENYS_1
#error 2 nivell definits a la vegada!
#endif
#ifdef BIOGAME_NIVELLS_HANGAR
#error 2 nivell definits a la vegada!
#endif
#endif

#ifdef BIOGAME_NIVELLS_NIVELL_MENYS_1
#ifdef BIOGAME_NIVELLS_HANGAR
#error 2 nivell definits a la vegada!
#endif
#endif

#ifndef BIOGAME_VERSIO_FINAL
#ifndef BIOGAME_NIVELLS_NIVELL_MENYS_2
#ifndef BIOGAME_NIVELLS_NIVELL_MENYS_1
#ifndef BIOGAME_NIVELLS_HANGAR
#error No hi ha cap nivell definit
#endif
#endif
#endif
#endif
#endif