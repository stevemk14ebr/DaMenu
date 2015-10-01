#pragma once

class ComboBoxElement :public MenuElement
{
public:
	virtual ~ComboBoxElement() = default;
	virtual void Draw(RenderInterface& Renderer) override;
	virtual ElementType GetType() override;

	struct Context
	{
		Vector2f m_Position;
		Vector2f m_Size;
		Color m_FillColor;
		Color m_FillColorMouseOver;
		Color m_FillColorMouseDown;

		//Not Required
		Color m_TextColor;
		Context()
		{
			m_TextColor = Color::Black();
		}
	};
	ComboBoxElement(const Context& Ctx);
	uint32_t AddComboButton(const std::string& Text);
protected:
	virtual void AddPosition(const Vector2f& NewAmount) override;
	virtual void SetPosition(const Vector2f& NewPos) override;
private:
	std::vector<ButtonElement*> m_ComboButtons;
	Context m_Ctx;
	uint32_t m_SelectedButton;
};

ComboBoxElement::ComboBoxElement(const Context& Ctx) :
	MenuElement(Ctx.m_Position, Ctx.m_Size)
{
	m_Ctx = Ctx;
	m_SelectedButton = 0;
}

void ComboBoxElement::Draw(RenderInterface& Renderer)
{
	for (MenuElement* Buttons : m_ComboButtons)
	{
		Buttons->Draw(Renderer);
	}
}

uint32_t ComboBoxElement::AddComboButton(const std::string& Text)
{
	ButtonElement::Context Ctx;
	Ctx.m_ButtonText = Text;
	Ctx.m_FillColor = m_Ctx.m_FillColor;
	Ctx.m_FillColorMouseDown = m_Ctx.m_FillColorMouseDown;
	Ctx.m_FillColorMouseOver = m_Ctx.m_FillColorMouseOver;
	Ctx.m_TextColor = m_Ctx.m_TextColor;
	Ctx.m_Position = Vector2f(m_Position.x, m_Position.y + m_Size.y + m_Size.y*m_ComboButtons.size());
	Ctx.m_Size = m_Ctx.m_Size;
	ButtonElement* Button = new ButtonElement(Ctx);
	m_ComboButtons.push_back(Button);
	return Button->GetId();
}

void ComboBoxElement::AddPosition(const Vector2f& NewAmount)
{
	MenuElement::AddPosition(NewAmount);
	for (MenuElement* Button : m_ComboButtons)
	{
		Button->AddPosition(NewAmount);
	}
}

void ComboBoxElement::SetPosition(const Vector2f& NewPos)
{
	Vector2f DeltaPos = m_Position - NewPos;
	MenuElement::SetPosition(NewPos);
	for (MenuElement* Button : m_ComboButtons)
	{
		Button->AddPosition(DeltaPos);
	}
}

ElementType ComboBoxElement::GetType()
{
	return ElementType::ComboBox;
}