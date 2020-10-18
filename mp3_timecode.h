/*
 * mp3chop
 *
 * (C) 2000-2020 Mike Crowe <mac@mcrowe.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

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
