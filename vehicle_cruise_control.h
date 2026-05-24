/*
 * File: vehicle_cruise_control.h
 *
 * Code generated for Simulink model 'vehicle_cruise_control'.
 *
 * Model version                  : 1.3
 * Simulink Coder version         : 26.1 (R2026a) 20-Nov-2025
 * C/C++ source code generated on : Sun May 24 10:35:49 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef vehicle_cruise_control_h_
#define vehicle_cruise_control_h_
#ifndef vehicle_cruise_control_COMMON_INCLUDES_
#define vehicle_cruise_control_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif                             /* vehicle_cruise_control_COMMON_INCLUDES_ */

#include "vehicle_cruise_control_types.h"
#include <float.h>
#include <math.h>
#include <string.h>

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T Sum;                          /* '<S45>/Sum' */
  real_T RoadDisturbance;              /* '<Root>/Road Disturbance' */
  real_T VehicleDynamics;              /* '<Root>/Vehicle Dynamics' */
} B_vehicle_cruise_control_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Filter_DSTATE;                /* '<S31>/Filter' */
  real_T Integrator_DSTATE;            /* '<S36>/Integrator' */
  real_T TransportDelay_RWORK[2049];   /* '<Root>/Transport Delay' */
  void *TransportDelay_PWORK[2];       /* '<Root>/Transport Delay' */
  int_T TransportDelay_IWORK[4];       /* '<Root>/Transport Delay' */
} DW_vehicle_cruise_control_T;

/* Continuous states (default storage) */
typedef struct {
  real_T VehicleDynamics_CSTATE;       /* '<Root>/Vehicle Dynamics' */
} X_vehicle_cruise_control_T;

/* State derivatives (default storage) */
typedef struct {
  real_T VehicleDynamics_CSTATE;       /* '<Root>/Vehicle Dynamics' */
} XDot_vehicle_cruise_control_T;

/* State disabled  */
typedef struct {
  boolean_T VehicleDynamics_CSTATE;    /* '<Root>/Vehicle Dynamics' */
} XDis_vehicle_cruise_control_T;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* Real-time Model Data Structure */
struct tag_RTM_vehicle_cruise_contro_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_vehicle_cruise_control_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis_vehicle_cruise_control_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[1];
  real_T odeF[3][1];
  ODE3_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    struct {
      uint8_T TID[2];
    } TaskCounters;

    time_T tStart;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block signals (default storage) */
extern B_vehicle_cruise_control_T vehicle_cruise_control_B;

/* Continuous states (default storage) */
extern X_vehicle_cruise_control_T vehicle_cruise_control_X;

/* Disabled states (default storage) */
extern XDis_vehicle_cruise_control_T vehicle_cruise_control_XDis;

/* Block states (default storage) */
extern DW_vehicle_cruise_control_T vehicle_cruise_control_DW;

/* Model entry point functions */
extern void vehicle_cruise_control_initialize(void);
extern void vehicle_cruise_control_step(void);
extern void vehicle_cruise_control_terminate(void);

