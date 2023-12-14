#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    baseInit();
    timersInit();
    layoutsInit();
    menuInit();
    widgetsInit();
    layoutsFill();
    connectionsInit();
}

// Функція для конвертування секунд у формат mm:ss
QString MainWindow::convertTime(int total_seconds, bool with_letters)
{
    QString hours,
            minutes,
            seconds;

    if (with_letters)
    {
        hours = QString::number(total_seconds / 3600);
        int remaining = total_seconds % 3600;
        minutes = QString::number(remaining / 60);
        seconds = QString::number(remaining % 60);
        if (hours.toInt() < 1)
            return minutes + tr(" min ") + seconds + tr(" sec");
        else
            return hours + tr(" hours, ") + minutes + tr(" min, ") + seconds + tr(" sec");
    }
    else
    {
        minutes = QString::number(total_seconds / 60);
        seconds = QString::number(total_seconds % 60);

        if (minutes.size() == 1)
        minutes.push_front("0");

        if (seconds.size() == 1)
        seconds.push_front("0");

        return minutes + ":" + seconds;
    }
}

// Завантаження налаштувань
void MainWindow::loadSettings()
{
    QSettings settings("PMDR0", "base");

    if (settings.contains("Locale/locale") &&
        settings.contains("Sounds/longBreakSoundPath") &&
        settings.contains("Sounds/roundSoundPath") &&
        settings.contains("Sounds/shortBreakSoundPath") &&
        settings.contains("Timings/roundTime") &&
        settings.contains("Timings/shortBreakTime") &&
        settings.contains("Timings/longBreakTime") &&
        settings.contains("Tray/isTrayEnabled") &&
        settings.contains("Tray/isTrayNotificationsEnabled"))
    {
        // Timings
        settings.beginGroup("Timings");
        Settings::round_time = settings.value("roundTime").toInt();
        Settings::short_break_time = settings.value("shortBreakTime").toInt();
        Settings::long_break_time = settings.value("longBreakTime").toInt();
        settings.endGroup();

        // Sounds
        settings.beginGroup("Sounds");
        Settings::round_sound.source().path() = settings.value("roundSoundPath").toString();
        Settings::short_break_sound.source().path() = settings.value("shortBreakSoundPath").toString();
        Settings::long_break_sound.source().path() = settings.value("longBreakSoundPath").toString();

        Settings::round_sound.setSource(QUrl::fromLocalFile(settings.value("roundSoundPath").toString()));
        Settings::short_break_sound.setSource(QUrl::fromLocalFile(settings.value("shortBreakSoundPath").toString()));
        Settings::long_break_sound.setSource(QUrl::fromLocalFile(settings.value("longBreakSoundPath").toString()));
        settings.endGroup();

        // Locale
        settings.beginGroup("Locale");
        Settings::locale = settings.value("locale").toString();
        settings.endGroup();

        // Tray
        settings.beginGroup("Tray");
        Settings::is_tray_enabled = settings.value("isTrayEnabled").value<Settings::TrayEnabled>();
        Settings::is_notification_enabled = settings.value("isTrayNotificationsEnabled").toBool();
        settings.endGroup();

        // Statistics
        settings.beginGroup("Statistics");

        settings.beginGroup("Total");
        Settings::total_seconds = settings.value("totalSeconds").toInt();
        Settings::total_rounds = settings.value("totalRounds").toInt();
        settings.endGroup();

        settings.beginGroup("Daily");
        Settings::today_seconds = settings.value("todaySeconds").toInt();
        Settings::today_rounds = settings.value("todayRounds").toInt();
        settings.endGroup();

        settings.beginGroup("Weekly");
        Settings::weekly_seconds = settings.value("weeklySeconds").toInt();
        Settings::weekly_rounds = settings.value("weeklyRounds").toInt();
        settings.endGroup();

        settings.beginGroup("Monthly");
        Settings::monthly_seconds = settings.value("monthlySeconds").toInt();
        Settings::monthly_rounds = settings.value("monthlyRounds").toInt();
        settings.endGroup();

        settings.beginGroup("Annual");
        Settings::annual_seconds = settings.value("annualSeconds").toInt();
        Settings::annual_rounds = settings.value("annualRounds").toInt();
        settings.endGroup();

        settings.endGroup();

        settings.beginGroup("Themes");
        Settings::current_theme = settings.value("currentTheme").value<Settings::Themes>();
        changeTheme(Settings::current_theme);
        settings.endGroup();
    }
    else
    {
        Settings::round_sound.setSource(QUrl::fromLocalFile("./sounds/sound1.wav"));
        Settings::short_break_sound.setSource(QUrl::fromLocalFile("./sounds/sound1.wav"));
        Settings::long_break_sound.setSource(QUrl::fromLocalFile("./sounds/sound1.wav"));
    }
}

