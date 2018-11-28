#include "log.h"

int main()
{
  log_init(LOG_ALL, 0, "./test.log");

  LOG_WRITE(LOG_DEBUG, "hell world%c", '\n');
  LOG_WRITE(LOG_DEBUG, "hell world%c", '\n');
  LOG_WRITE(LOG_DEBUG, "hell world%c", '\n');
  LOG_WRITE(LOG_DEBUG, "hell world%c", '\n');
  LOG_WRITE(LOG_DEBUG, "hell world%c", '\n');
  LOG_WRITE(LOG_DEBUG, "hell world%c", '\n');
  LOG_WRITE(LOG_DEBUG, "hell world%c", '\n');

  return 0;
}
