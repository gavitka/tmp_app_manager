#pragma once

#include "pch.h"

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QDebug>
#include "backend.h"

#include "hooks_dll/hook.h"

class KhEventFilter : public QAbstractNativeEventFilter
{

public:
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE
    {
        Q_UNUSED(eventType)
        MSG* msg = (MSG*)(message);
        if(msg->message == WM_KEYSTROKE) {
            BackEnd::getInstance()->kick();
        }
        return false;
    }
};
