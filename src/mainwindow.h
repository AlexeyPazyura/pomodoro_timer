#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settings.h"
#include "settings_form.h"
#include <QtWidgets>
#include <QSoundEffect>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // start_btn states
    enum StartButtonState
    {
        Start,
        Pause,
        Continue
    } start_btn_state;

    Settings::Pomodoro current_round;

    /// LAYOUTS
    QVBoxLayout *main_layout;		// Main layout that contains all the elements
    QHBoxLayout *rounds_layout; 	// Layout for showing rounds

    /// MENU
    QMenu *edit_menu;				// 'Edit' button at menu bar
    QAction *settings_action;		// 'Settings' action in 'Edit' menu section

    /// THEMES (DEFAULT ARE TURNED OFF)
    QMenu *themes_menu;				// 'Themes' button at menu bar
    QAction *reset_theme;			// Action in 'Themes' for resetting theme
    QMenu *set_theme;				// Sub-menu int 'Themes' for setting theme
    QAction *contrast_theme,		// "Contrast" theme switch button
            *simple_light_theme,	// "SimpleLight" theme switch button
            *pomodoro_mode_theme;	// "PomodoroMode" theme switch button

    QMenu *help_menu;				// 'Help' button at menu bar
    QAction *credits_action;		// Action in 'Help' for showing credits info
    QAction *statistics_action;		// Action in 'Help' for showing statistics info

    /// WIDGETS
    QLabel *time_left;				// Label for showing how much time left
    QDial *dial;					// Visual component for indicating time of timer
    QPushButton *start_btn,
                *stop_btn;

    QFrame *rounds;					// Container that consists rounds' circles
    QRadioButton *first_round,
                 *second_round,
                 *third_round,
                 *fourth_round;

    /// TIMERS
    QTimer *round_timer,			// Timer for rounds
           *short_break_timer,		// Timer for short breaks
           *long_break_timer;		// Timer for long breaks
    int timeout_counter,			// Seconds' counter
        pause_counter;				// Counter for control start_btn state

    /// TRAY
    QSystemTrayIcon *tray;			// Tray
    QMenu *tray_menu; 				// Tray's context menu
    QAction *tray_exit;				// Action in 'Tray' that closes app

    /// LOCALE
    QTranslator translator;			// Just translator

public:
    MainWindow(QWidget *parent = nullptr);

    // Method for converting seconds to hh:mm:ss format
    QString convertTime(int total_seconds, bool with_letters = false);

    void loadSettings();
    void reloadScreen();			// For proper work QTranslator
    void updateStatistics();
    void loadStatistics();
    void widgetsInit();
    void connectionsInit();
    void menuInit();
    void layoutsInit();
    void layoutsFill();
    void timersInit();
    void baseInit();
    void trayInit();

public slots:
    void startTimer();
    void stopTimer();

    void onRoundTimeout();			// Cycle during round time
    void onShortBreakTimeout();		// Cycle during short break time
    void onLongBreakTimeout();		// Cycle during long break time
    void onDialChange(int);			// Interactive dial processing

    void openSettings();			// Openning settings window (dialog)
    void openCredits();				// Openning credits window (dialog)
    void openStatistics();			// Openning statistics window (dialog)

    void exitApplication();

    void changeTheme(Settings::Themes theme_name);
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
