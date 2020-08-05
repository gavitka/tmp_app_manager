#pragma once

#include <QObject>
#include <QVector>

typedef struct _APPDATA {
    HWND hwnd;
    QString name;
    QString exename;
    bool is64;
    bool selected;
} APPDATA;

typedef struct _HOOKDATA {
    HHOOK hook;
    HMODULE hdll;
} HOOKDATA;

class AppManager : public QObject
{
    Q_OBJECT

public:

    AppManager(QObject* parent = nullptr);

    void add(HWND hwnd);


    void select(int i);
    int size();
    const APPDATA at(int i);

public slots:

    void installHooks();
    void uninstallHooks();
    void refresh();

signals:

    void listChanged();

private:

    void InstallSingleHook(HWND hwnd);

    QVector<APPDATA> m_data;
    bool m_hooksactive = false;
    std::vector<HOOKDATA> m_hooks;

};


BOOL CALLBACK windowListCallback(HWND hwnd, LPARAM lParam);
