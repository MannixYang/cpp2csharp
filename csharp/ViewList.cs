using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;


public class View
{
    public IntPtr nativePtr;
    protected bool ownsMemory;
    public int Id { get; }

    internal View(IntPtr ptr, bool ownsMemory = false)
    {
        nativePtr = ptr;
        this.ownsMemory = ownsMemory;
        Id = NativeMethods.View_getId(nativePtr);
    }
}

public class ViewList : IDisposable, IEnumerable<KeyValuePair<int, View>>
{
    internal IntPtr nativePtr;

    public ViewList() {
        nativePtr = NativeMethods.ViewList_new();
    }

    public IEnumerator<KeyValuePair<int, View>> GetEnumerator() {
        IntPtr begin = NativeMethods.ViewList_begin(nativePtr);
        IntPtr end   = NativeMethods.ViewList_end(nativePtr);

        while (!NativeMethods.ViewIterator_equals(begin, end)) {
            int key = NativeMethods.ViewIterator_key(begin);
            IntPtr viewPtr = NativeMethods.ViewIterator_value(begin);
            View view = new View(viewPtr, false);
            yield return new KeyValuePair<int, View>(key, view);
            NativeMethods.ViewIterator_next(begin);
        }

        NativeMethods.ViewIterator_delete(begin);
        NativeMethods.ViewIterator_delete(end);
    }


    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

    public void Dispose() {
        if (nativePtr != IntPtr.Zero) {
            NativeMethods.ViewList_delete(nativePtr);
            nativePtr = IntPtr.Zero;
        }
    }
}

public static class KeyValuePairExtensions
{
    public static void Deconstruct<TKey, TValue>(this KeyValuePair<TKey, TValue> kvp, out TKey key, out TValue value)
    {
        key = kvp.Key;
        value = kvp.Value;
    }
}
