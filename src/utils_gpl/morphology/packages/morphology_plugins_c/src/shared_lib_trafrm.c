//---- GPL ---------------------------------------------------------------------
//
// Copyright (C)  Stichting Deltares, 2011-2015.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 3.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// contact: delft3d.support@deltares.nl
// Stichting Deltares
// P.O. Box 177
// 2600 MH Delft, The Netherlands
//
// All indications and logos of, and references to, "Delft3D" and "Deltares"
// are registered trademarks of Stichting Deltares, and remain the property of
// Stichting Deltares. All rights reserved.
//
//------------------------------------------------------------------------------
// $Id: shared_lib_trafrm.c 4612 2015-01-21 08:48:09Z mourits $
// $HeadURL: https://svn.oss.deltares.nl/repos/delft3d/branches/research/Deltares/20160119_tidal_turbines/src/utils_gpl/morphology/packages/morphology_plugins_c/src/shared_lib_trafrm.c $
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32)
#  include <windows.h>
#elif defined(salford32)
#  include <windows.h>
#elif defined(HAVE_CONFIG_H)
#  include <dlfcn.h>
#endif

#if defined(WIN32)
#  define PERFORM_FUNCTION_EQTRAN  PERF_FUNCTION_EQTRAN
#  define PERFORM_FUNCTION_EROSILT PERF_FUNCTION_EROSILT
#  define STDCALL
#elif defined(salford32)
#  define PERFORM_FUNCTION_EQTRAN  PERF_FUNCTION_EQTRAN
#  define PERFORM_FUNCTION_EROSILT PERF_FUNCTION_EROSILT
#  define STDCALL __stdcall
#elif defined(HAVE_CONFIG_H)
#   include "config.h"
#  define PERFORM_FUNCTION_EQTRAN  FC_FUNC(perf_function_eqtran,PERFORM_FUNCTION_EQTRAN)
#  define PERFORM_FUNCTION_EROSILT FC_FUNC(perf_function_erosilt,PERFORM_FUNCTION_EROSILT)
#  define STDCALL
#endif

/*
 *
 * Connection routine between F90 (main) -> C (interface) -> F90 (DLL).
 * Special attention to the WINAPI define, which is needed if the DLL is written in F90
 *
 */

#if defined(WIN32)
    typedef HMODULE DllHandle;
#elif defined(salford32)
    typedef HMODULE DllHandle;
#elif defined(HAVE_CONFIG_H)
    typedef void * DllHandle;
#endif

typedef struct {
    DllHandle   dllHandle;
} SharedDLL;

/*
 * ============================================================================
 */
char * strFcpy(char * str_1, int len);
void RemoveTrailingBlanks_dll(char * String);

/*
 * ============================================================================
 */
#if defined(WIN32)
long STDCALL PERFORM_FUNCTION_EQTRAN(long   * sharedDLLHandle    ,
                              char   * function           ,
                              long   * dll_integers       ,
                              long   * max_integers       ,
                              double * dll_reals          ,
                              long   * max_reals          ,
                              char   * dll_strings        ,
                              long   * max_strings        ,
                              long   * sbc_total          ,
                              double * sbc                ,
                              double * sbcu               ,
                              double * sbcv               ,
                              double * sbwu               ,
                              double * sbwv               ,
                              long   * equi_conc          ,
                              double * cesus              ,
                              double * ssus               ,
                              double * sswu               ,
                              double * sswv               ,
                              double * t_relax            ,
                              char   * message            ,
                              long     length_function    ,
                              long     length_dll_strings ,
                              long     length_message     )
#elif defined(salford32)
extern "C" PERFORM_FUNCTION_EQTRAN(  long   * sharedDLLHandle    ,
                              char   * function           ,
                              long   * dll_integers       ,
                              long   * max_integers       ,
                              double * dll_reals          ,
                              long   * max_reals          ,
                              char   * dll_strings        ,
                              long   * max_strings        ,
                              long   * sbc_total          ,
                              double * sbc                ,
                              double * sbcu               ,
                              double * sbcv               ,
                              double * sbwu               ,
                              double * sbwv               ,
                              long   * equi_conc          ,
                              double * cesus              ,
                              double * ssus               ,
                              double * sswu               ,
                              double * sswv               ,
                              double * t_relax            ,
                              char   * message            ,
                              long     length_function    ,
                              long     length_dll_strings ,
                              long     length_message     )
#elif defined (HAVE_CONFIG_H)
long STDCALL PERFORM_FUNCTION_EQTRAN(long   * sharedDLLHandle    ,
                              char   * function           ,
                              long   * dll_integers       ,
                              long   * max_integers       ,
                              double * dll_reals          ,
                              long   * max_reals          ,
                              char   * dll_strings        ,
                              long   * max_strings        ,
                              long   * sbc_total          ,
                              double * sbc                ,
                              double * sbcu               ,
                              double * sbcv               ,
                              double * sbwu               ,
                              double * sbwv               ,
                              long   * equi_conc          ,
                              double * cesus              ,
                              double * ssus               ,
                              double * sswu               ,
                              double * sswv               ,
                              double * t_relax            ,
                              char   * message            ,
                              long     length_function    ,
                              long     length_dll_strings ,
                              long     length_message     )
