//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: HACK: Makes gameui link on linux by defining some dummy functions regarding particle effects.
//
// $NoKeywords: $
//===========================================================================//
#include "tier1/utlobjectreference.h"
class CSheet{
    DECLARE_REFERENCED_CLASS(CSheet)
};
#include "cbase.h"

//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CNewParticleEffect::CNewParticleEffect( CBaseEntity *pOwner, CParticleSystemDefinition *pEffect )
{
}

CNewParticleEffect::CNewParticleEffect( CBaseEntity *pOwner, const char* pEffectName )
{
}

void CNewParticleEffect::Construct()
{
}

CNewParticleEffect::~CNewParticleEffect(void)
{

}


//-----------------------------------------------------------------------------
// Refcounting
//-----------------------------------------------------------------------------
void CNewParticleEffect::AddRef()
{
}

void CNewParticleEffect::Release()
{
}

void CNewParticleEffect::NotifyRemove()
{
}

int CNewParticleEffect::IsReleased()
{
	return 0;
}


//-----------------------------------------------------------------------------
// Refraction and soft particle support
//-----------------------------------------------------------------------------
bool CNewParticleEffect::UsesPowerOfTwoFrameBufferTexture( void )
{
	return false;
}

bool CNewParticleEffect::UsesFullFrameBufferTexture( void )
{
	return false;
}

bool CNewParticleEffect::IsTwoPass( void )
{
	return false;
}


//-----------------------------------------------------------------------------
// Overrides for recording
//-----------------------------------------------------------------------------
void CNewParticleEffect::StopEmission( bool bInfiniteOnly, bool bRemoveAllParticles, bool bWakeOnStop )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNewParticleEffect::SetDormant( bool bDormant )
{
}

void CNewParticleEffect::SetControlPointEntity( int nWhichPoint, CBaseEntity *pEntity )
{
}


void CNewParticleEffect::SetControlPoint( int nWhichPoint, const Vector &v )
{
}


void CNewParticleEffect::RecordControlPointOrientation( int nWhichPoint )
{
}

void CNewParticleEffect::SetControlPointOrientation( int nWhichPoint, 
	const Vector &forward, const Vector &right, const Vector &up )
{
}

void CNewParticleEffect::SetControlPointOrientation( int nWhichPoint, const Quaternion &q )
{
}

void CNewParticleEffect::SetControlPointForwardVector( int nWhichPoint, const Vector &v )
{
}

void CNewParticleEffect::SetControlPointUpVector( int nWhichPoint, const Vector &v )
{
}

void CNewParticleEffect::SetControlPointRightVector( int nWhichPoint, const Vector &v )
{
}


//-----------------------------------------------------------------------------
// Called when the particle effect is about to update
//-----------------------------------------------------------------------------
void CNewParticleEffect::Update( float flTimeDelta )
{
}


//-----------------------------------------------------------------------------
// Bounding box
//-----------------------------------------------------------------------------
CNewParticleEffect* CNewParticleEffect::ReplaceWith( const char *pParticleSystemName )
{
    return nullptr;
}


//-----------------------------------------------------------------------------
// Bounding box
//-----------------------------------------------------------------------------
void CNewParticleEffect::SetParticleCullRadius( float radius )
{
}

bool CNewParticleEffect::RecalculateBoundingBox()
{
	return false;
}


void CNewParticleEffect::GetRenderBounds( Vector& mins, Vector& maxs )
{
}

void CNewParticleEffect::DetectChanges()
{
}

void CNewParticleEffect::DebugDrawBbox ( bool bCulled )
{
}


//-----------------------------------------------------------------------------
// Rendering
//-----------------------------------------------------------------------------
int CNewParticleEffect::DrawModel( int flags )
{
	return 0;
}

//-----------------------------------------------------------------------------
// Helper functions.
//-----------------------------------------------------------------------------
void DefaultRenderBoundsWorldspace( IClientRenderable *pRenderable, Vector &absMins, Vector &absMaxs )
{
}

CParticleCollection::CParticleCollection( void ){
}
CParticleCollection::~CParticleCollection( void ){
}


void CParticleCollection::operator delete(void *pData){
}

bool CParticleCollection::IsTranslucent() const{
    return false;
}

const char *CParticleCollection::GetName() const{
    return "if you're seeing this in console, @Nopey's linux linker hack has failed us all";
}
