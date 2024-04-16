#include "settings_form.h"

SettingsForm::SettingsForm(QDialog *parent)
    : QDialog{parent}
{
    baseInit();
    layoutsInit();
    widgetsInit();
    layoutsFill();
    connectionsInit();
}

void SettingsForm::save_settings(bool is_reset)
{
    QSettings settings("PMDR0", "base");

    // Timings
    settings.beginGroup("Timings");
    if (is_reset)
    {
        settings.setValue("roundTime", Settings::DEFAULT_ROUND_TIME_IN_SEC);
        settings.setValue("shortBreakTime", Settings::DEFAULT_SHORT_BREAK_TIME_IN_SEC);
        settings.setValue("longBreakTime", Settings::DEFAULT_LONG_BREAK_TIME_IN_SEC);
    }
    else
    {
        settings.setValue("roundTime", Settings::round_time);
        settings.setValue("shortBreakTime", Settings::short_break_time);
        settings.setValue("longBreakTime", Settings::long_break_time);
    }
    settings.endGroup();

    // Sounds
    settings.beginGroup("Sounds");
    if (is_reset)
    {
        settings.setValue("roundSoundPath", "./sounds/sound1.wav");
        settings.setValue("shortBreakSoundPath", "./sounds/sound1.wav");
        settings.setValue("longBreakSoundPath", "./sounds/sound1.wav");
    }
    else
    {
        settings.setValue("roundSoundPath", Settings::round_sound.source().path());
        settings.setValue("shortBreakSoundPath", Settings::short_break_sound.source().path());
        settings.setValue("longBreakSoundPath", Settings::long_break_sound.source().path());
    }
    settings.endGroup();

    // Locale
    settings.beginGroup("Locale");
    if (is_reset)
        settings.setValue("locale", "en");
    else
        settings.setValue("locale", Settings::locale);
    settings.endGroup();

    // Tray
    settings.beginGroup("Tray");
    if (is_reset)
    {
        settings.setValue("isTrayEnabled", Settings::Postponed);
        settings.setValue("isTrayNotificationsEnabled", false);
    }
    else
    {
        if (tray_roll->currentText() == tr("Tray"))
            settings.setValue("isTrayEnabled", Settings::Enabled);
        else if (tray_roll->currentText() == tr("Don't tray"))
            settings.setValue("isTrayEnabled", Settings::Disabled);
        else if (tray_roll->currentText() == tr("Postpone"))
            settings.setValue("isTrayEnabled", Settings::Postponed);

        settings.setValue("isTrayNotificationsEnabled", Settings::is_notification_enabled);
    }
    settings.endGroup();

    // Statistics
    settings.beginGroup("Statistics");
    if (is_reset && statistics_reset == QMessageBox::Yes)
    {
        settings.setValue("Total/totalSeconds", 0);
        settings.setValue("Total/totalRounds", 0);
        settings.setValue("Daily/todaySeconds", 0);
        settings.setValue("Daily/todayRounds", 0);
        settings.setValue("Weekly/weeklySeconds", 0);
        settings.setValue("Weekly/weeklyRounds", 0);
        settings.setValue("Monthly/monthlySeconds", 0);
        settings.setValue("Monthly/monthlyRounds", 0);
        settings.setValue("Annual/annualSeconds", 0);
        settings.setValue("Annual/annualRounds", 0);
    }
    settings.endGroup();
}