// Оновлення екрану для роботи QTranslator
void MainWindow::reloadScreen()
{
    edit_menu->setTitle(tr("Edit"));
    settings_action->setText(tr("Settings"));

    themes_menu->setTitle(tr("Themes"));
    reset_theme->setText(tr("Reset theme"));
    set_theme->setTitle(tr("Set theme"));

    help_menu->setTitle(tr("Help"));
    statistics_action->setText(tr("Statistics"));
    credits_action->setText(tr("Credits"));

    if (start_btn_state == Pause)
        start_btn->setText(tr("Pause"));
    else if (start_btn_state == Start)
        start_btn->setText(tr("Start"));
    else if (start_btn_state == Continue)
        start_btn->setText(tr("Continue"));

    stop_btn->setText(tr("Stop"));
}

// Оновлення статистичних даних
void MainWindow::updateStatistics()
{
    QSettings settings("PMDR0", "base");

    Settings::total_seconds++;
    Settings::today_seconds++;
    Settings::weekly_seconds++;
    Settings::monthly_seconds++;
    Settings::annual_seconds++;

    if (QDate::currentDate().day() != settings.value("Statistics/Daily/currentDay"))
    {
        settings.setValue("Statistics/Daily/currentDay", QDate::currentDate().day());
        Settings::today_seconds = 0;
        Settings::today_rounds = 0;
    }

    if (QDate::currentDate().weekNumber() != settings.value("Statistics/Weekly/currentWeek"))
    {
        settings.setValue("Statistics/Weekly/currentWeek", QDate::currentDate().weekNumber());
        Settings::weekly_seconds = 0;
        Settings::weekly_rounds = 0;
    }

    if (QDate::currentDate().month() != settings.value("Statistics/Monthly/currentMonth"))
    {
        settings.setValue("Statistics/Monthly/currentMonth", QDate::currentDate().month());
        Settings::monthly_rounds = 0;
        Settings::monthly_seconds = 0;
    }

    if (QDate::currentDate().year() != settings.value("Statistics/Annual/currentYear"))
    {
        settings.setValue("Statistics/Annual/currentYear", QDate::currentDate().year());
        Settings::annual_rounds = 0;
        Settings::annual_seconds = 0;
    }
}

// Завантаження статистичних даних
void MainWindow::loadStatistics()
{
    QSettings settings("PMDR0", "base");

    settings.beginGroup("Statistics");

    settings.beginGroup("Total");
    settings.setValue("totalSeconds", Settings::total_seconds);
    settings.setValue("totalRounds", Settings::total_rounds);
    settings.endGroup();

    settings.beginGroup("Daily");
    settings.setValue("currentDay", QDate::currentDate().day());
    settings.setValue("todaySeconds", Settings::today_seconds);
    settings.setValue("todayRounds", Settings::today_rounds);
    settings.endGroup();

    settings.beginGroup("Weekly");
    settings.setValue("currentWeek", QDate::currentDate().weekNumber());
    settings.setValue("weeklySeconds", Settings::weekly_seconds);
    settings.setValue("weeklyRounds", Settings::weekly_rounds);
    settings.endGroup();

    settings.beginGroup("Monthly");
    settings.setValue("currentMonth", QDate::currentDate().month());
    settings.setValue("monthlySeconds", Settings::monthly_seconds);
    settings.setValue("monthlyRounds", Settings::monthly_rounds);
    settings.endGroup();

    settings.beginGroup("Annual");
    settings.setValue("currentYear", QDate::currentDate().year());
    settings.setValue("annualSeconds", Settings::annual_seconds);
    settings.setValue("annualRounds", Settings::annual_rounds);
    settings.endGroup();

    settings.endGroup();
}

