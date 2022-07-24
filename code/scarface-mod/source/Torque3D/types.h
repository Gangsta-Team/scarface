//------------------------------------------------------------------------------
//-------------------------------------- Basic Types...

typedef signed char        S8;      ///< Compiler independent Signed Char
typedef unsigned char      U8;      ///< Compiler independent Unsigned Char

typedef signed short       S16;     ///< Compiler independent Signed 16-bit short
typedef unsigned short     U16;     ///< Compiler independent Unsigned 16-bit short

typedef signed int         S32;     ///< Compiler independent Signed 32-bit integer
typedef unsigned int       U32;     ///< Compiler independent Unsigned 32-bit integer

typedef float              F32;     ///< Compiler independent 32-bit float
typedef double             F64;     ///< Compiler independent 64-bit float

struct EmptyType {};             ///< "Null" type used by templates

#define TORQUE_UNUSED(var) (void)var

//------------------------------------------------------------------------------
//------------------------------------- String Types

typedef char           UTF8;        ///< Compiler independent 8  bit Unicode encoded character
typedef unsigned short UTF16;       ///< Compiler independent 16 bit Unicode encoded character
typedef unsigned int   UTF32;       ///< Compiler independent 32 bit Unicode encoded character

typedef const char* StringTableEntry;

//------------------------------------------------------------------------------
//-------------------------------------- Type constants...
#define __EQUAL_CONST_F F32(0.000001)                             ///< Constant float epsilon used for F32 comparisons

extern const F32 Float_Inf;
static const F32 Float_One  = F32(1.0);                           ///< Constant float 1.0
static const F32 Float_Half = F32(0.5);                           ///< Constant float 0.5
static const F32 Float_Zero = F32(0.0);                           ///< Constant float 0.0
static const F32 Float_Pi   = F32(3.14159265358979323846);        ///< Constant float PI
static const F32 Float_2Pi  = F32(2.0 * 3.14159265358979323846);  ///< Constant float 2*PI
static const F32 Float_InversePi = F32(1.0 / 3.14159265358979323846); ///< Constant float 1 / PI
static const F32 Float_HalfPi = F32(0.5 * 3.14159265358979323846);    ///< Constant float 1/2 * PI
static const F32 Float_2InversePi = F32(2.0 / 3.14159265358979323846);///< Constant float 2 / PI
static const F32 Float_Inverse2Pi = F32(0.5 / 3.14159265358979323846);///< Constant float 0.5 / PI

static const F32 Float_Sqrt2 = F32(1.41421356237309504880f);          ///< Constant float sqrt(2)
static const F32 Float_SqrtHalf = F32(0.7071067811865475244008443f);  ///< Constant float sqrt(0.5)

static const S8  S8_MIN  = S8(-128);                              ///< Constant Min Limit S8
static const S8  S8_MAX  = S8(127);                               ///< Constant Max Limit S8
static const U8  U8_MAX  = U8(255);                               ///< Constant Max Limit U8

static const S16 S16_MIN = S16(-32768);                           ///< Constant Min Limit S16
static const S16 S16_MAX = S16(32767);                            ///< Constant Max Limit S16
static const U16 U16_MAX = U16(65535);                            ///< Constant Max Limit U16

static const S32 S32_MIN = S32(-2147483647 - 1);                  ///< Constant Min Limit S32
static const S32 S32_MAX = S32(2147483647);                       ///< Constant Max Limit S32
static const U32 U32_MAX = U32(0xffffffff);                       ///< Constant Max Limit U32

static const F32 F32_MIN = F32(1.175494351e-38F);                 ///< Constant Min Limit F32
static const F32 F32_MAX = F32(3.402823466e+38F);                 ///< Constant Max Limit F32