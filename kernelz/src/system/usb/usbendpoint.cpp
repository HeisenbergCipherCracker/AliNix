#include <system/usb/usbendpoint.h>

using namespace HeisenOs;
using namespace HeisenOs::common;
using namespace HeisenOs::system;

USBEndpoint::USBEndpoint(struct ENDPOINT_DESC* src)
{
    this->endpointNumber = src->end_point & 0xF;
    this->dir = (src->end_point & (1<<7)) ? EndpointDirection::In : EndpointDirection::Out;
    this->type = (EndpointType)(src->bm_attrbs & 0b11);
    this->maxPacketSize = src->max_packet_size;
    this->interval = src->interval;
}

bool USBEndpoint::Toggle()
{
    this->toggleState = !this->toggleState;
    return !this->toggleState; // Return original value
}

void USBEndpoint::SetToggle(bool v)
{
    this->toggleState = v;
}