#include "LCSTools.h"
#include "LCSToolsImpl.h"
#include <QDebug>
#include <chrono>

LCSTools::LCSTools(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<LCSState>("LCSState");
    thread_ = new QThread();
    worker_ = new LCSToolsImpl();
    worker_->moveToThread(thread_);

    connect(worker_, &LCSToolsImpl::stateChanged, this, &LCSTools::setState);
    connect(worker_, &LCSToolsImpl::statusChanged, this, &LCSTools::setStatus);
    connect(worker_, &LCSToolsImpl::leaguePathChanged, this, &LCSTools::setLeaguePath);
    connect(worker_, &LCSToolsImpl::reportError, this, &LCSTools::reportError);

    connect(worker_, &LCSToolsImpl::blacklistChanged, this, &LCSTools::blacklistChanged);
    connect(worker_, &LCSToolsImpl::ignorebadChanged, this, &LCSTools::ignorebadChanged);
    connect(worker_, &LCSToolsImpl::progressStart, this, &LCSTools::progressStart);
    connect(worker_, &LCSToolsImpl::progressItems, this, &LCSTools::progressItems);
    connect(worker_, &LCSToolsImpl::progressData, this, &LCSTools::progressData);
    connect(worker_, &LCSToolsImpl::progressEnd, this, &LCSTools::progressEnd);
    connect(worker_, &LCSToolsImpl::initialized, this, &LCSTools::initialized);
    connect(worker_, &LCSToolsImpl::modDeleted, this, &LCSTools::modDeleted);
    connect(worker_, &LCSToolsImpl::installedMod, this, &LCSTools::installedMod);
    connect(worker_, &LCSToolsImpl::profileSaved, this, &LCSTools::profileSaved);
    connect(worker_, &LCSToolsImpl::profileLoaded, this, &LCSTools::profileLoaded);
    connect(worker_, &LCSToolsImpl::profileDeleted, this, &LCSTools::profileDeleted);
    connect(worker_, &LCSToolsImpl::updateProfileStatus, this, &LCSTools::updateProfileStatus);
    connect(worker_, &LCSToolsImpl::modCreated, this, &LCSTools::modCreated);
    connect(worker_, &LCSToolsImpl::modEditStarted, this, &LCSTools::modEditStarted);
    connect(worker_, &LCSToolsImpl::modInfoChanged, this, &LCSTools::modInfoChanged);
    connect(worker_, &LCSToolsImpl::modWadsAdded, this, &LCSTools::modWadsAdded);
    connect(worker_, &LCSToolsImpl::modWadsRemoved, this, &LCSTools::modWadsRemoved);

    connect(this, &LCSTools::changeLeaguePath, worker_, &LCSToolsImpl::changeLeaguePath);
    connect(this, &LCSTools::changeBlacklist, worker_, &LCSToolsImpl::changeBlacklist);
    connect(this, &LCSTools::changeIgnorebad, worker_, &LCSToolsImpl::changeIgnorebad);
    connect(this, &LCSTools::init, worker_, &LCSToolsImpl::init);
    connect(this, &LCSTools::deleteMod, worker_, &LCSToolsImpl::deleteMod);
    connect(this, &LCSTools::exportMod, worker_, &LCSToolsImpl::exportMod);
    connect(this, &LCSTools::installFantomeZip, worker_, &LCSToolsImpl::installFantomeZip);
    connect(this, &LCSTools::saveProfile, worker_, &LCSToolsImpl::saveProfile);
    connect(this, &LCSTools::loadProfile, worker_, &LCSToolsImpl::loadProfile);
    connect(this, &LCSTools::deleteProfile, worker_, &LCSToolsImpl::deleteProfile);
    connect(this, &LCSTools::runProfile, worker_, &LCSToolsImpl::runProfile);
    connect(this, &LCSTools::stopProfile, worker_, &LCSToolsImpl::stopProfile);
    connect(this, &LCSTools::makeMod, worker_, &LCSToolsImpl::makeMod);
    connect(this, &LCSTools::startEditMod, worker_, &LCSToolsImpl::startEditMod);
    connect(this, &LCSTools::changeModInfo, worker_, &LCSToolsImpl::changeModInfo);
    connect(this, &LCSTools::addModWads, worker_, &LCSToolsImpl::addModWads);
    connect(this, &LCSTools::removeModWads, worker_, &LCSToolsImpl::removeModWads);

    connect(this, &LCSTools::destroyed, worker_, &LCSToolsImpl::deleteLater);
    connect(worker_, &LCSTools::destroyed, thread_, &QThread::deleteLater);

    thread_->start();
}

LCSTools::~LCSTools(){}

LCSToolsImpl::LCSState LCSTools::getState() {
    return state_;
}

QString LCSTools::getStatus() {
    return status_;
}

void LCSTools::setState(LCSState value) {
    if (state_ != value) {
        state_ = value;
        emit stateChanged(value);
    }
}

void LCSTools::setStatus(QString status) {
    if (status_ != status) {
        status_ = status;
        emit statusChanged(status);
    }
}

QString LCSTools::getLeaguePath() {
    return leaguePath_;
}

void LCSTools::setLeaguePath(QString value) {
    if (leaguePath_ != value) {
        leaguePath_ = value;
        emit leaguePathChanged(value);
    }
}

