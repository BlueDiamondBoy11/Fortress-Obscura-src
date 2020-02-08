//====== Copyright � 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include "tf_weapon_sword.h"
#include "decals.h"

// Client specific.
#ifdef CLIENT_DLL
#include "c_tf_player.h"
// Server specific.
#else
#include "tf_player.h"
#endif

//=============================================================================
//
// Weapon Sword tables.
//
IMPLEMENT_NETWORKCLASS_ALIASED( TFSword, DT_TFWeaponSword )

BEGIN_NETWORK_TABLE( CTFSword, DT_TFWeaponSword )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFSword )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_sword, CTFSword );
PRECACHE_WEAPON_REGISTER( tf_weapon_sword );

//=============================================================================
//
// Weapon Sword functions.
//

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CTFSword::CTFSword()
{
}
