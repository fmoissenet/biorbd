#ifndef S2MACTUATORLINEAR_H
#define S2MACTUATORLINEAR_H

#include "biorbdConfig.h"
#include "s2mActuator.h"
#include "s2mGenCoord.h"

class BIORBD_API s2mActuatorLinear : public s2mActuator
{
    public:
        s2mActuatorLinear(int direction,
                          double T0,
                          double pente,
                          unsigned int dofIdx,
                          const s2mString &jointName = "");
        ~s2mActuatorLinear(){}
        virtual double torqueMax(const s2mGenCoord &Q) const;

    protected:

        virtual void setType(){m_type = "Linear";}             // Quel type d'actuator


        // mx+b
        double m_m;      // Pente
        double m_b; // Torque à zéro


private:
};
#endif // S2MACTUATORLINEAR_H
