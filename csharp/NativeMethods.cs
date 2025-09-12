using System;
using System.Runtime.InteropServices;

internal class NativeMethods
{

    // Calculator
    [DllImport("libMyLib", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr Calculator_Create();

    [DllImport("libMyLib", CallingConvention = CallingConvention.Cdecl)]
    public static extern void Calculator_Destroy(IntPtr handle);

    [DllImport("libMyLib", CallingConvention = CallingConvention.Cdecl)]
    public static extern int Calculator_Add(IntPtr handle, int a, int b);

    [DllImport("libMyLib", CallingConvention = CallingConvention.Cdecl)]
    public static extern int Calculator_Sub(IntPtr handle, int a, int b);

    // Vector2D
    [DllImport("libMyLib", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr Vector2D_Create(double x, double y);

    [DllImport("libMyLib", CallingConvention = CallingConvention.Cdecl)]
    public static extern void Vector2D_Destroy(IntPtr handle);

    [DllImport("libMyLib", CallingConvention = CallingConvention.Cdecl)]
    public static extern double Vector2D_Length2(IntPtr handle);

    // Animal
    [DllImport("libAnimal", EntryPoint = "Cat_new")]
    internal static extern IntPtr Cat_new();

    [DllImport("libAnimal", EntryPoint = "Cat_delete")]
    internal static extern void Cat_delete(IntPtr c);

    [DllImport("libAnimal", EntryPoint = "Cat_weight")]
    internal static extern int Cat_weight(IntPtr c);

    [DllImport("libAnimal", EntryPoint = "Cat_copy")]
    internal static extern IntPtr Cat_copy(IntPtr c);

    [DllImport("libAnimal", EntryPoint = "Animal_delete")]
    internal static extern int Animal_delete(IntPtr a);

    [DllImport("libAnimal", EntryPoint = "Animal_weight")]
    internal static extern int Animal_weight(IntPtr a);


    [DllImport("libAnimal", EntryPoint = "Animal_to_Cat")]
    internal static extern IntPtr Animal_to_Cat(IntPtr a);

    [DllImport("libAnimal", EntryPoint = "Cat_Equals")]
    [return: MarshalAs(UnmanagedType.I1)]
    internal static extern bool Cat_Equals(IntPtr lhs, IntPtr rhs);


    // View
    [DllImport("libView")] internal static extern int View_getId(IntPtr v);
    // 容器 迭代器
    [DllImport("libView")] internal static extern IntPtr ViewList_new();
    [DllImport("libView")] internal static extern void ViewList_delete(IntPtr v);
    [DllImport("libView")] internal static extern IntPtr ViewList_begin(IntPtr v);
    [DllImport("libView")] internal static extern IntPtr ViewList_end(IntPtr v);
    [DllImport("libView")] internal static extern void ViewIterator_delete(IntPtr it);
    [DllImport("libView")] internal static extern void ViewIterator_next(IntPtr it);
    [DllImport("libView")] [return: MarshalAs(UnmanagedType.I1)]
    internal static extern bool ViewIterator_equals(IntPtr a, IntPtr b);
    [DllImport("libView")] internal static extern int ViewIterator_key(IntPtr it);
    [DllImport("libView")] internal static extern IntPtr ViewIterator_value(IntPtr it);


    [DllImport("libStringObj")] internal static extern IntPtr csharp_new_string_obj();
    [DllImport("libStringObj")] internal static extern int csharp_string_obj_set(IntPtr it, bool b, [MarshalAs(UnmanagedType.LPWStr)]string s);
    [DllImport("libStringObj")][return: MarshalAs(UnmanagedType.LPWStr)]
    internal static extern string csharp_string_obj_get(IntPtr it, bool b);

    protected class StringHelper
    {

        [return: MarshalAs(UnmanagedType.LPWStr)]
        public delegate string StringDelegate(IntPtr cString);
        static StringDelegate stringDelegate = new StringDelegate(CreateString);

        [DllImport("libStringObj", EntryPoint = "RegisterStringCallback")]
        public static extern void RegisterStringCallback(StringDelegate stringDelegate);

        static string CreateString(IntPtr cString)
        {
            string s = Marshal.PtrToStringUni(cString);
            return s;
        }

        static StringHelper()
        {
            RegisterStringCallback(stringDelegate);
            Console.WriteLine("This is StringHelper()");
        }
    }

    static StringHelper string_helper = new StringHelper();
}
