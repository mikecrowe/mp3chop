#ifndef MP3_TIMECODE_H
#define MP3_TIMECODE_H

class TimeCode
{
    int h;

public:
    static const TimeCode Zero;
    
    TimeCode() {}
    explicit TimeCode(int hundredths) : h(hundredths) {}
    TimeCode(int hours, int minutes, int seconds, int hundredths) :
		h(hours*360000 + minutes*6000 + seconds * 100 + hundredths) {}

    void Set(int hours, int minutes, int seconds, int hundredths)
    {
		h = hours*360000 + minutes*6000 + seconds * 100 + hundredths;
    }
    void Set(int hundredths)
    {
	h = hundredths;
    }
    int GetHundredths() const
    {
	return h % 100;
    }
    int GetSeconds() const
    {
	return (h/100)%60;
    }
    int GetMinutes() const
    {
	return (h/6000)%60;
    }
    int GetHours() const
    {
	return (h/360000);
    }
    int GetAsMinutes() const
    {
	return (h/6000);
    }
    int GetAsSeconds() const
    {
	return h/100;
    }
    int GetAsHundredths() const
    {
	return h;
    }

    TimeCode &operator+=(int hundredths)
    {
	h += hundredths;
	return *this;
    }
    TimeCode &operator-=(int hundredths)
    {
	h -= hundredths;
	return *this;
    }

    TimeCode &operator*=(int multiplier)
    {
	h *= multiplier;
	return *this;
    }

    bool IsNormalised() const
    {
	return true;
    }
};

inline TimeCode operator+(const TimeCode tc1, const TimeCode tc2)
{
    return TimeCode(tc1.GetAsHundredths() + tc2.GetAsHundredths());
}

inline TimeCode operator-(const TimeCode tc1, const TimeCode tc2)
{
    return TimeCode(tc1.GetAsHundredths() - tc2.GetAsHundredths());
}

inline bool operator<=(const TimeCode tc1, const TimeCode tc2)
{
    return tc1.GetAsHundredths() <= tc2.GetAsHundredths();
}

inline bool operator<(const TimeCode tc1, const TimeCode tc2)
{
    return tc1.GetAsHundredths() < tc2.GetAsHundredths();
}

inline bool operator==(const TimeCode tc1, const TimeCode tc2)
{
    return tc1.GetAsHundredths() == tc2.GetAsHundredths();
}

inline bool operator!=(const TimeCode tc1, const TimeCode tc2)
{
    return tc1.GetAsHundredths() != tc2.GetAsHundredths();
}

inline bool operator>(const TimeCode tc1, const TimeCode tc2)
{
    return tc1.GetAsHundredths() > tc2.GetAsHundredths();
}

inline bool operator>=(const TimeCode tc1, const TimeCode tc2)
{
    return tc1.GetAsHundredths() >= tc2.GetAsHundredths();
}

#endif
