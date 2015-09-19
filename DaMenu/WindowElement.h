#pragma once

class WindowElement :public MenuElement
{
public:
	virtual ~WindowElement() =default;
	virtual void Draw(RenderInterface& Renderer) override;
	virtual void OnMouseDown(const MouseMessage& Msg) override;
	virtual void OnMouseUp(const MouseMessage& Msg) override;
	virtual void OnMouseMove(const MouseMessage& Msg) override;
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
	Context m_Ctx;
	bool m_IsMouseDown;
	Vector2f m_DragOffsetFromPosition;
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
}

void WindowElement::OnMouseDown(const MouseMessage& Msg)
{
	if (Msg.GetButton() == MouseMessage::MouseButton::Left)
	{
		m_IsMouseDown = true;
		m_DragOffsetFromPosition = m_Position-Msg.GetLocation();
	}
	m_eMouseDown.Invoke(Msg);
}

void WindowElement::OnMouseUp(const MouseMessage& Msg)
{
	if(Msg.GetButton() == MouseMessage::MouseButton::Left)
		m_IsMouseDown = false;
	m_eMouseUp.Invoke(Msg);
}

void WindowElement::OnMouseMove(const MouseMessage& Msg)
{
	if (m_IsMouseDown)
		m_Position = Msg.GetLocation()+m_DragOffsetFromPosition;
	m_eMouseMove.Invoke(Msg);
}