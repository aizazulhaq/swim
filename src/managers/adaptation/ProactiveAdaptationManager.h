/*******************************************************************************
 * Simulator of Web Infrastructure and Management
 * Copyright (c) 2016 Carnegie Mellon University.
 * All Rights Reserved.
 *  
 * THIS SOFTWARE IS PROVIDED "AS IS," WITH NO WARRANTIES WHATSOEVER. CARNEGIE
 * MELLON UNIVERSITY EXPRESSLY DISCLAIMS TO THE FULLEST EXTENT PERMITTED BY LAW
 * ALL EXPRESS, IMPLIED, AND STATUTORY WARRANTIES, INCLUDING, WITHOUT
 * LIMITATION, THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, AND NON-INFRINGEMENT OF PROPRIETARY RIGHTS.
 *  
 * Released under a BSD license, please see license.txt for full terms.
 * DM-0003883
 *******************************************************************************/

#ifndef __PLASASIM_REACTIVEADAPTATIONMANAGER_H_
#define __PLASASIM_REACTIVEADAPTATIONMANAGER_H_

#include "BaseAdaptationManager.h"
#include "managers/adaptation/proactive/include/SDPAdaptationManager.h"
#include "managers/adaptation/dartam/include/dartam/DartConfigurationManager.h"
#include "managers/adaptation/proactive/include/timeseries/TimeSeriesPredictor.h"

/**
 * Simple reactive adaptation manager
 */
class ProactiveAdaptationManager : public BaseAdaptationManager
{
  public:
    ProactiveAdaptationManager();
  protected:
    virtual Tactic* evaluate();

    std::shared_ptr<pladapt::Environment> getEnv(double val);

    std::shared_ptr<const dart::am2::DartConfigurationManager> configMgr;
    std::unique_ptr<pladapt::SDPAdaptationManager> adaptnMgr;
    std::unique_ptr<pladapt::timeseries::TimeSeriesPredictor> predictor;
};

#endif
