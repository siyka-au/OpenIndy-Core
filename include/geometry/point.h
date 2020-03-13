#ifndef POINT_H
#define POINT_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"

namespace oi{

/*!
 * \brief The Point class
 */
class OI_CORE_EXPORT Point : public Geometry
{
    friend class Station;
    Q_OBJECT

public:
    Point(const bool &isNominal, QObject *parent = 0);
    Point(const bool &isNominal, const Position &xyz, QObject *parent = 0);
    Point(int id, const bool &isNominal, const Position &xyz, QObject *parent = 0);

    Point(const Point &copy, QObject *parent = 0);

    Point &operator=(const Point &copy);

    ~Point();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum PointUnknowns{
        unknownCenterX = 0,
        unknownCenterY,
        unknownCenterZ
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasPosition() const;

    //###########################
    //get or set point parameters
    //###########################

    const Position &getPosition() const;

    void setPoint(const Position &xyz);

    const double &getDistance() const;
    void setDistance(const double distance);

    //#############################
    //get or set unknown parameters
    //#############################

    virtual QMap<GeometryParameters, QString> getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits,
                                                          const QMap<DimensionType, int> &displayDigits) const;
    virtual void setUnknownParameters(const QMap<GeometryParameters, double> &parameters);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    QString getDisplayDistance(const UnitType &type, const int &digits, const bool &showDiff) const;
    double distance; // depends on function
};

}

#endif // POINT_H
