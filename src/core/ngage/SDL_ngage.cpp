#include <e32std.h>

GLDEF_C TInt E32Dll(TDllReason aReason)
{
    switch (aReason) {
    case EDllProcessAttach:
        break;
    case EDllProcessDetach:
        break;
    case EDllThreadAttach:
        break;
    case EDllThreadDetach:
        break;
    }
    return KErrNone;
}
