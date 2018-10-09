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

NS_LOG_COMPONENT_DEFINE("LAB1");

int main(int argc, char *argv[]) {
  bool verbose = true;
  uint32_t nWifi = 6;

  CommandLine cmd;
  cmd.AddValue("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);
  cmd.Parse(argc, argv);

  if (nWifi > 18) {
    std::cout << "Number of wifi nodes " << nWifi
              << " specified exceeds the mobility bounding box" << std::endl;
    exit(1);
  }

  if (verbose) {
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
  }

  /* Nodes */
  NodeContainer ap;
  NodeContainer stas;
  ap.Create(1);
  stas.Create(4);

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
                               StringValue("DsssRate1Mbps"), "ControlMode",
                               StringValue("DsssRate1Mbps"));

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
  positionAlloc->Add(Vector(10.0, 0.0, 1.0));
  positionAlloc->Add(Vector(0.0, 10.0, 1.0));
  positionAlloc->Add(Vector(10.0, 10.0, 1.0));

  mobility.SetPositionAllocator(positionAlloc);
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(stas);

  Ptr<ListPositionAllocator> positionAllocAP =
      CreateObject<ListPositionAllocator>();
  positionAllocAP->Add(Vector(5.0, 8.6, 1.0));
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

  bool ipRecvTos = true;
  bool ipRecvTtl = true;

  // Transmitter 0 (ID = 0)
  OnOffHelper onOffHelper0(
      "ns3::UdpSocketFactory",
      InetSocketAddress(
          wifiInterfaces.GetAddress(1),
          dlPort)); // OnOffApplication, UDP traffic, Please refer the ns-3 API
  onOffHelper0.SetAttribute(
      "OnTime", StringValue("ns3::ConstantRandomVariable[Constant=5000]"));
  onOffHelper0.SetAttribute(
      "OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  onOffHelper0.SetAttribute(
      "DataRate", DataRateValue(DataRate("20.0Mbps"))); // Traffic Bit Rate
  onOffHelper0.SetAttribute("PacketSize", UintegerValue(1000));
  onOffApp.Add(onOffHelper0.Install(stas.Get(0)));

  // Transmitter 1 (ID = 2)
  OnOffHelper onOffHelper1(
      "ns3::UdpSocketFactory",
      InetSocketAddress(
          wifiInterfaces.GetAddress(3),
          dlPort)); // OnOffApplication, UDP traffic, Please refer the ns-3 API
  onOffHelper1.SetAttribute(
      "OnTime", StringValue("ns3::ConstantRandomVariable[Constant=5000]"));
  onOffHelper1.SetAttribute(
      "OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  onOffHelper1.SetAttribute(
      "DataRate", DataRateValue(DataRate("20.0Mbps"))); // Traffic Bit Rate
  onOffHelper1.SetAttribute("PacketSize", UintegerValue(1000));
  onOffApp.Add(onOffHelper1.Install(stas.Get(2)));

  // Receiver 0 (ID = 1)
  TypeId tid0 = TypeId::LookupByName("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink0 = Socket::CreateSocket(stas.Get(1), tid0);
  InetSocketAddress local0 =
      InetSocketAddress(wifiInterfaces.GetAddress(1), dlPort);
  recvSink0->SetIpRecvTos(ipRecvTos);
  recvSink0->SetIpRecvTtl(ipRecvTtl);
  recvSink0->Bind(local0);

  // Receiver 1 (ID = 3)
  TypeId tid1 = TypeId::LookupByName("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink1 = Socket::CreateSocket(stas.Get(3), tid1);
  InetSocketAddress local1 =
      InetSocketAddress(wifiInterfaces.GetAddress(3), dlPort);
  recvSink1->SetIpRecvTos(ipRecvTos);
  recvSink1->SetIpRecvTtl(ipRecvTtl);
  recvSink1->Bind(local1);

  Simulator::Stop(Seconds(100.0));
  /* PCAP tracing */
  phy.EnablePcap("results/WIFI_STA", stas, true);
  phy.EnablePcap("results/WIFI_AP", ap, true);

  Simulator::Run();
  Simulator::Destroy();
  return 0;
};
