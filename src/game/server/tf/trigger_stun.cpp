//======= Copyright � 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: CTF Flag Capture Zone.
//
//=============================================================================//
#include "cbase.h"
#include "trigger_stun.h"
#include "tf_player.h"
#include "tf_item.h"
#include "tf_team.h"
#include "tf_gamerules.h"

//=============================================================================
//
// Trigger stun tables.
//

BEGIN_DATADESC( CTriggerStun )

// Keyfields.
DEFINE_KEYFIELD( m_flTriggerDelay, FIELD_FLOAT, "trigger_delay" ),
DEFINE_KEYFIELD( m_flStunDuration, FIELD_TIME, "stun_duration" ),
DEFINE_KEYFIELD( m_flMoveSpeedReduction, FIELD_FLOAT, "move_speed_reduction"),
DEFINE_KEYFIELD( m_iStunType, FIELD_INTEGER, "stun_type"),
DEFINE_KEYFIELD( m_bStunEffects, FIELD_BOOLEAN, "stun_effects"),

// Functions.
DEFINE_FUNCTION( Touch ),

// Inputs.

// Outputs.
DEFINE_OUTPUT( m_outputOnStun, "OnStunPlayer" ),

END_DATADESC()

LINK_ENTITY_TO_CLASS( trigger_stun, CTriggerStun );

//=============================================================================
//
// Trigger stun functions.
//

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTriggerStun::Spawn()
{
	BaseClass::Spawn();
	InitTrigger();
	SetNextThink( -1.0f );
	ThinkSet( NULL );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CTriggerStun::StunEntity( CBaseEntity *pOther )
{
	CTFPlayer *pPlayer = ToTFPlayer( pOther );

	// Don't stun the player again if they're already stunned
	if ( IsTouching( pOther ) && pPlayer && !pPlayer->m_Shared.InCond( TF_COND_HALF_STUN ) && !pPlayer->m_Shared.InCond( TF_COND_PHASE ) )
	{
		int iStunType = m_iStunType;

		if ( !m_bStunEffects && m_iStunType > STUN_BIG )
			iStunType = STUN_NO_EFFECT;

		pPlayer->m_Shared.StunPlayer( m_flStunDuration, 1.0 - m_flMoveSpeedReduction, iStunType, NULL );
		m_outputOnStun.FireOutput( pOther, this );
		m_stunEntities.AddToTail( pOther );

		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTriggerStun::StunThink()
{
	int iStunCount = 0;

	m_stunEntities.RemoveAll();

	touchlink_t *root = ( touchlink_t * )GetDataObject( TOUCHLINK );
	if ( root )
	{
		for ( touchlink_t *link = root->nextLink; link != root; link = link->nextLink )
		{
			CBaseEntity *pTouch = link->entityTouched;
			if ( pTouch )
			{
				if ( StunEntity( pTouch ) )
				{
					iStunCount++;
				}
			}
		}
	}

	if ( iStunCount > 0 )
	{	
		SetNextThink( gpGlobals->curtime + m_flStunDuration );
	}
	else
	{
		ThinkSet( NULL );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTriggerStun::Touch( CBaseEntity *pOther )
{
	if ( m_pfnThink == NULL )
	{
		SetThink( &CTriggerStun::StunThink );
		SetNextThink( gpGlobals->curtime + m_flTriggerDelay );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CTriggerStun::EndTouch( CBaseEntity *pOther )
{
	if ( pOther && IsTouching( pOther ) )
	{
		if ( !m_stunEntities.HasElement( pOther ) )
		{
			StunEntity( pOther );
		}
	}
	BaseClass::EndTouch( pOther );
}
