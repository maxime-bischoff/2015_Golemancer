
--MenuCursor = Mesh.createQuad({bitmap = "../Assets/Hud/ThickCircle.tga",scale = Vector(0.9,0.9,0.9)} ) ;--Hud BackGround
merge("Data/Hud/Menu.pak")
merge("Data/Fonts/agency.pak")
merge("Data/Menus/MainMenu.pak")
merge("Data/Menus/PauseGO_Menu.pak")

fontGabriola = Font.find("agency")
planeTitle =  Mesh.find ( "PlaneMenu")
planeTitle:shaderPriority( 500 );

planePause = Mesh.find ( "MenuPause")
planePause:shaderPriority( 500 );
planePause:visible ( false )
planeGameOver = Mesh.find ( "MenuGameOver")
planeGameOver:shaderPriority( 500 );
planeGameOver:visible ( false )

Menu = Transform.new()
MenuTitles = {}

menus = { }

creditsScreen =  Mesh.find("CreditsScreen")
creditsScreen:visible(false)

mainMenu =  Transform.find("MainMenu")
mainMenu:visible(false)

VictoryScreen = Mesh.find("CreditsScreen")
creditsScreen:visible(false)

local selectedMenuStyle =
{
  font = "agency",             -- use string to define the font        -- scale vector
  color = "white",              -- use string to define the color (run the cmd Mesh.createText() to retrieve the list of predifined font color)
  alignx="center",              -- use string to define alignement id 0
  charStyle = "normal",       -- use string to define style id 1
  charOffset=Vector(.006,-.006) -- shadow offset x/y
}


 local unselectedMenuStyle =
{
  font = "agency",       -- use string to define the font         -- scale vector
  color = "white",            -- use string to define the color (run the cmd Mesh.createText() to retrieve the list of predifined font color)
  alignx="center",            -- use string to define alignement id 0
  charStyle = "normal",         -- use string to define style id 1
  charOffset=Vector(.006,-.006)     -- shadow offset x/y
}

buttonUnselectedColor = Vector(0.5,0.5,0.5,1)
buttonSelectedColor = Vector(1,1,1,1)


function ShowMainMenu(bShow)
  mainMenu:visible(bShow)
end


function InitMainMenu()

  local buttonStartTransform   = Transform.find("button_start")
  local buttonCreditsTransform = Transform.find("button_credits")

  idMainMenu = GetIdMenu("MainMenu")
print("IDMENU  = "..idMainMenu)
  if( idMainMenu == -1) then return end

  menus[idMainMenu].buttons[1].textMesh:parent(buttonStartTransform)
  menus[idMainMenu].buttons[1].textMesh:position(0,0,0)
  menus[idMainMenu].buttons[2].textMesh:parent(buttonCreditsTransform)
  menus[idMainMenu].buttons[2].textMesh:position(0,0,0)

end









-- OLD BUTTON
function AddButton( name, ID, isSelected, canBeBuy, count )
  ID = ID + 1 ;
  if isSelected then
  if (canBeBuy) then
    MenuTitles[ID] = Mesh.createText{ pos= Vector(0,0.75- ID/count ,0) , str= name}
    MenuTitles[ID]:shader():matEmissive(Vector(1,0,0,1))
  else
    MenuTitles[ID] = Mesh.createText{ pos= Vector(0,0.75- ID/count ,0) , str= name}
    MenuTitles[ID]:shader():matEmissive(Vector(0.9,0.3,0.3,1))
  end
  else
   if (canBeBuy) then
    MenuTitles[ID] = Mesh.createText{ pos= Vector(0,0.75- ID/count ,0) , str= name}
    MenuTitles[ID]:shader():matEmissive(Vector(1,1,1,1))
  else
    MenuTitles[ID] = Mesh.createText{ pos= Vector(0,0.75- ID/count ,0) , str= name}
    MenuTitles[ID]:shader():matEmissive(Vector(0.5,0.5,0.5,1))
  end
  end
end






function SelectButton( menuName, idButton, bSelect)

  idButton = idButton + 1

  idMenu =  GetIdMenu(menuName)

  if idMenu ~= -1 and idButton <= #(menus[idMenu].buttons) then

    if( bSelect ) then
      menus[idMenu].buttons[idButton].textMesh:shader():matEmissive(buttonSelectedColor )
      menus[idMenu].buttons[idButton].textMesh:scale(Vector(2,2,1))
    else
      menus[idMenu].buttons[idButton].textMesh:shader():matEmissive(  buttonUnselectedColor )
      menus[idMenu].buttons[idButton].textMesh:scale(Vector(1.7,1.7,1))
    end
  end
end



function LoadTitleScreen()

  planeTitle:position(0,2,1)


  ShowHUD(false)

end




function GetIdMenu(menuName)

 for index = 1, #menus  do

    if menus[index].name == menuName then

      return index

    end

  end

  return -1

end


function CreateButton(menuName, buttonText, id)

  bFound = false
  index = GetIdMenu(menuName)


  if index == -1 then

    menus[ #menus + 1 ] = {}
    menus[ #menus ].name = menuName
    menus[ #menus ].buttons = {}


  end
  if  menuName == "MainMenu" then
    menus[ #menus ].buttons[id] = {}
    menus[ #menus ].buttons[id].text = buttonText
    menus[ #menus ].buttons[id].textMesh = Mesh.createText{ str = buttonText,
                                                                  style = unselectedMenuStyle,
                                                                  pos = Vector(0, 0.5 - id /5 + 0.05, 0) ,
                                                                  overlay =true ,
                                                                  alignx = 1}
  elseif menuName == "PauseMenu" then
    menus[ #menus ].buttons[id] = {}
    menus[ #menus ].buttons[id].text = buttonText
    menus[ #menus ].buttons[id].textMesh = Mesh.createText{ str = buttonText,
                                                                  style = unselectedMenuStyle,
                                                                  pos = Vector(0, 0.5 - id/7 - 0.13, 0) ,
                                                                  overlay =true ,
                                                                  alignx = "center"}
  elseif menuName == "GameOverMenu" then
    menus[ #menus ].buttons[id] = {}
    menus[ #menus ].buttons[id].text = buttonText
    menus[ #menus ].buttons[id].textMesh = Mesh.createText{ str = buttonText,
                                                                  style = unselectedMenuStyle,
                                                                  pos = Vector(0, 0.32 - id /5  , 0) ,
                                                                  overlay =true ,
                                                                  alignx = "center"}



  end


end


function ShowMenu(menuName, show)

idMenu = GetIdMenu(menuName)

  if idMenu ~= -1 then
    for idButton = 1, #(menus[idMenu].buttons) do
      menus[idMenu].buttons[idButton].textMesh:visible(show)
    end
      if menuName == "MainMenu" then
        planeTitle:visible( show )
       cam:position ( CameraPositionStart)
        cam:rotation ( 0,180,0)
        elseif menuName == "PauseMenu" then 
          planePause:visible ( show )
          tScreenCenter:visible ( not show )
          elseif menuName == "GameOverMenu" then
            planeGameOver:visible ( show)
            tScreenCenter:visible ( not show )
     end
  end
end

function ShowCredits ( show )
  creditsScreen:visible( show )
end

function ShowVitoryScreen( show )

  VictoryScreen:visible(show)
  print("victory")

end
