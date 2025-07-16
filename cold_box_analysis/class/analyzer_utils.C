// ________________________________________ //
// Author: Henrique Souza
// Filename: analyzer_utils.C
// Created: 2025-07-06
// ________________________________________ //


#include "MYCODES.h"

void ANALYZER::load_device_map(string experiment) {
  map_channel_device.clear();
  if (experiment == "NP02") {
    map_channel_device = {
      {"Ch10636.", "C4(1)"},
      {"Ch10637.", "C4(2)"},
      {"Ch10634.", "C3(1)"},
      {"Ch10635.", "C3(2)"},
      {"Ch10632.", "C1(1)"},
      {"Ch10633.", "C1(2)"},
      {"Ch10630.", "C2(1)"},
      {"Ch10631.", "C2(2)"},
      {"Ch1064.",  "C8(1)"},
      {"Ch1066.",  "C8(2)"},
      {"Ch1065.",  "C7(1)"},
      {"Ch1067.",  "C7(2)"},
      {"Ch1060.",  "C5(1)"},
      {"Ch1062.",  "C5(2)"},
      {"Ch1061.",  "C6(1)"},
      {"Ch1063.",  "C6(2)"},

      {"Ch10747.", "M1(1)"},
      {"Ch10745.", "M1(2)"},
      {"Ch10740.", "M2(1)"},
      {"Ch10742.", "M2(2)"},
      {"Ch1070.",  "M5(1)"},
      {"Ch1077.",  "M5(2)"},
      {"Ch10720.", "M6(1)"},
      {"Ch10727.", "M6(2)"},

      {"Ch10745.", "M3(1)"},
      {"Ch10744.", "M3(2)"},
      {"Ch10743.", "M4(1)"},
      {"Ch10741.", "M4(2)"},
      {"Ch10730.", "M7(1)"},
      {"Ch10737.", "M7(2)"},
      {"Ch10710.", "M8(1)"},
      {"Ch10717.", "M8(2)"},
      // Add more channels and devices as needed
    };
    
    for (map<string, string>::iterator i = map_channel_device.begin(); i != map_channel_device.end(); ++i)
      map_device_channel[i->second] = i->first;

  }
  else {
    cout << "Unknown experiment: " << experiment << endl;
  }
}

string ANALYZER::getDevice(int idx){
  if (idx >= (int)channels.size()) {
    cout << "Invalid channel index: " << idx << endl;
    return "";
  }
  if (idx < 0) idx = this->kch;
  
  string channel_key = this->schannel[idx];
  auto it = map_channel_device.find(channel_key);
  
  if (it != map_channel_device.end()) {
    return it->second; // Return the device name
  }
  else {
    cout << "Channel not found in device map: " << channel_key << endl;
    cout << "Returning branch name instead." << endl;
    return channel_key; // Return the channel name if not found in the map

  }
}

bool ANALYZER::setDevice(string device) {
  auto it = map_device_channel.find(device);
  if (it != map_device_channel.end()) {
    string channel_name = it->second;
    return this->setChannel(channel_name);
  } else {
    cout << "Device " << device << " not found in device map." << endl;
    return false;
  }
}
