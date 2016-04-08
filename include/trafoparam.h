#ifndef TRAFOPARAM_H
#define TRAFOPARAM_H

#include <QObject>
#include <QPointer>
#include <QtCore/qmath.h>
#include <QDateTime>

#include "feature.h"
#include "statistic.h"
#include "oivec.h"
#include "oimat.h"

namespace oi{

using namespace math;

class CoordinateSystem;

/*!
 * \brief The TrafoParam class
 */
class OI_CORE_EXPORT TrafoParam : public Feature
{
    friend class OiJob;
    Q_OBJECT

public:
    explicit TrafoParam(QObject *parent = 0);

    TrafoParam(const TrafoParam &copy, QObject *parent = 0);

    TrafoParam &operator=(const TrafoParam &copy);

    ~TrafoParam();

    //#################################
    //get or set trafo param attributes
    //#################################

    //used state
    const bool &getIsUsed() const;
    void setIsUsed(const bool &isUsed);

    //coordinate systems
    const QPointer<CoordinateSystem> &getStartSystem() const;
    const QPointer<CoordinateSystem> &getDestinationSystem() const;
    bool setCoordinateSystems(const QPointer<CoordinateSystem> &from,
                              const QPointer<CoordinateSystem> &to);

    //movement
    const bool &getIsMovement() const;
    void setIsMovement(const bool &isMovement);

    //bundle
    const bool &getIsBundle() const;
    void setIsBundle(const bool &isBundle);

    //time stamp
    const QDateTime &getValidTime() const;
    void setValidTime(const QDateTime &validTime);

    //statistic
    const Statistic &getStatistic() const;
    void setStatistic(const Statistic &statistic);

    //datum transformation
    const bool &getIsDatumTrafo() const;
    void setIsDatumTrafo(const bool &isDatumTrafo);

    //bundle transformation
    const QPointer<TrafoParam> &getBundleParent() const;
    void setBundleParent(const QPointer<TrafoParam> &parent);
    const QList<QPointer<TrafoParam> > &getBundleChildren() const;
    void setBundleChildren(const QList<QPointer<TrafoParam> > &children);

    //####################################
    //get or set transformation parameters
    //####################################

    //getter
    const OiMat &getHomogenMatrix() const;
    const OiVec &getTranslation() const;
    const OiVec &getRotation() const;
    const OiVec &getScale() const;

    //setter
    bool setTransformationParameters(const OiVec &rotation, const OiVec &translation, const OiVec &scale);
    bool setTransformationParameters(const OiMat &rotation, const OiMat &translation, const OiMat &scale);
    bool setTransformationParameters(const OiMat &homogenMatrix);

    //#############################
    //get or set unknown parameters
    //#############################

    //unknown parameters
    virtual QMap<TrafoParamParameters, QString> getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits,
                                                          const QMap<DimensionType, int> &displayDigits) const;
    virtual void setUnknownParameters(const QMap<TrafoParamParameters, double> &parameters);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

    QString getDisplayStDev(const UnitType &type, const int &digits) const;

    QString getDisplayStartSystem() const;
    QString getDisplayDestinationSystem() const;
    QString getDisplayTranslationX(const UnitType &type, const int &digits) const;
    QString getDisplayTranslationY(const UnitType &type, const int &digits) const;
    QString getDisplayTranslationZ(const UnitType &type, const int &digits) const;
    QString getDisplayRotationX(const UnitType &type, const int &digits) const;
    QString getDisplayRotationY(const UnitType &type, const int &digits) const;
    QString getDisplayRotationZ(const UnitType &type, const int &digits) const;
    QString getDisplayScaleX(const int &digits) const;
    QString getDisplayScaleY(const int &digits) const;
    QString getDisplayScaleZ(const int &digits) const;
    QString getDisplayIsUsed() const;
    QString getDisplayValidTime() const;
    QString getDisplayIsMovement() const;
    QString getDisplayIsDatumTransformation() const;

signals:

    //###########################################
    //signals to inform about trafo param changes
    //###########################################

    void transformationParameterChanged(const int &tpId);
    void coordinateSystemsChanged(const int &tpId);
    void isUsedChanged(const int &tpId);
    void validTimeChanged(const int &tpId);
    void isMovementChanged(const int &tpId);
    void isBundleChanged(const int &tpId);
    void bundleParentChanged(const int &tpId);
    void bundleChildrenChanged(const int &tpId);

private:

    //##############################
    //general trafo param attributes
    //##############################

    //coordinate systems
    QPointer<CoordinateSystem> from;
    QPointer<CoordinateSystem> to;

    //statistic
    Statistic statistic;

    bool isDatumTrafo;
    bool isUsed; //true if this trafo param object is used for transformation (false if not)
    bool isBundleTrafo; //true if the transformation represents a bundle adjustment

    //movement
    bool isMovement;
    QDateTime validTime; //used for movements to decide which observations shall be "moved"

    //bundle transformation
    QPointer<TrafoParam> bundleParent; //parent of each bundle transformation
    QList<QPointer<TrafoParam> > bundleChildren; //all bundle transformations with this as parent

    //#########################
    //transformation parameters
    //#########################

    OiVec translation;
    OiVec rotation;
    OiVec scale;

    OiMat homogenMatrix;

};

}

#endif // TRAFOPARAM_H
