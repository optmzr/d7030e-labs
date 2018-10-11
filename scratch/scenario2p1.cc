/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-model.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/olsr-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/wifi-module.h"
#include <iostream>

// Default Network Topology
//
//  Wifi 10.1.1.0
//
//  *            *
//  |            |
// n0(transm)   n1(receiver)

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("LAB2");

int main(int argc, char *argv[]) {
  uint32_t seed = 15;
  uint32_t payload = 1000;
  std::string rate("DsssRate1Mbps");
  std::string sta_prefix("result/WIFI_STA");
  std::string ap_prefix("result/WIFI_AP");

  CommandLine cmd;
  cmd.AddValue("seed", "Seed", seed);
  cmd.AddValue("payload", "Payload", payload);
  cmd.AddValue("rate", "Rate", rate);
  cmd.AddValue("sta", "STA prefix", sta_prefix);
  cmd.AddValue("ap", "AP prefix", ap_prefix);
  cmd.Parse(argc, argv);

  /* Seed the random generator */
  RngSeedManager::SetSeed(seed);

  /* Nodes */
  NodeContainer ap;
  NodeContainer stas;
  ap.Create(1);
  stas.Create(2);

  /* Wi-Fi part */
  Ptr<YansWifiChannel> wifiChannel =
      CreateObject<YansWifiChannel>(); // create a pointer for channel object
  Ptr<TwoRayGroundPropagationLossModel> lossModel =
      CreateObject<TwoRayGroundPropagationLossModel>(); // create a pointer for
                                                        // propagation loss
                                                        // model
  wifiChannel->SetPropagationLossModel(
      lossModel); // install propagation loss model
  Ptr<ConstantSpeedPropagationDelayModel> delayModel =
      CreateObject<ConstantSpeedPropagationDelayModel>();
  wifiChannel->SetPropagationDelayModel(
      delayModel); // install propagation delay model

  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold",
                     StringValue("2200"));
  // disable fragmentation
  Config::SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold",
                     StringValue("2200"));

  YansWifiPhyHelper phy = YansWifiPhyHelper::Default();
  phy.SetChannel(wifiChannel);
  phy.Set("TxPowerEnd", DoubleValue(16));
  phy.Set("TxPowerStart", DoubleValue(16));
  phy.Set("EnergyDetectionThreshold", DoubleValue(-80));
  phy.Set("CcaMode1Threshold", DoubleValue(-99));
  phy.Set("ChannelNumber", UintegerValue(7));

  WifiHelper wifi = WifiHelper();
  wifi.SetStandard(WIFI_PHY_STANDARD_80211b);

  Ssid ssid = Ssid("wifi-default");
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode",
                               StringValue(rate), "ControlMode",
                               StringValue(rate));

  WifiMacHelper mac = WifiMacHelper();

  // setup ap.
  NetDeviceContainer apDevices;
  mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
  apDevices = wifi.Install(phy, mac, ap);
  // setup stas.
  mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing",
              BooleanValue(false));
  NetDeviceContainer staDevices;
  staDevices = wifi.Install(phy, mac, stas);

  /* Deployment */
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc =
      CreateObject<ListPositionAllocator>();
  positionAlloc->Add(Vector(0.0, 0.0, 1.0));
  positionAlloc->Add(Vector(251.1, 0.0, 1.0));

  mobility.SetPositionAllocator(positionAlloc);
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(stas);
  Ptr<ListPositionAllocator> positionAllocAP =
      CreateObject<ListPositionAllocator>();
  positionAllocAP->Add(Vector(251.1 / 2, 8.6, 1.0));
  mobility.SetPositionAllocator(positionAllocAP);
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(ap);

  /* Stack of protocols */
  // Set up internet stack
  InternetStackHelper stack;
  stack.Install(ap);
  stack.Install(stas);
  Ipv4AddressHelper address;

  /* Ip addresation */
  address.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer wifiInterfaces;
  Ipv4InterfaceContainer wifiAPInterface;

  wifiAPInterface = address.Assign(apDevices);
  wifiInterfaces = address.Assign(staDevices);

  /* Application part */
  uint16_t dlPort = 1000;
  ApplicationContainer onOffApp;

  OnOffHelper onOffHelper(
      "ns3::UdpSocketFactory",
      InetSocketAddress(
          wifiInterfaces.GetAddress(1),
          dlPort)); // OnOffApplication, UDP traffic, Please refer the ns-3 API
  onOffHelper.SetAttribute(
      "OnTime",
      StringValue("ns3::NormalRandomVariable[Mean=10.0|Variance=2.0]"));
  onOffHelper.SetAttribute(
      "OffTime",
      StringValue("ns3::NormalRandomVariable[Mean=5.0|Variance=1.0]"));
  onOffHelper.SetAttribute(
      "DataRate", DataRateValue(DataRate("20.0Mbps"))); // Traffic Bit Rate
  onOffHelper.SetAttribute("PacketSize", UintegerValue(payload));
  onOffApp.Add(onOffHelper.Install(stas.Get(0)));

  // Receiver socket on Sta2
  TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink = Socket::CreateSocket(stas.Get(1), tid);
  InetSocketAddress local =
      InetSocketAddress(wifiInterfaces.GetAddress(1), dlPort);
  bool ipRecvTos = true;
  recvSink->SetIpRecvTos(ipRecvTos);
  bool ipRecvTtl = true;
  recvSink->SetIpRecvTtl(ipRecvTtl);
  recvSink->Bind(local);

  Simulator::Stop(Seconds(100.0));
  /* PCAP tracing */
  phy.EnablePcap(sta_prefix, stas, true);
  phy.EnablePcap(ap_prefix, ap, true);

  Simulator::Run();
  Simulator::Destroy();
  return 0;
};
