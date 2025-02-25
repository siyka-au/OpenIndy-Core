#ifndef OIJOB_H
#define OIJOB_H

#include <QObject>
#include <QPointer>
#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QMultiMap>
#include <QIODevice>
#include <QSet>
#include <QRegExp>
#include <QStringRef>

#include "featurecontainer.h"
#include "featureattributes.h"
#include "function.h"
#include "types.h"
#include "util.h"

class ProjectExchanger;

namespace oi{

enum CompatibilyCheckResult{
    eCheckResult_match= 0,
    eCheckResult_job_wo_valid_version,
    eCheckResult_oi_wo_valid_version,
    eCheckResult_oi_lt_job,
    eCheckResult_oi_gt_job,
    eCheckResult_job_lt_oi_22_1
};

/*!
 * \brief The OiJob class
 * Represents an OpenIndy job (holds all features and active states)
 */
class OI_CORE_EXPORT OiJob : public QObject
{
    friend class ::ProjectExchanger;
    Q_OBJECT

public:
    explicit OiJob(QObject *parent = 0);

    ~OiJob();

    //####################################
    //get or set meta information of a job
    //####################################

    const QString &getJobName() const;
    void setJobName(const QString &jobName);

    const QPointer<QFileDevice> &getJobDevice() const;
    void setJobDevice(const QPointer<QFileDevice> &jobDevice);

    void setDigest(const QString &);
    const QString &getDigest() const;

    const CompatibilyCheckResult checkCompatibilty() const;
    void setLoadedProjectVersion(const QString &loadedVersion);

    int generateUniqueId();

    bool validateFeatureName(const QString &name, const FeatureTypes &type, const bool &isNominal = false,
                             const QPointer<CoordinateSystem> &nominalSystem = QPointer<CoordinateSystem>(NULL));

    //###########################################
    //methods to access the current feature state
    //###########################################

    //number of features
    int getFeatureCount() const;
    int getGeometryCount() const;
    int getFeatureCount(const FeatureTypes &type) const;
    int getFeatureCount(const QString &group) const;
    int getFeatureCount(const QPair<QString, bool> &mConfig) const;

    //access a list of all feature ids and names
    const QList<int> &getFeatureIdList() const;
    const QStringList &getFeatureNameList() const;
    const QStringList &getFeatureGroupList() const;
    const QList<QPair<QString, bool> > &getUsedMeasurementConfigs() const;

    //access feature lists
    const QList<QPointer<FeatureWrapper> > &getFeaturesList() const;
    const QList<QPointer<CoordinateSystem> > &getCoordinateSystemsList() const;
    QList<QPointer<CoordinateSystem> > getStationSystemsList() const;
    QList<QPointer<CoordinateSystem> > getBundleSystemList() const;
    const QList<QPointer<Station> > &getStationsList() const;
    const QList<QPointer<TrafoParam> > &getTransformationParametersList() const;
    const QList<QPointer<FeatureWrapper> > &getGeometriesList() const;

    //access specific features
    QPointer<FeatureWrapper> getFeatureById(const int &featureId) const;
    QList<QPointer<FeatureWrapper> > getFeaturesByName(const QString &name, const bool startWith = false) const;
    QList<QPointer<FeatureWrapper> > getFeaturesByGroup(const QString &group) const;
    QList<QPointer<FeatureWrapper> > getFeaturesByType(const FeatureTypes &type) const;
    QList<QPointer<Geometry> > getGeometriesByMConfig(const QPair<QString, bool> &mConfig) const;

    //access active features
    const QPointer<FeatureWrapper> &getActiveFeature() const;
    const QPointer<Station> &getActiveStation() const;
    const QPointer<CoordinateSystem> &getActiveCoordinateSystem() const;

    //get or set feature groups
    const QString &getActiveGroup() const;
    bool setActiveGroup(const QString &group);

    //######################
    //add or remove features
    //######################

    //add features
    bool addFeature(const QPointer<FeatureWrapper> &feature);
    QList<QPointer<FeatureWrapper> > addFeatures(const FeatureAttributes &fAttr);
    bool addFeatures(const QList<QPointer<FeatureWrapper> > &features, bool overwrite =false);

