#pragma once
#include <vector>
class ElementManager
{
public:
	ElementManager(InputManagerInterface* InputManager,RenderInterface* Renderer);
	void AddElement(MenuElement* Element);
	void Render();
	void UpdateInput();
	void ProcessKeyboardMessage(const KeyboardMessage& Msg);
	void ProcessMouseMessage(const MouseMessage& Msg);
private:
	std::vector<MenuElement*> m_Elements;
	InputManagerInterface* m_InputManager;
	RenderInterface* m_Renderer;
};

ElementManager::ElementManager(InputManagerInterface* InputManager,RenderInterface* Renderer)
{
	m_InputManager = InputManager;
	m_Renderer = Renderer;
	m_Renderer->Init();
	m_InputManager->EventKeyboardMessage() += 
		std::bind(&ElementManager::ProcessKeyboardMessage, this,
			std::placeholders::_1);
	m_InputManager->EventMouseMessage() +=
		std::bind(&ElementManager::ProcessMouseMessage,this,
			std::placeholders::_1);
}

void ElementManager::AddElement(MenuElement* Element)
{
	m_Elements.push_back(Element);
}

void ElementManager::Render()
{
	m_Renderer->PreFrame();
	for (MenuElement* Element : m_Elements)
	{
		Element->Draw(*m_Renderer);
	}
	m_Renderer->Present();
}

void ElementManager::ProcessMouseMessage(const MouseMessage& Msg)
{
	switch (Msg.GetEvent())
	{
	case MouseMessage::MouseEvent::Move:
		for (MenuElement* Element : m_Elements)
		{
			if(!Element->IsPointInControl(Msg.GetLocation()) &&
				Element->IsCursorInElement())
			{
				Element->OnMouseLeave(Msg);
			}else if(Element->IsPointInControl(Msg.GetLocation()) &&
				!Element->IsCursorInElement())
			{
				Element->OnMouseEnter(Msg);
			}

			if (Element->IsCursorInElement())
			{
				Element->OnMouseMove(Msg);
			}
		}
		break;
	case MouseMessage::MouseEvent::BtnDown:
		for (MenuElement* Element : m_Elements)
		{
			if(!Element->IsPointInMouseDownZone(Msg.GetLocation()))
				continue;

			Element->OnMouseDown(Msg);
		}
		break;
	case MouseMessage::MouseEvent::BtnUp:
		for (MenuElement* Element : m_Elements)
		{
			if (!Element->IsPointInMouseDownZone(Msg.GetLocation()))
				continue;

			Element->OnMouseUp(Msg);
		}
		break;
	default:
		break;
	}
}

void ElementManager::ProcessKeyboardMessage(const KeyboardMessage& Msg)
{

}

void ElementManager::UpdateInput()
{
	m_InputManager->PollKeyboard();
	m_InputManager->PollMouse();
}