void SettingsForm::widgetsInit()
{
    // FOR MAIN_LAYOUT
    // Round
    round_front_lbl = new QLabel(tr("Round time:"));
    round_time_slider = new QSlider(Qt::Horizontal);
    round_time_slider->setRange(0, SLIDER_MINS);
    round_time_slider->setValue(Settings::round_time / Settings::SEC_IN_MIN);
    round_back_lbl = new QLabel(QString::number(round_time_slider->value()) + tr(" min"));

    round_sound_front_lbl = new QLabel(tr("Sound:"));
    round_sound_play_btn = new QPushButton(tr("Play"));
    round_sound_back_lbl = new QLabel(Settings::round_sound.source().fileName());
    round_sound_change_btn = new QPushButton(tr("Change sound"));
    old_round_sound_path = Settings::round_sound.source().path();

    // Short break
    short_front_lbl = new QLabel(tr("Short break time:"));
    short_break_slider = new QSlider(Qt::Horizontal);
    short_break_slider->setRange(0, SLIDER_MINS);
    short_break_slider->setValue(Settings::short_break_time / Settings::SEC_IN_MIN);
    short_back_lbl = new QLabel(QString::number(short_break_slider->value()) + tr(" min"));

    short_sound_front_lbl = new QLabel(tr("Sound:"));
    short_sound_play_btn = new QPushButton(tr("Play"));
    short_sound_back_lbl = new QLabel(Settings::short_break_sound.source().fileName());
    short_sound_change_btn = new QPushButton(tr("Change sound"));
    old_short_sound_path = Settings::short_break_sound.source().path();

    // Long break
    long_front_lbl = new QLabel(tr("Long break time:"));
    long_break_slider = new QSlider(Qt::Horizontal);
    long_break_slider->setRange(0, SLIDER_MINS);
    long_break_slider->setValue(Settings::long_break_time / Settings::SEC_IN_MIN);
    long_back_lbl = new QLabel(QString::number(long_break_slider->value()) + tr(" min"));

    long_sound_front_lbl = new QLabel(tr("Sound:"));
    long_sound_play_btn = new QPushButton(tr("Play"));
    long_sound_back_lbl = new QLabel(Settings::long_break_sound.source().fileName());
    long_sound_change_btn = new QPushButton(tr("Change sound"));
    old_long_sound_path = Settings::long_break_sound.source().path();

    pull_up_settings = new QCheckBox(tr("Pull-up settings"));
    pull_up_settings->setChecked(true);

    tray_roll_front_lbl = new QLabel(tr("Rolling up into tray:"));
    tray_roll = new QComboBox;
    tray_roll->addItem(tr("Tray"));
    tray_roll->addItem(tr("Don't tray"));
    tray_roll->addItem(tr("Postpone"));
    if (Settings::is_tray_enabled == Settings::Enabled)
        tray_roll->setCurrentText(tr("Tray"));
    else if (Settings::is_tray_enabled == Settings::Disabled)
        tray_roll->setCurrentText(tr("Don't tray"));
    else if (Settings::is_tray_enabled == Settings::Postponed)
        tray_roll->setCurrentText(tr("Postpone"));

    enable_tray_notifications = new QCheckBox(tr("Enable tray notifications"));
    if (Settings::is_notification_enabled == true)
        enable_tray_notifications->setChecked(true);
    else
        enable_tray_notifications->setChecked(false);

    languages_front_lbl = new QLabel(tr("Language:"));
    languages = new QComboBox;
    languages->addItem(tr("English"));
    languages->addItem(tr("Ukrainian"));
    languages->addItem(tr("Russian"));
    if (Settings::locale == "en")
        languages->setCurrentIndex(0);
    else if (Settings::locale == "ua")
        languages->setCurrentIndex(1);
    else if (Settings::locale == "ru")
        languages->setCurrentIndex(2);

    factory_reset_btn = new QPushButton(tr("Factory reset"));


    // FOR BUTTONS_LAYOUT
    accept_btn = new QPushButton(tr("Accept"));
    cancel_btn = new QPushButton(tr("Cancel"));
}

void SettingsForm::layoutsInit()
{
    base_layout = new QVBoxLayout;
    main_layout	= new QGridLayout;
    buttons_layout = new QHBoxLayout;
    buttons_layout->setAlignment(Qt::AlignBottom);
}