    //remove features
    bool removeFeature(const int &featureId);
    bool removeFeature(const QPointer<FeatureWrapper> &feature);
    bool removeFeatures(const QSet<int> &featureIds);
    bool removeFeatures(const QList<QPointer<FeatureWrapper> > &features);
    void removeAll();

    //##########################################
    //add or remove functions and input elements
    //##########################################

    //add or remove functions
    void addFunction(const QPointer<Function> &function);
    void removeFunction(const int &functionIndex);

    //add input elements to a feature's function
    void addInputObservation(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const QPointer<Observation> &observation);
    void addInputReading(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const QPointer<Reading> &reading);
    void addInputFeature(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const QPointer<FeatureWrapper> &feature);

    //remove input elements from a feature's function
    void removeInputElement(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const int &elementId);

    //##########################
    //add or remove observations
    //##########################

    //add new observations
    void addMeasurementResults(const int &geomId, const QList<QPointer<Reading> > &readings);

    //remove existing observations
    void removeObservations(const int &featureId);
    void removeObservations(const int &featureId, const QList<int> selectedIds);
    void removeAllObservations();
    void enableObservations(const int &featureId);
    void disableObservations(const int &featureId);

    //###############################
    //set use state of input elements
    //###############################

    void setShouldBeUsed(const QPointer<FeatureWrapper> &target, const int &functionIndex, const int &neededElementIndex,
                         const int &elementId, const bool &use, const bool &recalc);

    void createTemplateFromJob();

signals:

    //#########################################
    //signals to inform about job state changes
    //#########################################

    //active feature changes
    void activeFeatureChanged();
    void activeStationChanged();
    void activeCoordinateSystemChanged();

    //feature(s) added or removed
    void featureSetChanged();
    void coordSystemSetChanged();
    void stationSetChanged();
    void trafoParamSetChanged();
    void geometrySetChanged();

    //group(s) added or removed
    void availableGroupsChanged();
    void activeGroupChanged();

    //general feature attributes changed
    void featureAttributesChanged();

    //feature specific attributes changed
    void featureNameChanged(const int &featureId, const QString &oldName);
    void featureGroupChanged(const int &featureId, const QString &oldGroup);
    void featureCommentChanged(const int &featureId);
    void featureIsUpdatedChanged(const int &featureId); //currently not connected
    void featureIsSolvedChanged(const int &featureId);
    void featureFunctionsChanged(const int &featureId);
    void featureUsedForChanged(const int &featureId);
    void featurePreviouslyNeededChanged(const int &featureId);

    //geometry specific attributes changed
    void geometryIsCommonChanged(const int &featureId);
    void geometryNominalsChanged(const int &featureId);
    void geometryActualChanged(const int &featureId);
    void geometryObservationsChanged(const int &featureId);
    void geometryNominalSystemChanged(const int &featureId);
    void geometryStatisticChanged(const int &featureId);
    void geometrySimulationDataChanged(const int &featureId);
    void geometryMeasurementConfigChanged(const int &featureId, const QString &oldMConfig, bool oldIsSaved);

    //coordinate system specific attributes changed
    void systemObservationsChanged(const int &featureId, const int &obsId);
    void systemTrafoParamsChanged(const int &featureId);
    void systemsNominalsChanged(const int &featureId);

    //station specific attributes changed
    void stationSensorChanged(const int &featureId);

    //transformation parameter specific attributes changed
    void trafoParamParametersChanged(const int &featureId);
    void trafoParamSystemsChanged(const int &featureId);
    void trafoParamIsUsedChanged(const int &featureId);
    //void trafoParamValidTimeChanged(const int &featureId);
    //void trafoParamIsMovementChanged(const int &featureId);
    void trafoParamIsDatumChanged(const int &featureId);

    //###################################################
    //general signals to inform OpenIndy about job issues
    //###################################################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage) const;

    //##############################
    //signals to force recalculation
    //##############################

    void recalcFeature(const QPointer<Feature> &feature);
    void recalcFeatureSet();

    //#####################################
    //signals forwarded from FeatureUpdater
    //#####################################

    void featuresRecalculated();
    void featureRecalculated(const int &featureId);
    void trafoParamRecalculated(const int &featureId);

private slots:

    //###########################################################################
    //internal slots that are called when a feature changes (or is added/removed)
    //###########################################################################

    //active feature changes
    void setActiveFeature(const int &featureId);
    void setActiveStation(const int &featureId);
    void setActiveCoordinateSystem(const int &featureId);

    //feature specific attributes changed
    void setFeatureName(const int &featureId, const QString &oldName);
    void setFeatureGroup(const int &featureId, const QString &oldGroup);
    void setFeatureComment(const int &featureId);
    void setFeatureIsUpdated(const int &featureId); //currently not connected
    void setFeatureIsSolved(const int &featureId);
    void setFeatureFunctions(const int &featureId);
    void setFeatureUsedFor(const int &featureId);
    void setFeaturePreviouslyNeeded(const int &featureId);

    //geometry specific attributes changed
    void setGeometryIsCommon(const int &featureId);
    void setGeometryNominals(const int &featureId);
    void setGeometryActual(const int &featureId);
    void setGeometryObservations(const int &featureId);
    void setGeometryNominalSystem(const int &featureId);
    void setGeometryStatistic(const int &featureId);
    void setGeometrySimulationData(const int &featureId);
    void setGeometryMeasurementConfig(const int &featureId, const QString &oldMConfig, bool oldIsSaved);

    //coordinate system specific attributes changed
    void setSystemObservations(const int &featureId, const int &obsId);
    void setSystemTrafoParams(const int &featureId);
    void setSystemsNominals(const int &featureId);

    //station specific attributes changed
    void setStationSensor(const int &featureId);

    //transformation parameter specific attributes changed
    void setTrafoParamParameters(const int &featureId);
    void setTrafoParamSystems(const int &featureId);
    void setTrafoParamIsUsed(const int &featureId);
    //void setTrafoParamValidTime(const int &featureId);
    //void setTrafoParamIsMovement(const int &featureId);
    void setTrafoParamIsDatum(const int &featureId);

    //element deleted
    void elementAboutToBeDeleted(const int &elementId, const QString &name, const QString &group, const FeatureTypes &type);

private:

    //##############################
    //connect or disconnect features
    //##############################

    void connectFeature(const QPointer<FeatureWrapper> &feature);
    void disconnectFeature(const QPointer<FeatureWrapper> &feature);

    //##############
    //helper methods
    //##############

    QStringList createFeatureNames(const QString &name, const int &count) const;
    QPointer<FeatureWrapper> createFeatureWrapper(const FeatureTypes &type, bool isNominal = false) const;
    bool checkAndSetUpNewFeature(const QPointer<FeatureWrapper> &feature, bool overwrite = false);

    bool canRemoveFeature(const QPointer<FeatureWrapper> &feature) const;

    bool checkCircleWarning(const QPointer<Feature> &activeFeature, const QPointer<Feature> &usedForActiveFeature);

    void setUpDependencies(const InputElement &element, const QPointer<Feature> &feature);
    void resetDependencies(const InputElement &element, const QPointer<Feature> &feature);

    //###################################################################################
    //add features when a project has been loaded (only accessible from ProjectExchanger)
    //###################################################################################

    void addFeaturesFromXml(const QList<QPointer<FeatureWrapper> > &features);

private:

    //#########################
    //meta information of a job
    //#########################

    QString jobName; // the name under that the job is saved
    QPointer<QFileDevice> jobDevice; // the device where the job is saved

    // the last calculated digest of the job content
    QString digest;

    //################################
    //active features and active group
    //################################

    //active features
    QPointer<FeatureWrapper> activeFeature;
    QPointer<Station> activeStation;
    QPointer<CoordinateSystem> activeCoordinateSystem;

    //active group
    QString activeGroup;

    //################################
    //feature container and current id
    //################################

    FeatureContainer featureContainer; //all features of this job

    int nextId; //the next free id an element of this job could get

    void enableOrDisableObservations(const int &featureId, bool enable);
    void enableOrDisableStationObservations(QPointer<Station> station, bool enable);
    void enableOrDisableGeometryObservations(const int &featureId, bool enable, QPointer<Station> station);

    QString loadedVersion;

};

}

#endif // OIJOB_H
