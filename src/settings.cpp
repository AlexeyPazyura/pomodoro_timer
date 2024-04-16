#include "settings.h"

// Rounds setting up
int Settings::round_time = Settings::DEFAULT_ROUND_TIME_IN_SEC;
QSoundEffect Settings::round_sound;

// Short break setting up
int Settings::short_break_time = Settings::DEFAULT_SHORT_BREAK_TIME_IN_SEC;
QSoundEffect Settings::short_break_sound;

// Long break setting up
int Settings::long_break_time = Settings::DEFAULT_LONG_BREAK_TIME_IN_SEC;
QSoundEffect Settings::long_break_sound;

// Themes
Settings::Themes Settings::current_theme = Settings::Default;
QFont Settings::current_font = QFont("Arial", 20);
qreal Settings::current_opacity = 0.97;
QPalette Settings::round_color = QPalette("#5499DE");
QPalette Settings::short_break_color = QPalette("#66DE6C");
QPalette Settings::long_break_color = QPalette("#DCDE66");

// Statistics
int Settings::total_rounds = 0;
int Settings::total_seconds = 0;

int Settings::today_rounds = 0;
int Settings::today_seconds = 0;

int Settings::weekly_rounds = 0;
int Settings::weekly_seconds = 0;

int Settings::monthly_rounds = 0;
int Settings::monthly_seconds = 0;

int Settings::annual_rounds = 0;
int Settings::annual_seconds = 0;

// Other
Settings::TrayEnabled Settings::is_tray_enabled = Settings::Postponed;
QString Settings::locale = "en";
bool Settings::is_notification_enabled = false;
