//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======//
//
// Purpose: CTF AmmoPack.
//
//=============================================================================//

#include "cbase.h"
#include "tf_player.h"
#include "tf_weaponbase.h"
#include "tf_shareddefs.h"
#include "tf_weapon_builder.h"
#include "tf_gamerules.h"
#include "tf_team.h"
#include "in_buttons.h"
#include "engine/IEngineSound.h"
#include "entity_weapon_spawner.h"
#include "tf_weapon_parse.h"

#include "tier0/memdbgon.h"

//#define TF_WEAPON_PICKUP_SOUND		"AmmoPack.Touch"

extern ConVar ofd_instagib;
extern ConVar ofd_multiweapons;

ConVar mp_weaponstay( "mp_weaponstay", "0", FCVAR_NOTIFY, "Weapons dont dissapeer.");
ConVar ofd_allow_allclass_pickups( "ofd_allow_allclass_pickups", "0", FCVAR_REPLICATED | FCVAR_NOTIFY, "Non Merc Classes can pickup weapons.");
 
//-----------------------------------------------------------------------------
// Purpose: Spawn function for the Weapon Spawner
//-----------------------------------------------------------------------------


BEGIN_DATADESC(CWeaponSpawner)

// Inputs.
DEFINE_KEYFIELD(m_iszWeaponName, FIELD_STRING, "weaponname"),
DEFINE_KEYFIELD(m_iszWeaponModel, FIELD_STRING, "model"),
DEFINE_KEYFIELD(m_iszWeaponModelOLD, FIELD_STRING, "powerup_model"),
DEFINE_KEYFIELD(m_iszPickupSound, FIELD_STRING, "pickup_sound"),
DEFINE_KEYFIELD(m_bDisableSpin, FIELD_BOOLEAN, "disable_spin"),
DEFINE_KEYFIELD(m_bDisableShowOutline, FIELD_BOOLEAN, "disable_glow"),
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CWeaponSpawner, DT_WeaponSpawner )
	SendPropBool( SENDINFO( m_bDisableSpin ) ),
	SendPropBool( SENDINFO( m_bDisableShowOutline ) ),
	SendPropBool( SENDINFO( m_bRespawning ) ),
	SendPropBool( SENDINFO( bInitialDelay ) ),
	SendPropTime( SENDINFO( m_flRespawnTick ) ),
	SendPropTime( SENDINFO( fl_RespawnTime ) ),
	SendPropTime( SENDINFO( fl_RespawnDelay ) ),
END_SEND_TABLE()

LINK_ENTITY_TO_CLASS( dm_weapon_spawner, CWeaponSpawner );

void CWeaponSpawner::Spawn( void )
{
	m_nRenderFX = kRenderFxNone;
	if ( ofd_instagib.GetInt() <= 0 && TFGameRules() && !TFGameRules()->IsGGGamemode() ) 
	{
		Precache();
		SetWeaponModel();
		BaseClass::Spawn();
		ResetSequence( LookupSequence("spin") );
	}
}

void CWeaponSpawner::SetWeaponModel( void )
{
	if ( m_iszWeaponModel == MAKE_STRING( "" ) )
	{
		if( m_iszWeaponModelOLD == MAKE_STRING( "" ) )
		{
			CTFWeaponBase *pWeapon = (CTFWeaponBase * )CreateEntityByName( STRING(m_iszWeaponName) );
			WEAPON_FILE_INFO_HANDLE	hWpnInfo = LookupWeaponInfoSlot( pWeapon->GetClassname() );
			Assert( hWpnInfo != GetInvalidWeaponInfoHandle() );
			CTFWeaponInfo *pWeaponInfo = dynamic_cast<CTFWeaponInfo*>( GetFileWeaponInfoFromHandle( hWpnInfo ) );
			Assert( pWeaponInfo && "Failed to get CTFWeaponInfo in weapon spawn" );
			if ( pWeapon && pWeaponInfo )
				SetModel( pWeaponInfo->szWorldModel );
			return;
		}
		else
			m_iszWeaponModel = m_iszWeaponModelOLD;
	}
	SetModel( STRING(m_iszWeaponModel) );
}

//-----------------------------------------------------------------------------
// Purpose: Precache function for the ammopack
//-----------------------------------------------------------------------------
void CWeaponSpawner::Precache( void )
{
	PrecacheScriptSound( STRING( m_iszPickupSound) );
	if ( m_iszWeaponModel == MAKE_STRING( "" ) )
	{
		if( m_iszWeaponModelOLD == MAKE_STRING( "" ) )
		{
			CTFWeaponBase *pWeapon = (CTFWeaponBase * )CreateEntityByName( STRING(m_iszWeaponName) );
			WEAPON_FILE_INFO_HANDLE	hWpnInfo = LookupWeaponInfoSlot( pWeapon->GetClassname() );
			Assert( hWpnInfo != GetInvalidWeaponInfoHandle() );
			CTFWeaponInfo *pWeaponInfo = dynamic_cast<CTFWeaponInfo*>( GetFileWeaponInfoFromHandle( hWpnInfo ) );
			Assert( pWeaponInfo && "Failed to get CTFWeaponInfo in weapon spawn" );
			if ( pWeapon && pWeaponInfo )
			{
				DevMsg("Guess te model is %s \n", (MAKE_STRING(pWeaponInfo->szWorldModel)) );
				PrecacheModel( pWeaponInfo->szWorldModel );
			}
			return;
		}
		else
			m_iszWeaponModel = m_iszWeaponModelOLD;
	}
	PrecacheModel( STRING(m_iszWeaponModel) );
}

