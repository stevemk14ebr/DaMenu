#pragma once
#include "Menu/Misc/Vector.h"
using Vector2f = Vector2<float>;
#include "Menu/Misc/IDGenerator.h"

#include "Menu/Input/MouseEvents.h"
#include "Menu/Input/KeyboardMessage.h"
#include "Menu/Input/EventDispatcher.h"
#include "Menu/Input/InputManageInterface.h"
#include "Menu/Input/WinAPIInputManager.h"

#include "Menu/Rendering/RenderInterface.h"
#include "Menu/Rendering/DXTKRenderer.h"

#include <string.h>
#include "Menu/Elements/MenuElement.h"
#include "Menu/Elements/ButtonElement.h"
#include "Menu/Elements/WindowElement.h"
#include "Menu/Elements/SliderElement.h"
#include "Menu/Elements/CheckBoxElement.h"

#include "Menu/ElementManager.h"