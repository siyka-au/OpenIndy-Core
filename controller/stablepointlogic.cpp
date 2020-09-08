#include "stablepointlogic.h"

using namespace oi;
using namespace oi::math;

StablePointLogic::StablePointLogic(MeasurementConfig config, QObject *parent) : QObject(parent),
    pointIsStable(false), config(config)
{

}

StablePointLogic::StablePointLogic(const StablePointLogic &copy, QObject *parent) : QObject(parent), pointIsStable(copy.pointIsStable), config(copy.config){
}

StablePointLogic &StablePointLogic::operator=(const StablePointLogic &copy) {
    this->pointIsStable = copy.pointIsStable;
    this->config = copy.config;
    return *this;
}

/*!
 * \brief WatchWindowDialog::~WatchWindowDialog
 */
StablePointLogic::~StablePointLogic(){
    //
}

// signal  controller
void StablePointLogic::checkStablePoint() {
    qDebug() << "checkStablePoint";

    if(this->pointIsStable) {
        this->pointIsStable = false;

        emit this->startMeasurement();
    }
}

/*!
 * \brief WatchWindowDialog::realTimeReading
 * \param reading
 */
void StablePointLogic::realTimeReading(const QVariantMap &reading){

    qDebug() << "realTimeReading" << reading;

    if(!reading.contains("x") || !reading.contains("y") || !reading.contains("z")){
        return;
    }

    OiVec xyz(3);
    xyz.setAt(0, reading.value("x").toDouble());
    xyz.setAt(1, reading.value("y").toDouble());
    xyz.setAt(2, reading.value("z").toDouble());

    if(lastXyz.getSize() > 0) { // 2-n call
        double dot;
        OiVec v = xyz - lastXyz;
        OiVec::dot(dot, v, v);
        double distance = qSqrt(dot);

        qDebug() << "distance" << distance;
        if(distance <= this->config.getStablePointMinDistance()) {
            // timer starten
            this->elapsed.restart();
            // if time up
            this->pointIsStable = true;
        } else {
            this->pointIsStable = false;
            //this->elapsed
            // timer stoppen
        }

    }
    lastXyz = xyz;

}

void StablePointLogic::stopStablePointMeasurement(){
    emit this->stopStreaming();

}

void StablePointLogic::startStablePointMeasurement(){
    qDebug() << "start timer";
    QPointer<QTimer> timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkStablePoint()));
    timer->start(250);

    emit this->startStreaming(ReadingTypes::eCartesianReading);
}
