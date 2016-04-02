
merge("Data/Characters/Boss3.pak")

SLEEP = 0
AIMING = 1
ATTACKING = 2
CHARGINGSLAM = 3
COOLDOWN = 5
HANDMINISLAM = 10
AWAKING = 11

tBoss = {}

tBoss.State = SLEEP
--tBoss.Body = Mesh.createBox( 1.5, 3 , 1.5, Vector(0.2,0.2,0.2,1))
--tBoss.LeftHandDebug = Mesh.createBox( 0.5, 0.5 , 0.5, Vector(0,1,1,1))
--tBoss.RightHandDebug = Mesh.createBox( 0.5, 0.5 , 0.5, Vector(1,0,0,1))



--tBoss.BodyJoint = SkinMesh.find("Boss_Mesh")
tBoss.Body = SkinMesh.find("BossBody_Mesh")--Transform.new()
--tBoss.BodyJoint:parent( tBoss.Body )
tBoss.Anim = {}
tBoss.Anim.Fist = AnimData.find("Boss_Fist")
tBoss.Anim.Idle = AnimData.find("Boss_Idle")
tBoss.Anim.Slam = AnimData.find("Boss_Slam")
tBoss.Anim.Aim = AnimData.find("Boss_Aim")
tBoss.Anim.Fall = AnimData.find("Boss_Fall")
tBoss.Anim.Open = AnimData.find("Boss_Open")
tBoss.Anim.Awaken = AnimData.find("Boss_Body_Awaken")


tBoss.Lock = {}

tBoss.Lock[1] = Mesh.find("Lock_2")
tBoss.Lock[2] = Mesh.find("Lock_3")
tBoss.Lock[3] = Mesh.find("Lock_1")

--tBoss.LockVisible = 0;

tBoss.Lock[1]:shaderPriority(200)
tBoss.Lock[2]:shaderPriority(200)
tBoss.Lock[3]:shaderPriority(200)







tBoss.LeftHandJoint = SkinMesh.find("LeftHand_Mesh")
tBoss.RightHandJoint  = SkinMesh.find("RightHand_Mesh")

tBoss.LeftHandIK = Transform.find("ref_RightHand_global")
tBoss.RightHandIK = Transform.find("ref_LeftHand_global")
--ikDebug1 = Mesh.createBox( 0.5, 0.5 , 0.5, Vector(1,1,0,1))
--ikDebug2 = Mesh.createBox( 0.5, 0.5 , 0.5, Vector(1,0,1,1))

--tBoss.LeftHandPos = Transform.find("LeftHandPos")
--tBoss.RightHandPos = Transform.find("RightHandPos")
tBoss.LeftHand = Transform.new()
tBoss.RightHand = Transform.new()

--tBoss.RightHandAim = Transform.find( "RightElbowPoleVector" )
--tBoss.LeftHandAim =Transform.find( "LeftElbowPoleVector" )

--tBoss.LeftHand  :position( tBoss.LeftHandJoint  : position() )
--tBoss.RightHand :position( tBoss.RightHandJoint : position() )

tBoss.LeftHandJoint :parent( tBoss.LeftHand  )
tBoss.RightHandJoint:parent( tBoss.RightHand )
--tBoss.LeftHandJoint :position(0,0,0)
--tBoss.RightHandJoint:position(0,0,0)
--tBoss.LeftHand:rotation(0,180,0)
--tBoss.RightHand:rotation(0,180,0)

tBoss.LookatPosLeft  = Transform.new()--Joint.find("CenterSpine2") :position()
tBoss.LookatPosRight = Transform.new()-- Joint.find("CenterSpine2"):position()


tBoss.LeftHandJoint:visible(false)
tBoss.RightHandJoint:visible(false)
tBoss.Body:visible(false)
--tBoss.LeftHand:visible(false)
--tBoss.RightHand:visible(false)

tBoss.LeftHandJoint:animPlay( tBoss.Anim.Idle,true )
tBoss.RightHandJoint:animPlay( tBoss.Anim.Idle,true )

function CreateBoss(PositionX, PositionZ, Height, LeftHandX, LeftHandZ, RightHandX, RightHandZ)

	tBoss.Body:position(PositionX, Height+ 1.5, PositionZ)
	tBoss.LeftHand:position(LeftHandX, Height+1, LeftHandZ)
	tBoss.RightHand:position(RightHandX, Height+1, RightHandZ)

	tBoss.Body:visible(true)
	tBoss.LeftHandJoint:visible(true)
	tBoss.RightHandJoint:visible(true)
	tBoss.Body:animPlay( tBoss.Anim.Fall, false )

	tBoss.Lock[1]:visible(false)
	tBoss.Lock[2]:visible(false)
	tBoss.Lock[3]:visible(false)

	tBoss.Healthpoint = {}

	tBoss.Healthpoint[1] = ExplosionSphere:clone("BOSS_HP_1")
	tBoss.Healthpoint[2] = ExplosionSphere:clone("BOSS_HP_2")
	tBoss.Healthpoint[3] = ExplosionSphere:clone("BOSS_HP_3")

	tBoss.Healthpoint[1]:parent( Joint.find( "LifePoint1" ) )
	tBoss.Healthpoint[2]:parent( Joint.find( "LifePoint2" ) )
	tBoss.Healthpoint[3]:parent( Joint.find( "LifePoint3" ) )

	tBoss.Healthpoint[1]:position(0,0,0)
	tBoss.Healthpoint[2]:position(0,0,0)
	tBoss.Healthpoint[3]:position(0,0,0)

	tBoss.Healthpoint[1]:visible(true)
	tBoss.Healthpoint[2]:visible(true)
	tBoss.Healthpoint[3]:visible(true)


