using System;

public class StringObj : IDisposable
{
    public IntPtr nativePtr;
    protected bool ownsMemory;

    internal StringObj(IntPtr ptr, bool ownsMemory = false)
    {
        nativePtr = ptr;
        this.ownsMemory = ownsMemory;
    }

    internal StringObj()
    {
        nativePtr = NativeMethods.csharp_new_string_obj();
        ownsMemory = true;
    }

    public String get(){ return NativeMethods.csharp_string_obj_get(nativePtr, ownsMemory); }

    public void set(String s) { NativeMethods.csharp_string_obj_set(nativePtr, ownsMemory, s); }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing) {
        if (ownsMemory && nativePtr != IntPtr.Zero) {
            // NativeMethods.Animal_delete(nativePtr);
            nativePtr = IntPtr.Zero;
        }
    }
}
