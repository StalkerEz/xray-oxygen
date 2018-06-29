#include "common.h"
#include "lmodel.h"
#include "shadow.h"



Texture2D	s_water;


#ifdef GBUFFER_OPTIMIZATION
float4 main ( float2 tc : TEXCOORD0, float2 tcJ : TEXCOORD1, float4 pos2d : SV_Position ) : SV_Target
#else
float4 main ( float2 tc : TEXCOORD0, float2 tcJ : TEXCOORD1 ) : SV_Target
#endif
{
#ifdef GBUFFER_OPTIMIZATION
	gbuffer_data gbd = gbuffer_load_data( tc, pos2d );
#else
	gbuffer_data gbd = gbuffer_load_data( tc );
#endif
	float4 _P = float4( gbd.P, 1.0 );
	//_P.w = 1.f;

	float4 PS = mul( m_shadow,  _P );

//	float s	= sample_hw_pcf( PS, float4(0,0,0,0) );

	float s	= shadow		(PS);

//	float s	= shadowtest_sun( PS, tcJ );

//	float 	d 	= sunmask( _P );

	float2 		tc1	= mul( m_sunmask, _P );		//

//	tc1 /= 10;

	tc1 /= 2;

	tc1 = frac(tc1);

	float4 water = s_water.SampleLevel( smp_linear, tc1, 0 );

//	water.xy = tc1;
//	water.z = 0;

//	d*= s;

//	return s;
//	return float4(s,0,0,0);
//	return float4(0,0,0,s/2);
//	return float4(0,0,d,s/2);

	water.xyz = (water.xzy-0.5)*2;

//	water.y	*= -1;

	water.xyz = mul( m_V, water.xyz );

	water *= s;

	return float4(water.xyz,s/2);
}
/*
#ifdef GBUFFER_OPTIMIZATION
float4 main ( float2 tc : TEXCOORD0, float2 tcJ : TEXCOORD1, float4 pos2d : SV_Position ) : SV_Target
#else
float4 main ( float2 tc : TEXCOORD0, float2 tcJ : TEXCOORD1 ) : SV_Target
#endif
{
#ifdef GBUFFER_OPTIMIZATION
	gbuffer_data gbd = gbuffer_load_data( tc, pos2d );
#else
	gbuffer_data gbd = gbuffer_load_data( tc );
#endif
	//float4 	_P	= tex2D( s_position, tc );
	//float4	_N	= tex2D( s_normal, tc );

	float4 	_P = float4( gbd.P, gbd.mtl )
	float4	_N = float4( gbd.N, gbd.hemi );

	// ----- light-model
	float	m	= xmaterial;
# ifndef USE_R2_STATIC_SUN
			m 	= _P.w;
# endif
	float4	light	= plight_infinity ( m, _P, _N, Ldynamic_dir );

	// ----- shadow
  	float4 	P4 	= float4( _P.x, _P.y, _P.z, 1.f);
	float4 	PS	= mul( m_shadow, P4 );
	float 	s 	= sunmask( P4 );
	#ifdef 	USE_SJITTER
	  		s 	*= shadowtest_sun( PS, tcJ );
	#else
	  		s 	*= shadow( PS );
	#endif

	return 		blend( Ldynamic_color * light * s, tc );
}
*/
