/*
 * File: vehicle_cruise_control.c
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

#include "vehicle_cruise_control.h"
#include "rtwtypes.h"
#include "vehicle_cruise_control_private.h"

/* Block signals (default storage) */
B_vehicle_cruise_control_T vehicle_cruise_control_B;

/* Continuous states */
X_vehicle_cruise_control_T vehicle_cruise_control_X;

/* Disabled State Vector */
XDis_vehicle_cruise_control_T vehicle_cruise_control_XDis;

/* Block states (default storage) */
DW_vehicle_cruise_control_T vehicle_cruise_control_DW;

/* Real-time model */
static RT_MODEL_vehicle_cruise_contr_T vehicle_cruise_control_M_;
RT_MODEL_vehicle_cruise_contr_T *const vehicle_cruise_control_M =
  &vehicle_cruise_control_M_;

/*
 * Time delay interpolation routine
 *
 * The linear interpolation is performed using the formula:
 *
 * (t2 - tMinusDelay)         (tMinusDelay - t1)
 * u(t)  =  ----------------- * u1  +  ------------------- * u2
 * (t2 - t1)                  (t2 - t1)
 */
real_T rt_TDelayInterpolate(
  real_T tMinusDelay,                 /* tMinusDelay = currentSimTime - delay */
  real_T tStart,
  real_T *uBuf,
  int_T bufSz,
  int_T *lastIdx,
  int_T oldestIdx,
  int_T newIdx,
  real_T initOutput,
  boolean_T discrete,
  boolean_T minorStepAndTAtLastMajorOutput)
{
  int_T i;
  real_T yout, t1, t2, u1, u2;
  real_T* tBuf = uBuf + bufSz;

  /*
   * If there is only one data point in the buffer, this data point must be
   * the t= 0 and tMinusDelay > t0, it ask for something unknown. The best
   * guess if initial output as well
   */
  if ((newIdx == 0) && (oldestIdx ==0 ) && (tMinusDelay > tStart))
    return initOutput;

  /*
   * If tMinusDelay is less than zero, should output initial value
   */
  if (tMinusDelay <= tStart)
    return initOutput;

  /* For fixed buffer extrapolation:
   * if tMinusDelay is small than the time at oldestIdx, if discrete, output
   * tailptr value,  else use tailptr and tailptr+1 value to extrapolate
   * It is also for fixed buffer. Note: The same condition can happen for transport delay block where
   * use tStart and and t[tail] other than using t[tail] and t[tail+1].
   * See below
   */
  if ((tMinusDelay <= tBuf[oldestIdx] ) ) {
    if (discrete) {
      return(uBuf[oldestIdx]);
    } else {
      int_T tempIdx= oldestIdx + 1;
      if (oldestIdx == bufSz-1)
        tempIdx = 0;
      t1= tBuf[oldestIdx];
      t2= tBuf[tempIdx];
      u1= uBuf[oldestIdx];
      u2= uBuf[tempIdx];
      if (t2 == t1) {
        if (tMinusDelay >= t2) {
          yout = u2;
        } else {
          yout = u1;
        }
      } else {
        real_T f1 = (t2-tMinusDelay) / (t2-t1);
        real_T f2 = 1.0 - f1;

        /*
         * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
         */
        yout = f1*u1 + f2*u2;
      }

      return yout;
    }
  }

  /*
   * When block does not have direct feedthrough, we use the table of
   * values to extrapolate off the end of the table for delays that are less
   * than 0 (less then step size).  This is not completely accurate.  The
   * chain of events is as follows for a given time t.  Major output - look
   * in table.  Update - add entry to table.  Now, if we call the output at
   * time t again, there is a new entry in the table. For very small delays,
   * this means that we will have a different answer from the previous call
   * to the output fcn at the same time t.  The following code prevents this
   * from happening.
   */
  if (minorStepAndTAtLastMajorOutput) {
    /* pretend that the new entry has not been added to table */
    if (newIdx != 0) {
      if (*lastIdx == newIdx) {
        (*lastIdx)--;
      }

      newIdx--;
    } else {
      if (*lastIdx == newIdx) {
        *lastIdx = bufSz-1;
      }

      newIdx = bufSz - 1;
    }
  }

  i = *lastIdx;
  if (tBuf[i] < tMinusDelay) {
    /* Look forward starting at last index */
    while (tBuf[i] < tMinusDelay) {
      /* May occur if the delay is less than step-size - extrapolate */
      if (i == newIdx)
        break;
      i = ( i < (bufSz-1) ) ? (i+1) : 0;/* move through buffer */
    }
  } else {
    /*
     * Look backwards starting at last index which can happen when the
     * delay time increases.
     */
    while (tBuf[i] >= tMinusDelay) {
      /*
       * Due to the entry condition at top of function, we
       * should never hit the end.
       */
      i = (i > 0) ? i-1 : (bufSz-1);   /* move through buffer */
    }

    i = ( i < (bufSz-1) ) ? (i+1) : 0;
  }

  *lastIdx = i;
  if (discrete) {
    /*
     * tempEps = 128 * eps;
     * localEps = max(tempEps, tempEps*fabs(tBuf[i]))/2;
     */
    double tempEps = (DBL_EPSILON) * 128.0;
    double localEps = tempEps * fabs(tBuf[i]);
    if (tempEps > localEps) {
      localEps = tempEps;
    }

    localEps = localEps / 2.0;
    if (tMinusDelay >= (tBuf[i] - localEps)) {
      yout = uBuf[i];
    } else {
      if (i == 0) {
        yout = uBuf[bufSz-1];
      } else {
        yout = uBuf[i-1];
      }
    }
  } else {
    if (i == 0) {
      t1 = tBuf[bufSz-1];
      u1 = uBuf[bufSz-1];
    } else {
      t1 = tBuf[i-1];
      u1 = uBuf[i-1];
    }

    t2 = tBuf[i];
    u2 = uBuf[i];
    if (t2 == t1) {
      if (tMinusDelay >= t2) {
        yout = u2;
      } else {
        yout = u1;
      }
    } else {
      real_T f1 = (t2-tMinusDelay) / (t2-t1);
      real_T f2 = 1.0 - f1;

      /*
       * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
       */
      yout = f1*u1 + f2*u2;
    }
  }

  return(yout);
}

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  vehicle_cruise_control_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  vehicle_cruise_control_step();
  vehicle_cruise_control_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  vehicle_cruise_control_step();
  vehicle_cruise_control_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void vehicle_cruise_control_step(void)
{
  /* local block i/o variables */
  real_T rtb_FilterCoefficient;
  real_T rtb_IntegralGain;
  real_T rtb_DownhillDisturbance;
  real_T rtb_UphillDisturbance_tmp;
  real_T tmp_0;
  int32_T tmp_1;
  int32_T tmp_2;
  boolean_T tmp;
  if (rtmIsMajorTimeStep(vehicle_cruise_control_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&vehicle_cruise_control_M->solverInfo,
                          ((vehicle_cruise_control_M->Timing.clockTick0+1)*
      vehicle_cruise_control_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(vehicle_cruise_control_M)) {
    vehicle_cruise_control_M->Timing.t[0] = rtsiGetT
      (&vehicle_cruise_control_M->solverInfo);
  }

  /* Step: '<Root>/Reference Speed' incorporates:
   *  Step: '<Root>/Downhill Disturbance'
   *  Step: '<Root>/Uphill Disturbance'
   */
  rtb_UphillDisturbance_tmp = vehicle_cruise_control_M->Timing.t[0];

  /* TransportDelay: '<Root>/Transport Delay' */
  rtb_DownhillDisturbance = rt_TDelayInterpolate
    (vehicle_cruise_control_M->Timing.t[0] - 0.001,
     vehicle_cruise_control_DW.TransportDelay_RWORK[0],(real_T *)
     vehicle_cruise_control_DW.TransportDelay_PWORK[0],
     vehicle_cruise_control_DW.TransportDelay_IWORK[3],
     &vehicle_cruise_control_DW.TransportDelay_IWORK[2],
     vehicle_cruise_control_DW.TransportDelay_IWORK[0],
     vehicle_cruise_control_DW.TransportDelay_IWORK[1],0.0,false,false);

  /* Step: '<Root>/Reference Speed' */
  if (rtb_UphillDisturbance_tmp < 0.01) {
    tmp_0 = 0.0;
  } else {
    tmp_0 = 16.67;
  }

  /* Sum: '<Root>/Speed Error' incorporates:
   *  Step: '<Root>/Reference Speed'
   */
  rtb_DownhillDisturbance = tmp_0 - rtb_DownhillDisturbance;
  tmp = (rtmIsMajorTimeStep(vehicle_cruise_control_M) &&
         vehicle_cruise_control_M->Timing.TaskCounters.TID[1] == 0);
  if (tmp) {
    /* Gain: '<S39>/Filter Coefficient' incorporates:
     *  DiscreteIntegrator: '<S31>/Filter'
     *  Gain: '<S29>/Derivative Gain'
     *  Sum: '<S31>/SumD'
     */
    rtb_FilterCoefficient = (0.0 * rtb_DownhillDisturbance -
      vehicle_cruise_control_DW.Filter_DSTATE) * 100.0;

    /* Sum: '<S45>/Sum' incorporates:
     *  DiscreteIntegrator: '<S36>/Integrator'
     *  Gain: '<S41>/Proportional Gain'
     */
    vehicle_cruise_control_B.Sum = (800.0 * rtb_DownhillDisturbance +
      vehicle_cruise_control_DW.Integrator_DSTATE) + rtb_FilterCoefficient;
  }

  /* Step: '<Root>/Uphill Disturbance' */
  if (rtb_UphillDisturbance_tmp < 15.0) {
    tmp_1 = 0;
  } else {
    tmp_1 = -200;
  }

  /* Step: '<Root>/Downhill Disturbance' */
  if (rtb_UphillDisturbance_tmp < 30.0) {
    tmp_2 = 0;
  } else {
    tmp_2 = 150;
  }

  /* Sum: '<Root>/Road Disturbance' incorporates:
   *  Step: '<Root>/Downhill Disturbance'
   *  Step: '<Root>/Uphill Disturbance'
   */
  vehicle_cruise_control_B.RoadDisturbance = (vehicle_cruise_control_B.Sum +
    (real_T)tmp_1) + (real_T)tmp_2;
  if (tmp) {
    /* Gain: '<S33>/Integral Gain' */
    rtb_IntegralGain = 40.0 * rtb_DownhillDisturbance;
  }

  /* TransferFcn: '<Root>/Vehicle Dynamics' */
  vehicle_cruise_control_B.VehicleDynamics = 0.0;
  vehicle_cruise_control_B.VehicleDynamics += 0.0008333333333333334 *
    vehicle_cruise_control_X.VehicleDynamics_CSTATE;
  if (rtmIsMajorTimeStep(vehicle_cruise_control_M)) {
    /* Update for TransportDelay: '<Root>/Transport Delay' */
    if (vehicle_cruise_control_DW.TransportDelay_IWORK[1] <
        vehicle_cruise_control_DW.TransportDelay_IWORK[3] - 1) {
      vehicle_cruise_control_DW.TransportDelay_IWORK[1]++;
    } else {
      vehicle_cruise_control_DW.TransportDelay_IWORK[1] = 0;
    }

    if (vehicle_cruise_control_DW.TransportDelay_IWORK[0] ==
        vehicle_cruise_control_DW.TransportDelay_IWORK[1]) {
      if (vehicle_cruise_control_DW.TransportDelay_IWORK[0] <
          vehicle_cruise_control_DW.TransportDelay_IWORK[3] - 1) {
        vehicle_cruise_control_DW.TransportDelay_IWORK[0]++;
      } else {
        vehicle_cruise_control_DW.TransportDelay_IWORK[0] = 0;
      }
    }

    ((real_T *)vehicle_cruise_control_DW.TransportDelay_PWORK[0])
      [vehicle_cruise_control_DW.TransportDelay_IWORK[1]] =
      vehicle_cruise_control_B.VehicleDynamics;
    ((real_T *)vehicle_cruise_control_DW.TransportDelay_PWORK[0])
      [vehicle_cruise_control_DW.TransportDelay_IWORK[1] +
      vehicle_cruise_control_DW.TransportDelay_IWORK[3]] =
      vehicle_cruise_control_M->Timing.t[0];

    /* End of Update for TransportDelay: '<Root>/Transport Delay' */
    if (rtmIsMajorTimeStep(vehicle_cruise_control_M) &&
        vehicle_cruise_control_M->Timing.TaskCounters.TID[1] == 0) {
      /* Update for DiscreteIntegrator: '<S31>/Filter' */
      vehicle_cruise_control_DW.Filter_DSTATE += rtb_FilterCoefficient;

      /* Update for DiscreteIntegrator: '<S36>/Integrator' */
      vehicle_cruise_control_DW.Integrator_DSTATE += rtb_IntegralGain;
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(vehicle_cruise_control_M)) {
    rt_ertODEUpdateContinuousStates(&vehicle_cruise_control_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++vehicle_cruise_control_M->Timing.clockTick0;
    vehicle_cruise_control_M->Timing.t[0] = rtsiGetSolverStopTime
      (&vehicle_cruise_control_M->solverInfo);

    {
      /* Update absolute timer for sample time: [1.0s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 1.0, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      vehicle_cruise_control_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void vehicle_cruise_control_derivatives(void)
{
  XDot_vehicle_cruise_control_T *_rtXdot;
  _rtXdot = ((XDot_vehicle_cruise_control_T *) vehicle_cruise_control_M->derivs);

  /* Derivatives for TransferFcn: '<Root>/Vehicle Dynamics' */
  _rtXdot->VehicleDynamics_CSTATE = -0.04166666666666667 *
    vehicle_cruise_control_X.VehicleDynamics_CSTATE;
  _rtXdot->VehicleDynamics_CSTATE += vehicle_cruise_control_B.RoadDisturbance;
}

/* Model initialize function */
void vehicle_cruise_control_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&vehicle_cruise_control_M->solverInfo,
                          &vehicle_cruise_control_M->Timing.simTimeStep);
    rtsiSetTPtr(&vehicle_cruise_control_M->solverInfo, &rtmGetTPtr
                (vehicle_cruise_control_M));
    rtsiSetStepSizePtr(&vehicle_cruise_control_M->solverInfo,
                       &vehicle_cruise_control_M->Timing.stepSize0);
    rtsiSetdXPtr(&vehicle_cruise_control_M->solverInfo,
                 &vehicle_cruise_control_M->derivs);
    rtsiSetContStatesPtr(&vehicle_cruise_control_M->solverInfo, (real_T **)
                         &vehicle_cruise_control_M->contStates);
    rtsiSetNumContStatesPtr(&vehicle_cruise_control_M->solverInfo,
      &vehicle_cruise_control_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&vehicle_cruise_control_M->solverInfo,
      &vehicle_cruise_control_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&vehicle_cruise_control_M->solverInfo,
      &vehicle_cruise_control_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&vehicle_cruise_control_M->solverInfo,
      &vehicle_cruise_control_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&vehicle_cruise_control_M->solverInfo,
      (boolean_T**) &vehicle_cruise_control_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&vehicle_cruise_control_M->solverInfo,
                          (&rtmGetErrorStatus(vehicle_cruise_control_M)));
    rtsiSetRTModelPtr(&vehicle_cruise_control_M->solverInfo,
                      vehicle_cruise_control_M);
  }

  rtsiSetSimTimeStep(&vehicle_cruise_control_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&vehicle_cruise_control_M->solverInfo,
    false);
  rtsiSetIsContModeFrozen(&vehicle_cruise_control_M->solverInfo, false);
  vehicle_cruise_control_M->intgData.y = vehicle_cruise_control_M->odeY;
  vehicle_cruise_control_M->intgData.f[0] = vehicle_cruise_control_M->odeF[0];
  vehicle_cruise_control_M->intgData.f[1] = vehicle_cruise_control_M->odeF[1];
  vehicle_cruise_control_M->intgData.f[2] = vehicle_cruise_control_M->odeF[2];
  vehicle_cruise_control_M->contStates = ((X_vehicle_cruise_control_T *)
    &vehicle_cruise_control_X);
  vehicle_cruise_control_M->contStateDisabled = ((XDis_vehicle_cruise_control_T *)
    &vehicle_cruise_control_XDis);
  vehicle_cruise_control_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&vehicle_cruise_control_M->solverInfo, (void *)
                    &vehicle_cruise_control_M->intgData);
  rtsiSetSolverName(&vehicle_cruise_control_M->solverInfo,"ode3");
  rtmSetTPtr(vehicle_cruise_control_M, &vehicle_cruise_control_M->Timing.tArray
             [0]);
  vehicle_cruise_control_M->Timing.stepSize0 = 1.0;

  {
    real_T tmp;

    /* Start for TransportDelay: '<Root>/Transport Delay' */
    vehicle_cruise_control_DW.TransportDelay_RWORK[0] = 0.0;
    vehicle_cruise_control_DW.TransportDelay_IWORK[3] = 1024;
    vehicle_cruise_control_DW.TransportDelay_PWORK[0] =
      &vehicle_cruise_control_DW.TransportDelay_RWORK[1];
    ((real_T *)vehicle_cruise_control_DW.TransportDelay_PWORK[0])[0] = 0.0;
    tmp = vehicle_cruise_control_M->Timing.t[0];
    ((real_T *)vehicle_cruise_control_DW.TransportDelay_PWORK[0])[1024] = tmp;

    /* InitializeConditions for TransportDelay: '<Root>/Transport Delay' */
    vehicle_cruise_control_DW.TransportDelay_IWORK[0] = 0;
    vehicle_cruise_control_DW.TransportDelay_IWORK[1] = 0;
    vehicle_cruise_control_DW.TransportDelay_IWORK[2] = 0;
    ((real_T *)vehicle_cruise_control_DW.TransportDelay_PWORK[0])[0] = 0.0;
    ((real_T *)vehicle_cruise_control_DW.TransportDelay_PWORK[0])[1024] = tmp;

    /* InitializeConditions for TransferFcn: '<Root>/Vehicle Dynamics' */
    vehicle_cruise_control_X.VehicleDynamics_CSTATE = 0.0;
  }
}

/* Model terminate function */
void vehicle_cruise_control_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
