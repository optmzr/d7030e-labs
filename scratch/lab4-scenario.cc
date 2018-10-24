/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "ns3/applications-module.h"
#include "ns3/config-store.h"
#include "ns3/core-module.h"
#include "ns3/epc-helper.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/lte-helper.h"
#include "ns3/lte-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/propagation-loss-model.h"

using namespace ns3;

/*
 * Instantiates one eNodeB which attaches one UE, and a remote host starts a
 * downlink flow for UE.
 */
NS_LOG_COMPONENT_DEFINE("LAB4");

int main(int argc, char *argv[]) {
  double simTime = 20;
  double appDataRate = 50;
  std::string outputPath = "";
  std::string antennaType = "ParabolicAntennaModel";
  int x, y, z = 0;

  CommandLine cmd;

  cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
  cmd.AddValue("outputPath", "The path of the trace outputs", outputPath);
  cmd.AddValue("x", "X-axis for UE vector", x);
  cmd.AddValue("y", "Y-axis for UE vector", y);
  cmd.AddValue("z", "Z-axis for UE vector", z);
  cmd.AddValue("appDataRate", "Application data rate [Mbps]", appDataRate);
  cmd.AddValue("antennaType",
               "The antenna type to use, see: "
               "https://www.nsnam.org/docs/models/html/"
               "antenna-design.html#provided-models",
               antennaType);

  cmd.Parse(argc, argv);

  // Define the path for the generated trace files.
  if (outputPath != "") {
    Config::SetDefault("ns3::RadioBearerStatsCalculator::DlRlcOutputFilename",
                       StringValue(outputPath + "/DlRlcStats.txt"));
    Config::SetDefault("ns3::RadioBearerStatsCalculator::UlRlcOutputFilename",
                       StringValue(outputPath + "/UlRlcStats.txt"));
    Config::SetDefault("ns3::RadioBearerStatsCalculator::DlPdcpOutputFilename",
                       StringValue(outputPath + "/DlPdcpStats.txt"));
    Config::SetDefault("ns3::RadioBearerStatsCalculator::UlPdcpOutputFilename",
                       StringValue(outputPath + "/UlPdcpStats.txt"));
    Config::SetDefault("ns3::MacStatsCalculator::DlOutputFilename",
                       StringValue(outputPath + "/DlMacStats.txt"));
    Config::SetDefault("ns3::MacStatsCalculator::UlOutputFilename",
                       StringValue(outputPath + "/UlMacStats.txt"));
  }

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults();

  // Parse again so you can override default values from the command line.
  cmd.Parse(argc, argv);

  NS_LOG_DEBUG("Configuration: \n"
               << "UE Vector: (" << x << ", " << y << ", " << z << ")\n"
               << "App. data rate: " << appDataRate << " Mbps\n"
               << "Simulation time: " << simTime << "\n"
               << "Output path: " << outputPath << "\n"
               << "Antenna type: " << antennaType);

  // Configure the LTE+EPC system. Don't touch these before you already
  // understand the whole LTE system and ns-3 source codes.
  Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();
  Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper>();
  lteHelper->SetEpcHelper(epcHelper);
  lteHelper->SetSchedulerType("ns3::PfFfMacScheduler");

  Config::SetDefault("ns3::LteAmc::AmcModel", EnumValue(LteAmc::PiroEW2010));

  lteHelper->SetEnbAntennaModelType("ns3::" + antennaType);

  lteHelper->SetEnbDeviceAttribute("DlEarfcn", UintegerValue(100));
  lteHelper->SetEnbDeviceAttribute("UlEarfcn", UintegerValue(100 + 18000));
  lteHelper->SetEnbDeviceAttribute("DlBandwidth", UintegerValue(50));
  lteHelper->SetEnbDeviceAttribute("UlBandwidth", UintegerValue(50));

  lteHelper->SetAttribute("PathlossModel",
                          StringValue("ns3::TwoRayGroundPropagationLossModel"));

  // Define P-Gateway in EPC.
  Ptr<Node> pgw = epcHelper->GetPgwNode();

  // Create a single RemoteHost.
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create(1);
  Ptr<Node> remoteHost = remoteHostContainer.Get(0);
  InternetStackHelper internet;
  internet.Install(remoteHostContainer);

  // Create the Internet.
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("100Gb/s")));
  p2ph.SetDeviceAttribute("Mtu", UintegerValue(1500));
  p2ph.SetChannelAttribute("Delay", TimeValue(Seconds(0.010)));
  NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign(internetDevices);

  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting =
      ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4>());
  remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"),
                                             Ipv4Mask("255.0.0.0"), 1);

  NodeContainer ueNodes;
  NodeContainer enbNodes;
  enbNodes.Create(1);
  ueNodes.Create(1); // Default: there is only 1 UE in this example.

  // Install Mobility Model.
  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

  mobility.Install(enbNodes);
  mobility.Install(ueNodes);

  Ptr<MobilityModel> MM;

  // Define the location of eNodeB.
  MM = enbNodes.Get(0)->GetObject<MobilityModel>();
  MM->SetPosition(Vector3D(0, 0, 30)); // Mast at 30 meters height

  // Define the location of the first UE.
  MM = ueNodes.Get(0)->GetObject<MobilityModel>();
  MM->SetPosition(Vector3D(x, y, z)); // Distance between UE and eNodeB.

  // Install LTE Devices to the nodes.
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice(enbNodes);
  NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice(ueNodes);

  // Install the IP stack on the UEs.
  internet.Install(ueNodes);
  Ptr<NetDevice> ueLteDevice = ueLteDevs.Get(0);
  Ipv4InterfaceContainer ueIpIface =
      epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevice));
  // Set the default gateway for the UE.
  Ptr<Ipv4StaticRouting> ueStaticRouting;
  ueStaticRouting =
      ipv4RoutingHelper.GetStaticRouting(ueNodes.Get(0)->GetObject<Ipv4>());
  ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);

  // Attach one UE per eNodeB.
  lteHelper->Attach(ueLteDevs, enbLteDevs.Get(0));

  // Install and start applications on UE and remote host.
  uint16_t dlPort = 1000;
  Ptr<Ipv4> ueIpv4;
  int32_t interface;
  Ipv4Address ueAddr;
  ApplicationContainer onOffApp;

  ueIpv4 = ueNodes.Get(0)->GetObject<Ipv4>();
  interface = ueIpv4->GetInterfaceForDevice(
      ueNodes.Get(0)->GetDevice(0)->GetObject<LteUeNetDevice>());
  NS_ASSERT(interface >= 0);
  NS_ASSERT(ueIpv4->GetNAddresses(interface) == 1);
  ueAddr = ueIpv4->GetAddress(interface, 0).GetLocal();

  OnOffHelper onOffHelper("ns3::UdpSocketFactory",
                          InetSocketAddress(ueAddr, dlPort));
  onOffHelper.SetAttribute(
      "OnTime", StringValue("ns3::ConstantRandomVariable[Constant=5000]"));
  onOffHelper.SetAttribute(
      "OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  onOffHelper.SetAttribute(
      "DataRate",
      DataRateValue(DataRate(std::to_string(appDataRate) + "Mbps")));
  onOffHelper.SetAttribute("PacketSize", UintegerValue(1024));
  onOffApp.Add(onOffHelper.Install(remoteHost));

  // LTE QoS bearer
  EpsBearer bearer(EpsBearer::NGBR_VOICE_VIDEO_GAMING);
  lteHelper->ActivateDedicatedEpsBearer(ueLteDevs.Get(0), bearer,
                                        EpcTft::Default());

  lteHelper->EnableTraces();
  p2ph.EnablePcapAll(outputPath + "/LTE");

  Simulator::Stop(Seconds(simTime));
  Simulator::Run();

  Simulator::Destroy();
  return 0;
}
