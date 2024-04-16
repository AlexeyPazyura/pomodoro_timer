#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include "settings.h"
#include <QtWidgets>

class SettingsForm : public QDialog
{
    Q_OBJECT

private:
    /// CONSTANTS
    // Amount of minutes to show
    const int SLIDER_MINS = 5 * Settings::SEC_IN_MIN;
    int statistics_reset;

    /// LAYOUTS
    QVBoxLayout *base_layout;
    QGridLayout *main_layout;				// GUI layout
    QHBoxLayout *buttons_layout;			// Layout for 'Accept' and 'Cancel' buttons

    /// WIDGETS
    /// FOR MAIN_LAYOUT
    // Round
    QLabel *round_front_lbl;
    QSlider *round_time_slider;
    QLabel *round_back_lbl;

    QLabel *round_sound_front_lbl;
    QPushButton *round_sound_play_btn;
    QLabel *round_sound_back_lbl;
    QPushButton *round_sound_change_btn;
    QString old_round_sound_path;

    // Short break
    QLabel *short_front_lbl;
    QSlider *short_break_slider;
    QLabel *short_back_lbl;

    QLabel *short_sound_front_lbl;
    QPushButton *short_sound_play_btn;
    QLabel *short_sound_back_lbl;
    QPushButton *short_sound_change_btn;
    QString old_short_sound_path;

    // Long break
    QLabel *long_front_lbl;
    QSlider *long_break_slider;
    QLabel *long_back_lbl;

    QLabel *long_sound_front_lbl;
    QPushButton *long_sound_play_btn;
    QLabel *long_sound_back_lbl;
    QPushButton *long_sound_change_btn;
    QString old_long_sound_path;


    QCheckBox *pull_up_settings;

    QLabel *tray_roll_front_lbl;
    QComboBox *tray_roll;
    QCheckBox *enable_tray_notifications;

    QLabel *languages_front_lbl;
    QComboBox *languages;

    QPushButton *factory_reset_btn;


    /// FOR BUTTONS_LAYOUT
    QPushButton *accept_btn;
    QPushButton *cancel_btn;

public:
    explicit SettingsForm(QDialog *parent = nullptr);
    void save_settings(bool is_reset = 0);
    void widgetsInit();
    void layoutsInit();
    void layoutsFill();
    void connectionsInit();
    void baseInit();

public slots:
    void save_changings();

    void play_round_sound();
    void change_round_sound();

    void play_short_break_sound();
    void change_short_break_sound();

    void play_long_break_sound();
    void change_long_break_sound();

    void round_slider_changed(int);
    void short_slider_changed(int);
    void long_slider_changed(int);

    void pull_up_settings_changed(int);

    void factory_reset();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // SETTINGSFORM_H