// Ініціалізація віджетів
void MainWindow::widgetsInit()
{
    dial = new QDial;
    dial->setMinimumHeight(250);
    dial->setRange(0, Settings::round_time);

    time_left->setText(convertTime(Settings::round_time));
    time_left->setAlignment(Qt::AlignCenter);
    time_left->setFont(Settings::current_font);
    time_left->setMaximumHeight(25);

    start_btn = new QPushButton(tr("Start"));
    start_btn->setMinimumWidth(150);
    start_btn->setMinimumHeight(25);
    start_btn_state = Start;

    stop_btn = new QPushButton(tr("Stop"));
    stop_btn->setMinimumWidth(150);
    stop_btn->setMinimumHeight(25);

    rounds = new QFrame;
    rounds->setLayout(rounds_layout);
    rounds->setMaximumHeight(35);
    rounds->setMaximumWidth(100);
    first_round = new QRadioButton;
    first_round->setChecked(true);
    second_round = new QRadioButton;
    third_round = new QRadioButton;
    fourth_round = new QRadioButton;
}

// Ініціалізація сигналів та слотів
void MainWindow::connectionsInit()
{
    // Обробка натискання на пункти меню
    connect(settings_action, SIGNAL(triggered()), this, SLOT(openSettings()));
    connect(credits_action, SIGNAL(triggered()), this, SLOT(openCredits()));
    connect(statistics_action, SIGNAL(triggered()), this, SLOT(openStatistics()));

    // Теми
    connect(pomodoro_mode_theme, &QAction::triggered, this, [this](){
        changeTheme(Settings::PomodoroMode);
    });
    connect(contrast_theme, &QAction::triggered, this, [this](){
        changeTheme(Settings::Contrast);
    });
    connect(simple_light_theme, &QAction::triggered, this, [this](){
        changeTheme(Settings::SimpleLight);
    });
    connect(reset_theme, &QAction::triggered, this, [this](){
        changeTheme(Settings::Default);
    });

    // Робота з таймером
    connect(start_btn, SIGNAL(clicked()), this, SLOT(startTimer()));
    connect(stop_btn, SIGNAL(clicked()), this, SLOT(stopTimer()));
    connect(round_timer, SIGNAL(timeout()), this, SLOT(onRoundTimeout()));
    connect(short_break_timer, SIGNAL(timeout()), this, SLOT(onShortBreakTimeout()));
    connect(long_break_timer, SIGNAL(timeout()), this, SLOT(onLongBreakTimeout()));

    // Обробки змін
    connect(dial, SIGNAL(valueChanged(int)), this, SLOT(onDialChange(int)));
}

// Ініціалізація верхнього меню
void MainWindow::menuInit()
{
    edit_menu = new QMenu(tr("Edit"));
    settings_action = new QAction(tr("Settings"));
    edit_menu->addAction(settings_action);
    menuBar()->addMenu(edit_menu);

    themes_menu = new QMenu(tr("Themes"));
    reset_theme = new QAction(tr("Reset theme"));
    set_theme = new QMenu(tr("Set theme"));
    contrast_theme = new QAction("Contrast");
    simple_light_theme = new QAction("SimpleLight");
    pomodoro_mode_theme = new QAction("PomodoroMode");
    set_theme->addAction(contrast_theme);
    set_theme->addAction(simple_light_theme);
    set_theme->addAction(pomodoro_mode_theme);
    themes_menu->addMenu(set_theme);
    themes_menu->addAction(reset_theme);
    // Розкоментуй, якщо хочеш щоб з'явились теми в застосунку
    //menuBar()->addMenu(themes_menu);

    help_menu = new QMenu(tr("Help"));
    credits_action = new QAction(tr("Credits"));
    statistics_action = new QAction(tr("Statistics"));
    help_menu->addAction(statistics_action);
    help_menu->addAction(credits_action);
    menuBar()->addMenu(help_menu);
}

