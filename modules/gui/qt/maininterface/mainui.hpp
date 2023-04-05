#ifndef MAINUI_HPP
#define MAINUI_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <QObject>
#include <QQmlEngine>
#include <QQmlError>
#include <QQuickItem>

class VLCVarChoiceModel;
class MainCtx;
struct qt_intf_t;

class MainUI : public QObject
{
    Q_OBJECT

public:
    explicit MainUI(qt_intf_t *_p_intf, MainCtx* mainCtx, QWindow* interfaceWindow, QObject *parent = nullptr);
    ~MainUI();

    bool setup(QQmlEngine* engine);

    inline QQmlComponent* getComponent() const {return m_component;}
    Q_REQUIRED_RESULT
    QQuickItem* createRootItem();

private slots:
    void onQmlWarning(const QList<QQmlError>& errors);

private:
    void registerQMLTypes();
    static QObject* getMainCtxInstance(QQmlEngine *, QJSEngine *);

    qt_intf_t* m_intf = nullptr;
    MainCtx* m_mainCtx = nullptr;
    QWindow*       m_interfaceWindow = nullptr;

    QQmlComponent* m_component = nullptr;
    QQuickItem* m_rootItem = nullptr;
};

#endif // MAINUI_HPP
