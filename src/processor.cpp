#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
long Processor::Idle(){return idle_;}
long Processor::Active(){return active_;}
long Processor::Total(){return total_;}
void Processor::Store(long idle, long active, long total)
{
 
  active_=active;
  idle_=idle;
  total_=total;
}

float Processor::Utilization() 
{
  long total1 = Processor::Total();
  long active1 = Processor::Active();
  long idle1 = Processor::Idle();

  long total2 = LinuxParser::Jiffies();
  long active2 = LinuxParser::ActiveJiffies();
  long idle2 = LinuxParser::IdleJiffies();
  float totalDelta,idleDetla,utilization;

  Processor::Store(idle2,active2,total2);

  totalDelta = total2-total1;
  idleDetla = idle2-idle1;

  utilization = (totalDelta - idleDetla);
  utilization /= totalDelta;
  return utilization;
}