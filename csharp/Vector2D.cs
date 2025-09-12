using System;

public class Vector2D : IDisposable
{
    private IntPtr handle;

    public Vector2D(double x, double y)
    {
        handle = NativeMethods.Vector2D_Create(x, y);
    }

    public double Length2() => NativeMethods.Vector2D_Length2(handle);

    public void Dispose()
    {
        if (handle != IntPtr.Zero)
        {
            NativeMethods.Vector2D_Destroy(handle);
            handle = IntPtr.Zero;
        }
    }
}
