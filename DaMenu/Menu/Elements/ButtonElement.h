#pragma once

class ButtonElement: public MenuElement
{
public:
	friend class ComboBoxElement;
	virtual ~ButtonElement() = default;
	virtual void Draw(RenderInterface& Renderer) override;
	virtual ElementType GetType() override;
	void SetText(const std::string& Txt);

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
protected:
	virtual void OnMouseDown(const MouseMessage& Msg) override;
	virtual void OnMouseUp(const MouseMessage& Msg) override;
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
	else if (m_CursorInElement)
		BtnColor = m_Ctx.m_FillColorMouseOver;

	Renderer.BeginLine();
	Renderer.DrawFilledBox(m_Position, m_Size,BtnColor);
	Renderer.DrawLineBox(m_Position, m_Size, Color::Black());
	Renderer.EndLine();
	Vector2f BtnTxtSize=Renderer.MeasureString("%s", m_Ctx.m_ButtonText.c_str());
	Vector2f DeltaSize = m_Size - BtnTxtSize;
	DeltaSize /= 2;
	Renderer.BeginText();
	Renderer.RenderText(Vector2f(m_Position.x+DeltaSize.x,m_Position.y+DeltaSize.y), m_Ctx.m_TextColor, "%s", m_Ctx.m_ButtonText.c_str());
	Renderer.EndText();
}

void ButtonElement::OnMouseDown(const MouseMessage& Msg)
{
	MenuElement::OnMouseDown(Msg);
	m_IsMouseDown = true;
}

void ButtonElement::OnMouseUp(const MouseMessage& Msg)
{
	MenuElement::OnMouseUp(Msg);
	m_IsMouseDown = false;
}

void ButtonElement::SetText(const std::string& Txt)
{
	m_Ctx.m_ButtonText = Txt;
}

ElementType ButtonElement::GetType()
{
	return ElementType::Button;
}