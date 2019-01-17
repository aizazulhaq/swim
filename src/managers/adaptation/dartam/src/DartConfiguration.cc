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

#include <managers/adaptation/dartam/include/dartam/DartConfiguration.h>
#include <typeinfo>

namespace dart {
namespace am2 {

DartConfiguration::DartConfiguration(
        unsigned servers,
        unsigned dimmer,
        unsigned ttcAddServer) :
                servers(servers),
                dimmer(dimmer),
                ttcAddServer(ttcAddServer)
{
}

DartConfiguration::~DartConfiguration() {
}

void DartConfiguration::printOn(std::ostream& os) const {
	os << "(s=" << servers
	        << ", d=" << dimmer
			<< ", p=" << ttcAddServer
			<< ')';
}

unsigned DartConfiguration::getServers() const {
	return servers;
}

unsigned DartConfiguration::getDimmer() const {
	return dimmer;
}

// Returns the number of periods until the adaptation tactic completes
unsigned DartConfiguration::getTtcAddServer() const {
	return ttcAddServer;
}

void DartConfiguration::setServers(unsigned servers) {
	this->servers = servers;
}

void DartConfiguration::setDimmer(unsigned dimmer) {
	this->dimmer = dimmer;
}

void DartConfiguration::setTtcAddServer(unsigned ttcAddServer) {
	this->ttcAddServer = ttcAddServer;
}

bool DartConfiguration::equals(const Configuration& other) const {
    try {
        const DartConfiguration& otherConf = dynamic_cast<const DartConfiguration&>(other);
        return servers == otherConf.servers
                && dimmer == otherConf.dimmer
        		&& ttcAddServer == otherConf.ttcAddServer;
    }
    catch(std::bad_cast&) {}
    return false;
}

} /* namespace am2 */
} /* namespace dart */
