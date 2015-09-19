#pragma once

class WindowElement :public MenuElement
{
public:
	virtual ~WindowElement() =default;
	virtual void Draw(RenderInterface& Renderer) override;
	virtual void OnMouseDown(const MouseMessage& Msg) override;
	virtual void OnMouseUp(const MouseMessage& Msg) override;
	virtual void OnMouseMove(const MouseMessage& Msg) override;

	uint32_t AddSubElement(MenuElement* Element);

	struct Context
	{
		//Required
		std::string m_WindowName;
		Color m_FillColor;
		Color m_TitleFillColor;
		Vector2f m_Position;
		Vector2f m_Size;

		//Not Required to be set
		float m_TitleBarHeight;
		float m_BorderWidth;
		Color m_TextColor;
		Context()
		{
			m_TitleBarHeight = 25;
			m_BorderWidth = 5;
			m_TextColor = Color(0, 0, 0);
		}
	};

	WindowElement(const Context& Ctx);
private:
	bool PointInRibbon(const Vector2f& Point);
	bool PointInClient(const Vector2f& Point);

	Context m_Ctx;
	bool m_IsMouseDown;
	Vector2f m_DragOffsetFromPosition;
	std::vector<MenuElement*> m_SubElements;
};

WindowElement::WindowElement(const WindowElement::Context& Ctx):
	MenuElement(Ctx.m_Position,Ctx.m_Size)
{
	m_Ctx = Ctx;
	m_IsMouseDown = false;
}

void WindowElement::Draw(RenderInterface& Renderer)
{
	Renderer.DrawFilledBox(m_Position, m_Size, m_Ctx.m_TitleFillColor);

	Vector2f InnerPanelPosition = Vector2f(m_Position.x + m_Ctx.m_BorderWidth, m_Position.y + m_Ctx.m_TitleBarHeight);
	Vector2f InnerPanelSize = Vector2f(m_Size.x - (m_Ctx.m_BorderWidth * 2), m_Size.y - m_Ctx.m_TitleBarHeight - m_Ctx.m_BorderWidth);
	Renderer.DrawFilledBox(InnerPanelPosition,InnerPanelSize, m_Ctx.m_FillColor);
	Renderer.DrawLineBox(m_Position, m_Size, Color(0, 0, 0));
	Renderer.RenderText(m_Position, m_Ctx.m_TextColor, "%s", m_Ctx.m_WindowName.c_str());
	for (MenuElement* Element : m_SubElements)
	{
		Element->Draw(Renderer);
	}
}

void WindowElement::OnMouseDown(const MouseMessage& Msg)
{
	if (Msg.GetButton() == MouseMessage::MouseButton::Left && PointInRibbon(Msg.GetLocation()))
	{
		m_IsMouseDown = true;
		m_DragOffsetFromPosition = m_Position-Msg.GetLocation();
	}
	
	m_eMouseDown.Invoke(Msg);

	if (!PointInClient(Msg.GetLocation()))
		return;

	for (MenuElement* Element : m_SubElements)
	{
		if (Element->IsPointInMouseDownZone(Msg.GetLocation()))
			Element->OnMouseDown(Msg);
	}
}

void WindowElement::OnMouseUp(const MouseMessage& Msg)
{
	if(Msg.GetButton() == MouseMessage::MouseButton::Left)
		m_IsMouseDown = false;
	m_eMouseUp.Invoke(Msg);
	for (MenuElement* Element : m_SubElements)
	{
		Element->OnMouseUp(Msg);
	}
}

void WindowElement::OnMouseMove(const MouseMessage& Msg)
{
	if (m_IsMouseDown)
	{
		Vector2f NewPosition = Msg.GetLocation() + m_DragOffsetFromPosition;
		Vector2f DeltaPosition = NewPosition - m_Position;
		m_Position = NewPosition;
		m_ePositionChanged.Invoke(m_Position);
		for (MenuElement* Element : m_SubElements)
		{
			/*DO NOT call elements PositionChanged event since the 
			relative position to form has no changed*/
			Element->AddPosition(DeltaPosition);
		}
	}
	m_eMouseMove.Invoke(Msg);

	for (MenuElement* Element : m_SubElements)
	{
		if (!Element->IsPointInControl(Msg.GetLocation()) &&
			Element->IsCursorInElement())
		{
			Element->OnMouseLeave(Msg);
		}else if (Element->IsPointInControl(Msg.GetLocation()) &&
			!Element->IsCursorInElement()) 
		{
			Element->OnMouseEnter(Msg);
		}

		if (Element->IsCursorInElement())
		{
			Element->OnMouseMove(Msg);
		}
	}
}

bool WindowElement::PointInRibbon(const Vector2f& Point)
{
	Vector2f InnerPanelPosition = Vector2f(m_Position.x + m_Ctx.m_BorderWidth, m_Position.y + m_Ctx.m_TitleBarHeight);
	if (Point.x > m_Position.x && Point.x < (m_Position.x + m_Size.x) &&
		Point.y > m_Position.y && Point.y < InnerPanelPosition.y)
		return true;
	return false;
}

bool WindowElement::PointInClient(const Vector2f& Point)
{
	Vector2f InnerPanelPosition = Vector2f(m_Position.x + m_Ctx.m_BorderWidth, m_Position.y + m_Ctx.m_TitleBarHeight);
	Vector2f InnerPanelSize = Vector2f(m_Size.x - (m_Ctx.m_BorderWidth * 2), m_Size.y - m_Ctx.m_TitleBarHeight - m_Ctx.m_BorderWidth);
	if (Point.x > InnerPanelPosition.x  && Point.x < (InnerPanelPosition.x + InnerPanelSize.x) &&
		Point.y > InnerPanelPosition.y && Point.y < (InnerPanelPosition.y + InnerPanelSize.y))
		return true;
	return false;
}

uint32_t WindowElement::AddSubElement(MenuElement* Element)
{
	for (MenuElement* Element2 : m_SubElements)
	{
		if (Element2->GetId() == Element->GetId())
			return Element2->GetId();
	}
	//Make sub element relative to window
	Element->AddPosition(m_Position); 
	m_SubElements.push_back(Element);
	return Element->GetId();
}