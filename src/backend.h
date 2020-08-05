#pragma once

#include "pch.h"

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QColor>
#include <QTimer>

#include "appmanager.h"

class BackEnd: public QObject
{
Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(AppManager* appManager READ appManager NOTIFY appManagerChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:

    static BackEnd *getInstance();
    static QObject* qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    QString text();
    void setText(QString value);

    QColor color();
    void setColor(QColor value);

    AppManager *appManager();

    void kick();

public slots:

    void refreshApps();
    void colorChange();

signals:

    void textChanged();
    void appManagerChanged();
    void colorChanged();

private:

    BackEnd(QObject* parent = nullptr);

    static BackEnd* m_instance;
    QString m_text;
    AppManager *m_appmanager;
    QColor m_color;
    QTimer m_timer;
};