void SettingsForm::layoutsFill()
{
    // MAIN LAYOUT
    setLayout(base_layout);
    base_layout->addLayout(main_layout);
    base_layout->addLayout(buttons_layout);

    main_layout->addWidget(round_front_lbl, 0, 0);
    main_layout->addWidget(round_time_slider, 0, 1);
    main_layout->addWidget(round_back_lbl, 0, 2);

    main_layout->addWidget(round_sound_front_lbl, 1, 0);
    main_layout->addWidget(round_sound_play_btn, 1, 1);
    main_layout->addWidget(round_sound_back_lbl, 1, 2);
    main_layout->addWidget(round_sound_change_btn, 2, 1);


    main_layout->addWidget(short_front_lbl, 3, 0);
    main_layout->addWidget(short_break_slider, 3, 1);
    main_layout->addWidget(short_back_lbl, 3, 2);

    main_layout->addWidget(short_sound_front_lbl, 4, 0);
    main_layout->addWidget(short_sound_play_btn, 4, 1);
    main_layout->addWidget(short_sound_back_lbl, 4, 2);
    main_layout->addWidget(short_sound_change_btn, 5, 1);


    main_layout->addWidget(long_front_lbl, 6, 0);
    main_layout->addWidget(long_break_slider, 6, 1);
    main_layout->addWidget(long_back_lbl, 6, 2);

    main_layout->addWidget(long_sound_front_lbl, 7, 0);
    main_layout->addWidget(long_sound_play_btn, 7, 1);
    main_layout->addWidget(long_sound_back_lbl, 7, 2);
    main_layout->addWidget(long_sound_change_btn, 8, 1);


    main_layout->addWidget(pull_up_settings, 9, 1);

    main_layout->addWidget(tray_roll_front_lbl, 10, 0);
    main_layout->addWidget(tray_roll, 10, 1);

    main_layout->addWidget(enable_tray_notifications, 11, 1);

    main_layout->addWidget(languages_front_lbl, 12, 0);
    main_layout->addWidget(languages, 12, 1);

    main_layout->addWidget(factory_reset_btn, 13, 1);

    // BUTTONS_LAYOUT
    buttons_layout->addWidget(accept_btn);
    buttons_layout->addWidget(cancel_btn);
}

void SettingsForm::connectionsInit()
{
    // Saving, canceling, resetting
    connect(accept_btn, SIGNAL(clicked()), this, SLOT(save_changings()));
    connect(cancel_btn, SIGNAL(clicked()), this, SLOT(close()));
    connect(factory_reset_btn, SIGNAL(clicked()), this, SLOT(factory_reset()));

    // Sound part
    // Round
    connect(round_sound_play_btn, SIGNAL(clicked()), this, SLOT(play_round_sound()));
    connect(round_sound_change_btn, SIGNAL(clicked()), this, SLOT(change_round_sound()));
    // Short break
    connect(short_sound_play_btn, SIGNAL(clicked()), this, SLOT(play_short_break_sound()));
    connect(short_sound_change_btn, SIGNAL(clicked()), this, SLOT(change_short_break_sound()));
    // Long break
    connect(long_sound_play_btn, SIGNAL(clicked()), this, SLOT(play_long_break_sound()));
    connect(long_sound_change_btn, SIGNAL(clicked()), this, SLOT(change_long_break_sound()));

    // Changing sliders
    connect(round_time_slider, SIGNAL(valueChanged(int)), this, SLOT(round_slider_changed(int)));
    connect(short_break_slider, SIGNAL(valueChanged(int)), this, SLOT(short_slider_changed(int)));
    connect(long_break_slider, SIGNAL(valueChanged(int)), this, SLOT(long_slider_changed(int)));

    // Pulling up sliders
    connect(pull_up_settings, SIGNAL(stateChanged(int)), this, SLOT(pull_up_settings_changed(int)));
}

void SettingsForm::baseInit()
{
    setFixedSize(400, 450);
    statistics_reset = QMessageBox::No;
}

void SettingsForm::save_changings()
{
    Settings::round_time = round_time_slider->value() * Settings::SEC_IN_MIN;
    Settings::short_break_time = short_break_slider->value() * Settings::SEC_IN_MIN;
    Settings::long_break_time = long_break_slider->value() * Settings::SEC_IN_MIN;

    if (enable_tray_notifications->isChecked())
        Settings::is_notification_enabled = true;
    else
        Settings::is_notification_enabled = false;

    if (languages->currentIndex() == 0)
        Settings::locale = "en";
    else if (languages->currentIndex() == 1)
        Settings::locale = "ua";
    else if (languages->currentIndex() == 2)
        Settings::locale = "ru";

    save_settings();
    accept();
    close();
}

void SettingsForm::play_round_sound()
{
    Settings::round_sound.play();
}

