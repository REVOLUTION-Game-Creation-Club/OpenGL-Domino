#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#include "resource.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glaux.lib")
#pragma comment(lib, "winmm.lib")						// Added for timeGetTime()
//#pragma comment(lib, "revsound.lib")						// Added for timeGetTime()