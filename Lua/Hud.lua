

merge("Data/Hud/SimplePlane.pak")
PrimitivePlane = Mesh.find("plane")
PrimitivePlane:visible(false)
merge("Data/Hud/Hud.pak")
merge("Data/Hud/Hud_Shader.pak")


fadePlane =  Mesh.find("Fade_Plane")
fadePlane:layerOrder(1000)

damagesPlane = Mesh.find("Hud_Damages")
damagesPlane:shader():setParam("opacity",0)

Shader_Hud_no_mana = ShaderFx.find("Shader_No_Mana")
Shader_Hud_Curve = ShaderFx.find("Shader_Curve")
Shader_Hud_Freeze = ShaderFx.find("Shader_Freeze")
Shader_Hud_Shield = ShaderFx.find("Shader_Shield")
Shader_Fade  = ShaderFx.find("Shader_Fade")
Shader_Cursor_HUD  = ShaderFx.find("Shader_Cursot")
Shader_Circle_HUD  = ShaderFx.find("Shader_Circle")
Shader_TrickCircle_HUD  = ShaderFx.find("Shader_ThickCircle")
Shader_Beam_HUD = ShaderFx.find("Shader_Beam")
Shader_R2_HUD = ShaderFx.find ( "Shader_R2")
Shader_Show =  ShaderFx.find ( "Shader_Show")

mHudBar = Mesh.find("HudBar")
mHudBar:scale(0.25,0.25,0.25)
mHudBar:position(0,.9,0)
mHudBar:shader ( Shader_Hud_no_mana)



creditsPlane = Mesh.find( "Credits_Plane")
creditsPlane:shaderPriority( 2000 );
creditsPlane:visible(false);

-- // SHADER // --

mHudWatch =  Mesh.find("WatchBar")
--mHudWatch:scale(0.25,0.25,0.25)
--mHudWatch:position(0,.9,0)
mHudWatch:layerOrder( 1000)
mHudWatch:visible( false ) ;
mHudWatch:shadingGroup(-1)
mHudWatch:parent (mHudBar )
mHudWatch:shader ( Shader_Show )

mHealth = Mesh.find("HealthBar")
mHealth:parent(mHudBar )
mHealth:shader():matDiffuse( 1,0,0)

mEnergy = Mesh.find("EnergyBar")
mEnergy:shader():matDiffuse( 0,1,1)

mBackground = Mesh.find("BackGround")
mBackground:shader():matDiffuse( 0.5,0.5,0.5)



mHealth:scale(0.5,1,1)

mEnergy:scale(0.05,0.25,1)

function CreatePlane( Position, Size, texturename, Color)
	newPlane = PrimitivePlane:clone(  )

	newPlane:position(Position)
	newPlane:scale(Size)
	local TexLoaded = Texture.new()
	TexLoaded:loadFile(texturename)
	newPlane:shaderPriority( 2 );
	newPlane:visible(false)

	return newPlane
end

-- // :: SHADER STICK :: // --

Soft_Right = ShaderFx.find ( "Soft_Right");
Soft_Left = ShaderFx.find ( "Soft_Left");
Shader_Charge = ShaderFx.find ( "Shader_Charge");
Shader_Arc_Of_Bolt_Right = ShaderFx.find ( "Arc_Of_Bolt_Right");
Shader_Arc_Of_Bolt_Left = ShaderFx.find ( "Arc_Of_Bolt_Left");
Shader_Curve_Right = ShaderFx.find ( "Curve_Right");
Shader_Curve_Left = ShaderFx.find ( "Curve_Left");
Shader_Simple_Bolt = ShaderFx.find ( "Simple_Bolt");
Shader_Activate_Orb = ShaderFx.find ( "Activate_Orb");

-- // :: SHADER :: // --


tSpellButton = Mesh.find ("Rune_Button" );
tSpellButton:visible(false );
tSpellButton:shader( Shader_R2_HUD )

tStickPlane = Mesh.find ( "Stick_Button");
tStickPlane:visible ( false ) ;
tStickPlane:shader( Shader_Arc_Of_Bolt_Left );


fTimerVisible = 0.5;
fCurrentTimer = 0;
fCurrentTimerHud = 0;
bOrbeVisible = false ;

tScreenCenter =  Mesh.createQuad({ scale = Vector(0.05,0.05,0.05)} ) ;
tScreenCenter:shader ( Shader_Cursor_HUD:clone())

-- // :: Draw all Hud :: // --

-- // :: Hud Wheel :: // --

