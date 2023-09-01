/* ============================================================================== //
// This file is apart of the Cobalt Programming Language. Cobalt is under the MIT //
// License. Read `cobalt.h` for license information.                              //
// ============================================================================== */

typedef struct {
  float x;
  float y;
  float z;
} Vec3;
typedef struct {
    int x; // position x
    int y; // position y
    int z; // position z
    int ox; // orientation x
    int oy; // orientation y
    int oz; // orientation z
    int sx; // scale x
    int sy; // scale y
    int sz; // scale z
    int px; // pivot x
    int py; // pivot y
    int pz; // pivot z
} Transform;
typedef struct {
  float x;
  float y;
} Vec2;
typedef struct {
  int r;
  int g;
  int b;
} Color;