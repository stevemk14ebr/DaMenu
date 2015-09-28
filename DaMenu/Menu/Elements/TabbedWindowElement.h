#pragma once

class TabbedWindowElement : public MenuElement
{
public:
	virtual void Draw(RenderInterface& Renderer) override;
	virtual ElementType GetType() override;
	uint32_t AddTabPage(const std::string& TabName);
	struct Context
	{
		std::string m_WindowName;
		Color m_TabFillColor;
		Color m_TabTitleFillColor;
		Vector2f m_Size;
		Vector2f m_Position;

		//optional
		uint32_t m_DefaultTabFocusIndex;
		float m_TabBarHeight;
		Color m_TabTextColor;
		Context()
		{
			m_DefaultTabFocusIndex = 0;
			m_TabBarHeight = 25;
			m_TabTextColor = Color::Black();
		}
	};
	TabbedWindowElement(const Context& Ctx);
	~TabbedWindowElement();

	template<typename T>
	T* GetElementById(const uint32_t Id);
private:
	Context m_Ctx;
	std::vector<TabbedWindowPageElement*> m_TabPages;
	uint32_t m_TabInFocus;
};

TabbedWindowElement::TabbedWindowElement(const Context& Ctx):
	MenuElement(Ctx.m_Position,Ctx.m_Size)
{
	m_Ctx = Ctx;
	m_TabInFocus = m_Ctx.m_DefaultTabFocusIndex;
}

TabbedWindowElement::~TabbedWindowElement()
{
	for (MenuElement* Element : m_TabPages)
	{
		delete Element;
	}
	m_TabPages.clear();
}

uint32_t TabbedWindowElement::AddTabPage(const std::string& TabName)
{
	TabbedWindowPageElement::Context TabCtx;
	TabCtx.m_WindowName = TabName;
	TabCtx.m_FillColor = m_Ctx.m_TabFillColor;
	TabCtx.m_TitleFillColor = m_Ctx.m_TabTitleFillColor;
	TabCtx.m_Position = m_Position;
	TabCtx.m_Size = m_Size;
	TabCtx.m_TitleBarHeight = m_Ctx.m_TabBarHeight;
	TabCtx.m_TextColor = m_Ctx.m_TabTextColor;
	TabbedWindowPageElement* TabPage = new TabbedWindowPageElement(TabCtx);
	m_TabPages.push_back(TabPage);
	return TabPage->GetId();
}

void TabbedWindowElement::Draw(RenderInterface& Renderer)
{
	if (m_TabInFocus > m_TabPages.size())
		return;

	m_TabPages.at(m_TabInFocus)->Draw(Renderer);
}

ElementType TabbedWindowElement::GetType()
{
	return ElementType::Window;
}

template<typename T>
T* TabbedWindowElement::GetElementById(const uint32_t Id)
{
	for (MenuElement* Element : m_TabPages)
	{
		if (Element->GetId() == Id)
			return dynamic_cast<T*>(Element);
		
		if (Element->GetType() == ElementType::Window)
		{
			WindowElement* WindowElem = dynamic_cast<WindowElement*>(Element);
			if (WindowElem == nullptr)
				continue;

			T* RetElement = WindowElem->GetElementById<T>(Id);
			if (RetElement == nullptr)
				continue;
			return RetElement;
		}
	}
	return nullptr;
}