end

function UpdateBoss(PositionX, PositionZ, Height, RotationY , LeftHandX, LeftHandZ, RightHandX, RightHandZ, BehaviorState, RightHandState, LeftHandState)
	tBoss.Body:position( PositionX , Height -1 , -PositionZ)
	tBoss.Body:rotation( 0, RotationY - 90, 0 )

 	tBoss.LookatPosRight:position(Transform.find("RightElbowPos"):worldPosition());
 	tBoss.LookatPosLeft :position(Transform.find("LeftElbowPos" ):worldPosition());


	if BehaviorState == SLEEP then --Sleep


		LeftPos = Lerp(tBoss.LeftHand:position(),  Vector( LeftHandX, Height+1, -LeftHandZ ), 0.8)
		RightPos = Lerp(tBoss.RightHand:position(),  Vector( RightHandX, Height+1, -RightHandZ ), 0.8)
		if( tBoss.State ~= BehaviorState) then
			tBoss.LeftHandJoint:animPlay( tBoss.Anim.Idle,true )
			tBoss.RightHandJoint:animPlay( tBoss.Anim.Idle,true )
		end



	elseif BehaviorState == AIMING then --Aiming



		LeftPos = Lerp(tBoss.LeftHand:position(),  Vector( LeftHandX, Height+1, -LeftHandZ ), 0.8)
		RightPos = Lerp(tBoss.RightHand:position(),  Vector( RightHandX, Height+1, -RightHandZ ), 0.8)
		if( tBoss.State ~= BehaviorState) then
			tBoss.LeftHandJoint:animPlay( tBoss.Anim.Idle,true )
			tBoss.RightHandJoint:animPlay( tBoss.Anim.Idle,true )
				tBoss.Body:animPlay( tBoss.Anim.Awaken, true )
		end

	elseif BehaviorState == ATTACKING then--Attacking
		--if( tBoss.State ~= BehaviorState) then
		--	tBoss.LeftHandJoint:animPlay( tBoss.Anim.Fist )
		--	tBoss.RightHandJoint:animPlay( tBoss.Anim.Fist )
		--end

		nNewLeftHeight = Height+ 1;
		nNewRightHeight = Height+ 1;

		if RightHandState == 0 then
			tBoss.RightHandJoint:animPlay( tBoss.Anim.Fist );
		else
			tBoss.RightHandJoint:animPlay( tBoss.Anim.Aim )
			nNewRightHeight = Lerp(	tBoss.RightHand:position().y, tPlayer:position().y+1, 0.5 )

		end
		if LeftHandState == 0 then
			tBoss.LeftHandJoint:animPlay( tBoss.Anim.Fist );

		else
			tBoss.LeftHandJoint:animPlay( tBoss.Anim.Aim )
			nNewLeftHeight = Lerp(	tBoss.LeftHand:position().y, tPlayer:position().y+1, 0.5 )

		end


		LeftPos  = Lerp(tBoss.LeftHand:position(),Vector(LeftHandX, nNewLeftHeight , -LeftHandZ), 0.9)
		RightPos = Lerp(tBoss.RightHand:position(),Vector(RightHandX, nNewRightHeight, -RightHandZ), 0.9)

	elseif BehaviorState == CHARGINGSLAM then--Charging Slam
		if( tBoss.State ~= BehaviorState) then
			tBoss.LeftHandJoint:animPlay( tBoss.Anim.Aim )
			tBoss.RightHandJoint:animPlay( tBoss.Anim.Aim )
		end
		nNewHeight = Lerp(	tBoss.LeftHand:position().y, Height+ 3, 0.5 )

		LeftPos  = Lerp(tBoss.LeftHand:position(),Vector(LeftHandX, nNewHeight , -LeftHandZ), 0.8)   --Vector(LeftHandX, nNewHeight , -LeftHandZ)
		RightPos = Lerp(tBoss.RightHand:position(),Vector(RightHandX, nNewHeight, -RightHandZ), 0.8)   --Vector(RightHandX, nNewHeight, -RightHandZ)

	elseif BehaviorState == COOLDOWN then--Cooldown
			if( tBoss.State ~= BehaviorState) then
				tBoss.Body:animPlay( tBoss.Anim.Fall  )

			end

			LeftPos  = Lerp(tBoss.LeftHand:position(),Vector(LeftHandX, Height+ 0.5 , -LeftHandZ), 0.8)
			RightPos = Lerp(tBoss.RightHand:position(),Vector(RightHandX, Height+ 0.5, -RightHandZ), 0.8)


	elseif BehaviorState == HANDMINISLAM then -- Hand Mini Slam



		if RightHandState == 3 then
			RightPos = Lerp(tBoss.RightHand:position(),  Vector( RightHandX, tPlayer:position().y+3, -RightHandZ ), 0.8)
			tBoss.RightHandJoint:animPlay( tBoss.Anim.Fist );
		else
			RightPos = Lerp(tBoss.RightHand:position(),  Vector( RightHandX, tPlayer:position().y+0.5, -RightHandZ ), 0.8)
			tBoss.RightHandJoint:animPlay( tBoss.Anim.Slam )
		end

		if LeftHandState == 3 then
			LeftPos = Lerp(tBoss.LeftHand:position(),  Vector( LeftHandX, tPlayer:position().y+3, -LeftHandZ ), 0.8)
			tBoss.LeftHandJoint:animPlay( tBoss.Anim.Fist );
		else
			LeftPos = Lerp(tBoss.LeftHand:position(),  Vector( LeftHandX, tPlayer:position().y+0.5, -LeftHandZ ), 0.8)
			tBoss.LeftHandJoint:animPlay( tBoss.Anim.Slam )
		end

	elseif BehaviorState ==	AWAKING then

		if( tBoss.State ~= BehaviorState) then
			tBoss.Body:animPlay( tBoss.Anim.Awaken, true )
		end

	else
		tBoss.LeftHandJoint:animPlay( tBoss.Anim.Aim )
		tBoss.RightHandJoint:animPlay( tBoss.Anim.Aim )
		tBoss.Body:animPlay( tBoss.Anim.Idle  )

		LeftPos = Lerp(tBoss.LeftHand:position(),Vector(LeftHandX, Height+ math.random(0.5,1.5) , -LeftHandZ), 0.8)  --Vector(LeftHandX, Height+ math.random(0.5,1.5) , -LeftHandZ)
		RightPos =Lerp(tBoss.RightHand:position(),Vector(RightHandX, Height+ math.random(0.5,1.5), -RightHandZ), 0.8)  -- Vector(RightHandX, Height+math.random(0.5,1.5), -RightHandZ)
	end




	tBoss.RightHand:position(	(LeftPos))
	tBoss.LeftHand:position(	(RightPos))
	tBoss.LeftHandIK :position( LeftPos )
	tBoss.RightHandIK:position( RightPos )

	tBoss.LeftHand:lookat( tBoss.LookatPosLeft :position() ) --Vector ( PositionX, Height + 1.5, PositionZ))--
	tBoss.RightHand:lookat(tBoss.LookatPosRight:position() ) --Vector ( PositionX, Height + 1.5, PositionZ))--

	--tBoss.LeftHand:rotate(0,180,0)
	--tBoss.RightHand:rotate(0,180,0)

	tBoss.State = BehaviorState


	--tBoss.LeftHandDebug :position( tBoss.LookatPosLeft :position())
	--tBoss.RightHandDebug:position( tBoss.LookatPosRight:position() )

	--tBoss.RightHandAim

	--ikDebug1:position( tBoss.LeftHandAim :worldPosition())
	--ikDebug2:position( tBoss.RightHandAim:worldPosition())
	--tBoss.LeftHand :rotate( 90, 90, 0)
	--tBoss.RightHand:rotate( 90, 90, 0)
	--print( tBoss.LeftHand :position());
	--print( tBoss.RightHand:position());
end

function OpenBoss( )
	tBoss.Body:animPlay( tBoss.Anim.Open  )

	tBoss.Lock[1]:visible(false)
	tBoss.Lock[2]:visible(false)
	tBoss.Lock[3]:visible(false)

end

function ShowLock( LockID )
	tBoss.Lock[ LockID ]:visible(true)
	print ("ShowLock " .. LockID)

end

function SetHP( Healthpoints )
	if( Healthpoints < 3 ) then
		tBoss.Healthpoint[1]:visible(false)
	end
	if( Healthpoints < 2 ) then
		tBoss.Healthpoint[2]:visible(false)
	end
	if( Healthpoints < 1 ) then
		tBoss.Healthpoint[3]:visible(false)
	end

end

function DestroyBoss( )
	tBoss.Body:visible(false)
	tBoss.LeftHandJoint:visible(false)
	tBoss.RightHandJoint:visible(false)
	--tBoss.LeftHand:visible(false)
	--tBoss.RightHand:visible(false)
end
