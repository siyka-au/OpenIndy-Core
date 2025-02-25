#ifndef FUNCTION_H
#define FUNCTION_H

#include <QMap>
#include <QMultiMap>
#include <QStringList>
#include <QtXml>
#include <QObject>
#include <QPointer>

#include "point.h"
#include "line.h"
#include "plane.h"
#include "sphere.h"
#include "circle.h"
#include "cone.h"
#include "cylinder.h"
#include "ellipsoid.h"
#include "hyperboloid.h"
#include "nurbs.h"
#include "paraboloid.h"
#include "pointcloud.h"
#include "station.h"
#include "coordinatesystem.h"
#include "trafoparam.h"
#include "observation.h"
#include "statistic.h"
#include "scalarentitydistance.h"
#include "scalarentityangle.h"
#include "scalarentitytemperature.h"
#include "scalarentitymeasurementseries.h"
#include "reading.h"
#include "slottedhole.h"
#include "torus.h"
#include "ellipse.h"
#include "pluginmetadata.h"
#include "featurewrapper.h"
#include "types.h"
#include "util.h"

namespace oi{

//############################################
//helper classes to define function parameters
//############################################

enum InputElementKey {
    eNotSet = -1,
    eDefault = 0,
    // ...
    eDummyPoint = 10000
};

/*!
 * \brief The NeededElement class
 * Save the needed element types that are necessary to solve a function
 */
class NeededElement{
public:
    ElementTypes typeOfElement; //type of the needed input element
    QString description; //optional description for the needed element to show in the GUI
    bool infinite; //only one element or as many elements as one wants?
    InputElementKey key = eNotSet; // if key not set InputElementKey is NeedeElement index
};

/*!
 * \brief The InputElement class
 * Save the input elements that are used to solve a function
 */
class InputElement{
public:
    InputElement() : isUsed(true), shouldBeUsed(true), id(-1){}
    InputElement(const int &id) : isUsed(true), shouldBeUsed(true), id(id){}

    //! custom comparison operator to compare input elements by their id
    bool operator==(const InputElement &other){
        return this->id == other.id;
    }

    int id; //the id of the element
    QString label;
    ElementTypes typeOfElement; //the type of the element

    //single parameters of this element that shall not be used in function calculation
    //e.g. ignore the radius of an input sphere
    QList<GeometryParameters> ignoredDestinationParams;

    //single parameters of the feature to be calculated that this element shall not have effect on
    //e.g. use a point only for a circle's plane, not the radius
    QList<GeometryParameters> ignoredTargetParams;

    bool shouldBeUsed; //true if this element should be used in function calculation (user specified)
    bool isUsed; //true if this element is used in function calculation (plugin specified)

    //element pointers (only valid for the specified element type)
    QPointer<Station> station;
    QPointer<CoordinateSystem> coordSystem;
    QPointer<TrafoParam> trafoParam;

    QPointer<Circle> circle;
    QPointer<Cone> cone;
    QPointer<Cylinder> cylinder;
    QPointer<Ellipse> ellipse;
    QPointer<Ellipsoid> ellipsoid;
    QPointer<Hyperboloid> hyperboloid;
    QPointer<Line> line;
    QPointer<Nurbs> nurbs;
    QPointer<Paraboloid> paraboloid;
    QPointer<Plane> plane;
    QPointer<Point> point;
    QPointer<PointCloud> pointCloud;
    QPointer<ScalarEntityAngle> scalarEntityAngle;
    QPointer<ScalarEntityDistance> scalarEntityDistance;
    QPointer<ScalarEntityMeasurementSeries> scalarEntityMeasurementSeries;
    QPointer<ScalarEntityTemperature> scalarEntityTemperature;
    QPointer<SlottedHole> slottedHole;
    QPointer<Sphere> sphere;
    QPointer<Torus> torus;

    QPointer<Geometry> geometry;

    QPointer<Observation> observation;
    QPointer<Reading> polarReading;
    QPointer<Reading> directionReading;
    QPointer<Reading> distanceReading;
    QPointer<Reading> cartesianReading;
    QPointer<Reading> temperatureReading;
    QPointer<Reading> levelReading;
    QPointer<Reading> undefinedReading;
};

/*!
 * \brief The FixedParameter class
 * Save a value for each parameter that shall be fixed
 */
class FixedParameter{
public:
    FixedParameter() : value(0.0){}
    FixedParameter(const GeometryParameters &param) : value(0.0), parameter(param){}

    //! custom comparison operator to compare fixed parameters
    bool operator==(const FixedParameter &other){
        if(this->parameter == other.parameter){
            return true;
        }
        return false;
    }

    GeometryParameters parameter;
    double value;
};

//#####################
//function class itself
//#####################

/*!
 * \brief The Function class
 */
class OI_CORE_EXPORT Function : public QObject
{
    friend class Feature;
    Q_OBJECT

public:
    Function(QObject *parent = 0);

    virtual ~Function();

    //##############################
    //function initialization method
    //##############################

    virtual void init();

    //###################################################################
    //public exec method that calls the appropriate protected exec method
    //###################################################################