// Ініціалізація макетів
void MainWindow::layoutsInit()
{
    QWidget *central_widget = new QWidget;
    main_layout = new QVBoxLayout;
    rounds_layout = new QHBoxLayout;
    setCentralWidget(central_widget);
    central_widget->setLayout(main_layout);
}

// Заповнення макетів віджетами
void MainWindow::layoutsFill()
{
    // ROUNDS_LAYOUT
    rounds_layout->addWidget(first_round);
    rounds_layout->addWidget(second_round);
    rounds_layout->addWidget(third_round);
    rounds_layout->addWidget(fourth_round);

    // MAIN_LAYOUT
    main_layout->addWidget(time_left);
    main_layout->addWidget(dial);
    main_layout->addWidget(rounds, 0, Qt::AlignCenter);
    main_layout->addWidget(start_btn, 0, Qt::AlignCenter);
    main_layout->addWidget(stop_btn, 0, Qt::AlignCenter);
}

// Ініціалізація таймерів
void MainWindow::timersInit()
{
    timeout_counter = 0;
    pause_counter = 0;
    timeout_counter = 0;
    round_timer = new QTimer(this);
    short_break_timer = new QTimer(this);
    long_break_timer = new QTimer(this);
}

// Базова ініціалізація
void MainWindow::baseInit()
{
    setFixedSize(300, 400);
    time_left = new QLabel;
    loadSettings();
    updateStatistics();

    setWindowOpacity(Settings::current_opacity);

    if (Settings::locale == "en")
        translator.load("./translations/Pomodoro_en", ".");
    else if (Settings::locale == "ua")
        translator.load("./translations/Pomodoro_ua", ".");
    else if (Settings::locale == "ru")
        translator.load("./translations/Pomodoro_ru", ".");
    qApp->installTranslator(&translator);

    setPalette(Settings::round_color);
    current_round = Settings::Round;
}

void MainWindow::trayInit()
{
    tray = new QSystemTrayIcon(QIcon("./icons/icon2.ico"));
    tray_exit = new QAction(tr("Exit"));
    tray_menu = new QMenu;
    tray_menu->addAction(tray_exit);
    tray->setContextMenu(tray_menu);
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(tray_exit, SIGNAL(triggered()), this, SLOT(exitApplication()));
}

// Старт/Пауза/Продовження таймеру
void MainWindow::startTimer()
{
    if (start_btn->text() == tr("Start"))
    {
        if (current_round == Settings::Round)
        {
            dial->setRange(0, Settings::round_time);
            round_timer->start(1000);
        }
        else if (current_round == Settings::ShortBreak)
        {
            dial->setRange(0, Settings::short_break_time);
            short_break_timer->start(1000);
        }
        else if (current_round == Settings::LongBreak)
        {
            dial->setRange(0, Settings::long_break_time);
            long_break_timer->start(1000);
        }

        start_btn->setText(tr("Pause"));
        start_btn_state = Pause;
    }
    else
    {
        if (++pause_counter % 2 == 0)
        {
            if (current_round == Settings::Round)
                round_timer->start(1000);
            else if (current_round == Settings::ShortBreak)
                short_break_timer->start(1000);
            else if (current_round == Settings::LongBreak)
                long_break_timer->start(1000);

            start_btn->setText(tr("Pause"));
            start_btn_state = Pause;
        }
        else
        {
            short_break_timer->stop();
            round_timer->stop();
            start_btn->setText(tr("Continue"));
            start_btn_state = Continue;
        }
    }
}