/* Real-time Model object */
extern RT_MODEL_vehicle_cruise_contr_T *const vehicle_cruise_control_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Scope' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'vehicle_cruise_control'
 * '<S1>'   : 'vehicle_cruise_control/Discrete PID Controller'
 * '<S2>'   : 'vehicle_cruise_control/Discrete PID Controller/Anti-windup'
 * '<S3>'   : 'vehicle_cruise_control/Discrete PID Controller/D Gain'
 * '<S4>'   : 'vehicle_cruise_control/Discrete PID Controller/External Derivative'
 * '<S5>'   : 'vehicle_cruise_control/Discrete PID Controller/Filter'
 * '<S6>'   : 'vehicle_cruise_control/Discrete PID Controller/Filter ICs'
 * '<S7>'   : 'vehicle_cruise_control/Discrete PID Controller/I Gain'
 * '<S8>'   : 'vehicle_cruise_control/Discrete PID Controller/Ideal P Gain'
 * '<S9>'   : 'vehicle_cruise_control/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S10>'  : 'vehicle_cruise_control/Discrete PID Controller/Integrator'
 * '<S11>'  : 'vehicle_cruise_control/Discrete PID Controller/Integrator ICs'
 * '<S12>'  : 'vehicle_cruise_control/Discrete PID Controller/N Copy'
 * '<S13>'  : 'vehicle_cruise_control/Discrete PID Controller/N Gain'
 * '<S14>'  : 'vehicle_cruise_control/Discrete PID Controller/P Copy'
 * '<S15>'  : 'vehicle_cruise_control/Discrete PID Controller/Parallel P Gain'
 * '<S16>'  : 'vehicle_cruise_control/Discrete PID Controller/Reset Signal'
 * '<S17>'  : 'vehicle_cruise_control/Discrete PID Controller/Saturation'
 * '<S18>'  : 'vehicle_cruise_control/Discrete PID Controller/Saturation Fdbk'
 * '<S19>'  : 'vehicle_cruise_control/Discrete PID Controller/Sum'
 * '<S20>'  : 'vehicle_cruise_control/Discrete PID Controller/Sum Fdbk'
 * '<S21>'  : 'vehicle_cruise_control/Discrete PID Controller/Tracking Mode'
 * '<S22>'  : 'vehicle_cruise_control/Discrete PID Controller/Tracking Mode Sum'
 * '<S23>'  : 'vehicle_cruise_control/Discrete PID Controller/Tsamp - Integral'
 * '<S24>'  : 'vehicle_cruise_control/Discrete PID Controller/Tsamp - Ngain'
 * '<S25>'  : 'vehicle_cruise_control/Discrete PID Controller/postSat Signal'
 * '<S26>'  : 'vehicle_cruise_control/Discrete PID Controller/preInt Signal'
 * '<S27>'  : 'vehicle_cruise_control/Discrete PID Controller/preSat Signal'
 * '<S28>'  : 'vehicle_cruise_control/Discrete PID Controller/Anti-windup/Passthrough'
 * '<S29>'  : 'vehicle_cruise_control/Discrete PID Controller/D Gain/Internal Parameters'
 * '<S30>'  : 'vehicle_cruise_control/Discrete PID Controller/External Derivative/Error'
 * '<S31>'  : 'vehicle_cruise_control/Discrete PID Controller/Filter/Disc. Forward Euler Filter'
 * '<S32>'  : 'vehicle_cruise_control/Discrete PID Controller/Filter ICs/Internal IC - Filter'
 * '<S33>'  : 'vehicle_cruise_control/Discrete PID Controller/I Gain/Internal Parameters'
 * '<S34>'  : 'vehicle_cruise_control/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S35>'  : 'vehicle_cruise_control/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S36>'  : 'vehicle_cruise_control/Discrete PID Controller/Integrator/Discrete'
 * '<S37>'  : 'vehicle_cruise_control/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S38>'  : 'vehicle_cruise_control/Discrete PID Controller/N Copy/Disabled'
 * '<S39>'  : 'vehicle_cruise_control/Discrete PID Controller/N Gain/Internal Parameters'
 * '<S40>'  : 'vehicle_cruise_control/Discrete PID Controller/P Copy/Disabled'
 * '<S41>'  : 'vehicle_cruise_control/Discrete PID Controller/Parallel P Gain/Internal Parameters'
 * '<S42>'  : 'vehicle_cruise_control/Discrete PID Controller/Reset Signal/Disabled'
 * '<S43>'  : 'vehicle_cruise_control/Discrete PID Controller/Saturation/Passthrough'
 * '<S44>'  : 'vehicle_cruise_control/Discrete PID Controller/Saturation Fdbk/Disabled'
 * '<S45>'  : 'vehicle_cruise_control/Discrete PID Controller/Sum/Sum_PID'
 * '<S46>'  : 'vehicle_cruise_control/Discrete PID Controller/Sum Fdbk/Disabled'
 * '<S47>'  : 'vehicle_cruise_control/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S48>'  : 'vehicle_cruise_control/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S49>'  : 'vehicle_cruise_control/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S50>'  : 'vehicle_cruise_control/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S51>'  : 'vehicle_cruise_control/Discrete PID Controller/postSat Signal/Forward_Path'
 * '<S52>'  : 'vehicle_cruise_control/Discrete PID Controller/preInt Signal/Internal PreInt'
 * '<S53>'  : 'vehicle_cruise_control/Discrete PID Controller/preSat Signal/Forward_Path'
 */
#endif                                 /* vehicle_cruise_control_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
