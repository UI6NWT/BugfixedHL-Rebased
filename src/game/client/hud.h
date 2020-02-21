/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
//  hud.h
//
// class CHud declaration
//
// CHud handles the message, calculation, and drawing the HUD
//

#include <vector>
#include "global_consts.h"
#include "hud/base.h"

#define RGB_YELLOWISH 0x00FFA000 //255,160,0
#define RGB_REDISH    0x00FF1010 //255,160,0
#define RGB_GREENISH  0x0000A000 //0,160,0

#define DHN_DRAWZERO 1
#define DHN_2DIGITS  2
#define DHN_3DIGITS  4
#define MIN_ALPHA    100

#define HUDELEM_ACTIVE 1

#define HUD_ACTIVE       1
#define HUD_INTERMISSION 2

struct extra_player_info_t
{
	short frags;
	short deaths;
	short playerclass;
	short health; // UNUSED currently, spectator UI would like this
	bool dead; // UNUSED currently, spectator UI would like this
	short teamnumber;
	char teamname[MAX_TEAM_NAME];
};

struct team_info_t
{
	char name[MAX_TEAM_NAME];
	short frags;
	short deaths;
	short ping;
	short packetloss;
	short ownteam;
	short players;
	int already_drawn;
	int scores_overriden;
	int teamnumber;
};

#include "player_info.h"

//
//-----------------------------------------------------
//

class CHud
{
public:
	HSPRITE m_hsprCursor;
	float m_flTime; // the current client time
	float m_fOldTime; // the time at which the HUD was last redrawn
	double m_flTimeDelta; // the difference between flTime and fOldTime
	Vector m_vecOrigin;
	Vector m_vecAngles;
	int m_iKeyBits;
	int m_iHideHUDDisplay;
	int m_iFOV;
	int m_Teamplay;
	int m_iRes;
	int m_iFontHeight;
	int m_iWeaponBits;
	int m_fPlayerDead;
	int m_iIntermission;

	// Cvars
	cvar_t *m_pCvarStealMouse;
	cvar_t *m_pCvarDraw;
	cvar_t *default_fov;

	// sprite indexes
	int m_HUD_number_0;

	// Screen information
	SCREENINFO m_scrinfo;

	// HUD exports
	CHud();
	~CHud();
	void Init(void);
	void VidInit(void);
	void Think(void);
	int Redraw(float flTime, int intermission);
	int UpdateClientData(client_data_t *cdata, float time);

	// Draw functions
	int DrawHudNumber(int x, int y, int iFlags, int iNumber, int r, int g, int b);
	int DrawHudString(int x, int y, int iMaxX, char *szString, int r, int g, int b);
	int DrawHudStringReverse(int xpos, int ypos, int iMinX, char *szString, int r, int g, int b);
	int DrawHudNumberString(int xpos, int ypos, int iMinX, int iNumber, int r, int g, int b);
	int GetNumWidth(int iNumber, int iFlags);

	// Sprite functions
	HSPRITE GetSprite(int index);
	wrect_t &GetSpriteRect(int index);
	int GetSpriteIndex(const char *SpriteName); // gets a sprite index, for use in the m_rghSprites[] array

	// User messages
	int MsgFunc_Damage(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_Logo(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_ResetHUD(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_InitHUD(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_ViewMode(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_SetFOV(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_Concuss(const char *pszName, int iSize, void *pbuf);

	float GetSensitivity();

private:
	HSPRITE m_hsprLogo;
	int m_iLogo;
	client_sprite_t *m_pSpriteList;
	int m_iSpriteCount = 0;
	int m_iSpriteCountAllRes;
	float m_flMouseSensitivity;
	int m_iConcussionEffect;
	std::vector<CHudElem *> m_HudList;

	// the memory for these arrays are allocated in the first call
	// to CHud::VidInit(), when the hud.txt and associated sprites are loaded.
	std::vector<HSPRITE> m_rghSprites; /*[HUD_SPRITE_COUNT]*/ // the sprites loaded from hud.txt
	std::vector<wrect_t> m_rgrcRects; /*[HUD_SPRITE_COUNT]*/
	std::vector<char> m_rgszSpriteNames; /*[HUD_SPRITE_COUNT][MAX_SPRITE_NAME_LENGTH]*/

	template <typename T>
	inline T *RegisterHudElem()
	{
		T *p = new T();
		m_HudList.push_back(p);
		return p;
	}
};

inline HSPRITE CHud::GetSprite(int index)
{
	return (index < 0) ? 0 : m_rghSprites[index];
}

inline wrect_t &CHud::GetSpriteRect(int index)
{
	return m_rgrcRects[index];
}

extern CHud gHUD;

extern int g_iPlayerClass;
extern int g_iTeamNumber;
extern int g_iUser1;
extern int g_iUser2;
extern int g_iUser3;
