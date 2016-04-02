#ifdef HLSL
	#define VS_PROFILE				vs_3_0
	#define PS_PROFILE				ps_3_0
	#define WORLDVIEWPROJECTION		register(c0)
	#define WORLD					register(c4)
	#define VIEW					register(c8)
	#define PROJECTION				register(c12)
    #define WORLDINVERSETRANSPOSE	register(c16)    
    #define VIEWINVERSETRANSPOSE	register(c20)
	#if HLSL < 40
		#if PSDOMAIN
			#define SV_POSITION		VPOS
		#else
			#define SV_POSITION		SV_POSITION
		#endif
	#endif
#else
	#define VS_PROFILE				latest
	#define PS_PROFILE				latest
	#define register(reg)			reg
	#if PSDOMAIN
		#define SV_POSITION			WPOS
	#else
		#define SV_POSITION			POSITION
	#endif
#endif

//Untweakables
float4x4 worldViewProj : WORLDVIEWPROJECTION;
float4x4 world   : WORLD;
float4x4 worldInverseTranspose : WORLDINVERSETRANSPOSE;
float4x4 viewInverseTranspose : VIEWINVERSETRANSPOSE;
float4x4 view : VIEW;

