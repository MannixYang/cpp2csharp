using System;
using System.Runtime.InteropServices;

public class StringObj : SafeHandle
{
    private object owner;

    internal StringObj(IntPtr ptr, object owner = null):base(ptr, owner == null)
    {
        this.owner = owner;
    }

    internal bool own_memory { get { return this.owner == null; } }
    public override bool IsInvalid { get { return this.handle == IntPtr.Zero; } }

    [DllImport("libStringObj")] 
    internal static extern IntPtr cmid_new_string_obj();
    internal StringObj():base(IntPtr.Zero, true)
    {
        this.handle =  cmid_new_string_obj();
    }

    [DllImport("libStringObj")]
    [return: MarshalAs(UnmanagedType.LPWStr)]
    internal static extern string cmid_string_obj_get(IntPtr it, bool b);
    public String get(){ 
        return cmid_string_obj_get(this.handle, this.own_memory); 
    }

    [DllImport("libStringObj")]
    internal static extern int cmid_string_obj_set(IntPtr it, bool b, [MarshalAs(UnmanagedType.LPWStr)] string s);
    public void set(String s) { 
        cmid_string_obj_set(this.handle, this.own_memory, s); 
    }


    [DllImport("libStringObj")]
    internal static extern IntPtr cmid_string_obj_delete(IntPtr it, bool b);
    protected override bool ReleaseHandle()
    {
        if (this.owner == null && !this.IsInvalid)
            cmid_string_obj_delete(this.handle, this.own_memory);

        SetHandleAsInvalid();
        return true;
    }

}
