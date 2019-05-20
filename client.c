#include "client.h"

Client createClient(double arrival, double serviceStart, double departure) {
  Client c;
  
  c.arrivalTime = arrival;
  c.serviceStartTime = serviceStart;
  c.departureTime = departure;
  
  return c;
}