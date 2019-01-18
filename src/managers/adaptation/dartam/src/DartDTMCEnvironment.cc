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

#include <managers/adaptation/dartam/include/dartam/DartDTMCEnvironment.h>
#include <managers/adaptation/dartam/include/dartam/DartSimpleEnvironment.h>
#include <managers/adaptation/proactive/include/timeseries/TimeSeriesPredictor.h>

#include <vector>
#include <boost/math/distributions/poisson.hpp>

using namespace std;

namespace dart {
namespace am2 {

using namespace pladapt;

namespace {
struct DistributionApproximationParams {
	int points;
	vector<double> quantiles;
	vector<double> probabilities;
};

const DistributionApproximationParams ApproxParams[] = {
		{ // E_PT
				3, // points
				{ 0.05, 0.5, 0.95 }, // quantiles
				{ 0.185, 0.630, 0.185 } //probabilities
		},
		{ // POINT
				1, // points
				{ 0.5 }, // quantiles
				{ 1.0 } //probabilities
		}
};

}

DartDTMCEnvironment::DartDTMCEnvironment(DistributionApproximation approx)
	: EnvironmentDTMCPartitioned(1 + ApproxParams[approx].points)
{

	/*
	 * The environment has one root state, which is not used, and
	 * three other states, one for each point in the
	 * discretized distribution. Each state value is a vector that holds
	 * the value for each point in space over the horizon for one of the
	 * points in the discrete distribution.
	 *
	 * To model the independence of the distributions, each state in the
	 * DTMC has transitions to all the other states according to the
	 * probabilities of the distribution discretization.
	 *
	 * The value of the root state is never used if put in the part 0;
	 * only the transitions out of it are used, so its value is set to some bogus value.
	 */
    // TODO: figure out how to predict the environment
    setStateValue(0, std::make_shared<DartSimpleEnvironment>(0));
    assignToPart(0,0);

    // get the timeseries predictor
    boost::math::poisson_distribution<> dist(17);

    TransitionMatrix& tm = getTransitionMatrix();
    for (int q = 0; q < ApproxParams[approx].points; q++) {
        setStateValue(q, std::make_shared<DartSimpleEnvironment>(boost::math::quantile(dist, ApproxParams[approx].quantiles[q])));
        assignToPart(1, q);
        tm(0, q) = ApproxParams[approx].probabilities[q];
    }
}

DartDTMCEnvironment::~DartDTMCEnvironment() {
}

} /* namespace am2 */
} /* namespace dart */
