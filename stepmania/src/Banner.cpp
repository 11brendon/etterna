#include "global.h"
/*
-----------------------------------------------------------------------------
 File: Banner.h

 Desc: The song's banner displayed in SelectSong.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
-----------------------------------------------------------------------------
*/

#include "RageUtil.h"

#include "Banner.h"
#include "PrefsManager.h"
#include "SongManager.h"
#include "RageBitmapTexture.h"
#include "ThemeManager.h"

Banner::Banner()
{
	m_bScrolling = false;
	m_fPercentScrolling = 0;

	LoadFallback();
}

bool Banner::Load( RageTextureID ID )
{
	/* Song banners often have HOT PINK color keys. */
	ID.bHotPinkColorKey = true;

	return CroppedSprite::Load( ID );
};

void Banner::Update( float fDeltaTime )
{
	CroppedSprite::Update( fDeltaTime );

	if( m_bScrolling )
	{
        m_fPercentScrolling += fDeltaTime/2;
		m_fPercentScrolling -= (int)m_fPercentScrolling;

		const RectF *pTextureRect = m_pTexture->GetTextureCoordRect(0);

		float fTexCoords[8] = 
		{
			0+m_fPercentScrolling, pTextureRect->bottom,	// bottom left
			0+m_fPercentScrolling, pTextureRect->top,		// top left
			1+m_fPercentScrolling, pTextureRect->bottom,	// bottom right
			1+m_fPercentScrolling, pTextureRect->top,		// top right
		};
		Sprite::SetCustomTextureCoords( fTexCoords );
	}
}

void Banner::SetScrolling( bool bScroll, float Percent)
{
	m_bScrolling = bScroll;
	m_fPercentScrolling = Percent;

	/* Set up the texture coord rects for the current state. */
	Update(0);
}

void Banner::LoadFromSong( Song* pSong )		// NULL means no song
{
	Sprite::TurnShadowOff();

	if( pSong == NULL )					LoadFallback();
	else if( pSong->HasBanner() )		Load( pSong->GetBannerPath() );
	else if( PREFSMAN->m_bUseBGIfNoBanner  &&  pSong->HasBackground() )	Load( pSong->GetBackgroundPath() );
	else								LoadFallback();

	m_bScrolling = false;
}

void Banner::LoadFromGroup( CString sGroupName )
{
	CString sGroupBannerPath = SONGMAN->GetGroupBannerPath( sGroupName );

	if( sGroupName == GROUP_ALL_MUSIC )
		Load( THEME->GetPathTo("Graphics","all music banner") );
	else if( sGroupBannerPath != "" )
		Load( sGroupBannerPath );
	else
		LoadFallback();

	m_bScrolling = false;
}

void Banner::LoadFromCourse( Course* pCourse )		// NULL means no course
{
	Sprite::TurnShadowOff();

	if( pCourse == NULL )						LoadFallback();
	else if( pCourse->m_sBannerPath != "" )		Load( pCourse->m_sBannerPath );
	else										LoadFallback();

	m_bScrolling = false;
}

void Banner::LoadFallback()
{
	Load( THEME->GetPathTo("Graphics","fallback banner") );
}

void Banner::LoadRoulette()
{
	RageTextureID ID(THEME->GetPathTo("Graphics","select music roulette banner"));
	ID.iMipMaps = 0;
	Load( ID );
	m_bScrolling = true;
}
