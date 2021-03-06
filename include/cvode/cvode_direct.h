/* -----------------------------------------------------------------
 * Programmer(s): Daniel R. Reynolds @ SMU
 * -----------------------------------------------------------------
 * SUNDIALS Copyright Start
 * Copyright (c) 2002-2020, Lawrence Livermore National Security
 * and Southern Methodist University.
 * All rights reserved.
 *
 * See the top-level LICENSE and NOTICE files for details.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * SUNDIALS Copyright End
 * -----------------------------------------------------------------
 * Header file for the deprecated direct linear solver interface in
 * CVODE; these routines now just wrap the updated CVODE generic
 * linear solver interface in cvode_ls.h.
 * -----------------------------------------------------------------*/

#ifndef _CVDLS_H
#define _CVDLS_H

#include <cvode/cvode_ls.h>

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif


/*=================================================================
  Function Types (typedefs for equivalent types in cvode_ls.h)
  =================================================================*/

typedef CVLsJacFn CVDlsJacFn;

/*===================================================================
  Exported Functions (wrappers for equivalent routines in cvode_ls.h)
  ===================================================================*/

SUNDIALS_EXPORT int CVDlsSetLinearSolver(void *cvode_mem, SUNLinearSolver LS,
                                         SUNMatrix A);

SUNDIALS_EXPORT int CVDlsSetJacFn(void *cvode_mem, CVDlsJacFn jac);

SUNDIALS_EXPORT int CVDlsGetWorkSpace(void *cvode_mem, long int *lenrwLS,
                                      long int *leniwLS);

SUNDIALS_EXPORT int CVDlsGetNumJacEvals(void *cvode_mem, long int *njevals);

SUNDIALS_EXPORT int CVDlsGetNumRhsEvals(void *cvode_mem, long int *nfevalsLS);

SUNDIALS_EXPORT int CVDlsGetLastFlag(void *cvode_mem, long int *flag);

SUNDIALS_EXPORT char *CVDlsGetReturnFlagName(long int flag);


#ifdef __cplusplus
}
#endif

#endif