tHud = CreatePlane( tFocusTransform:position(), Vector(0.9,0.9,0.9), Vector(1,1,1,1) )
tHud:shader(Shader_TrickCircle_HUD:clone(0))
tHud:shadingGroup(-1)
tHud:shaderPriority( 100 );
tHud:scale( 0.35,0.35,0.35)
tHud:visible ( false )

tHudOverlay = tHud:clone();
tHudOverlay:scale( 0.5,0.5,0.5)
tHudOverlay:overlay(true);

tHudCursor = CreatePlane( tFocusTransform:position(), Vector(0.1,0.1,0.1), Vector(0,1,1,1) )
tHudCursor:shader ( Shader_Cursor_HUD:clone(0))
tHudCursor:shaderPriority(400)
tHudCursor:parent(tHud)
tHudCursor:shadingGroup(-1)
tHudCursor:visible ( false )

tHudCursorOverlay = tHudCursor:clone();
tHudCursorOverlay:overlay(true);
tHudCursorOverlay:parent (tHudOverlay )
tHudCursorOverlay:visible ( true )

tLineWheel = {} ;
tLineWheelOverlay = {};
tHudWheel = {} ;
tHudWheelOverlay= {};

for i = 1 , 6 do
	tHudWheel[i] = CreatePlane( tFocusTransform:position(), Vector(0.1,0.1,0.1), Vector(1,1,1,1) )
	tHudWheel[i]:shader(Shader_Circle_HUD:clone(0) )
	tHudWheel[i]:shadingGroup(-1)
	tHudWheel[i]:visible(false )
	tHudWheelOverlay[i] = tHudWheel[i]:clone();
	tHudWheelOverlay[i]:overlay( true );
end


tBarreFocus = Mesh.createQuad({ scale = Vector(0.1,0.1,0.1)} ) ;
tBarreFocus:overlay( true ) ;
ChargeMax = 3 ;
PosXStartCharge = 0 ;


--//---- Missions Text ---- // --

tMissionText = Mesh.createText{pos= Vector(-1.2,0.9,0), str="Mission text not defined yet", scale=Vector(1,1,1), font = fontGabriola}

function PlayAnimCombo ( count )

end

function SetOrbeButtonVisibility ( bool_value  )
	tSpellButton:visible( bool_value );
	tStickPlane:visible( bool_value );
end

function ClignoteOrbeButton (  )

	tStickPlane:visible( false );
	tcurrent_button = tSpellButton;


if fCurrentTimer < fTimerVisible then
	fCurrentTimer = fCurrentTimer + Clock.deltaTime();

else
	fCurrentTimer = 0;
	if bOrbeVisible == true then
		tcurrent_button:visible(false );
		bOrbeVisible = false ;
	else
		tcurrent_button:visible(true );
		bOrbeVisible = true ;
	end
end

end

function DrawPointWheel ( PosX, PosY, index  )

	tHudWheel[index + 1 ]:position(  PosX -0.8  ,   PosY+0.4,  0.02  );
	tHudWheel[index + 1 ]:parent( tHud )
	tHudWheel[index + 1 ]:shaderPriority( 600 );
	tHudWheelOverlay[index + 1 ]:position(  PosX -0.8  ,   PosY+0.4,  0.02  );
	tHudWheelOverlay[index + 1]:parent ( tHudOverlay);
end

function DrawRuneWheel(  PosCenterX, PosCenterY, PosCursorX, PosCursorY,  bVisible )
		--tHud:visible ( bVisible )
	--	tHud:rotation( cam:rotation() )
		--tHud:position( Vector(PosCenterX,tPlayer:position().y +0.65, -PosCenterY ))
tHud:visible ( false )
		tHudOverlay:visible( bVisible)
		tHudOverlay:position( Vector( 1,-0.70,0.90))

	--	tHudCursor:position ( PosCursorX  , PosCursorY ,  0.01)
		tHudCursorOverlay:position ( PosCursorX  , PosCursorY ,  0.01 );
	for i = 1 , 6 do
		--tHudWheel[i]:visible ( bVisible )
		tHudWheelOverlay[i]:visible ( bVisible )
	end
end

function InitLineWheel ( Scale, Angle, Index )
	tLineWheel[Index +1] = CreatePlane( tPlayer:position(), Vector(0.05,Scale/1.25,0.05), Vector(0,1,1,1) )
	tLineWheel[Index +1]:shader ( Shader_Beam_HUD:clone() )
	tLineWheel[Index +1 ]:rotation( 0,  0 , math.deg( Angle + 3.14/2) )
	tLineWheel[Index +1 ]:position().z = 0.015
	tLineWheel[Index +1 ]:parent( tHud  )
	tLineWheel[Index +1 ]:shaderPriority( 500 );

	tLineWheelOverlay[Index + 1] = tLineWheel[Index +1 ]:clone();
	tLineWheelOverlay[Index + 1]:parent( tHudOverlay );
	tLineWheelOverlay[Index + 1]:overlay(true);