void SettingsForm::change_round_sound()
{
    Settings::round_sound.stop();

    QFileDialog *select_file = new QFileDialog;
    QUrl new_path = QUrl::fromLocalFile(select_file->getOpenFileName(this, "Sound file", "./sounds", "Sound files (*.wav)"));

    if (!new_path.isEmpty())
        Settings::round_sound.setSource(new_path);

    round_sound_back_lbl->setText(Settings::round_sound.source().fileName());
}

void SettingsForm::play_short_break_sound()
{
    Settings::short_break_sound.play();
}

void SettingsForm::change_short_break_sound()
{
    Settings::short_break_sound.stop();

    QFileDialog *select_file = new QFileDialog;
    QUrl new_path = QUrl::fromLocalFile(select_file->getOpenFileName(this, "Sound file", "./sounds", "Sound files (*.wav)"));

    if (!new_path.isEmpty())
        Settings::short_break_sound.setSource(new_path);

    short_sound_back_lbl->setText(Settings::short_break_sound.source().fileName());
}

void SettingsForm::play_long_break_sound()
{
    Settings::long_break_sound.play();
}

void SettingsForm::change_long_break_sound()
{
    Settings::long_break_sound.stop();

    QFileDialog *select_file = new QFileDialog;
    QUrl new_path = QUrl::fromLocalFile(select_file->getOpenFileName(this, "Sound file", "./sounds", "Sound files (*.wav)"));

    if (!new_path.isEmpty())
        Settings::long_break_sound.setSource(new_path);

    long_sound_back_lbl->setText(Settings::long_break_sound.source().fileName());
}

void SettingsForm::round_slider_changed(int value)
{
    round_back_lbl->setText(QString::number(value) + tr(" min"));

    if (pull_up_settings->isChecked())
        pull_up_settings_changed(true);
}

void SettingsForm::short_slider_changed(int value)
{
    int short_break_time = short_break_slider->value();
    int round_time = short_break_time * 5;
    int long_break_time = short_break_time * 3;

    short_back_lbl->setText(QString::number(value) + tr(" min"));

    if (pull_up_settings->isChecked())
    {
        if (value <= SLIDER_MINS / 5)
            short_back_lbl->setText(QString::number(value) + tr(" min"));
        else
            short_break_slider->setValue(SLIDER_MINS / 5);

        if (round_time <= SLIDER_MINS)
        {
            round_time_slider->setValue(round_time);
            long_break_slider->setValue(long_break_time);
        }
    }
}

void SettingsForm::long_slider_changed(int value)
{
    int long_break_time = long_break_slider->value();
    int short_break_time = long_break_time / 3;
    int round_time = short_break_time * 5;

    long_back_lbl->setText(QString::number(value) + tr(" min"));

    if (pull_up_settings->isChecked())
    {
        short_break_slider->setValue(short_break_time);
        round_time_slider->setValue(round_time);
    }
}

void SettingsForm::pull_up_settings_changed(int value)
{
    // If 'pull-up' checkbox enabled
    if (value != 0)
    {
        int round_time = round_time_slider->value();
        int short_break_time = round_time / 5;
        int long_break_time = short_break_time * 3;
        short_break_slider->setValue(short_break_time);
        long_break_slider->setValue(long_break_time);
    }
}

void SettingsForm::factory_reset()
{
    int res = QMessageBox::warning(this, tr("Warning!"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (res == QMessageBox::Yes)
    {
        QSettings settings("PMDR0", "base");

        settings.remove("Locale");
        settings.remove("Sounds");
        settings.remove("Timings");
        settings.remove("Tray");

        statistics_reset = QMessageBox::warning(this, tr("Statistics"), tr("Do you want to clear statistics?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (statistics_reset == QMessageBox::Yes)
            settings.remove("Statistics");

        qApp->quit();
        QProcess::startDetached(qApp->applicationFilePath());
        save_settings(true);
    }
}

void SettingsForm::closeEvent(QCloseEvent *event)
{
    Settings::round_sound.setSource(QUrl::fromLocalFile(old_round_sound_path));
    Settings::short_break_sound.setSource(QUrl::fromLocalFile(old_short_sound_path));
    Settings::long_break_sound.setSource(QUrl::fromLocalFile(old_long_sound_path));
}
