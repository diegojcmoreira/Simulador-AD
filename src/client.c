#include "client.h"

Client createClient(double arrival, double serviceStart, double departure, int waintingListSize, int round) {
  Client c;
  
  c.arrivalTime = arrival;
  c.serviceStartTime = serviceStart;
  c.departureTime = departure;
  c.waintingListSize = waintingListSize;
  c.myRound = round;
  
  return c;
}