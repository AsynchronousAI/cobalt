/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

#ifndef windowsDEFINED
#define windowsDEFINED

#include <windows.h>
#undef CreateSemaphore
#undef CreateEvent
#undef SetEvent
#undef ResetEvent
#undef GetObject

#endif /* windowsDEFINED */