// Зупинка таймеру
void MainWindow::stopTimer()
{
    round_timer->stop();
    short_break_timer->stop();
    long_break_timer->stop();
    timeout_counter	= 0;
    time_left->setText(convertTime(Settings::round_time - timeout_counter));
    dial->setValue(0);
    start_btn->setText(tr("Start"));
    start_btn_state = Start;
}

// Цикл програми під час раунда
void MainWindow::onRoundTimeout()
{
    updateStatistics();

    current_round = Settings::Round;
    setPalette(Settings::round_color);
    dial->setRange(0, Settings::round_time);

    dial->setValue(++timeout_counter);
    time_left->setText(convertTime(Settings::round_time - timeout_counter));

    if (timeout_counter > Settings::round_time)
    {
        Settings::total_rounds++;
        Settings::today_rounds++;
        Settings::weekly_rounds++;
        Settings::monthly_rounds++;
        Settings::annual_rounds++;

        round_timer->stop();
        timeout_counter	= 0;

        current_round = Settings::ShortBreak;
        setPalette(Settings::short_break_color);

        time_left->setText(convertTime(Settings::short_break_time - timeout_counter));
        dial->setRange(0, Settings::short_break_time);
        dial->setValue(0);

        if (first_round->isChecked())
        {
            first_round->setChecked(false);
            second_round->setChecked(true);

            Settings::short_break_sound.play();

            short_break_timer->start(1000);
        }
        else if (second_round->isChecked())
        {
            second_round->setChecked(false);
            third_round->setChecked(true);

            Settings::short_break_sound.play();

            short_break_timer->start(1000);
        }
        else if (third_round->isChecked())
        {
            third_round->setChecked(false);
            fourth_round->setChecked(true);

            Settings::short_break_sound.play();

            short_break_timer->start(1000);
        }
        else if (fourth_round->isChecked())
        {
            fourth_round->setChecked(false);
            first_round->setChecked(true);

            current_round = Settings::LongBreak;
            setPalette(Settings::long_break_color);
            dial->setRange(0, Settings::long_break_time);
            Settings::long_break_sound.play();

            time_left->setText(convertTime(Settings::long_break_time - timeout_counter));
            dial->setRange(0, Settings::long_break_time);

            long_break_timer->start(1000);
        }

        if (Settings::is_notification_enabled)
        {
            int current_break;
            if (current_round == Settings::ShortBreak)
            current_break = Settings::short_break_time;
            else if (current_round == Settings::LongBreak)
            current_break = Settings::long_break_time;

            QSystemTrayIcon *temp_tray = new QSystemTrayIcon(QIcon("./icons/icon2.ico"));
            temp_tray->show();
            temp_tray->showMessage(tr("Round has been ended"), tr("Now you can have a rest for ") +
                                          QString::number(current_break / Settings::SEC_IN_MIN) +
                                          tr(" minutes!"));
            delete temp_tray;
        }
    }
}

// Цикл програми під час короткої перерви
void MainWindow::onShortBreakTimeout()
{
    updateStatistics();

    dial->setValue(++timeout_counter);
    time_left->setText(convertTime(Settings::short_break_time - timeout_counter));

    if (timeout_counter > Settings::short_break_time)
    {
        if (Settings::is_notification_enabled)
        {
            QSystemTrayIcon *temp_tray = new QSystemTrayIcon(QIcon("./icons/icon2.ico"));
            temp_tray->show();
            temp_tray->showMessage(tr("Short break has been ended"), tr("Now you should focus on your task for ") +
                     QString::number(Settings::round_time / Settings::SEC_IN_MIN) +
                      tr(" minutes!"));
            delete temp_tray;
        }

        Settings::round_sound.play();
        short_break_timer->stop();
        timeout_counter = 0;

        current_round = Settings::Round;
        start_btn->setText(tr("Start"));
        start_btn_state = Start;
        setPalette(Settings::round_color);

        time_left->setText(convertTime(Settings::round_time - timeout_counter));
        dial->setRange(0, Settings::round_time);
        dial->setValue(0);
    }
}

