#pragma once

template<typename T>
class SliderElement : public MenuElement
{
public:
	virtual ~SliderElement() = default;
	virtual void Draw(RenderInterface& Renderer) override;
	virtual bool IsPointInMouseDownZone(const Vector2f& Point) override;
	virtual void AddPosition(const Vector2f& NewAmount) override;
	virtual void OnMouseDown(const MouseMessage& Msg) override;
	virtual void OnMouseUp(const MouseMessage& Msg) override;
	virtual void OnMouseMove(const MouseMessage& Msg) override;

	T GetValue();

	struct Context
	{
		std::string m_SliderText;
		Color m_Color;
		Vector2f m_Position;
		Vector2f m_Size;
		
		T m_Min;
		T m_Max;
		T m_DefaultValue;

		float m_SliderWidth;
		Color m_TextColor;
		Context()
		{
			m_TextColor = Color::Black();
			m_SliderWidth = 5;
		}
	};
	SliderElement(const Context& Ctx);
private:
	T Re_Range(float x, float inmin, float inmax, T outmin, T outmax)
	{
		double PercentInRange = (x - inmin) / (inmax - inmin); 
		double NewVal = PercentInRange*(outmax - outmin) + outmin; //Just algebraic re-arrangement
		return (T)NewVal;
	}
	std::string HelpSNPrintf(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		char Buffer[1024];
		vsnprintf(Buffer, 1024, format, args);
		va_end(args);
		return std::string(Buffer);
	}
	
	Context m_Ctx;
	T m_Min;
	T m_Max;
	T m_Value;

	bool m_MouseDown;
	Vector2f m_SliderPos;
	Vector2f m_SliderOffset;
};

template<typename T>
SliderElement<T>::SliderElement(typename const SliderElement<T>::Context& Ctx):
	MenuElement(Ctx.m_Position,Ctx.m_Size)
{
	m_Ctx = Ctx;
	m_Min = m_Ctx.m_Min;
	m_Max = m_Ctx.m_Max;
	m_Value = m_Ctx.m_DefaultValue;
	m_SliderPos = m_Position;
	m_MouseDown = false;
}

template<typename T>
void SliderElement<T>::Draw(RenderInterface& Renderer)
{
	Renderer.DrawFilledBox(m_SliderPos,Vector2f(m_Ctx.m_SliderWidth,m_Size.y), m_Ctx.m_Color);
	Renderer.DrawLine(Vector2f(m_Position.x, m_Position.y + (m_Size.y / 2)), Vector2f(m_Position.x + m_Size.x, m_Position.y + (m_Size.y / 2)), Color::Black());

	if (std::is_integral<T>::value)
	{
		std::string IntegralMin = HelpSNPrintf("%d", m_Min);
		Vector2f MinStrSz = Renderer.MeasureString(IntegralMin.c_str());
		Renderer.RenderText(Vector2f(m_Position.x - MinStrSz.x-5, m_Position.y), m_Ctx.m_TextColor, IntegralMin.c_str());

		Renderer.RenderText(Vector2f(m_Position.x + m_Size.x + 5, m_Position.y), m_Ctx.m_TextColor, "%d", m_Max);
		std::string Val = HelpSNPrintf("%d", m_Value);
		Vector2f ValStrSz = Renderer.MeasureString(Val.c_str());
		Renderer.RenderText(Vector2f(m_SliderPos.x - ValStrSz.x / 2, m_SliderPos.y + m_Size.y), m_Ctx.m_TextColor, Val.c_str());
	}else if (std::is_floating_point<T>::value) {
		std::string FloatMin = HelpSNPrintf("%.1f", m_Min);
		Vector2f StrSz = Renderer.MeasureString(FloatMin.c_str());
		Renderer.RenderText(Vector2f(m_Position.x - StrSz.x-5, m_Position.y), m_Ctx.m_TextColor, FloatMin.c_str());

		Renderer.RenderText(Vector2f(m_Position.x + m_Size.x+ 5, m_Position.y), m_Ctx.m_TextColor,"%.1f",m_Max);
		std::string Val = HelpSNPrintf("%.1f", m_Value);
		Vector2f ValStrSz = Renderer.MeasureString(Val.c_str());
		Renderer.RenderText(Vector2f(m_SliderPos.x - ValStrSz.x / 2, m_SliderPos.y + m_Size.y), m_Ctx.m_TextColor, Val.c_str());
	}
}

template<typename T>
bool SliderElement<T>::IsPointInMouseDownZone(const Vector2f& Point)
{
	if (Point.x > m_SliderPos.x && Point.x < (m_SliderPos.x + m_Ctx.m_SliderWidth) &&
		Point.y > m_SliderPos.y && Point.y < (m_SliderPos.y + m_Ctx.m_Size.y))
		return true;
	return false;
}

template<typename T>
void SliderElement<T>::AddPosition(const Vector2f& NewAmount)
{
	m_Position += NewAmount;
	m_SliderPos += NewAmount;
}

template<typename T>
void SliderElement<T>::OnMouseDown(const MouseMessage& Msg)
{
	m_MouseDown = true;
	m_SliderOffset = m_SliderPos - Msg.GetLocation();
	m_eMouseDown.Invoke(Msg);
}

template<typename T>
void SliderElement<T>::OnMouseUp(const MouseMessage& Msg)
{
	m_MouseDown = false;
	m_eMouseUp.Invoke(Msg);
}

template<typename T>
void SliderElement<T>::OnMouseMove(const MouseMessage& Msg)
{
	if (m_MouseDown)
	{
		Vector2f MousePos = Msg.GetLocation()+m_SliderOffset;
		if (MousePos.x > m_Position.x && MousePos.x < (m_Position.x + m_Size.x - m_Ctx.m_SliderWidth))
		{
			m_SliderPos.x = MousePos.x;
			m_Value = Re_Range(m_SliderPos.x, m_Position.x, (m_Position.x + m_Size.x), m_Min, m_Max);
		}else if (MousePos.x <= m_Position.x) {
			m_Value = m_Min;
		}else if (MousePos.y >= (m_Position.x + m_Size.x - m_Ctx.m_SliderWidth)){
			m_Value = m_Max;
		}
	}
	m_eMouseMove.Invoke(Msg);
}

template<typename T>
T SliderElement<T>::GetValue()
{
	return m_Value;
}
