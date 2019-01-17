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

#include <managers/adaptation/dartam/include/dartam/DartConfigurationManager.h>
#include <managers/adaptation/dartam/include/dartam/DartConfiguration.h>

#include <memory>
#include <vector>
#include <iostream>

using namespace std;

namespace dart {
namespace am2 {

DartConfigurationManager::DartConfigurationManager(
    unsigned servers, unsigned dimmers, unsigned addServerLatencyPeriods) :
            servers(servers), dimmers(dimmers), addServerLatencyPeriods(addServerLatencyPeriods)
{
    for (unsigned s = 0; s < servers; s++) {
        for (unsigned d = 0; d < dimmers; d++) {
            for (unsigned p = 0; p < addServerLatencyPeriods; p++) {
                configSpace.insert(new DartConfiguration(s,d,p));
            }
        }
    }
    cout << "[DartConfigurationManager created, config space initialized" << endl;
}

DartConfigurationManager::~DartConfigurationManager() {
}

const pladapt::ConfigurationSpace& DartConfigurationManager::getConfigurationSpace() const {
	return configSpace;
}

std::unique_ptr<pladapt::Configuration> DartConfigurationManager::getConfigurationFromYaml(
		const YAML::Node& configDetails) const {
    unsigned s = configDetails["s"].as<int>();
    unsigned d = configDetails["d"].as<int>();
    unsigned p = configDetails["addServerProgress"].as<int>();

	return std::unique_ptr<pladapt::Configuration>(
			new DartConfiguration(s,d,p));
}

std::shared_ptr<DartConfiguration> DartConfigurationManager::getCurrentConfig() {
    return currentConfig;
}

void DartConfigurationManager::setCurrentConfig(const DartConfiguration& other) {
    currentConfig = std::make_shared<DartConfiguration>(other);
}

} /* namespace am2 */
} /* namespace dart */

unsigned dart::am2::DartConfigurationManager::getAddServerLatencyPeriods() const {
	return addServerLatencyPeriods;
}
