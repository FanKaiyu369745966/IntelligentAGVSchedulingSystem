#include "CallerAttribute.h"

CallerAttribute::CallerAttribute()
	: m_sigCall(static_cast<unsigned char>(Signal::Signal_Off))
	, m_sigCancel(static_cast<unsigned char>(Signal::Signal_Off))
	, m_sigScream(static_cast<unsigned char>(Signal::Signal_Off))
{
}

CallerAttribute::CallerAttribute(const unsigned char& scream, const unsigned char& call, const unsigned char& cancel)
	: m_sigCall(call)
	, m_sigCancel(cancel)
	, m_sigScream(scream)
{
}

CallerAttribute::CallerAttribute(const CallerAttribute& attr)
{
	m_sigCall = attr.m_sigCall;
	m_sigCancel = attr.m_sigCancel;
	m_sigScream = attr.m_sigScream;
}

void CallerAttribute::operator=(const CallerAttribute& attr)
{
	m_sigCall = attr.m_sigCall;
	m_sigCancel = attr.m_sigCancel;
	m_sigScream = attr.m_sigScream;

	return;
}

bool CallerAttribute::operator==(const CallerAttribute& attr)
{
	if (m_sigCall != attr.m_sigCall
		|| m_sigCancel != attr.m_sigCancel
		|| m_sigScream != attr.m_sigScream)
	{
		return false;
	}

	return true;
}

bool CallerAttribute::operator!=(const CallerAttribute& attr)
{
	return !(*this == attr);
}

bool CallerAttribute::UpdateSignal(const CallerAttribute& attr)
{
	bool update = false;
	if (UpdateCallSignal(attr.m_sigCall))
	{
		update = true;
	}

	if (UpdateCancelSignal(attr.m_sigCall))
	{
		update = true;
	}

	if (UpdateScreamSignal(attr.m_sigCall))
	{
		update = true;
	}

	return update;
}

bool CallerAttribute::UpdateSignal(const SignalType& type, const Signal& signal)
{
	switch (type)
	{
	case SignalType::Signal_Call:
		return UpdateCallSignal(signal);
	case SignalType::Signal_Cancel:
		return UpdateCancelSignal(signal);
	case SignalType::Signal_Scream:
		return UpdateScreamSignal(signal);
	}

	return false;
}

bool CallerAttribute::UpdateCallSignal(const unsigned char& signal)
{
	if (m_sigCall != signal)
	{
		m_sigCall = signal;
		return true;
	}

	return false;
}

bool CallerAttribute::UpdateCancelSignal(const unsigned char& signal)
{
	if (m_sigCancel != signal)
	{
		m_sigCancel = signal;
		return true;
	}

	return false;
}

bool CallerAttribute::UpdateScreamSignal(const unsigned char& signal)
{
	if (m_sigScream != signal)
	{
		m_sigScream = signal;
		return true;
	}

	return false;
}

QString CallerAttribute::GetSignalInfo(const Signal& sig) const
{
	QString str = "unknow";
	switch (sig)
	{
	case Signal::Signal_Off:
		str = QString::fromLocal8Bit("无");
		break;
	case Signal::Signal_On:
		str = QString::fromLocal8Bit("有信号");
		break;
	case Signal::Signal_ToStation:
		str = QString::fromLocal8Bit("取成品至热处理车间");
		break;
	case Signal::Signal_ToLib:
		str = QString::fromLocal8Bit("取成品入库");
		break;
	case Signal::Signal_PutBlank1:
		str = QString::fromLocal8Bit("运送毛坯至CNC");
		break;
	case Signal::Signal_PutBlank2:
		str = QString::fromLocal8Bit("运送毛坯至磨齿机1");
		break;
	case Signal::Signal_PutBlank3:
		str = QString::fromLocal8Bit("运送毛坯至磨齿机3");
		break;
	case Signal::Signal_PutTray1:
		str = QString::fromLocal8Bit("运送空托盘至滚齿机");
		break;
	case Signal::Signal_PutTray2:
		str = QString::fromLocal8Bit("运送空托盘至磨齿机1");
		break;
	case Signal::Signal_PutTray3:
		str = QString::fromLocal8Bit("运送空托盘至磨齿机2");
		break;
	default:
		str = "unknow";
		break;
	}

	return str;
}
