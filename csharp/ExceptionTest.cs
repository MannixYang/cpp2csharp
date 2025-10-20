using System;
using System.Runtime.InteropServices;
internal class PINVOKE
{
    internal static class PendingException
    {
        // 每个线程独立的异常槽
        [ThreadStatic]
        private static global::System.Exception  thread_pending_exception = null;


        public static bool pending { get { return thread_pending_exception != null; } }


        public static void set(Exception e)
        {
            if (thread_pending_exception != null)
            {
                throw new ApplicationException(
                    "FATAL: Previous pending exception was not retrieved before setting a new one.",
                    e);
            }
            thread_pending_exception = e;
        }

        public static Exception retrieve()
        {
            var e = thread_pending_exception;
            thread_pending_exception = null;
            return e;
        }
    }

    protected class ExceptionHelper
    {
        public delegate void StdExceptionDelegate(string message);
        static StdExceptionDelegate std_exception_delegate = new StdExceptionDelegate(set_std_exception);

        [DllImport("exception_test")]
        internal static extern void std_exception_callback_register(StdExceptionDelegate exception_delegate);
        static void set_std_exception(string message)
        {
            PendingException.set(new System.Exception(message));
        }
        static ExceptionHelper()
        {
            Console.WriteLine("This is ExceptionHelper()");
            std_exception_callback_register(std_exception_delegate);
        }
    }
    static ExceptionHelper exception_helper = new ExceptionHelper();

    [DllImport("exception_test")]
    internal static extern void csharp_test_exception();
}


public class Global{


    public static void test_exception(){
        PINVOKE.csharp_test_exception();
        if (PINVOKE.PendingException.pending) 
            throw PINVOKE.PendingException.retrieve();
    }
}