// Цикл програми під час довгої перерви
void MainWindow::onLongBreakTimeout()
{
    updateStatistics();

    dial->setValue(++timeout_counter);
    time_left->setText(convertTime(Settings::long_break_time - timeout_counter));

    if (timeout_counter > Settings::long_break_time)
    {
        if (Settings::is_notification_enabled)
        {
            QSystemTrayIcon *temp_tray = new QSystemTrayIcon(QIcon("./icons/icon2.ico"));
            temp_tray->show();
            temp_tray->showMessage(tr("Long break has been ended"), tr("Now you should focus on your task for ") +
                 QString::number(Settings::round_time / Settings::SEC_IN_MIN) +
                  tr(" minutes!"));
            delete temp_tray;
        }

        Settings::round_sound.play();
        long_break_timer->stop();
        timeout_counter = 0;

        current_round = Settings::Round;
        start_btn->setText(tr("Start"));
        start_btn_state = Start;
        setPalette(Settings::round_color);

        time_left->setText(convertTime(Settings::round_time - timeout_counter));
        dial->setRange(0, Settings::round_time);
        dial->setValue(0);
    }
}

// Інтерактивна обробка візуального відображення таймеру
void MainWindow::onDialChange(int value)
{
    int difference = dial->value() - timeout_counter;
    Settings::total_seconds += difference;
    Settings::today_seconds += difference;
    Settings::weekly_seconds += difference;
    Settings::monthly_seconds += difference;
    Settings::annual_seconds += difference;

    if (current_round == Settings::Round)
    {
        timeout_counter = dial->value();
        time_left->setText(convertTime(Settings::round_time - value));
    }
    else if (current_round == Settings::ShortBreak)
    {
        timeout_counter = dial->value();
        time_left->setText(convertTime(Settings::short_break_time - value));
    }
    else if (current_round == Settings::LongBreak)
    {
        timeout_counter = dial->value();
        time_left->setText(convertTime(Settings::long_break_time - value));
    }
}

// Відкриття вікна налаштувань
void MainWindow::openSettings()
{
    Settings::round_sound.stop();
    Settings::short_break_sound.stop();
    Settings::long_break_sound.stop();

    SettingsForm *sf = new SettingsForm;
    sf->setModal(true);

    if (sf->exec() == QDialog::Accepted)
    {
        loadSettings();
        --timeout_counter;
        if (current_round == Settings::Pomodoro::Round)
            onRoundTimeout();
        else if (current_round == Settings::Pomodoro::ShortBreak)
            onShortBreakTimeout();
        else if (current_round == Settings::Pomodoro::LongBreak)
            onLongBreakTimeout();

        int translator_result;
        if (Settings::locale == "en")
            translator_result = translator.load("./translations/Pomodoro_en", ".");
        else if (Settings::locale == "ua")
            translator_result = translator.load("./translations/Pomodoro_ua", ".");
        else if (Settings::locale == "ru")
            translator_result = translator.load("./translations/Pomodoro_ru", ".");

        qApp->installTranslator(&translator);

        reloadScreen();
    }
}

// Відкриття вікна інформації про додаток
void MainWindow::openCredits()
{
    QMessageBox::information(this, tr("Credits"), tr("Version 2.3.1\n"
                                                     "Created by:\n"
                                                     "->Oleksii Paziura\n"));
}

// Відкриття вікна статистичних даних
void MainWindow::openStatistics()
{
    QMessageBox statistics;
    statistics.setWindowTitle(tr("Statistics"));
    statistics.setInformativeText(tr("Today rounds: ") + QString::number(Settings::today_rounds) +
                                  tr("\nToday seconds: ") + convertTime(Settings::today_seconds, true) +
                                  tr("\n\nWeekly rounds: ") + QString::number(Settings::weekly_rounds) +
                                  tr("\nWeekly seconds: ") + convertTime(Settings::weekly_seconds, true) +
                                  tr("\n\nMonthly rounds: ") + QString::number(Settings::monthly_rounds) +
                                  tr("\nMonthly seconds: ") + convertTime(Settings::monthly_seconds, true) +
                                  tr("\n\nAnnual rounds: ") + QString::number(Settings::annual_rounds) +
                                  tr("\nAnnual seconds: ") + convertTime(Settings::annual_seconds, true) +
                                  tr("\n\nTotal rounds: ") + QString::number(Settings::total_rounds) +
                                  tr("\nTotal seconds: ") + convertTime(Settings::total_seconds, true));
    statistics.setIcon(QMessageBox::Information);
    statistics.exec();
}

