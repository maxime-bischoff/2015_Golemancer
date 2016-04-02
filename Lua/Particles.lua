
-----------------------------------------------------------------------
particleSparks = ParticleSystem.new{ mAutoStart=true }
particleImpact = ParticleSystem.new{ mAutoStart=true }
particleCircle = ParticleSystem.new { mAutoStart = true}
particleFumee = ParticleSystem.new{ mAutoStart=true }
particleFreeze = ParticleSystem.new { mAutoStart =true }
particleBallImpact = ParticleSystem.new{ mAutoStart=true }
-----------------------------------------------------------------------
--Sphere = Mesh.createSphere( 0.5 )
--Sphere:shader():matDiffuse(0,0,0)
--Sphere:position(0,0,0)
-----------------------------------------------------------------------
sparks = Texture.new()
sparks:loadFile("Data/Particles/sparks.tga")

impact = Texture.new()
impact:loadFile("Data/Particles/Impact_Particles.tga")

circle = Texture.new()
circle:loadFile("Data/Particles/circle_particles.tga");

Nuage = Texture.new()
Nuage:loadFile("Data/Particles/cloud.tga")

flocon = Texture.new()
flocon:loadFile("Data/Particles/flocon.tga")

ballImpact = Texture.new()
ballImpact:loadFile("Data/Particles/BallImpact.tga")

impactParticlesArray =  {}

