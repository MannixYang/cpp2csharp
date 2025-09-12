using System;

public class Calculator : IDisposable
{
    private IntPtr handle;

    public Calculator()
    {
        handle = NativeMethods.Calculator_Create();
    }

    public int Add(int a, int b) => NativeMethods.Calculator_Add(handle, a, b);
    public int Sub(int a, int b) => NativeMethods.Calculator_Sub(handle, a, b);

    public void Dispose()
    {
        if (handle != IntPtr.Zero)
        {
            NativeMethods.Calculator_Destroy(handle);
            handle = IntPtr.Zero;
        }
    }
}
