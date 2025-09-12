using System;

public class Animal : IDisposable
{
    public IntPtr nativePtr;
    protected bool ownsMemory;

    internal Animal(IntPtr ptr, bool ownsMemory = false)
    {
        nativePtr = ptr;
        this.ownsMemory = ownsMemory;
    }

    public virtual int weight(){ return NativeMethods.Animal_weight(nativePtr); }

    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing) {
        if (ownsMemory && nativePtr != IntPtr.Zero) {
            NativeMethods.Animal_delete(nativePtr);
            nativePtr = IntPtr.Zero;
        }
    }
}

public class Cat : Animal
{
    public Cat() : base(NativeMethods.Cat_new(), true) { }

    internal Cat(IntPtr ptr, bool ownsMemory = false) : base(ptr, ownsMemory) { }

    protected override void Dispose(bool disposing)
    {
        if (ownsMemory && nativePtr != IntPtr.Zero)
        {
            NativeMethods.Cat_delete(nativePtr);
            nativePtr = IntPtr.Zero;
        }
    }

    public Animal copy(){
        return new Animal(NativeMethods.Cat_copy(nativePtr), false);
    }

    public override int weight() { return NativeMethods.Cat_weight(nativePtr); }

    public static bool operator ==(Cat lhs, Animal rhs)
    {
        return NativeMethods.Cat_Equals(lhs.nativePtr, rhs.nativePtr);
    }

    public static bool operator !=(Cat lhs, Animal rhs) => !(lhs == rhs);
}

public static class AnimalExtensions
{
    public static T As<T>(this Animal a) where T : Animal
    {
        if (typeof(T) == typeof(Cat))
        {
            IntPtr ptr = NativeMethods.Animal_to_Cat(a.nativePtr);
            return new Cat(ptr, false) as T;
        }
        throw new NotSupportedException($"As<{typeof(T).Name}> not supported");
    }
}
