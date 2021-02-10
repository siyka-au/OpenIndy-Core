#ifndef SPECIALFUNCTION__H
#define SPECIALFUNCTION__H

#include "function.h"

namespace oi{

/*!
 * \brief The SpecialFunction class
 * handles special function e. g. configured functions
 */
class OI_CORE_EXPORT SpecialFunction: public Function
{
    Q_OBJECT

public:
    SpecialFunction(QObject *parent = 0) : Function(parent){}

    virtual ~SpecialFunction(){}

protected:

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(Station &station){ return Function::exec(station); }
    bool exec(CoordinateSystem &coordinateSystem){ return Function::exec(coordinateSystem); }
    bool exec(TrafoParam &trafoParam){ return Function::exec(trafoParam); }

};

}

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

#define SPECIALFUNCTION_PREFIX de.openIndy.plugin.function.specialFunction.v
#define SpecialFunction_iidd STR2(CONCAT2(SPECIALFUNCTION_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // SPECIALFUNCTION__H
