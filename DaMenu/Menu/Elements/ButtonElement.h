#pragma once

class ButtonElement: public MenuElement
{
public:
	virtual ~ButtonElement() = default;
	virtual void Draw(RenderInterface& Renderer) override;
	virtual void OnMouseDown(const MouseMessage& Msg) override;
	virtual void OnMouseUp(const MouseMessage& Msg) override;

	struct Context
	{
		//Required
		std::string m_ButtonText;
		Color m_FillColor;
		Color m_FillColorMouseOver;
		Color m_FillColorMouseDown;
		Vector2f m_Position; //Relative To window
		Vector2f m_Size;

		//Not Required
		Color m_TextColor;
		Context()
		{
			m_TextColor = Color::Black();
		}
	};
	ButtonElement(const Context& Ctx);
private:
	Context m_Ctx;
	bool m_IsMouseDown;
};

ButtonElement::ButtonElement(const Context& Ctx):
	MenuElement(Ctx.m_Position,Ctx.m_Size)
{
	m_Ctx = Ctx;
	m_IsMouseDown = false;
}

void ButtonElement::Draw(RenderInterface& Renderer)
{
	Color BtnColor = m_Ctx.m_FillColor;
	if (m_IsMouseDown)
		BtnColor = m_Ctx.m_FillColorMouseDown;
	else
		BtnColor = m_CursorInElement ? m_Ctx.m_FillColorMouseOver : m_Ctx.m_FillColor;

	Renderer.DrawFilledBox(m_Position, m_Size,BtnColor);
	Renderer.DrawLineBox(m_Position, m_Size, Color::Black());
	Renderer.RenderText(m_Position, m_Ctx.m_TextColor, "%s", m_Ctx.m_ButtonText.c_str());
}

void ButtonElement::OnMouseDown(const MouseMessage& Msg)
{
	m_IsMouseDown = true;
	m_eMouseDown.Invoke(Msg);
}

void ButtonElement::OnMouseUp(const MouseMessage& Msg)
{
	m_IsMouseDown = false;
	m_eMouseUp.Invoke(Msg);
}

