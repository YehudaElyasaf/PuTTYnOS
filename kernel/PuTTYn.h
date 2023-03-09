#pragma once

#define _DEBUG

#ifdef _DEBUG
#define _NETWORKING_DEBUG_LOG //print message when sent and received
#endif

void initialize();
void main();
