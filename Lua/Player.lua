merge("Data/Characters/TFE_Player.pak")
--[[merge("Data/Player/Anim_Idle.pak")
merge("Data/Player/Anim_Walk.pak")
merge("Data/Player/Anim_Run.pak")]]
--merge("Data/Player/Anim_Sneak.pak")
--[[truc de camille
merge("Assets/Characters/Char_Player.pak")
merge("Assets/Player/Player_idle.anm")
merge("Assets/Player/Player_walk.anm")
]]



FOOTSTEPRUNFRAME = 5
FOOTSTEPRUNFRAME2 = 20

nPlayerHeight = 0.4

nPlayerScale =0.77
nPlayerY = 0 --nPlayerHeight * nPlayerScale

nGizmoSize =0.7
nGizmoBoxsize = nGizmoSize /12
sPlayerAnimState = "IDLE"

tOrigin = Mesh.createBox( nGizmoBoxsize,nGizmoBoxsize,nGizmoBoxsize, Vector(1,1,0,1) )

tX = Mesh.createBox( nGizmoBoxsize,nGizmoBoxsize,nGizmoBoxsize,  Vector(1,0,0,1) )
tX:parent(tOrigin)
tX:translate(nGizmoSize,0,0)

tY = Mesh.createBox( nGizmoBoxsize,nGizmoBoxsize,nGizmoBoxsize,  Vector(0,1,0,0) )
tY:parent(tOrigin)
tY:translate(0,nGizmoSize,0)

tZ = Mesh.createBox(nGizmoBoxsize,nGizmoBoxsize,nGizmoBoxsize,  Vector(0,0,1,1) )
tZ:parent(tOrigin)
tZ:translate(0,0,nGizmoSize)

tOrigin:visible(false)

tPlayerSkinMesh = SkinMesh.find("Player_Mesh");

tPlayerSkinMesh.mSkinExtendOffset = Vector(0,2,0)

tPlayer = Transform.new();
tPlayerSkinMesh:parent(tPlayer);


tPlayer:scale(nPlayerScale,nPlayerScale,nPlayerScale)

tTarget = Mesh.createBox( 0.1, 0.1,0.1,  Vector(0,1,1,1) )
tTarget:shader(): matEmissive(Vector(0.1,0.1,0.1,1))


particleFumee:position ( tPlayer:position().x, 2, tPlayer:position().z)

--||| SHIELD |||--
--tShield = Mesh.createSphere( 1, Vector( 0 , 1 , 1 , 0.5) )
--tShield:shader():blendMode( 'SRCALPHA_BLEND' )
--tShield:shaderPriority( 2000 );

ParticleSmokeIsPlaying = false
tPlayerSkinMesh:shader().shieldWidth = 0.05
------/// Update Player ///-----
function UpdatePlayer( PositionX, PositionZ, Height , Angle, iAnimState)

	tPlayer:position(PositionX, nPlayerY + Height  ,-PositionZ)
particleFumee:position ( tPlayer:position().x, tPlayer:position().y+0.05 , tPlayer:position().z)
	tOrigin:position(PositionX, nPlayerY + 2 ,-PositionZ)
	tPlayer:rotation(0, math.deg( Angle ) + 90,0)
	--tShield:position(PositionX, nPlayerY+0.5 + tPlayer:position().y  ,-PositionZ )
 	if(iAnimState == 0) then
		sPlayerAnimState = "IDLE"
		particleFumee:stop();
		ParticleSmokeIsPlaying = false
	elseif( iAnimState == 1 ) then
		sPlayerAnimState = "WALK"
	elseif( iAnimState == 2 ) then
		sPlayerAnimState = "RUN"
		if ParticleSmokeIsPlaying == false then
			particleFumee:play()
			ParticleSmokeIsPlaying = true
		end
	elseif (iAnimState == 3) and sPlayerAnimState ~= "DODGE" then
		PlayerBlender:frame( RollBlenderAnimData, 0 )
		--PlayerBlenderCursor = RollPosition;

		sPlayerAnimState = "DODGE"
	elseif ( iAnimState == 4 ) and sPlayerAnimState ~= "DEAD" then
		PlayerBlender:frame( DeathBlenderAnimData, 0 )
		sPlayerAnimState = "DEAD"
	elseif( iAnimState == 5 ) then
		sPlayerAnimState = "TELEPORT"

	end

	UpdateSoundCoolDown()
	--print(sPlayerAnimState)
end


function UpdateTarget(  nPositionX, nPositionZ, bVisible )

	tTarget:position( nPositionX , 1, -nPositionZ)
	tTarget:visible(bVisible)
end

function TeleportPlayer()
	PlayerBlenderCursor =  TeleportPosition;

end

function SetPlayerIdle()
	PlayerBlenderCursor =  IdlePosition;
	sPlayerAnimState = "IDLE"
end



function UpdateShield( nValue )

	tPlayerSkinMesh:shader().shieldOpacity = nValue

end
function KillPlayer()
	PlayerBlender:frame( DeathBlenderAnimData, 0 )
	PlayerBlender:speed( DeathBlenderAnimData, 1 )
	sPlayerAnimState = "DEAD"
	--PlayerBlenderCursor = DeathPosition;
end

---/// Anim Blender ///---

TransitionDuration = 0.05;--In Second

