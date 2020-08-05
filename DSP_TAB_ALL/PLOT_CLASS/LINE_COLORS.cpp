const RGBQUAD InitLineStyle[8] = {
{   0,   0, 255, 0},// ROJO. - 2
{   0, 128,   0, 0},
{ 255,   0,   0, 0},
{ 191,   0, 191, 0},
{   0, 128, 255, 0},
{ 100, 100, 100, 0},
{ 191, 191,   0, 0},
{   0, 255, 255, 0}
};



RGBQUAD LineColors[8] = {
{   0,   0, 255, 0},// ROJO. - 2
{   0, 128,   0, 0},
{ 255,   0,   0, 0},
{ 191,   0, 191, 0},
{   0, 128, 255, 0},
{ 100, 100, 100, 0},
{ 191, 191,   0, 0},
{   0, 255, 255, 0}
};
const RGBQUAD BLACKCOLOR = { 0, 0, 0, 0 };
const RGBQUAD WHITECOLOR = { 255, 255, 255, 0 };


const RGBQUAD jet[256] = {
{ 0, 0, 131, 0 },
{ 0, 0, 135, 0 },
{ 0, 0, 139, 0 },
{ 0, 0, 143, 0 },
{ 0, 0, 147, 0 },
{ 0, 0, 151, 0 },
{ 0, 0, 155, 0 },
{ 0, 0, 159, 0 },
{ 0, 0, 163, 0 },
{ 0, 0, 167, 0 },
{ 0, 0, 171, 0 },
{ 0, 0, 175, 0 },
{ 0, 0, 179, 0 },
{ 0, 0, 183, 0 },
{ 0, 0, 187, 0 },
{ 0, 0, 191, 0 },
{ 0, 0, 195, 0 },
{ 0, 0, 199, 0 },
{ 0, 0, 203, 0 },
{ 0, 0, 207, 0 },
{ 0, 0, 211, 0 },
{ 0, 0, 215, 0 },
{ 0, 0, 219, 0 },
{ 0, 0, 223, 0 },
{ 0, 0, 227, 0 },
{ 0, 0, 231, 0 },
{ 0, 0, 235, 0 },
{ 0, 0, 239, 0 },
{ 0, 0, 243, 0 },
{ 0, 0, 247, 0 },
{ 0, 0, 251, 0 },
{ 0, 0, 255, 0 },
{ 0, 4, 255, 0 },
{ 0, 8, 255, 0 },
{ 0, 12, 255, 0 },
{ 0, 16, 255, 0 },
{ 0, 20, 255, 0 },
{ 0, 24, 255, 0 },
{ 0, 28, 255, 0 },
{ 0, 32, 255, 0 },
{ 0, 36, 255, 0 },
{ 0, 40, 255, 0 },
{ 0, 44, 255, 0 },
{ 0, 48, 255, 0 },
{ 0, 52, 255, 0 },
{ 0, 56, 255, 0 },
{ 0, 60, 255, 0 },
{ 0, 64, 255, 0 },
{ 0, 68, 255, 0 },
{ 0, 72, 255, 0 },
{ 0, 76, 255, 0 },
{ 0, 80, 255, 0 },
{ 0, 84, 255, 0 },
{ 0, 88, 255, 0 },
{ 0, 92, 255, 0 },
{ 0, 96, 255, 0 },
{ 0, 100, 255, 0 },
{ 0, 104, 255, 0 },
{ 0, 108, 255, 0 },
{ 0, 112, 255, 0 },
{ 0, 116, 255, 0 },
{ 0, 120, 255, 0 },
{ 0, 124, 255, 0 },
{ 0, 128, 255, 0 },
{ 0, 131, 255, 0 },
{ 0, 135, 255, 0 },
{ 0, 139, 255, 0 },
{ 0, 143, 255, 0 },
{ 0, 147, 255, 0 },
{ 0, 151, 255, 0 },
{ 0, 155, 255, 0 },
{ 0, 159, 255, 0 },
{ 0, 163, 255, 0 },
{ 0, 167, 255, 0 },
{ 0, 171, 255, 0 },
{ 0, 175, 255, 0 },
{ 0, 179, 255, 0 },
{ 0, 183, 255, 0 },
{ 0, 187, 255, 0 },
{ 0, 191, 255, 0 },
{ 0, 195, 255, 0 },
{ 0, 199, 255, 0 },
{ 0, 203, 255, 0 },
{ 0, 207, 255, 0 },
{ 0, 211, 255, 0 },
{ 0, 215, 255, 0 },
{ 0, 219, 255, 0 },
{ 0, 223, 255, 0 },
{ 0, 227, 255, 0 },
{ 0, 231, 255, 0 },
{ 0, 235, 255, 0 },
{ 0, 239, 255, 0 },
{ 0, 243, 255, 0 },
{ 0, 247, 255, 0 },
{ 0, 251, 255, 0 },
{ 0, 255, 255, 0 },
{ 4, 255, 251, 0 },
{ 8, 255, 247, 0 },
{ 12, 255, 243, 0 },
{ 16, 255, 239, 0 },
{ 20, 255, 235, 0 },
{ 24, 255, 231, 0 },
{ 28, 255, 227, 0 },
{ 32, 255, 223, 0 },
{ 36, 255, 219, 0 },
{ 40, 255, 215, 0 },
{ 44, 255, 211, 0 },
{ 48, 255, 207, 0 },
{ 52, 255, 203, 0 },
{ 56, 255, 199, 0 },
{ 60, 255, 195, 0 },
{ 64, 255, 191, 0 },
{ 68, 255, 187, 0 },
{ 72, 255, 183, 0 },
{ 76, 255, 179, 0 },
{ 80, 255, 175, 0 },
{ 84, 255, 171, 0 },
{ 88, 255, 167, 0 },
{ 92, 255, 163, 0 },
{ 96, 255, 159, 0 },
{ 100, 255, 155, 0 },
{ 104, 255, 151, 0 },
{ 108, 255, 147, 0 },
{ 112, 255, 143, 0 },
{ 116, 255, 139, 0 },
{ 120, 255, 135, 0 },
{ 124, 255, 131, 0 },
{ 128, 255, 128, 0 },
{ 131, 255, 124, 0 },
{ 135, 255, 120, 0 },
{ 139, 255, 116, 0 },
{ 143, 255, 112, 0 },
{ 147, 255, 108, 0 },
{ 151, 255, 104, 0 },
{ 155, 255, 100, 0 },
{ 159, 255, 96, 0 },
{ 163, 255, 92, 0 },
{ 167, 255, 88, 0 },
{ 171, 255, 84, 0 },
{ 175, 255, 80, 0 },
{ 179, 255, 76, 0 },
{ 183, 255, 72, 0 },
{ 187, 255, 68, 0 },
{ 191, 255, 64, 0 },
{ 195, 255, 60, 0 },
{ 199, 255, 56, 0 },
{ 203, 255, 52, 0 },
{ 207, 255, 48, 0 },
{ 211, 255, 44, 0 },
{ 215, 255, 40, 0 },
{ 219, 255, 36, 0 },
{ 223, 255, 32, 0 },
{ 227, 255, 28, 0 },
{ 231, 255, 24, 0 },
{ 235, 255, 20, 0 },
{ 239, 255, 16, 0 },
{ 243, 255, 12, 0 },
{ 247, 255, 8, 0 },
{ 251, 255, 4, 0 },
{ 255, 255, 0, 0 },
{ 255, 251, 0, 0 },
{ 255, 247, 0, 0 },
{ 255, 243, 0, 0 },
{ 255, 239, 0, 0 },
{ 255, 235, 0, 0 },
{ 255, 231, 0, 0 },
{ 255, 227, 0, 0 },
{ 255, 223, 0, 0 },
{ 255, 219, 0, 0 },
{ 255, 215, 0, 0 },
{ 255, 211, 0, 0 },
{ 255, 207, 0, 0 },
{ 255, 203, 0, 0 },
{ 255, 199, 0, 0 },
{ 255, 195, 0, 0 },
{ 255, 191, 0, 0 },
{ 255, 187, 0, 0 },
{ 255, 183, 0, 0 },
{ 255, 179, 0, 0 },
{ 255, 175, 0, 0 },
{ 255, 171, 0, 0 },
{ 255, 167, 0, 0 },
{ 255, 163, 0, 0 },
{ 255, 159, 0, 0 },
{ 255, 155, 0, 0 },
{ 255, 151, 0, 0 },
{ 255, 147, 0, 0 },
{ 255, 143, 0, 0 },
{ 255, 139, 0, 0 },
{ 255, 135, 0, 0 },
{ 255, 131, 0, 0 },
{ 255, 128, 0, 0 },
{ 255, 124, 0, 0 },
{ 255, 120, 0, 0 },
{ 255, 116, 0, 0 },
{ 255, 112, 0, 0 },
{ 255, 108, 0, 0 },
{ 255, 104, 0, 0 },
{ 255, 100, 0, 0 },
{ 255, 96, 0, 0 },
{ 255, 92, 0, 0 },
{ 255, 88, 0, 0 },
{ 255, 84, 0, 0 },
{ 255, 80, 0, 0 },
{ 255, 76, 0, 0 },
{ 255, 72, 0, 0 },
{ 255, 68, 0, 0 },
{ 255, 64, 0, 0 },
{ 255, 60, 0, 0 },
{ 255, 56, 0, 0 },
{ 255, 52, 0, 0 },
{ 255, 48, 0, 0 },
{ 255, 44, 0, 0 },
{ 255, 40, 0, 0 },
{ 255, 36, 0, 0 },
{ 255, 32, 0, 0 },
{ 255, 28, 0, 0 },
{ 255, 24, 0, 0 },
{ 255, 20, 0, 0 },
{ 255, 16, 0, 0 },
{ 255, 12, 0, 0 },
{ 255, 8, 0, 0 },
{ 255, 4, 0, 0 },
{ 255, 0, 0, 0 },
{ 251, 0, 0, 0 },
{ 247, 0, 0, 0 },
{ 243, 0, 0, 0 },
{ 239, 0, 0, 0 },
{ 235, 0, 0, 0 },
{ 231, 0, 0, 0 },
{ 227, 0, 0, 0 },
{ 223, 0, 0, 0 },
{ 219, 0, 0, 0 },
{ 215, 0, 0, 0 },
{ 211, 0, 0, 0 },
{ 207, 0, 0, 0 },
{ 203, 0, 0, 0 },
{ 199, 0, 0, 0 },
{ 195, 0, 0, 0 },
{ 191, 0, 0, 0 },
{ 187, 0, 0, 0 },
{ 183, 0, 0, 0 },
{ 179, 0, 0, 0 },
{ 175, 0, 0, 0 },
{ 171, 0, 0, 0 },
{ 167, 0, 0, 0 },
{ 163, 0, 0, 0 },
{ 159, 0, 0, 0 },
{ 155, 0, 0, 0 },
{ 151, 0, 0, 0 },
{ 147, 0, 0, 0 },
{ 143, 0, 0, 0 },
{ 139, 0, 0, 0 },
{ 135, 0, 0, 0 },
{ 131, 0, 0, 0 },
{ 128, 0, 0, 0 }
};