    virtual bool exec(const QPointer<FeatureWrapper> &feature);

    //#############################################################
    //methods to get or set further information to solve a function
    //#############################################################

    const QList<NeededElement> &getNeededElements() const;
    const QList<FeatureTypes> &getApplicableFor() const;
    const PluginMetaData &getMetaData() const;

    const QMap<QString, int> &getIntegerParameter() const;
    const QMap<QString, double> &getDoubleParameter() const;
    const QMultiMap<QString, QString> &getStringParameter() const;

    void setScalarInputParams(const ScalarInputParams &params);
    const ScalarInputParams &getScalarInputParams();

    const QList<FixedParameter> &getFixedParameters() const;
    void fixParameter(const FixedParameter &parameter);
    void unfixParameter(const GeometryParameters &parameter);
    void unfixAllParameters();

    //####################
    //get function results
    //####################

    const QStringList &getResultProtocol() const;

    const Statistic &getStatistic() const;

    //###############
    //general getters
    //###############

    const int &getId() const;

    const bool &getSupportWeights();
    void setSupportWeights(bool supports);

    //###################
    //get or set elements
    //###################

    const QMap<int, QList<InputElement> > &getInputElements() const;
    InputElement getInputElement(const int &id) const;
    InputElement getInputElement(const int &id, const int &position) const;
    bool hasInputElement(const int &id) const;
    void addInputElement(const InputElement &element, const int &position);
    void removeInputElement(const int &id, const int &position);
    void removeInputElement(const int &id);
    void replaceInputElement(const InputElement &element, const int &position);

    //#################################
    //mark elements as used or not used
    //#################################

    bool getIsUsed(const int &position, const int &id);
    bool getShouldBeUsed(const int &position, const int &id);
    void setShouldBeUsed(const int &position, const int &id, const bool &state);

    //#############
    //clear results
    //#############

    virtual void clear();
    virtual void clearResults();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

signals:

    //################################################
    //signals to inform OpenIndy about function issues
    //################################################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

    void inputElementsChanged();
    void scalarInputParametersChanged();

protected:

    //############
    //exec methods
    //############

    virtual bool exec(Station &station);
    virtual bool exec(CoordinateSystem &coordinateSystem);
    virtual bool exec(TrafoParam &trafoParam);

    virtual bool exec(Circle &circle);
    virtual bool exec(Cone &cone);
    virtual bool exec(Cylinder &cylinder);
    virtual bool exec(Ellipse &ellipse);
    virtual bool exec(Ellipsoid &ellipsoid);
    virtual bool exec(Hyperboloid &hyperboloid);
    virtual bool exec(Line &line);
    virtual bool exec(Nurbs &nurbs);
    virtual bool exec(Paraboloid &paraboloid);
    virtual bool exec(Plane &plane);
    virtual bool exec(Point &point);
    virtual bool exec(PointCloud &pointCloud);
    virtual bool exec(ScalarEntityAngle &angle);
    virtual bool exec(ScalarEntityDistance &distance);
    virtual bool exec(ScalarEntityMeasurementSeries &measurementSeries);
    virtual bool exec(ScalarEntityTemperature &temperature);
    virtual bool exec(SlottedHole &slottedHole);
    virtual bool exec(Sphere &sphere);
    virtual bool exec(Torus &torus);

    //#################################
    //mark elements as used or not used
    //#################################

    void setIsUsed(const int &position, const int &id, const bool &state);

    //###########################
    //input and output parameters
    //###########################

    //meta information about the function (to tell OpenIndy what this function does)
    QList<NeededElement> neededElements;
    QList<FeatureTypes> applicableFor;
    PluginMetaData metaData;

    //the needed scalar input parameters including default values
    QMap<QString, int> integerParameters;
    QMap<QString, double> doubleParameters;
    QMultiMap<QString, QString> stringParameters;

    //user defined scalar input parameters
    ScalarInputParams scalarInputParams;

    //parameters of a geometry that should have a fixed value
    QList<FixedParameter> fixedParameters;

    QStringList resultProtocol;

    //##################
    //general attributes
    //##################

    int id;

    bool supportsWeights;

    Statistic statistic;

    QMap<int, QList<InputElement> > inputElements;

    void filterObservations(QList<QPointer<Observation> > &allUsableObservations, QList<QPointer<Observation> > &inputObservations);
    void addDisplayResidual(int elementId, double vr);
    void addDisplayResidual(int elementId, double vx, double vy, double vz, double v);
    void addDisplayResidual(int elementId, double vx, double vy, double vz, double v, double vi, double vj, double vk);

};

}

Q_DECLARE_METATYPE( oi::Function* )

#ifndef STR
#define STR(x) #x
#endif
#ifndef STR2
#define STR2(x) STR(x)
#endif

#ifndef CONCAT
#define CONCAT(a, b) a ## b
#endif
#ifndef CONCAT2
#define CONCAT2(a, b) CONCAT(a, b)
#endif

#define FUNCTION_PREFIX de.openIndy.plugin.function.v
#define Function_iid STR2(CONCAT2(FUNCTION_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // FUNCTION_H