// Вихід з застосунку
void MainWindow::exitApplication()
{
    qApp->quit();
}

// Обробник натискання на іконку трею
void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        show();
        delete tray;
    }
}

// Обробник зміни теми
void MainWindow::changeTheme(Settings::Themes theme_name)
{
    if (theme_name == Settings::Default)
    {
        Settings::round_color = QPalette("#5499DE");
        Settings::short_break_color = QPalette("#66DE6C");
        Settings::long_break_color = QPalette("#DCDE66");
        Settings::current_font = QFont("Arial", 20);
        Settings::current_opacity = 0.97;
    }
    else if (theme_name == Settings::Contrast)
    {
        Settings::round_color = QPalette("#FF282C");
        Settings::short_break_color = QPalette("#7700FF");
        Settings::long_break_color = QPalette("#0888FF");
        Settings::current_font = QFont("Calibri", 20);
        Settings::current_opacity = 1;
    }
    else if (theme_name == Settings::SimpleLight)
    {
        Settings::round_color = QPalette("#D6D4DE");
        Settings::short_break_color = QPalette("#98AEDE");
        Settings::long_break_color = QPalette("#DEDEAD");
        Settings::current_font = QFont("Calibri", 20);
        Settings::current_opacity = 1;
    }
    else if (theme_name == Settings::PomodoroMode)
    {
        Settings::round_color = QPalette("#FF3F4C");
        Settings::short_break_color = QPalette("#FF6C67");
        Settings::long_break_color = QPalette("#FF7354");
        Settings::current_font = QFont("Forte", 20);
        Settings::current_opacity = 1;
    }

    time_left->setFont(Settings::current_font);
    if (current_round == Settings::Round)
        setPalette(Settings::round_color);
    else if (current_round == Settings::ShortBreak)
        setPalette(Settings::short_break_color);
    else if (current_round == Settings::LongBreak)
        setPalette(Settings::long_break_color);

    QSettings settings("PMDR0", "base");
    settings.beginGroup("Themes");
    settings.setValue("currentTheme", theme_name);
    settings.endGroup();
}

// Обробник закриття вікна
void MainWindow::closeEvent(QCloseEvent *event)
{
    loadStatistics();

    if (Settings::is_tray_enabled == Settings::Postponed)
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Rolling up into a tray"));
        msg.setInformativeText(tr("Do you want to roll this window up into a tray?"));
        msg.setIcon(QMessageBox::Question);
        QPushButton *yes = msg.addButton(tr("Yes"), QMessageBox::YesRole);
        QPushButton *no = msg.addButton(tr("No"), QMessageBox::NoRole);
        QPushButton *postpone = msg.addButton(tr("No but ask me later"), QMessageBox::RejectRole);
        msg.exec();

        if (msg.clickedButton() == yes)
            Settings::is_tray_enabled = Settings::Enabled;
        else if (msg.clickedButton() == no)
            Settings::is_tray_enabled = Settings::Disabled;
        else if (msg.clickedButton() == postpone)
            Settings::is_tray_enabled = Settings::Postponed;

        QSettings settings("PMDR0", "base");
        settings.beginGroup("Tray");
        settings.setValue("isTrayEnabled", Settings::is_tray_enabled);
        settings.endGroup();
    }
    if (Settings::is_tray_enabled == Settings::Enabled)
    {
        hide();
        trayInit();
        tray->show();

        if (Settings::is_notification_enabled)
            tray->showMessage(tr("Pomodoro"), tr("Program is in tray"));

        event->ignore();
    }
}