PlayerBlender = AnimBlender.new("PlayerBlender");
PlayerBlender:init{ mode = "Space", root = tPlayer, ctrlNb = 6}

IdleAnimData = AnimData.find("Idle")
RollAnimData = AnimData.find("Roll")
WalkAnimData = AnimData.find("Walk")
RunAnimData = AnimData.find("Run")
DeathAnimData = AnimData.find("Death")
TeleportAnimData = AnimData.find("Teleport")



tPlayer:animPlay( IdleAnimData, true);

IdleBlenderAnimData = PlayerBlender:play(IdleAnimData);
RollBlenderAnimData = PlayerBlender:play(RollAnimData);
WalkBlenderAnimData = PlayerBlender:play(WalkAnimData);
RunBlenderAnimData = PlayerBlender:play(RunAnimData);
DeathBlenderAnimData = PlayerBlender:play(DeathAnimData);
TeleportBlenderAnimData = PlayerBlender:play(TeleportAnimData);




PlayerBlender:cyclic( IdleBlenderAnimData, true );
PlayerBlender:cyclic( RollBlenderAnimData, true );
PlayerBlender:cyclic( WalkBlenderAnimData, true );
PlayerBlender:cyclic( RunBlenderAnimData, true );
PlayerBlender:cyclic( DeathBlenderAnimData, false );
PlayerBlender:cyclic( TeleportBlenderAnimData, true );

RollPosition = Vector(-0.5,0,0,0.5);
IdlePosition = Vector(0,0,0,0.5);
WalkPosition = Vector(0.5,0,0,0.5);
RunPosition  = Vector(1,0,0,0.5);
DeathPosition  = Vector(0,1,0,0.5);
TeleportPosition  = Vector(0,-1,0,0.5);

PlayerBlenderCursor = WalkPosition;

PlayerBlender:spaceAnim( IdleBlenderAnimData , IdlePosition );
PlayerBlender:spaceAnim( RollBlenderAnimData , RollPosition );
PlayerBlender:spaceAnim( WalkBlenderAnimData , WalkPosition );
PlayerBlender:spaceAnim( RunBlenderAnimData , RunPosition );
PlayerBlender:spaceAnim( DeathBlenderAnimData , DeathPosition );
PlayerBlender:spaceAnim( TeleportBlenderAnimData , TeleportPosition );

PlayerBlender:synchronize( RollBlenderAnimData , true  );
PlayerBlender:synchronize( WalkBlenderAnimData , true  );
PlayerBlender:synchronize( RunBlenderAnimData , true  );


PlayerBlender:speed(DeathBlenderAnimData, 0)


---/// End Anim Blender ///---


function PlayerBlenderUpdate()
--	print( sPlayerAnimState , PlayerBlenderCursor)
	local ScaledLerpValue = Clock.deltaTime() / TransitionDuration;

	if(sPlayerAnimState == "IDLE") then
		--PlayerBlender:speed(DeathBlenderAnimData, 0)
		tPlayerSkinMesh:rotation( 0,0,0 )
		PlayerBlenderCursor = Lerp(PlayerBlenderCursor, IdlePosition , ScaledLerpValue)
	elseif( sPlayerAnimState == "WALK" ) then
		tPlayerSkinMesh:rotation( 0,0,0 )
		PlayerBlenderCursor = Lerp(PlayerBlenderCursor, WalkPosition, ScaledLerpValue )
	elseif( sPlayerAnimState == "RUN" ) then
		tPlayerSkinMesh:rotation( 0,0,0 )
		PlayerBlenderCursor = Lerp(PlayerBlenderCursor, RunPosition, ScaledLerpValue )
		--print(PlayerBlender:frame( RunBlenderAnimData ))
		if PlayerBlender:frame( RunBlenderAnimData ) >= FOOTSTEPRUNFRAME  and PlayerBlender:frame( RunBlenderAnimData ) < FOOTSTEPRUNFRAME+1  then
			PlayFootStep()
		end

		if PlayerBlender:frame( RunBlenderAnimData ) >= FOOTSTEPRUNFRAME2  and PlayerBlender:frame( RunBlenderAnimData ) < FOOTSTEPRUNFRAME2+1  then
			PlayFootStep()
		end

	elseif( sPlayerAnimState == "DODGE" ) then
		PlayerBlenderCursor = Lerp(PlayerBlenderCursor, RollPosition, ScaledLerpValue*2 )

	elseif( sPlayerAnimState == "DEAD" ) then
		--PlayerBlender:speed(DeathBlenderAnimData, 1)
		PlayerBlenderCursor =Lerp(PlayerBlenderCursor, DeathPosition, ScaledLerpValue*2 )
		if PlayerBlender:frame( DeathBlenderAnimData ) >= PlayerBlender:duration( DeathBlenderAnimData ) - 1 then
				--PlayerBlender:frame( DeathBlenderAnimData, PlayerBlender:duration( DeathBlenderAnimData ) - 1 )
				PlayerBlender:speed(DeathBlenderAnimData, 0)
		end
	elseif(sPlayerAnimState == "TELEPORT") then
		tPlayerSkinMesh:rotation( 0,0,0 )

		PlayerBlenderCursor = Lerp(PlayerBlenderCursor, TeleportPosition , ScaledLerpValue)
	end
	PlayerBlender:spacePos( PlayerBlenderCursor );
end
