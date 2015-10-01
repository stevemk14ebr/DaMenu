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
	};
	ComboBoxElement(const Context& Ctx);
private:
	std::vector<ButtonElement*> m_ComboButtons;
	Context m_Ctx;
};

ComboBoxElement::ComboBoxElement(const Context& Ctx) :
	MenuElement(Ctx.m_Position, Ctx.m_Size)
{
	m_Ctx = Ctx;
}

void ComboBoxElement::Draw(RenderInterface& Renderer)
{

}

ElementType ComboBoxElement::GetType()
{
	return ElementType::ComboBox;
}