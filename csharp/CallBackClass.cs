
using System;
using System.Reflection;
using System.Runtime.InteropServices;


namespace CallBackTest
{
    public delegate void CallPrintInvoker(IntPtr gcHandle);

    internal static class NativeMethods
    {
        private const string DllName = "callback.dll";
        // native
        [DllImport(DllName)]
        internal static extern void CS_call_print(IntPtr cb);

        [DllImport(DllName)]
        internal static extern void CS_ICallBack_print(IntPtr cb);

        // 注册静态函数给C++
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        internal static extern IntPtr CS_new_i_call_back(IntPtr self, CallPrintInvoker invoker);
    }

    public abstract class ICallBack : SafeHandle
    {
        protected object owner;

        // delegate
        private static readonly CallPrintInvoker invoker = CallPrintInvokerDelegate;
        private static void CallPrintInvokerDelegate(IntPtr self)
        {
            var obj = (ICallBack)GCHandle.FromIntPtr(self).Target;
            obj.print();
        }

        private GCHandle self;
        internal ICallBack(IntPtr ptr, object owner = null) : base(ptr, owner == null)
        {
            this.owner = owner;
        }

        public abstract void print();

        public IntPtr get_handle() { return handle;  }

        protected ICallBack() : base(IntPtr.Zero, true)
        {
            self = GCHandle.Alloc(this, GCHandleType.Normal);
            this.handle = NativeMethods.CS_new_i_call_back(GCHandle.ToIntPtr(self), invoker);
        }

        public override bool IsInvalid { get { return this.handle == IntPtr.Zero; } }

        protected override bool ReleaseHandle()
        {
            if (self.IsAllocated)
                self.Free();
            return true;
        }
    }

    static class Global
    {
        public static void call_print(ICallBack cb)
        {
            NativeMethods.CS_call_print(cb.get_handle());
        }

    }
}