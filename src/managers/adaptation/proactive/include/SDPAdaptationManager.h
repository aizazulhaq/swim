/*******************************************************************************
 * PLA Adaptation Manager
 *
 * Copyright 2017 Carnegie Mellon University. All Rights Reserved.
 * 
 * NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 * INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
 * UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS
 * TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE
 * OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE
 * MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND
 * WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 *
 * Released under a BSD-style license, please see license.txt or contact
 * permission@sei.cmu.edu for full terms.
 *
 * [DISTRIBUTION STATEMENT A] This material has been approved for public release
 * and unlimited distribution. Please see Copyright notice for non-US Government
 * use and distribution.
 ******************************************************************************/

#ifndef _PLADAPT_SDPADAPTATIONMANAGER_H_
#define _PLADAPT_SDPADAPTATIONMANAGER_H_

#include <managers/adaptation/proactive/include/AdaptationManager.h>
#include <managers/adaptation/proactive/include/EnvironmentDTMCPartitioned.h>
#include <managers/adaptation/proactive/include/ReachabilityRelation.h>
#include <managers/adaptation/proactive/include/UtilityFunction.h>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <memory>
#include <set>
#include <string>
#include <yaml-cpp/yaml.h>

typedef boost::numeric::ublas::mapped_matrix<bool> ReachabilityMatrix;


// set to one to remove the optimization of partitioned environment state space
#define PLADAPT_SDP_NOPARTITION 0

// set to one to support getting a full strategy after a call to evaluate()
#define SUPPORTS_GET_STRATEGY 1


namespace pladapt {

/**
 * Adaptation manager using Alloy+SDP approach
 */
class SDPAdaptationManager : public AdaptationManager
{
	std::string reachArgs;

  protected:
    std::unique_ptr<ReachabilityRelation> pImmediateReachabilityRelation; /**< the isReachableImmediately relation annotated with tactics*/
    std::unique_ptr<ReachabilityRelation> pStepReachabilityRelation; /**< the reachability relation for the passing of one time step */

    std::shared_ptr<ReachabilityMatrix> pReachableImmediately; /**< the isReachableImmediately relation */

    /**
     * The isReachableFromConfig relation.
     *
     * Config c' is reachable from c if ct is the config that results after one
     * period elapses, and c' is reachable immediately from ct
     */
    std::shared_ptr<ReachabilityMatrix> pReachableFromConfig;

    std::shared_ptr<const ConfigurationManager> pConfigMgr;

    std::shared_ptr<Strategy> lastStrategy; /**< last strategy computed by evaluate() */

    /**
     * Executes Alloy to generate pImmediateReachabilityRelation and pReachableImmediately
     */
    void loadImmediateReachabilityRelation();

    /**
     * Executes Alloy to generate pReachableFromConfig
     */
    void loadReachabilityRelation();

    virtual bool isReachableImmediately(unsigned fromConfigIndex, unsigned toConfigIndex) const;
    virtual bool isReachableFromConfig(unsigned fromConfigIndex, unsigned toConfigIndex) const;

  public:

    SDPAdaptationManager();

    virtual void initialize(std::shared_ptr<const ConfigurationManager> configMgr);
    virtual TacticList evaluate(const Configuration& currentConfigObj, const EnvironmentDTMCPartitioned& envDTMC,
    		const UtilityFunction& utilityFunction, unsigned horizon, unsigned &transitionsEvaluated);
    virtual bool supportsStrategy() const;
    virtual std::shared_ptr<Strategy> getStrategy();
    virtual ~SDPAdaptationManager();
};

}

#endif
