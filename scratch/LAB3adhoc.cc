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

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/mobility-model.h"
#include "ns3/csma-module.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/internet-module.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/olsr-module.h"
#include <iostream>
#include "ns3/constant-rate-wifi-manager.h"
#include "ns3/nqos-wifi-mac-helper.h"


           
        
using namespace ns3;



NS_LOG_COMPONENT_DEFINE ("LAB3");


int
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nWifi = 6;
  std::string phyMode("DsssRate1Mbps");
  double nodeDistande = 200;


  CommandLine cmd;
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.Parse (argc,argv);

  if (nWifi > 18)
    {
      std::cout << "Number of wifi nodes " << nWifi << 
                   " specified exceeds the mobility bounding box" << std::endl;
      exit (1);
    }

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

/////////////////////////////Nodes/////////////////////////////  
  //TODO
  // Create nodes
  NodeContainer staNodes;
  staNodes.Create(nWifi);

 /////////////////////////////Wi-Fi part///////////////////////////// 

  //TODO
  // Create WifiChannel with PropagationLossModel and SpeedPropagationDelayModel
  Ptr<YansWifiChannel> wifiChannel = CreateObject <YansWifiChannel>(); // pointer to wifichannel object
  Ptr<TwoRayGroundPropagationLossModel> lossModel = CreateObject<TwoRayGroundPropagationLossModel> (); // 
  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject <ConstantSpeedPropagationDelayModel>();
  wifiChannel->SetPropagationLossModel(lossModel);
  wifiChannel->SetPropagationDelayModel(delayModel);


  //Physical layer of WiFi
  Config::SetDefault("ns3::ConstantRateWifiManager::DataMode", StringValue(phyMode));
  
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  
  phy.Set("TxPowerEnd", DoubleValue(16));
  phy.Set("TxPowerStart", DoubleValue(16));
  phy.Set("EnergyDetectionThreshold", DoubleValue(-80));
  phy.Set("CcaMode1Threshold", DoubleValue(-99));
  phy.Set("ChannelNumber", UintegerValue(7));
  //TODO
  //Attach WiFi channel to physical layer
  phy.SetChannel(wifiChannel);
  phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);

  //TODO
  //Create WiFi helper and set standart and RemoteStationManager
  WifiHelper wifi = WifiHelper();
  wifi.SetStandard(WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode",
	  StringValue(phyMode), "ControlMode",
	  StringValue(phyMode));
  //Mac layer
  // Note, chosen mac helper has QoS inactivated.
  // My understandning is that wikipedia says that QoS was introduces in 2005, i.e not included in the 802.11b standard.
  // hence, the choice of no QoS should be right.
  NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();
  mac.SetType ("ns3::AdhocWifiMac");

/////////////////////////////Devices///////////////////////////// 
  NetDeviceContainer devices = wifi.Install(phy, mac, staNodes);


  //TODO
  // Create network devices
  // Attach devices and all parts of WiFi system and Nodes
  



 /////////////////////////////Deployment///////////////////////////// 
//TODO
// Create mobility model with constant positions and deploy all devices in a line with 400m distance between them Z coordinate for all should be 1m!! 
  // lab specs specifies 200m distance between nodes, contradictory to this comment that says 400. 
  // loop places nodes with a distance of set distance in beginning constant declarations of main.
  
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
  for (int n = 0; n < nWifi; n++) {
	  positionAlloc->Add(Vector((n * nodeDistance), 0.0, 1.0));
  }

  mobility.SetPositionAllocator(positionAlloc);
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(staNodes);
/////////////////////////////Stack of protocols///////////////////////////// 

  
//  Enable OLSR routing
   OlsrHelper olsr;

 //  Install the routing protocol
   Ipv4ListRoutingHelper list;
   list.Add (olsr, 10);

  // Set up internet stack

  InternetStackHelper stack;
  stack.SetRoutingHelper (list);
  stack.Install (staNodes);

  Ipv4AddressHelper address;

 /////////////////////////////Ip addresation/////////////////////////////  
 //TODO
 //Create Ipv4InterfaceContainer 
 // assign IP addresses to WifiDevices into Ipv4InterfaceContainer
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer wifiInterfaces;
  wifiInterfaces = address.Assign(staNodes);

/////////////////////////////Application part///////////////////////////// 
 
   uint16_t dlPort = 1000; //Port number
    
    //Sending application on the first station
    
    ApplicationContainer onOffApp;
    OnOffHelper onOffHelper("ns3::UdpSocketFactory", InetSocketAddress(wifiInterfaces.GetAddress (nWifi-1), dlPort)); //OnOffApplication, UDP traffic,
    onOffHelper.SetAttribute("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=5000]"));
    onOffHelper.SetAttribute("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
    onOffHelper.SetAttribute("DataRate", DataRateValue(DataRate("10.0Mbps"))); //Traffic Bit Rate
    onOffHelper.SetAttribute("PacketSize", UintegerValue(300)); // Packet size
    onOffApp.Add(onOffHelper.Install(wifiStaNodes.Get(0)));  
 
    
  //Opening receiver socket on the last station
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink = Socket::CreateSocket (wifiStaNodes.Get (nWifi-1), tid);
  InetSocketAddress local = InetSocketAddress (wifiInterfaces.GetAddress (nWifi-1), dlPort);
  bool ipRecvTos = true;
  recvSink->SetIpRecvTos (ipRecvTos);
  bool ipRecvTtl = true;
  recvSink->SetIpRecvTtl (ipRecvTtl);
  recvSink->Bind (local);


///////////////USE WHEN WORKING WITH TCP TO FILL IN ARP TABLES//////////////////////
/*
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (wifiStaNodes.Get (nWifi-1));
  serverApps.Start (Seconds (2.0));
  serverApps.Stop (Seconds (6.0));

  UdpEchoClientHelper echoClient (wifiInterfaces.GetAddress (nWifi-1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (100));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (100));

  ApplicationContainer clientApps = echoClient.Install (wifiStaNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (6.0));
*/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////TCP app///////////////////////////
/*
  uint16_t port = 50000;
  Address sinkLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);
  ApplicationContainer sinkApp = sinkHelper.Install (wifiStaNodes.Get (nWifi-1));
  sinkApp.Start (Seconds (10.0));
  sinkApp.Stop (Seconds (100.0));
  // Create the OnOff applications to send TCP to the server (client part)
  OnOffHelper clientHelper ("ns3::TcpSocketFactory", Address ());
  clientHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  clientHelper.SetAttribute("DataRate", DataRateValue(DataRate("10.0Mbps"))); //Traffic Bit Rate
  clientHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
  ApplicationContainer clientApp;
      AddressValue clientAddress(InetSocketAddress (wifiInterfaces.GetAddress (nWifi-1), port));
      clientHelper.SetAttribute ("Remote", clientAddress);
      clientApp=clientHelper.Install (wifiStaNodes.Get (0));
      clientApp.Start (Seconds (10.0));
      clientApp.Stop (Seconds (100.0));
*/

////////////////////////////////////////////////////////////


/////////////////////////////Application part///////////////////////////// 

  Simulator::Stop (Seconds (100.0));
/////////////////////////////PCAP tracing/////////////////////////////   
   //TODO 
   //Enable PCAP tracing for all devices

  Simulator::Run ();
  Simulator::Destroy ();
return 0;
};