//-----------------------------------------------------------------------------
// Purpose: MyTouch function for the ammopack
//-----------------------------------------------------------------------------
bool CWeaponSpawner::MyTouch( CBasePlayer *pPlayer )
{
	bool bSuccess = false;

	if ( ValidTouch( pPlayer ) )
	{
		CTFPlayer *pTFPlayer = ToTFPlayer( pPlayer );
		if ( !pTFPlayer )
			return false;
	
		bSuccess = true;
		CTFWeaponBase *pWeapon = (CTFWeaponBase *)pTFPlayer->GiveNamedItem( STRING(m_iszWeaponName) );
		
		for ( int iWeapon = 0; iWeapon < TF_WEAPON_COUNT; ++iWeapon )
		{		
			CTFWeaponBase *pCarriedWeapon = (CTFWeaponBase *)pTFPlayer->GetWeapon( iWeapon );
			if ( !ofd_multiweapons.GetBool() && pCarriedWeapon && pWeapon && pCarriedWeapon->GetSlot() == pWeapon->GetSlot() && pCarriedWeapon != pWeapon && pTFPlayer->m_nButtons & IN_USE )
			{
					pTFPlayer->DropWeapon( pCarriedWeapon );
					if ( pCarriedWeapon )
					{
						pTFPlayer->Weapon_Detach( pCarriedWeapon );
						UTIL_Remove( pCarriedWeapon );
						pCarriedWeapon = NULL;				
					}
			}
			else if ( pCarriedWeapon == pWeapon || ( !ofd_allow_allclass_pickups.GetBool() && !pTFPlayer->GetPlayerClass()->IsClass( TF_CLASS_MERCENARY ) ) ) 
			{
				bSuccess = false;
				if ( pTFPlayer->RestockAmmo(0.5f) )
				{
					CSingleUserRecipientFilter filter( pTFPlayer );
					EmitSound( filter, entindex(), STRING(m_iszPickupSound) );
					if ( mp_weaponstay.GetBool() )
					{
						bSuccess = false;
					}
					else
					{
						bSuccess = true;
						m_nRenderFX = kRenderFxDistort;
					}
					if ( pWeapon )
					{
						pTFPlayer->Weapon_Detach( pWeapon );
						UTIL_Remove( pWeapon );
						pWeapon = NULL;
					}
					
				}
				if ( pTFPlayer->RestockCloak(0.5f) )
				{
					CSingleUserRecipientFilter filter( pTFPlayer );
					EmitSound( filter, entindex(), STRING(m_iszPickupSound) );
					if ( mp_weaponstay.GetBool() )
					{
						bSuccess = false;
					}
					else
					{
						bSuccess = true;
						m_nRenderFX = kRenderFxDistort;
					}
					if ( pWeapon )
					{
						pTFPlayer->Weapon_Detach( pWeapon );
						UTIL_Remove( pWeapon );
						pWeapon = NULL;
					}
					
				}				
				int iMaxMetal = pTFPlayer->GetPlayerClass()->GetData()->m_aAmmoMax[TF_AMMO_METAL];
				if ( pTFPlayer->GiveAmmo( ceil(iMaxMetal * PackRatios[GetPowerupSize()]), TF_AMMO_METAL, true ) )
				{
					CSingleUserRecipientFilter filter( pTFPlayer );
					EmitSound( filter, entindex(), STRING(m_iszPickupSound) );
					if ( mp_weaponstay.GetBool() )
					{
						bSuccess = false;
					}
					else
					{
						bSuccess = true;
						m_nRenderFX = kRenderFxDistort;
					}
					if ( pWeapon )
					{
						pTFPlayer->Weapon_Detach( pWeapon );
						UTIL_Remove( pWeapon );
						pWeapon = NULL;
					}
					return bSuccess;
				}
				if ( pWeapon )
				{
					pTFPlayer->Weapon_Detach( pWeapon );
					UTIL_Remove( pWeapon );
					pWeapon = NULL;
				}
				return bSuccess;
			}
		}
		// did we give them anything?
		if ( bSuccess )
		{
			if ( !ofd_multiweapons.GetBool() )
			{
				if ( pTFPlayer->m_nButtons & IN_USE )
				{}
				else
				{
					if ( pWeapon )
					{
						pTFPlayer->Weapon_Detach( pWeapon );
						UTIL_Remove( pWeapon );
						pWeapon = NULL;
					}					
					return false;
				}
			}
			CSingleUserRecipientFilter filter( pTFPlayer );
			EmitSound( filter, entindex(), STRING(m_iszPickupSound) );
			
			CTFWeaponBase *pGivenWeapon = (CTFWeaponBase *)pTFPlayer->GiveNamedItem( STRING(m_iszWeaponName) );
			pGivenWeapon->GiveTo( pTFPlayer );
			if ( mp_weaponstay.GetBool() )
			{
				bSuccess = false;
			}
			else
			{
				m_nRenderFX = kRenderFxDistort;
			}
		}
		if ( pWeapon )
		{
			pTFPlayer->Weapon_Detach( pWeapon );
			UTIL_Remove( pWeapon );
			pWeapon = NULL;
		}
	}
	
	return bSuccess;
}

CWeaponSpawner::CWeaponSpawner()
{
	m_flRespawnTick = 0.0f;
	ResetSequence( LookupSequence("spin") );
}

CBaseEntity* CWeaponSpawner::Respawn( void )
{
	CBaseEntity *ret = BaseClass::Respawn();
	m_nRenderFX = kRenderFxDistort;
	m_flRespawnTick = GetNextThink();
	ResetSequence( LookupSequence("spin") );
	return ret;
}
