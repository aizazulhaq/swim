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

#include <managers/adaptation/dartam/include/dartam/DartUtilityFunction.h>
#include <managers/adaptation/dartam/include/dartam/DartConfiguration.h>
#include <managers/adaptation/dartam/include/dartam/DartSimpleEnvironment.h>
#include <util/MMcQueue.h>
#include <cmath>
#include <iostream>

using namespace pladapt;
using namespace std;

namespace dart {
namespace am2 {

DartUtilityFunction::DartUtilityFunction(double serviceRate)
    : serviceRate(serviceRate) {
    // no-op
}

DartUtilityFunction::~DartUtilityFunction() {
    // no-op
}

double DartUtilityFunction::getAdditiveUtility(
		const pladapt::Configuration& config,
		const pladapt::Environment& env,
		int time) const {
    const DartConfiguration dartConfig  = dynamic_cast<const DartConfiguration&>(config);
    const DartSimpleEnvironment dartEnv = dynamic_cast<const DartSimpleEnvironment&>(env);
    // calculate cost:
    unsigned provisionedServers = dartConfig.getServers() + (dartConfig.getTtcAddServer() > 0 ? 1 : 0);
    double cost = 1.0 - (provisionedServers / 12.0d);
    double dimm = dartConfig.getDimmer() / 9.0d;

    double mu   = serviceRate*pow(1.2d, 9.0d - dartConfig.getDimmer());
    double rate = MMcQueue::totalTime(dartConfig.getServers(), mu, 60000/dartEnv.getProbOfObject(), 60000/dartEnv.getProbOfObject());
    double resp = 1.0-min(1.0, rate/3000);

    return 0.4d*cost + 0.1d*dimm + 0.5d*resp;
}

double DartUtilityFunction::getMultiplicativeUtility(
		const pladapt::Configuration& config,
		const pladapt::Environment& env,
		int time) const {
    return 1.0d;
}

double DartUtilityFunction::getFinalReward(
		const pladapt::Configuration& config,
		const pladapt::Environment& env,
		int time) const {
	return 0.0d;
}

} /* namespace am2 */
} /* namespace dart */