end

function DrawLineWheel ( PosLineX, PosLineY,  Index )
	tLineWheel[Index +1 ]:position(   PosLineX / 2.5 , PosLineY / 2.5 , 0.015) ;
	tLineWheelOverlay[Index +1 ]:position(   PosLineX / 2.5 , PosLineY / 2.5 , 0.015) ;
end

function ClearRune ( iIndex )
	tLineWheel[iIndex +1 ]:position(-20,-20,0) ;
	tLineWheelOverlay[iIndex +1 ]:position( -20,-20,0) ;
end

function DrawLife ( LifeScale, EnergyScale )
	mHealth:scale( LifeScale, 1 , 1 )
	mEnergy:scale( EnergyScale, 1, 1 )
end

function PlayDamageHud( shield )
	if shield then
	damagesPlane:shader():setParam("matColor", Vector(0,1,1))

	damagesPlane:animPlay("Play_Hud_Damages")
	else
	damagesPlane:shader():setParam("matColor", Vector(1,0,0))

	damagesPlane:animPlay("Play_Hud_Damages")
	end

end

function DrawMana ( Count, bHalf )
	CurrentMana = CountStartMana   - Count ;
	for i = CurrentMana , CountStartMana do
		tMana[i]:shader().technoColor.r = 0.10;
		tMana[i]:shader().technoColor.g = 1;
		tMana[i]:shader().technoColor.b = 0.99;
		tMana[i]:position ( i * -0.1 , -0.8);
	end
	for i = 1 , CurrentMana   do
		tMana[i]:shader().technoColor.r = 0;
	tMana[i]:shader().technoColor.g = 0;
	tMana[i]:shader().technoColor.b = 0;

	end

end

function DrawCharge ( Scale )
	if  Scale < ChargeMax then
	tBarreFocus:scale(Vector ( Scale / ChargeMax  , 0.07, 1  )) ;
	tBarreFocus:position( 0.50- ( ChargeMax - Scale ) * 0.05 , -0.8);
	end

end

function SetMissionText( MissionText )

	tMissionText:text( MissionText )

end

function SetFadeOpacity( opacity )
	if ( opacity  ~= 0 ) then
		fadePlane:visible(true)
		idMenu = GetIdMenu("PauseMenu")
    		for idButton = 1, #(menus[idMenu].buttons) do
     			menus[idMenu].buttons[idButton].textMesh:visible(false)
  			end
  		idMenu = GetIdMenu("GameOverMenu")
  		for idButton = 1, #(menus[idMenu].buttons) do
     			menus[idMenu].buttons[idButton].textMesh:visible(false)
  			end
		planePause:visible ( false )
    	planeGameOver:visible ( false )
		else
		fadePlane:visible(false)
	end
	fadePlane:shader():setParam("opacity", opacity)

end


function ShowHUD(show)

  mHudBar:visible(show)
  mHudWatch:visible ( false )
  mHealth:visible(show)
 -- mHudBG:visible(show)
 tHudCursorOverlay:visible ( false )
  tMissionText:visible(show)
  if textTuto ~= nil then
  	textTuto:visible(show)
  end
  tHud:visible ( show )
  tScreenCenter:visible ( show )
  tBarreFocus:visible(show)
  for i = 1 , 6 do
 	 tHudWheel[i]:visible ( show )
  end
end

HudLastState = 0;
function ShowHud( show )
	mHudWatch:visible ( show )
end

function ClignoteHud ()
	if fCurrentTimerHud < fTimerVisible then
		fCurrentTimerHud = fCurrentTimerHud + Clock.deltaTime();
	else
		fCurrentTimerHud = 0;
		--if mHudWatch:visible() == true then
	--		mHudWatch:visible(false);
	--	else
	--		mHudWatch:visible(true );
	--	end
	end
end

function HudUpdate ( state )
if HudLastState ~= state then
		if state == 0 then
		mHudBar:shader(Shader_Hud_no_mana)
		elseif state == 1 then
		mHudBar:shader(Shader_Hud_Curve)
		elseif state == 2 then
		mHudBar:shader(Shader_Hud_Freeze)
		elseif state == 3 then
		mHudBar:shader(Shader_Hud_Shield)
		end
		HudLastState = state
	end
end