-----------------------------------------------------------------------
globalScale = 0.05
scaleSparkEmmiter = 0.5;
scaleSpark = 0.2;
-----------------------------------------------------------------------
particleSparks.mEmitterArray[1] =
{
	mParticleNb =		20,
	mVolume =			Vector( scaleSparkEmmiter, scaleSparkEmmiter, scaleSparkEmmiter, 0 ),
	mPath =	{},
	mRate = -1,
	mDirection = Vector(0,0,0),
	mTexDivideU =	2,
	mTexDivideV =	2,
	mCollisionMode = 0,
	mTransform = nil,
	mCpuFade =	0,

	mLifeArray=
	{
		{
			mLifeDuration =	Vector( 0.05,0.05 ),					-- lifetime (min/max) in seconds
			mRebirth =		1,
			mTexOffsetU =	0,
			mTexOffsetV =	0,
			mResetPos =		1,
			mStartingAngleMin = 0,
			mStartingAngleMax = 360,

			mScaleMin =			Vector( scaleSpark, scaleSpark, scaleSpark ),
			mScaleMax =			Vector( scaleSpark, scaleSpark, scaleSpark ),

			mRamp =
			{
				{ mT=0.0, mColor = Vector( 0.8, 0, 0, 0 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=0.1, mColor = Vector( 1, 0.5, 0.5, 1 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=0.2, mColor = Vector( 1, 1, 1, 1 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=1.0, mColor = Vector( 1, 0, 0, 0 ), mScaleFactor = Vector( 1, 1, 1 ) }
			}
		},
		{

			mResetPos = 0,
			mResetSpeed = 0,
			mResetColor = 1,
			mResetScale = 1,
			mResetRotation = 1,

			mLifeDuration =	Vector( 0.05,0.1 ),					-- lifetime (min/max) in seconds
			mRebirth =		1,

			mTexOffsetU =	0,
			mTexOffsetV =	1,
			-- If mResetPos = 0 Then each particle restarts from the previous position (position at the end of the previous life)
			mResetPos =		1,
			mStartingAngleMin = 0,
			mStartingAngleMax = 360,


      mScaleMin =			Vector( scaleSpark, scaleSpark, scaleSpark ),
			mScaleMax =			Vector( scaleSpark, scaleSpark, scaleSpark ),

			mRamp =
			{
				{ mT=0.0, mColor = Vector( 0, 0, 0, 0 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=0.1, mColor = Vector( 1, 0.5, 0.5, 1 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=0.2, mColor = Vector( 1, 1, 1, 1 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=1.0, mColor = Vector( 1, 0, 0, 0 ), mScaleFactor = Vector( 1, 1, 1 ) }
			}
		},
		{

			mResetPos = 0,
			mResetSpeed = 0,
			mResetColor = 1,
			mResetScale = 1,
			mResetRotation = 1,

			mLifeDuration =	Vector( 0.05,0.1 ),					-- lifetime (min/max) in seconds
			mRebirth =		1,

			mTexOffsetU =	1,
			mTexOffsetV =	1,
			-- If mResetPos = 0 Then each particle restarts from the previous position (position at the end of the previous life)
			mResetPos =		1,
			mStartingAngleMin = 0,
			mStartingAngleMax = 360,

      mScaleMin =			Vector( scaleSpark, scaleSpark, scaleSpark ),
			mScaleMax =			Vector( scaleSpark, scaleSpark, scaleSpark ),

	    mRamp =
			{
				{ mT=0.0, mColor = Vector( 0.5, 0.5, 0.5, 0 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=0.1, mColor = Vector( 1, 0.5, 0.5, 1 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=0.2, mColor = Vector( 1, 1, 1, 1 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=1.0, mColor = Vector( 1, 0, 0, 0 ), mScaleFactor = Vector( 1, 1, 1 ) }
			}
		}
	}
}


impactGlobalScale = 0.01
impactParticulScale = 0.8

particleImpact.mEmitterArray[1]=
{

  	mParticleNb =		1,

  	mVolume =			Vector( impactGlobalScale, impactGlobalScale, impactGlobalScale, 0 ),

  	mVolumeTexture = 0,

  	mPath =	{},

  	mRate = -1,

    mTransform = tFocusTransform,

  	mDirection = Vector(0,0,0),


  	mTexDivideU =	1,
  	mTexDivideV =	5,

  	-- 0: no geom, 1: geom specifically indicated, 2: All existing geoms
  	mCollisionMode = 0,
  	mBounce =	0.2,			-- Damping by normal vector of the contact point
  	mFriction = 0.2,			-- Damping by tangent vector of the contact point

  	mTransform = nil,

  	mCpuFade =	0,
  	mLifeArray =
  	{
  		{--|||||||||||||  FRAME 1 |||||||||||||--
  			mLifeDuration =	Vector( 0.02,0.02 ),					-- lifetime (min/max) in seconds
  			mRebirth =		1,
  			mTexOffsetU =	0,
  			mTexOffsetV =	4,
  			-- If mResetPos = 0 Then each particle restarts from the previous position (position at the end of the previous life)
  			mResetPos =		1,
  			mStartingAngleMin = 0,
  			mStartingAngleMax = 360,

  			mScaleMin =			Vector( impactParticulScale, impactParticulScale, impactParticulScale ),
  			mScaleMax =			Vector( impactParticulScale, impactParticulScale, impactParticulScale ),

  		},--|||||||||||||  FRAME 2 |||||||||||||--
  		{

  			mResetPos = 0,
  			mResetSpeed = 0,
  			mResetColor = 1,
  			mResetScale = 0,
  			mResetRotation = 0,

  			mLifeDuration =	Vector( 0.02,0.02 ),					-- lifetime (min/max) in seconds
  			mRebirth =		1,

  			mTexOffsetU =	0,
  			mTexOffsetV =	3,


  		},
  		{--|||||||||||||  FRAME 3 |||||||||||||--

  			mResetPos = 0,
  			mResetSpeed = 0,
  			mResetColor = 0,
  			mResetScale = 0,
  			mResetRotation = 0,

  			mLifeDuration =	Vector( 0.02,0.02 ),					-- lifetime (min/max) in seconds
  			mRebirth =		1,

  			mTexOffsetU =	0,
  			mTexOffsetV =	2,



  		},
  		{--|||||||||||||  FRAME 4 |||||||||||||--

  			mResetPos = 0,
  			mResetSpeed = 0,
  			mResetColor = 0,
  			mResetScale = 0,
  			mResetRotation = 0,

  			mLifeDuration =	Vector( 0.02,0.02 ),					-- lifetime (min/max) in seconds
  			mRebirth =		1,

  			mTexOffsetU =	0,
  			mTexOffsetV =	1,



  		},
  		{--|||||||||||||  FRAME 5 |||||||||||||--

  			mResetPos = 0,
  			mResetSpeed = 0,
  			mResetColor = 0,
  			mResetScale = 0,
  			mResetRotation = 0,

  			mLifeDuration =	Vector( 0.02,0.02),					-- lifetime (min/max) in seconds
  			mRebirth =		1,

  			mTexOffsetU =	0,
  			mTexOffsetV =	0,

  		}
  	}
}

circleGlobalScale = 0.50
circleScale = 0.05


circleSpeed = 2.0;
particleCircle.mEmitterArray[1] =
{
	mParticleNb =100,
	mVolume =			Vector(circleGlobalScale , 0.1, circleGlobalScale, 0 ),
	mPath =	{},
	mRate = -1,
	mDirection = Vector(0,0,0),
	mTexDivideU =	1,
	mTexDivideV =	1,
	mCollisionMode = 0,
	mTransform = nil,
	mCpuFade =	0,

	mLifeArray=
	{
		{
			mLifeDuration =	Vector( 1.5,3 ),					-- lifetime (min/max) in seconds
			mRebirth =		1,
			mTexOffsetU =	0,
			mTexOffsetV =	0,
			mResetPos =		1,
			mStartingAngleMin = 0,
			mStartingAngleMax = 360,
			mStartingVelocityMin =		Vector( -0.2, 0, -0.2 ),
			mStartingVelocityMax =		Vector( 0.2, 0, .2 ),
			mAirDamping = 				Vector(1,1,1,1),			-- X, Y, Z damping + global : values between 0.0 and 1.0 ( 1.0 = no damping )
			mForceMin =					Vector( 0, circleSpeed/2, 0 ),		-- force ( ex: gravity ) in global coord sys
			mForceMax =					Vector( 0, circleSpeed, 0 ),
			mAngularVelocity =			Vector( 360,0,360, 60 ),		-- ( BeginMin, BeginMax, EndMin, EndMax ) : absolute values
			mAngularVelocityDir =		1,							-- ( -1/1 : to force a negative / positive velocity, 0 : random, 2: direction of moving )
			mScaleDirParam =	Vector(1, 1),
			mScaleMin =			Vector( circleScale, circleScale, circleScale ),
			mScaleMax =			Vector( circleScale*0.2, circleScale*0.2, circleScale*0.2 ),

			mRamp =
			{
				{ mT=0.0, mColor = Vector( 0, 0, 1, 0 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=0.1, mColor = Vector( 1, 1, 1, 1 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=0.2, mColor = Vector( 1, 1, 1, 1 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=1.0, mColor = Vector( 1, 0, 0, 0 ), mScaleFactor = Vector( 1, 1, 1 ) }
			}
		}
	}
}




BallImpactFrameLength = 0.03
particleBallImpact.mEmitterArray[1] =
{

	mParticleNb =		2,
	mVolume =			Vector( 0, 0, 0, 1 ),
	mRate = -1,
	mTexDivideU =	2,
	mTexDivideV =	2,
	mTransform = nil,
	mLifeArray=
	{
		{---LIFE 1
			mLifeDuration =	Vector( BallImpactFrameLength,BallImpactFrameLength ),					-- lifetime (min/max) in seconds
			mRebirth =		1,
			mTexOffsetU =	0,
			mTexOffsetV =	1,
			mResetPos =		1,
			mStartingAngleMin = 0,
			mStartingAngleMax = 360,
			mScaleMin =			Vector( 0.8, 0.8, 0.8 ),
			mScaleMax =			Vector( 1.1, 1.1, 1.1 ),

		},
		{---LIFE 2

			mResetPos = 0,
			mResetSpeed = 0,
			mResetColor = 0,
			mResetScale = 0,
			mResetRotation = 0,
			mLifeDuration =	Vector( BallImpactFrameLength,BallImpactFrameLength ),
			mRebirth =		1,
			mTexOffsetU =	1,
			mTexOffsetV =	1,
		},
		{---LIFE 3

			mResetPos = 0,
			mResetSpeed = 0,
			mResetColor = 0,
			mResetScale = 0,
			mResetRotation = 0,
			mLifeDuration =	Vector( BallImpactFrameLength,BallImpactFrameLength ),
			mRebirth =		1,
			mTexOffsetU =	0,
			mTexOffsetV =	0,
		},
		{---LIFE 4

			mResetPos = 0,
			mResetSpeed = 0,
			mResetColor = 0,
			mResetScale = 0,
			mResetRotation = 0,
			mLifeDuration =	Vector( BallImpactFrameLength,BallImpactFrameLength ),
			mRebirth =		1,
			mTexOffsetU =	1,
			mTexOffsetV =	0,
		}--[[,
		{---LIFE 5 - Rest

			mResetPos = 0,
			mResetSpeed = 0,
			mResetColor = 0,
			mResetScale = 0,
			mResetRotation = 0,
			mLifeDuration =	Vector( 2,2 ),
			mRebirth =		1,
			mTexOffsetU =	1,
			mTexOffsetV =	0,

		}]]

	}

}



shader1 = Shader.new()
shader1:texture(0,Nuage)
shader1:blendMode("SRCALPHA_BLEND")
shader1:flags{nozwrite=1,unlit=1}




C_velocity = 0.5
C_vertical_velocity =0.5
C_gravity = -0.5


particleFumee.mEmitterArray[1] =
{

	mParticleNb =6,

	mShader = shader1,
	mCpuFade =	0,

	mRate = -1,

	mVolume = Vector( 0.2,0.2,0.2, 4),

	mLifeArray=
	{
		{
			mLifeDuration =	Vector( 0.1, 0.3),
			mRebirth =		1,
			mResetPos =		1,

			mStartingVelocityMin =		Vector( -0.25, -0.05, -0 ),
			mStartingVelocityMax =		Vector( 0, 0, 0 ),
				mStartingAngleMin =0,
			mStartingAngleMax = 0,
			mForceMin =					Vector( 0, 0, 0 ),
			mForceMax =					Vector( 0,-0.01 , 0 ),

			mScaleMin =			Vector( 0.5,0.5,0.5),
			mScaleMax =			Vector( 1,1,1 ),

			mRamp = {
		{mColor = Vector( 0.8,0.8,0.8, 0.2 ),
				mScaleFactor = Vector( 0.25,0.25,0.25 ) },
			{mColor = Vector( 0.8,0.8,0.8, 0.5 ),
				mScaleFactor = Vector(0.3,0.3,0.3 ) },
			{mColor = Vector( 0.8,0.8,0.8, 0 ),
				mScaleFactor = Vector( 0.5, 0.5, 0.5 ) } }
		}
	}

}



Input.setCtrlMapping(0,	KB_RETURN)
circleGlobalScale = 0.50
circleScale = 0.2


circleSpeed = 2.0;





particleFreeze.mEmitterArray[1] =
{
	mParticleNb =100,
	mVolume =			Vector(circleGlobalScale , 0.1, circleGlobalScale, 0 ),
	mPath =	{},
	mRate = 500,
	mDirection = Vector(0,0,0),
	mTexDivideU =	1,
	mTexDivideV =	1,
	mCollisionMode = 0,
	mTransform = nil,
	mCpuFade =	0,

		mLifeArray=
	{
		{
			mLifeDuration =	Vector( 1.5,3 ),					-- lifetime (min/max) in seconds
			mRebirth =		0,
			mTexOffsetU =	0,
			mTexOffsetV =	0,
			mResetPos =		1,
			mStartingAngleMin = 0,
			mStartingAngleMax = 360,
				mStartingVelocityMin =		Vector( -10, 0, -10 ),
			mStartingVelocityMax =		Vector(10, 1, 10 ),
			mAirDamping = 				Vector(1,1,1,1),			-- X, Y, Z damping + global : values between 0.0 and 1.0 ( 1.0 = no damping )
			mForceMin =					Vector( -2, 0, -2 ),		-- force ( ex: gravity ) in global coord sys
			mForceMax =					Vector( 2, 0, 2 ),
			mAngularVelocity =			Vector( 360,0,360, 60 ),		-- ( BeginMin, BeginMax, EndMin, EndMax ) : absolute values
			mAngularVelocityDir =		1,							-- ( -1/1 : to force a negative / positive velocity, 0 : random, 2: direction of moving )
			mScaleDirParam =	Vector(1, 1),
			mScaleMin =			Vector( circleScale, circleScale, circleScale ),
			mScaleMax =			Vector( circleScale*0.6, circleScale*0.6, circleScale*0.6),

			mRamp =
			{
				{ mT=0.0, mColor = Vector( 0, 0.5, 0.75, 0 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=0.1, mColor = Vector( 1, 1, 1, 1 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=0.2, mColor = Vector( 1, 1, 1, 1 ), mScaleFactor = Vector( 1, 1, 1 ) },
				{ mT=1.0, mColor = Vector( 1, 1, 1, 0 ), mScaleFactor = Vector( 1, 1, 1 ) }
			}
		}
	}

}

particleSparks.mEmitterArray[1].mShader:texture( 0, sparks )
particleSparks:position(1.89, 1.3995, 0.1639666, 1)
particleSparks:stop()

particleImpact.mEmitterArray[1].mShader:texture( 0, impact )
particleImpact:parent( tFocusTransform )
particleImpact:stop()

particleCircle.mEmitterArray[1].mShader:texture( 0, circle )
particleCircle:position(-10,-10,-10, 1)
particleCircle:stop()

particleBallImpact.mEmitterArray[1].mShader:texture( 0, ballImpact )
particleBallImpact:position(-10,-10,-10, 1)
particleBallImpact:stop()

particleFreeze.mEmitterArray[1].mShader:texture( 0, flocon )
particleFreeze:stop()


particleFumee:stop()

function FreezeAll()
particleFreeze:position ( tPlayer:position() )
particleFreeze:position().y = 2.0;
particleFreeze:play();
end



function UpdateImpactsParticles()
	i = 1
	while i <= #impactParticlesArray  do

	impactParticlesArray[i].countDown = impactParticlesArray[i].countDown - Clock:deltaTime()

		if ( impactParticlesArray[i].countDown <= 0)	 then
			impactParticlesArray[i].emitter:release()
			impactParticlesArray[i] = impactParticlesArray[#impactParticlesArray]
			impactParticlesArray[#impactParticlesArray] = nil
		end
		i = i + 1
	end
end



function CreateImpactParticles( posX,  posY, posZ)
	impactParticlesArray[#impactParticlesArray + 1] =  {}
	impactParticlesArray[#impactParticlesArray].emitter = particleBallImpact:clone();
	impactParticlesArray[#impactParticlesArray].emitter:position(posX, posY + 0.6, -posZ);
	impactParticlesArray[#impactParticlesArray].emitter:runUp( -1, 0.5 )
	impactParticlesArray[#impactParticlesArray].emitter:play();

	impactParticlesArray[#impactParticlesArray].countDown = BallImpactFrameLength*5
end

function CreateHeavyImpactParticles( posX,  posY, posZ)
	impactParticlesArray[#impactParticlesArray + 1] =  {}
	impactParticlesArray[#impactParticlesArray].emitter = particleImpact:clone();
	impactParticlesArray[#impactParticlesArray].emitter:position(posX, posY + 0.5, -posZ);
	impactParticlesArray[#impactParticlesArray].emitter:runUp( -1, 0.5 )
	impactParticlesArray[#impactParticlesArray].emitter:play();
	impactParticlesArray[#impactParticlesArray].countDown = BallImpactFrameLength*5
end
