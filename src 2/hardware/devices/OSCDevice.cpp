#include <string.h>

#include "OSCDevice.h"
#include "random.h"
#include "logging.h"

#include <oscpp/client.hpp>

OSCDevice::OSCDevice()
{
    channel_count = 512;
    address = sf::IpAddress("127.0.0.1");
    port_number = 53000;
    send_data_only_if_changed = false;
    for (int i = 0; i < 512; i++) {
        osc_addresses[i] = "/emptyepsilon/channel/" + std::to_string(i + 1);
        previous_values[i] = -1;
    }
    primary_ship = true;
    socket.bind(0); // Bind to random port
}

bool OSCDevice::configure(std::unordered_map<string, string> settings)
{
    if (settings.find("channels") != settings.end())
    {
        LOG(INFO) << "Setting number of channels for OSCDevice to: " << settings["channels"].toInt();
        channel_count = std::max(1, std::min(512, settings["channels"].toInt()));
    }
    if (settings.find("server_address") != settings.end())
    {
        LOG(INFO) << "Setting IP address for OSCDevice to: " << settings["server_address"];
        address = sf::IpAddress(settings["server_address"]);
    }
    if (settings.find("port_number") != settings.end())
    {
        LOG(INFO) << "Setting port number for OSCDevice to: " << settings["port_number"];
        port_number = settings["port_number"].toInt();
    }
    if (settings.find("send_data_only_if_changed") != settings.end())
    {
        LOG(INFO) << "Setting 'send data only if changed' for OSCDevice to: " << settings["send_data_only_if_changed"];
        send_data_only_if_changed = settings["send_data_only_if_changed"].strip().lower().startswith("y") || settings["send_data_only_if_changed"].strip().lower().startswith("t") || settings["send_data_only_if_changed"].strip().lower().startswith("1");
    }
    if (settings.find("primary_ship") != settings.end()) {
        LOG(INFO) << "Setting whether device is primary ship for OSCDevice to: " << settings["primary_ship"];
        primary_ship = settings["primary_ship"].strip().lower().startswith("y") || settings["primary_ship"].strip().lower().startswith("t") || settings["primary_ship"].strip().lower().startswith("1");
    }
    if (settings.find("address_prefix") != settings.end())
    {
        LOG(INFO) << "Setting address prefix for OSCDevice to: " << settings["address_prefix"].strip();
        for (int i = 0; i < 512; i++) {
            osc_addresses[i] = settings["address_prefix"].strip() + std::to_string(i);
        }
    }
    if (settings.find("channel_addresses") != settings.end())
    {
        int i = 0;
        for(string channel_address : settings["channel_addresses"].split(","))
        {
            LOG(INFO) << "Setting address for OSCDevice channel " << i << " to: " << channel_address.strip();
            osc_addresses[i] = channel_address.strip();
            i++;   
        }
    }

    return true;
}

//Set a hardware channel output. Value is 0.0 to 1.0 for no to max output.
void OSCDevice::setChannelData(int channel, float value)
{
    bool send = (!primary_ship && channel >= (getChannelCount() / 2)) || (primary_ship && channel < (getChannelCount() / 2));
    int rounded_value = static_cast<int>(value);
    if (send && (previous_values[channel] != rounded_value || !send_data_only_if_changed)) {
        previous_values[channel] = rounded_value;
        // LOG(INFO) << "Preparing OSC packet: create buffer."; // for: " << osc_addresses[channel] << ", with value: " << rounded_value;
        uint8_t* buffer = new uint8_t[maximum_udp_packet_size];
        // LOG(INFO) << "Preparing OSC packet: create packet.";
        OSCPP::Client::Packet packet(buffer, maximum_udp_packet_size);
        // LOG(INFO) << "Preparing OSC packet: get pointer to char array for address.";
        const char* osc_address = osc_addresses[channel].c_str();
        // LOG(INFO) << "Preparing OSC packet: write message contents.";
        packet.openMessage(osc_address, 1).int32(rounded_value).closeMessage();
        // LOG(INFO) << "OSC packet prepared for: " << osc_addresses[channel] << ", with value: " << rounded_value << ", packet size: " << packet.size();
        socket.send(buffer, packet.size(), address, port_number);
        // LOG(INFO) << "Packet sent to: " << address.toString() << ", port number: " << port_number;
        delete buffer;
    }
}

//Return the number of output channels supported by this device.
int OSCDevice::getChannelCount()
{
    return channel_count;
}
