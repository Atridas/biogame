#include "ComponentBGMController.h"
#include "Core.h"
#include "SoundManager.h"

#define BGM_CONTROLLER_FADE_OUT 50

CComponentBGMController* CComponentBGMController::AddToEntity(CGameEntity* _pEntity, bool _bMain)
{
  CComponentBGMController *l_pComp = new CComponentBGMController();
  assert(_pEntity && _pEntity->IsOk());
  if(l_pComp->Init(_pEntity, _bMain))
  {
    l_pComp->SetEntity(_pEntity);
    return l_pComp;
  }
  else
  {
    delete l_pComp;
    return 0;
  }
}

bool CComponentBGMController::Init(CGameEntity* _pEntity, bool _bMain)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CComponentBGMController::Init inicialitzant el component de background music.");

  m_iAliveEnemy = 0;

  if(_bMain)
    m_eCurrentMusic = CComponentBGMController::EMS_INIT_MENU;
  else
    m_eCurrentMusic = CComponentBGMController::EMS_INIT_LEVEL;

  m_szCurrentMusic = GetSongInfo(m_eCurrentMusic);

  SetCurrentSong(m_eCurrentMusic);

  SetOk(true);
  return IsOk();
}

//traducció dels noms de les cançons
string CComponentBGMController::GetSongInfo(const EMusicState _eState) const
{
  string l_szSongName = "bgm";

  switch(_eState)
  {
    case (CComponentBGMController::EMS_INIT_MENU):
      l_szSongName = "bgm_main_menu_init";
      break;
    case (CComponentBGMController::EMS_MAIN_MENU):
      l_szSongName = "bgm_main_menu";
      break;
    case (CComponentBGMController::EMS_INIT_LEVEL):
      l_szSongName = "bgm_init_level";
      break;
    case (CComponentBGMController::EMS_EXPLORE):
      l_szSongName = "bgm_explore";
      break;
    case (CComponentBGMController::EMS_BATTLE):
      l_szSongName = "bgm_battle";
      break;
    case (CComponentBGMController::EMS_BATTLE_END):
      l_szSongName = "bgm_battle_end";
      break;
    case (CComponentBGMController::EMS_BATTLE_TO_EXPLORE):
      l_szSongName = "bgm_battle_to_explore";
      break;
    default:
      l_szSongName = "bgm";
      break;
  }

  return l_szSongName;
}

//ordre de les cançons
CComponentBGMController::EMusicState CComponentBGMController::GetNextSong() const
{
  EMusicState l_eNextSong = CComponentBGMController::EMS_MAIN_MENU;

  switch(m_eCurrentMusic)
  {
  case (CComponentBGMController::EMS_INIT_MENU):
    l_eNextSong = CComponentBGMController::EMS_MAIN_MENU;
    break;
  case (CComponentBGMController::EMS_MAIN_MENU):
    l_eNextSong = CComponentBGMController::EMS_MAIN_MENU;
    break;
  case (CComponentBGMController::EMS_INIT_LEVEL):
    l_eNextSong = CComponentBGMController::EMS_EXPLORE;
    break;
  case (CComponentBGMController::EMS_EXPLORE):
    l_eNextSong = CComponentBGMController::EMS_EXPLORE;
    break;
  case (CComponentBGMController::EMS_BATTLE):
    l_eNextSong = CComponentBGMController::EMS_BATTLE;
    break;
  case (CComponentBGMController::EMS_BATTLE_END):
    l_eNextSong = CComponentBGMController::EMS_BATTLE_TO_EXPLORE;
    break;
  case (CComponentBGMController::EMS_BATTLE_TO_EXPLORE):
    l_eNextSong = CComponentBGMController::EMS_EXPLORE;
    break;
  case (CComponentBGMController::EMS_RR):
    l_eNextSong = CComponentBGMController::EMS_RR;
    break;
  default:
    l_eNextSong = CComponentBGMController::EMS_MAIN_MENU;
    break;
  }

  return l_eNextSong;
}

void CComponentBGMController::SetCurrentSong(const EMusicState _eState, bool _bReset)
{
  CSoundManager* l_pSM = CORE->GetSoundManager();

  m_eCurrentMusic = _eState;
  m_szCurrentMusic = GetSongInfo(_eState);

  //es comença des del principi
  //l_pSM->PlayMusic(m_szCurrentMusic, _bReset);
  l_pSM->ChangeMusic(m_szCurrentMusic, 0, _bReset);
}

void CComponentBGMController::PlayNextSong(const EMusicState _eState)
{
  CSoundManager* l_pSM = CORE->GetSoundManager();

  m_eCurrentMusic = _eState;
  m_szCurrentMusic = GetSongInfo(_eState);

  //es comença des del principi
  l_pSM->ChangeMusic(m_szCurrentMusic, BGM_CONTROLLER_FADE_OUT, true);
}

void CComponentBGMController::Update(float _fDeltaTime)
{
  CSoundManager* l_pSM = CORE->GetSoundManager();

  float l_fRemaining = l_pSM->GetMusicRemainingTime(m_szCurrentMusic);
  if(l_fRemaining <= BGM_CONTROLLER_FADE_OUT)
    PlayNextSong(GetNextSong());
}

void CComponentBGMController::Release()
{
  CSoundManager* l_pSM = CORE->GetSoundManager();
  
  l_pSM->StopMusics();
}

void CComponentBGMController::ReceiveEvent(const SEvent& _Event)
{
  if(_Event.Msg == SEvent::ENEMY_ALIVE)
  {
    //entrem en batalla
    if(m_iAliveEnemy == 0)
      PlayNextSong(CComponentBGMController::EMS_BATTLE);

    m_iAliveEnemy++;
  }
  else if(_Event.Msg == SEvent::ENEMY_DEAD)
  {
    if(m_iAliveEnemy > 0)
      m_iAliveEnemy--;

    //sortim de batalla
    if(m_iAliveEnemy == 0)
      PlayNextSong(CComponentBGMController::EMS_BATTLE_END);
  }
}

void CComponentBGMController::Enable()
{
  SetCurrentSong(m_eCurrentMusic, false);
}

void CComponentBGMController::Disable()
{
  CSoundManager* l_pSM = CORE->GetSoundManager();
  
  l_pSM->Pause(m_szCurrentMusic);
}