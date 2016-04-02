#pragma once
#pragma warning( disable : 4530)
#include "flib.h"
#include "flib_vec2.h"
#include "flib_vec3.h"

#include "lua.h"
#include "LuaFunctions.h"

#include <math.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <list>
#include <string>
#include "Time.h"
#include "Functions.h"

#include "Draw.h"
#include "Particle.h"
#include "Particle_manager.h"

#include "Fade.h"
#include "Game.h"
#include "World.h"
#include "Hitbox.h"
#include "Damage.h"
#include "Transform.h"
#include "Collision.h"
#include "SpellManager.h"
#include "Player.h"
#include "Mission.h"
#include "Solid.h"

#include "Enemy.h"

#include "EnemyVegetable.h"
#include "EnemyFastRanged.h"
#include "EnemyStrongAndSlow.h"

#include "Boss.h"

#include "Wall.h"
#include "Home.h"
#include "Trigger.h"
#include "Collectible.h"
#include "Camera.h"

#include "Menu/Menu.h"
#include "Menu/Menu_main.h"
#include "Menu/Menu_popup.h"

#include "Door.h"
#include "Room.h"
#include "Map.h"
#include "MapGenerator.h"
#include "Tuto.h"

#include "Living.h"
#include "Spellbook.h"
#include "Hud.h"
#include "Focus.h"

#include "XML\TSLL_Item.h"
#include "XML\TSLL_Root.h"
#include "XML\TSLL_TXML_Itemvalue.h"
#include "XML\TXML_Attribute.h"
#include "XML\TXML_Element.h"
#include "XML\TXML_Parser.h"
#include "XML\TXML_Root.h"

#define PI static_cast<float>(3.141593)
#define  PI_2 static_cast<float>(1.570796)
#define  PI_4 static_cast<float>( 0.785398f)

#define DegToRad  (PI * 2) / 360
#define RadToDeg  360 / (PI * 2)