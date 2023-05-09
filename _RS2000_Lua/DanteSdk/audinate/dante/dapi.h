/*
* File     : dapi.h
* Created  : August 2016
* Author   : James Westendorp
* Synopsis : Audinate Specific Environment setup
*
* This software is copyright (c) 2016-2017 Audinate Pty Ltd and/or its licensors
*
* Audinate Copyright Header Version 1 
*/

/**
 * @file dapi.h
 *  Dante API handler types and functions
 */

#ifndef _DAPI_H
#define _DAPI_H

#ifndef DAPI_FLAT_INCLUDE
#include "aud_platform.h"
#include "dante/dante_common.h"
#include "dante/dante_runtime.h"
#include "dante/dante_domains.h"
#include "dante/dapi_types.h"
#include "dante/domain_handler.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
/**
 *	Create a new dapi handler that internally 
 * 1) creates the event loop and logs
 * 2) creates and initializes the runtime
 * 3) creates and initializes the domain handler and
 * 4) creates the dante api handler
 */
aud_error_t dapi_new(dapi_t ** pdapi);

/**
 * Deletes the audinate specific envrionment and dante api handler
 */
void dapi_delete(dapi_t * dapi);

/** 
 * Get dante runtime (process data)
 */
dante_runtime_t * dapi_get_runtime(dapi_t * dapi);

/** 
 * Get dante domain handler client
 */
dante_domain_handler_t * dapi_get_domain_handler(dapi_t * dapi);

	
#ifdef __cplusplus
}
#endif

#endif
