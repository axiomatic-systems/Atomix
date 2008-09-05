/*****************************************************************
|
|      File: AtxSymbianHelper.c
|
|      Atomix - Helpers: SymbianOS Implementation
|
|      (c) 2002-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "AtxSymbianHelper.h"
#include "AtxSymbianUtils.h"

#include "AtxTypes.h"
#include "AtxResults.h"
#include "AtxDebug.h"
#include "AtxFile.h"
#include "AtxUtils.h"

#include "e32def.h"
#include "e32math.h"

#include "f32file.h"

#include "string.h" // symbian's string.h



/*----------------------------------------------------------------------
|       ATX_Symbian_GetRandomBytes
+---------------------------------------------------------------------*/
unsigned long
ATX_Symbian_GetRandomBytes(unsigned char *buf, unsigned long len)
{
    TUint32 acc = 0;
    for (unsigned long i = 0; i < len; ++i) {
      if (len % 4 == 0)
          acc = Math::Random();
      buf[i] = acc;
      acc >>= 8;
   }
    
    return len;
}


/*----------------------------------------------------------------------
|       ATX_Heap_Available
+---------------------------------------------------------------------*/
ATX_Int32
ATX_Heap_Available()
{
    RHeap& heap = User::Heap();
    TInt biggestblock = 0;
    /*TInt freemem =*/ 
    heap.Available(biggestblock);
    return biggestblock;
}
