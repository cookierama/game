#include "globals.h"
#include <stdlib.h>
#include <QtGlobal>

int mrand( const int min_val, const int max_val )
{
	return int( qrand() / (RAND_MAX + 1.0) * (max_val + 1 - min_val) + min_val );
}
