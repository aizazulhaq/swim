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

#include "ProactiveAdaptationManager.h"
#include "managers/adaptation/UtilityScorer.h"
#include "managers/execution/AllTactics.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <managers/adaptation/dartam/include/dartam/DartConfigurationManager.h>
#include <managers/adaptation/dartam/include/dartam/DartConfiguration.h>
#include <managers/adaptation/dartam/include/dartam/DartUtilityFunction.h>
#include <managers/adaptation/dartam/include/dartam/DartDTMCEnvironment.h>
#include <managers/adaptation/dartam/include/dartam/DartSimpleEnvironment.h>
#include <managers/adaptation/proactive/include/UtilityFunction.h>

#include <iterator>
#include <algorithm>
#include <iostream>
#include <memory>
#include <cmath>

using namespace std;

Define_Module(ProactiveAdaptationManager);

boost::random::mt19937 gen;

ProactiveAdaptationManager::ProactiveAdaptationManager() {
    predictor = unique_ptr<pladapt::timeseries::TimeSeriesPredictor>(pladapt::timeseries::TimeSeriesPredictor::getInstance("LES 0.99 0.001 0.97",10,10));
    configMgr = std::make_shared<dart::am2::DartConfigurationManager>(12,10,3);
    adaptnMgr = unique_ptr<pladapt::SDPAdaptationManager>(new pladapt::SDPAdaptationManager);
    adaptnMgr->initialize(configMgr);
}

/**
 * Reactive adaptation
 *
 * RT = response time
 * RTT = response time threshold
 *
 * - if RT > RTT, add a server if possible, if not decrease dimmer if possible
 * - if RT < RTT and spare utilization > 1
 *      -if dimmer < 1, increase dimmer else if servers > 1 and no server booting remove server
 */
Tactic* ProactiveAdaptationManager::evaluate() {

    unsigned horizon = 10;

    MacroTactic* pMacroTactic = new MacroTactic;
    Model* pModel = getModel();

    cout << "Current model: s=" << pModel->getServers() << " (" << pModel->getActiveServers()
            << "), d=" << pModel->getDimmerFactor() << endl;

    predictor->observe(pModel->getObservations().arrivalRate);

    bool isServerBooting = pModel->getServers() > pModel->getActiveServers();
    unsigned oneStep = pModel->getActiveServerCountIn(pModel->getEvaluationPeriod());
    unsigned twoStep = pModel->getActiveServerCountIn(pModel->getEvaluationPeriod()*2);
    unsigned maxServers  = pModel->getMaxServers();
    unsigned curServers  = pModel->getActiveServers();

    double dimmerStep  = 1.0 / (pModel->getNumberOfDimmerLevels() - 1);
    double dimmer      = pModel->getDimmerFactor();
    unsigned dimmerLvl = round(dimmer * (pModel->getNumberOfDimmerLevels()-1));

    auto currConfig = dart::am2::DartConfiguration(curServers-1, dimmerLvl, twoStep > oneStep ? 0 : oneStep > pModel->getServers() ? 1 : 2);
    auto dtmc       = predictor->createScenarioTree(1,10)->getEnvironmentDTMC([=](double x){
        return this->getEnv(x);
    });
    dart::am2::DartUtilityFunction utilFunction(0.001);
    auto tactics = adaptnMgr->evaluate(currConfig, dtmc, utilFunction, horizon);
    for (auto& tactic : tactics) {
        if (tactic == "DecDimmer" && dimmer > 0.0) {
            pMacroTactic->addTactic(new SetDimmerTactic(max(0.0, dimmer-dimmerStep)));
        } else if (tactic == "IncDimmer" && dimmer < 1.0) {
            pMacroTactic->addTactic(new SetDimmerTactic(min(1.0, dimmer+dimmerStep)));
        } else if (tactic == "AddServer" && !isServerBooting && pModel->getServers() < maxServers) {
            pMacroTactic->addTactic(new AddServerTactic);
        } else if (tactic == "RemoveServer" && !isServerBooting && pModel->getServers() > 1) {
            pMacroTactic->addTactic(new RemoveServerTactic);
        }
    }
    return pMacroTactic;
}

std::shared_ptr<pladapt::Environment> ProactiveAdaptationManager::getEnv(double val) {
    return std::make_shared<dart::am2::DartSimpleEnvironment>(val);
}
