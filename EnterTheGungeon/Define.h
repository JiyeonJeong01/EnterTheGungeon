#pragma once

#define WINCX 1280
#define WINCY 720

#define PI 3.141592

extern HWND g_hWnd;

#define MANAGER(objType, eType) static_cast<objType>(CManager::Get_Manager(eType))

enum ManagerType { M_INPUT, M_OBJECT, M_SCENE, M_UI, M_BMP, M_CAMERA, M_MAP, M_INVEN, M_SOUND, MGREND };
enum ObjectType { O_PLAYER, O_ENEMY, O_ITEM, O_PLBULLET, O_ENBULLET, O_INTERACTABLE, O_MAP, O_UI, OBJECTEND };
enum RenderType { RND_BACKGROUND, RND__GAMEBOJECT, RND_EFFECT, RND__UI, RND_END };
enum ObjectState { S_NOEVENT, S_DEAD };
enum SceneType { SC_INTRO, SC_MAIN, SC_STAGE01, SC_STAGE02, SC_TEST, SC_END};
enum Direction { D_UP, D_DOWN, D_LEFT, D_RIGHT, D_UL, D_UR, D_DL, D_DR, D_END };
enum CHANNELID { SOUND_BGM, SOUND_EFFECT, MAXCHANNEL };

