#include "client.h"

Client createClient(double arrival, double serviceStart, double departure, int waintingListSize) {
  Client c;
  
  c.arrivalTime = arrival;
  c.serviceStartTime = serviceStart;
  c.departureTime = departure;
  c.waintingListSize = waintingListSize;
  
  return c;
}