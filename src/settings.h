#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSoundEffect>
#include <QPalette>
#include <QFont>

class Settings
{
public:
    static const int SEC_IN_MIN = 60,
                     SEC_IN_HOUR = 3600,
                     SECOND_IN_MS = 1000;
    enum Pomodoro
    {
        Round,
        ShortBreak,
        LongBreak
    };
    static enum TrayEnabled
    {
        Enabled,
        Disabled,
        Postponed
    } is_tray_enabled;

    /// ROUNDS
    static int round_time;
    static QSoundEffect round_sound;
    static const int DEFAULT_ROUND_TIME_IN_MIN = 25;
    static const int DEFAULT_ROUND_TIME_IN_SEC = DEFAULT_ROUND_TIME_IN_MIN * SEC_IN_MIN;
    static QPalette round_color;

    /// SHORT BREAKS
    static int short_break_time;
    static QSoundEffect short_break_sound;
    static const int DEFAULT_SHORT_BREAK_TIME_IN_MIN = 5;
    static const int DEFAULT_SHORT_BREAK_TIME_IN_SEC = DEFAULT_SHORT_BREAK_TIME_IN_MIN * SEC_IN_MIN;
    static QPalette short_break_color;

    /// LONG BREAKS
    static int long_break_time;
    static QSoundEffect long_break_sound;
    static const int DEFAULT_LONG_BREAK_TIME_IN_MIN = 15;
    static const int DEFAULT_LONG_BREAK_TIME_IN_SEC = DEFAULT_LONG_BREAK_TIME_IN_MIN * SEC_IN_MIN;
    static QPalette long_break_color;

    /// THEMES (DEFAULT ARE TURNED OFF)
    static enum Themes
    {
        Contrast,
        PomodoroMode,
        SimpleLight,
        Default
    } current_theme;
    static QFont current_font;
    static qreal current_opacity;

    /// STATISTICS
    static int total_rounds,
               total_seconds,
               today_rounds,
               today_seconds,
               weekly_rounds,
               weekly_seconds,
               monthly_rounds,
               monthly_seconds,
               annual_rounds,
               annual_seconds;

    /// OTHER
    static QString locale;
    static bool is_notification_enabled;
};

#endif // SETTINGS_H
