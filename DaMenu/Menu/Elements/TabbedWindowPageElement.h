#pragma once

class TabbedWindowPageElement : public WindowElement
{
public:
	friend TabbedWindowElement;
	TabbedWindowPageElement(const Context& Ctx);
	virtual ~TabbedWindowPageElement() = default;
	virtual void Draw(RenderInterface& Renderer) override;
	virtual void OnMouseMove(const MouseMessage& Msg) override;
protected:
	virtual bool PointInClient(const Vector2f& Point) override;
	virtual bool PointInRibbon(const Vector2f& Point) override;

};

TabbedWindowPageElement::TabbedWindowPageElement(const Context& Ctx):
	WindowElement(Ctx)
{
	//Don't need a close button
	delete m_CloseButton;
	m_SubElements.clear();
}

void TabbedWindowPageElement::Draw(RenderInterface& Renderer)
{
	Renderer.DrawFilledBox(m_Position,m_Size, m_Ctx.m_FillColor);
	Renderer.DrawLineBox(m_Position, m_Size, Color::Black());
	for (MenuElement* Element : m_SubElements)
	{
		Element->Draw(Renderer);
	}
}

void TabbedWindowPageElement::OnMouseMove(const MouseMessage& Msg)
{
	MenuElement::OnMouseMove(Msg);
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

bool TabbedWindowPageElement::PointInClient(const Vector2f& Point)
{
	return MenuElement::IsPointInControl(Point);
}

bool TabbedWindowPageElement::PointInRibbon(const Vector2f& Point)
{
	//No ribbon, so always false
	return false;
}


