#include "backend.h"

#include "hooks_dll/hook.h"

BackEnd* BackEnd::m_instance = nullptr;


BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
    m_appmanager = new AppManager();

    m_timer.setInterval(200);
    connect(&m_timer, &QTimer::timeout, this, &BackEnd::colorChange);
    m_timer.start();

    QTimer::singleShot(1000, m_appmanager, &AppManager::refresh);
}


QObject* BackEnd::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return (QObject*)getInstance();
}


QString BackEnd::text()
{
    return m_text;
}


void BackEnd::setText(QString value)
{
    m_text = value;
    emit textChanged();
}

QColor BackEnd::color()
{
    return m_color;
}

void BackEnd::setColor(QColor value)
{
    m_color = value;
    emit colorChanged();
}


AppManager *BackEnd::appManager()
{
    return m_appmanager;
}

void BackEnd::refreshApps()
{
    m_appmanager->refresh();
}

void BackEnd::colorChange()
{
    setColor(QColor("green"));
}


BackEnd *BackEnd::getInstance()
{
    if(!m_instance) {
        m_instance = new BackEnd();
    }
    return m_instance;
}


void BackEnd::kick()
{
    setColor(QColor("red"));
}
