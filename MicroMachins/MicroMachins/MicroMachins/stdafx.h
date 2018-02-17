// stdafx.h : joyfull list of includes

/*Bonus : Joyfull list of all controls in games
- LCtrl + Time go faster (MyTime.cpp)
- LCTrl - Time go slower (MyTime.cpp)
- LCtrl * Time go to 1.0f (normal) (MyTime.cpp)
*/

// Non-evolutive code
#pragma once

#include "targetver.h"

#include <assert.h>
#include <conio.h>
#include <stdio.h>
#include <cstdio>
#include <cstddef> // offsetof, NULL
#include <iostream>
#include <assert.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <wchar.h>
#include <stdarg.h>
#include <windows.h>
#include <atlutil.h>

#include "SFML\Config.hpp"
#include <SFML\OpenGL.hpp>
#include <SFML\System\InputStream.hpp>
#include <SFML\Graphics.hpp>

#include "SFML\Graphics\Color.hpp"
#include "SFML\System\Vector2.hpp"

#include "Box2D\Box2D.h"

// Debug tools
#include "DebugDefine.h"
#include "MenuDebug.h"
#include "ImGuiMenuDebug.h"

// Evolutive code
#include "DebugTools.h"
#include "MyMath.h"
#include "Singleton.h"

#include "DrawableManager.h"
#include "PhysicsManager.h"

#include "Entity.h"
#include "EntityManager.h"

#include "MyTime.h"

#include "Car.h"

#include "RunLoop.h"