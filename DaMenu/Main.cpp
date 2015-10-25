// DaMenu.cpp : Defines the entry point for the console application.
//

#define NOMINMAX
#include <windows.h>
#include "DaMenu.h"

void SetupMenu(ElementManager& GUIManager)
{
	ButtonElement::Context BtnCtx;
	BtnCtx.m_ButtonText = "A Button";
	BtnCtx.m_FillColor = Color(.5f, .5f, .5f);
	BtnCtx.m_FillColorMouseOver = Color(.7f, .7f, .7f);
	BtnCtx.m_FillColorMouseDown = Color(.4f, .4f, .4f);
	BtnCtx.m_Position = Vector2f(50, 50);
	BtnCtx.m_Size = Vector2f(100, 20);
	ButtonElement* Button = new ButtonElement(BtnCtx);
	BtnCtx.m_Position = Vector2f(50, 195);
	ButtonElement* Button2 = new ButtonElement(BtnCtx);

	SliderElement<float>::Context SliderCtx;
	SliderCtx.m_SliderText = "A Slider";
	SliderCtx.m_Color = Color::Black();
	SliderCtx.m_DefaultValue = 5.1f;
	SliderCtx.m_Max = 10.f;
	SliderCtx.m_Min = -10.f;
	SliderCtx.m_Position = Vector2f(50, 100);
	SliderCtx.m_Size = Vector2f(100, 20);
	SliderElement<float>* Slider = new SliderElement<float>(SliderCtx);

	CheckBoxElement::Context CheckBoxCtx;
	CheckBoxCtx.m_FillColor = Color(.7f, .7f, .7f); 
	CheckBoxCtx.m_FillColorMouseOver = Color(.5f, .5f, .5f);
	CheckBoxCtx.m_FillColorChecked = Color(.4f, .4f, .4f);
	CheckBoxCtx.m_Position = Vector2f(50, 140);
	CheckBoxCtx.m_Size = Vector2f(20, 20);
	CheckBoxElement* CheckBox = new CheckBoxElement(CheckBoxCtx);
	
	WindowElement::Context Ctx;
	Ctx.m_FillColor = Color(.7f, .7f, .7f);
	Ctx.m_TitleFillColor = Color(.5f, .5f, .5f);
	Ctx.m_WindowName = "A Window";
	Ctx.m_Position = Vector2f(100, 100);
	Ctx.m_Size = Vector2f(400, 400);
	WindowElement* Window = new WindowElement(Ctx);
	Window->AddSubElement(Button);
	Window->AddSubElement(Slider);
	Window->AddSubElement(CheckBox);

	ComboBoxElement::Context ComboCtx;
	ComboCtx.m_FillColor = Color(.5f, .5f, .5f);
	ComboCtx.m_FillColorMouseOver = Color(.7f, .7f, .7f);
	ComboCtx.m_FillColorMouseDown = Color(.4f, .4f, .4f);
	ComboCtx.m_Position = Vector2f(50, 150);
	ComboCtx.m_Size = Vector2f(100, 20);
	ComboBoxElement* ComboBox = new ComboBoxElement(ComboCtx);
	ComboBox->AddComboButton("Option 1");
	ComboBox->AddComboButton("Option 2");
	ComboBox->EventSelectionChanged() += [](uint32_t Id, const std::string& Txt) {
		printf("Selection Changed:%d %s\n",Id,Txt.c_str());
	};

	LabelElement::Context LabelCtx;
	LabelCtx.m_Position = Vector2f(50, 130);
	LabelCtx.m_Size = Vector2f(100, 20);
	LabelCtx.m_Text = "A Label";
	LabelElement* Label = new LabelElement(LabelCtx);

	TabbedWindowElement::Context TabWinCtx;
	TabWinCtx.m_TabFillColor = Color(.7f, .7f, .7f);
	TabWinCtx.m_TabTitleFillColor = Color(.5f, .5f, .5f);
	TabWinCtx.m_TitleFillColor = Color(.5f, .5f, .5f);
	TabWinCtx.m_WindowName = "A Tabbed Window";
	TabWinCtx.m_Position = Vector2f(300, 100);
	TabWinCtx.m_Size = Vector2f(400, 400);
	TabWinCtx.m_DefaultTabFocusIndex = 0;
	TabbedWindowElement* TabbedWindow = new TabbedWindowElement(TabWinCtx);
	uint32_t Tab1PageId=TabbedWindow->AddTabPage("Tab 1");
	uint32_t Tab2PageId = TabbedWindow->AddTabPage("Tab 2");
	TabbedWindowPageElement* Tab1Page = TabbedWindow->GetElementById<TabbedWindowPageElement>(Tab1PageId);
	Tab1Page->AddSubElement(Button2);
	Tab1Page->AddSubElement(ComboBox);
	Tab1Page->AddSubElement(Label);

	GUIManager.AddElement(TabbedWindow);
	GUIManager.AddElement(Window);
}

int main()
{
	DXTKRenderer* Renderer=new DXTKRenderer();
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return 0;

	if (FAILED(Renderer->InitWindow(L"Elemental", GetModuleHandle(NULL), Vector2f(800, 800))))
		return 0;

	ElementManager GUIManager(new WinAPIInputManager(Renderer->GetWindowHWND()),Renderer);
	SetupMenu(GUIManager);

	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else {
			GUIManager.UpdateInput();
			GUIManager.Render();
		}
	}
    return (int)msg.wParam;
}

