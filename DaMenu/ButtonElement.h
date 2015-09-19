#pragma once

class ButtonElement: public MenuElement
{
public:
	virtual ~ButtonElement() = default;
	virtual void Draw(RenderInterface& Renderer) override;
	virtual void OnMouseDown(const MouseMessage& Msg) override;
	/*virtual void OnMouseUp(const MouseMessage& Msg) override;
	virtual void OnMouseEnter(const MouseMessage& Msg) override;
	virtual void OnMouseLeave(const MouseMessage& Msg) override;*/

	struct Context
	{
		//Required
		std::string m_ButtonText;
		Color m_FillColor;
		Color m_FillColorMouseOver;
		Vector2f m_Position; //Relative To window
		Vector2f m_Size;

		//Not Required
		Color m_TextColor;
		Context()
		{
			m_TextColor = Color(0, 0, 0);
		}
	};
	ButtonElement(const Context& Ctx);
private:
	Context m_Ctx;
	bool m_MouseOver;
};

ButtonElement::ButtonElement(const Context& Ctx):
	MenuElement(Ctx.m_Position,Ctx.m_Size)
{
	m_Ctx = Ctx;
	m_MouseOver = false;
}

void ButtonElement::Draw(RenderInterface& Renderer)
{
	Renderer.DrawFilledBox(m_Position, m_Size,m_MouseOver ? m_Ctx.m_FillColorMouseOver:m_Ctx.m_FillColor);
	Renderer.RenderText(m_Position, m_Ctx.m_TextColor, "%s", m_Ctx.m_ButtonText.c_str());
}

void ButtonElement::OnMouseDown(const MouseMessage& Msg)
{
	m_eMouseDown.Invoke(Msg);
}