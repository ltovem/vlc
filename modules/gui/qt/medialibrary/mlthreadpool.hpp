// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef MLTHREADPOOL_HPP
#define MLTHREADPOOL_HPP

#include <QObject>
#include <QRunnable>
#include <QQueue>
#include <QMap>
#include <QThreadPool>
#include <QMutex>

class MLThreadPool;

//internal task MLThreadPool
class MLThreadPoolSerialTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    MLThreadPoolSerialTask(MLThreadPool* parent, const QString& queueName);

    void run() override;

private:
    MLThreadPool* m_parent = nullptr;
    QString m_queueName;
};

/**
 * @brief The MLThreadPool act like a QThreadPool, with the difference that it allows tasks
 * to be run sequentially by specifying a queue name when starting the task.
 */
class MLThreadPool
{
public:
    explicit MLThreadPool();
    ~MLThreadPool();


    void setMaxThreadCount(size_t threadCount);

    /**
     * @brief start enqueue a QRunnable to be executed on the threadpool
     * @param task is the task to enqueue
     * @param queue, the name of the queue, all task with the same queue name will be
     * ran sequentially, if queue is null, task will be run without additional specific
     * constraint (like on QThreadPool)
     */
    void start(QRunnable* task, const char* queue = nullptr);

    /**
     * @brief tryTake atempt to the specified task from the queue if the task has not started
     * @return true if the task has been removed from the queue and was not started yet
     */
    bool tryTake(QRunnable* task);

private:
    friend class MLThreadPoolSerialTask;

    QRunnable* getNextTaskFromQueue(const QString& queueName);

    QMutex m_lock;
    QThreadPool m_threadpool;
    QMap<QString, QQueue<QRunnable*>> m_serialTasks;
};

#endif // MLTHREADPOOL_HPP