#endif
{

  long error = 1;
  long len = -1;
#if defined(WIN32)
  typedef void * (STDCALL * MyProc)(long   *, long   *,
                                    double *, long   *,
                                    char   *, long   *,
                                    long   *, double *, double *, double *, double *, double *,
                                    long   *, double *, double *, double *, double *, double *,
                                    char   *, long    , long    );
#elif defined (HAVE_CONFIG_H)
  typedef void * (STDCALL * MyProc)(long   *, long   *,
                                    double *, long   *,
                                    char   *, long   *,
                                    long   *, double *, double *, double *, double *, double *,
                                    long   *, double *, double *, double *, double *, double *,
                                    char   *, long    , long    );
#endif
  MyProc proc;
  char * fun_name;
  SharedDLL * sharedDLL = (SharedDLL *) (*sharedDLLHandle);

  fun_name = strFcpy(function, length_function);
  RemoveTrailingBlanks_dll(fun_name);

#if defined(WIN32)
  proc = (MyProc) GetProcAddress( sharedDLL->dllHandle, fun_name);
#elif defined(salford32)
  proc = (MyProc) GetProcAddress( sharedDLL->dllHandle, fun_name);
#elif defined(HAVE_CONFIG_H)
  proc = (MyProc) dlsym( sharedDLL->dllHandle, fun_name);
#endif

  if ( proc != NULL )
  {
     error = 0;
#if defined(WIN32)
     (void *) (*proc)(dll_integers, max_integers,
                      dll_reals   , max_reals   ,
                      dll_strings , max_strings ,
                      sbc_total   ,
                      sbc         ,
                      sbcu        ,
                      sbcv        ,
                      sbwu        ,
                      sbwv        ,
                      equi_conc   ,
                      cesus       ,
                      ssus        ,
                      sswu        ,
                      sswv        ,
                      t_relax     ,
                      message     ,
                      length_dll_strings, length_message );
#elif defined (HAVE_CONFIG_H)
     (void *) (*proc)(dll_integers, max_integers,
                      dll_reals   , max_reals   ,
                      dll_strings , max_strings ,
                      sbc_total   ,
                      sbc         ,
                      sbcu        ,
                      sbcv        ,
                      sbwu        ,
                      sbwv        ,
                      equi_conc   ,
                      cesus       ,
                      ssus        ,
                      sswu        ,
                      sswv        ,
                      t_relax     ,
                      message     , length_dll_strings, length_message );
#endif
  }
  free(fun_name); fun_name = NULL;

  return error;
}
/*
 * ============================================================================
 */
#if defined(WIN32)
long STDCALL PERFORM_FUNCTION_EROSILT(long   * sharedDLLHandle    ,
                              char   * function           ,
                              long   * dll_integers       ,
                              long   * max_integers       ,
                              double * dll_reals          ,
                              long   * max_reals          ,
                              char   * dll_strings        ,
                              long   * max_strings        ,
                              double * sink               ,
                              double * source             ,
                              char   * message            ,
                              long     length_function    ,
                              long     length_dll_strings ,
                              long     length_message     )
#elif defined(salford32)
extern "C" PERFORM_FUNCTION_EROSILT(  long   * sharedDLLHandle    ,
                              char   * function           ,
                              long   * dll_integers       ,
                              long   * max_integers       ,
                              double * dll_reals          ,
                              long   * max_reals          ,
                              char   * dll_strings        ,
                              long   * max_strings        ,
                              double * sink               ,
                              double * source             ,
                              char   * message            ,
                              long     length_function    ,
                              long     length_dll_strings ,
                              long     length_message     )
#elif defined (HAVE_CONFIG_H)
long STDCALL PERFORM_FUNCTION_EROSILT(long   * sharedDLLHandle    ,
                              char   * function           ,
                              long   * dll_integers       ,
                              long   * max_integers       ,
                              double * dll_reals          ,
                              long   * max_reals          ,
                              char   * dll_strings        ,
                              long   * max_strings        ,
                              double * sink               ,
                              double * source             ,
                              char   * message            ,
                              long     length_function    ,
                              long     length_dll_strings ,
                              long     length_message     )
#endif
{

  long error = 1;
  long len = -1;
#if defined(WIN32)
  typedef void * (STDCALL * MyProc)(long   *, long   *,
                                    double *, long   *,
                                    char   *, long   *,
                                    double *, double *,
                                    char   *, long    , long    );
#elif defined (HAVE_CONFIG_H)
  typedef void * (STDCALL * MyProc)(long   *, long   *,
                                    double *, long   *,
                                    char   *, long   *,
                                    double *, double *,
                                    char   *, long    , long    );
#endif
  MyProc proc;
  char * fun_name;
  SharedDLL * sharedDLL = (SharedDLL *) (*sharedDLLHandle);

  fun_name = strFcpy(function, length_function);
  RemoveTrailingBlanks_dll(fun_name);

#if defined(WIN32)
  proc = (MyProc) GetProcAddress( sharedDLL->dllHandle, fun_name);
#elif defined(salford32)
  proc = (MyProc) GetProcAddress( sharedDLL->dllHandle, fun_name);
#elif defined(HAVE_CONFIG_H)
  proc = (MyProc) dlsym( sharedDLL->dllHandle, fun_name);
#endif

  if ( proc != NULL )
  {
     error = 0;
#if defined(WIN32)
     (void *) (*proc)(dll_integers, max_integers,
                      dll_reals   , max_reals   ,
                      dll_strings , max_strings ,
                      sink        , source      ,
                      message     , length_dll_strings, length_message );
#elif defined (HAVE_CONFIG_H)
     (void *) (*proc)(dll_integers, max_integers,
                      dll_reals   , max_reals   ,
                      dll_strings , max_strings ,
                      sink        , source      ,
                      message     , length_dll_strings, length_message );
#endif
  }
  free(fun_name); fun_name = NULL;

  return error;
}
